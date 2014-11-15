//
//  Direct3DSpriteBatcher.cpp
//  insectoid-defense
//
//  Created by Stephen Gowen on 9/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "macros.h"
#include "Direct3DSpriteBatcher.h"
#include "BasicReaderWriter.h"
#include "GameConstants.h"
#include <stdlib.h>
#include <deque>
#include "TextureRegion.h"

ComPtr<ID3D11Device1> m_d3dDevice; // the device interface
ComPtr<ID3D11DeviceContext1> m_d3dContext; // the device context interface
ComPtr<ID3D11BlendState> blendstate; // the blend state interface
ComPtr<ID3D11SamplerState> samplerstate; // the sampler state interfaces
ComPtr<ID3D11VertexShader> vertexshader; // the vertex shader interface
ComPtr<ID3D11PixelShader> pixelshader; // the pixel shader interface
ComPtr<ID3D11InputLayout> inputlayout; // the input layout interface
ComPtr<ID3D11Buffer> constantbuffer; // the constant buffer interface
ComPtr<ID3D11Buffer> vertexbuffer; // the vertex buffer interface
ComPtr<ID3D11Buffer> indexbuffer; // the index buffer interface

DirectX::XMMATRIX matFinal;

static const size_t MaxBatchSize = 2048;
static const size_t MaxSpriteBatchSize = 128;
static const size_t VerticesPerSprite = 4;
static const size_t IndicesPerSprite = 6;

std::deque<TEXTURE_VERTEX> m_textureVertices;

Direct3DSpriteBatcher::Direct3DSpriteBatcher(ID3D11Device1 *d3dDevice, ID3D11DeviceContext1 *d3dContext)
{
	m_d3dDevice = ComPtr<ID3D11Device1>(d3dDevice);
	m_d3dContext = ComPtr<ID3D11DeviceContext1>(d3dContext);
	m_iNumSprites = 0;

	createIndexBuffer();

	D3D11_BLEND_DESC bd;
	bd.RenderTarget[0].BlendEnable = TRUE;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	bd.IndependentBlendEnable = FALSE;
	bd.AlphaToCoverageEnable = FALSE;

	m_d3dDevice->CreateBlendState(&bd, &blendstate);

	D3D11_SAMPLER_DESC sd;
	sd.Filter = D3D11_FILTER_ANISOTROPIC;
	sd.MaxAnisotropy = 16;
	sd.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sd.BorderColor[0] = 0.0f;
	sd.BorderColor[1] = 0.0f;
	sd.BorderColor[2] = 0.0f;
	sd.BorderColor[3] = 0.0f;
	sd.MinLOD = 0.0f;
	sd.MaxLOD = FLT_MAX;
	sd.MipLODBias = 0.0f;
	sd.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;       // linear filtering
	sd.MinLOD = 5.0f;                                  // mip level 5 will appear blurred

	m_d3dDevice->CreateSamplerState(&sd, &samplerstate);    // create the linear blur sampler

	// Create a Basic Reader-Writer class to load data from disk.  This class is examined
	// in the Resource Loading sample.
	BasicReaderWriter^ reader = ref new BasicReaderWriter();
	Platform::Array<byte>^ vertexShaderBytecode;
	Platform::Array<byte>^ pixelShaderBytecode;

	// Load the raw shader bytecode from disk and create shader objects with it.
	vertexShaderBytecode = reader->ReadData("TextureVertexShader.cso");
	pixelShaderBytecode = reader->ReadData("TexturePixelShader.cso");

	// initialize input layout
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// create and set the input layout
	m_d3dDevice->CreateInputLayout(ied, ARRAYSIZE(ied), vertexShaderBytecode->Data, vertexShaderBytecode->Length, &inputlayout);

	m_d3dDevice->CreateVertexShader(vertexShaderBytecode->Data, vertexShaderBytecode->Length, nullptr, &vertexshader);
	m_d3dDevice->CreatePixelShader(pixelShaderBytecode->Data, pixelShaderBytecode->Length, nullptr, &pixelshader);

	D3D11_BUFFER_DESC bd2 = { 0 };

	bd2.Usage = D3D11_USAGE_DEFAULT;
	bd2.ByteWidth = 64;
	bd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	m_d3dDevice->CreateBuffer(&bd2, nullptr, &constantbuffer);

	using namespace DirectX;

	// calculate the view transformation
	XMVECTOR vecCamPosition = XMVectorSet(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1, 0);
	XMVECTOR vecCamLookAt = XMVectorSet(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, 0);
	XMVECTOR vecCamUp = XMVectorSet(0, 1, 0, 0);
	XMMATRIX matView = XMMatrixLookAtRH(vecCamPosition, vecCamLookAt, vecCamUp);

	// calculate the projection transformation
	XMMATRIX matProjection = XMMatrixOrthographicRH(SCREEN_WIDTH, SCREEN_HEIGHT, -1.0, 1.0);

	// calculate the final matrix
	matFinal = matView * matProjection;
}

void Direct3DSpriteBatcher::beginBatch()
{
	m_textureVertices.clear();
	m_iNumSprites = 0;
}

void Direct3DSpriteBatcher::endBatchWithTexture(TextureWrapper &textureWrapper)
{
	if (m_iNumSprites > 0)
	{
		// set the blend state
		m_d3dContext->OMSetBlendState(blendstate.Get(), 0, 0xffffffff);

		// set the appropriate sampler state
		m_d3dContext->PSSetSamplers(0, 1, samplerstate.GetAddressOf());

		// tell the GPU which texture to use
		m_d3dContext->PSSetShaderResources(0, 1, &textureWrapper.texture);

		// set the primitive topology
		m_d3dContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		m_d3dContext->IASetInputLayout(inputlayout.Get());

		// set the shader objects as the active shaders
		m_d3dContext->VSSetShader(vertexshader.Get(), nullptr, 0);
		m_d3dContext->PSSetShader(pixelshader.Get(), nullptr, 0);

		m_d3dContext->IASetIndexBuffer(indexbuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

		// send the final matrix to video memory
		m_d3dContext->UpdateSubresource(constantbuffer.Get(), 0, 0, &matFinal, 0, 0);

		m_d3dContext->VSSetConstantBuffers(0, 1, constantbuffer.GetAddressOf());

		std::vector<TEXTURE_VERTEX> tempTextureVertices;
		while (m_iNumSprites > 0)
		{
			int len = m_textureVertices.size();
			for (int i = 0; i < len && i < MaxSpriteBatchSize * VerticesPerSprite; i++)
			{
				tempTextureVertices.push_back(m_textureVertices.front());
				m_textureVertices.pop_front();
			}

			// create the vertex buffer
			D3D11_BUFFER_DESC bd = { 0 };
			D3D11_SUBRESOURCE_DATA srd = { 0 };

			bd.ByteWidth = sizeof(TEXTURE_VERTEX)* tempTextureVertices.size();
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.Usage = D3D11_USAGE_DEFAULT;
			srd.pSysMem = &tempTextureVertices.front();

			m_d3dDevice->CreateBuffer(&bd, &srd, &vertexbuffer);

			// Set the vertex and index buffer
			UINT stride = sizeof(TEXTURE_VERTEX);
			UINT offset = 0;
			m_d3dContext->IASetVertexBuffers(0, 1, vertexbuffer.GetAddressOf(), &stride, &offset);

			int indexCount = m_iNumSprites > MaxSpriteBatchSize ? MaxSpriteBatchSize : m_iNumSprites;
			m_d3dContext->DrawIndexed(indexCount * IndicesPerSprite, 0, 0);

			m_iNumSprites -= MaxSpriteBatchSize;

			tempTextureVertices.clear();
		}
	}
}

void Direct3DSpriteBatcher::drawSprite(float x, float y, float width, float height, float angle, TextureRegion tr)
{
	if (angle != 0)
	{
		float halfWidth = width / 2;
		float halfHeight = height / 2;

		float rad = DEGREES_TO_RADIANS(angle);
		float cos = cosf(rad);
		float sin = sinf(rad);

		float x1 = -halfWidth * cos - (-halfHeight) * sin;
		float y1 = -halfWidth * sin + (-halfHeight) * cos;

		float x2 = halfWidth * cos - (-halfHeight) * sin;
		float y2 = halfWidth * sin + (-halfHeight) * cos;

		float x3 = halfWidth * cos - halfHeight * sin;
		float y3 = halfWidth * sin + halfHeight * cos;

		float x4 = -halfWidth * cos - halfHeight * sin;
		float y4 = -halfWidth * sin + halfHeight * cos;

		x1 += x;
		y1 += y;

		x2 += x;
		y2 += y;

		x3 += x;
		y3 += y;

		x4 += x;
		y4 += y;

		addVertexCoordinate(x1, y1, 0, 1, 1, 1, 1, tr.u1, tr.v2);
		addVertexCoordinate(x4, y4, 0, 1, 1, 1, 1, tr.u1, tr.v1);
		addVertexCoordinate(x3, y3, 0, 1, 1, 1, 1, tr.u2, tr.v1);
		addVertexCoordinate(x2, y2, 0, 1, 1, 1, 1, tr.u2, tr.v2);
	}
	else
	{
		drawSprite(x, y, width, height, tr);
	}

	m_iNumSprites++;
}

void Direct3DSpriteBatcher::drawSprite(float x, float y, float width, float height, float angle, Color &color, TextureRegion tr)
{
	if (angle != 0)
	{
		float halfWidth = width / 2;
		float halfHeight = height / 2;

		float rad = DEGREES_TO_RADIANS(angle);
		float cos = cosf(rad);
		float sin = sinf(rad);

		float x1 = -halfWidth * cos - (-halfHeight) * sin;
		float y1 = -halfWidth * sin + (-halfHeight) * cos;

		float x2 = halfWidth * cos - (-halfHeight) * sin;
		float y2 = halfWidth * sin + (-halfHeight) * cos;

		float x3 = halfWidth * cos - halfHeight * sin;
		float y3 = halfWidth * sin + halfHeight * cos;

		float x4 = -halfWidth * cos - halfHeight * sin;
		float y4 = -halfWidth * sin + halfHeight * cos;

		x1 += x;
		y1 += y;

		x2 += x;
		y2 += y;

		x3 += x;
		y3 += y;

		x4 += x;
		y4 += y;

		addVertexCoordinate(x1, y1, 0, color.red, color.green, color.blue, color.alpha, tr.u1, tr.v2);
		addVertexCoordinate(x4, y4, 0, color.red, color.green, color.blue, color.alpha, tr.u1, tr.v1);
		addVertexCoordinate(x3, y3, 0, color.red, color.green, color.blue, color.alpha, tr.u2, tr.v1);
		addVertexCoordinate(x2, y2, 0, color.red, color.green, color.blue, color.alpha, tr.u2, tr.v2);
	}
	else
	{
		drawSprite(x, y, width, height, color, tr);
	}

	m_iNumSprites++;
}

#pragma <Private>

void Direct3DSpriteBatcher::drawSprite(float x, float y, float width, float height, TextureRegion tr)
{
	float halfWidth = width / 2;
	float halfHeight = height / 2;
	float x1 = x - halfWidth;
	float y1 = y - halfHeight;
	float x2 = x + halfWidth;
	float y2 = y + halfHeight;

	addVertexCoordinate(x1, y1, 0, 1, 1, 1, 1, tr.u1, tr.v2);
	addVertexCoordinate(x1, y2, 0, 1, 1, 1, 1, tr.u1, tr.v1);
	addVertexCoordinate(x2, y2, 0, 1, 1, 1, 1, tr.u2, tr.v1);
	addVertexCoordinate(x2, y1, 0, 1, 1, 1, 1, tr.u2, tr.v2);
}

void Direct3DSpriteBatcher::drawSprite(float x, float y, float width, float height, Color &color, TextureRegion tr)
{
	float halfWidth = width / 2;
	float halfHeight = height / 2;
	float x1 = x - halfWidth;
	float y1 = y - halfHeight;
	float x2 = x + halfWidth;
	float y2 = y + halfHeight;

	addVertexCoordinate(x1, y1, 0, color.red, color.green, color.blue, color.alpha, tr.u1, tr.v2);
	addVertexCoordinate(x1, y2, 0, color.red, color.green, color.blue, color.alpha, tr.u1, tr.v1);
	addVertexCoordinate(x2, y2, 0, color.red, color.green, color.blue, color.alpha, tr.u2, tr.v1);
	addVertexCoordinate(x2, y1, 0, color.red, color.green, color.blue, color.alpha, tr.u2, tr.v2);
}

void Direct3DSpriteBatcher::addVertexCoordinate(float x, float y, float z, float r, float g, float b, float a, float u, float v)
{
	TEXTURE_VERTEX tv = { x, y, z, r, g, b, a, u, v };
	m_textureVertices.push_back(tv);
}

// Creates the SpriteBatch index buffer.
void Direct3DSpriteBatcher::createIndexBuffer()
{
	D3D11_BUFFER_DESC indexBufferDesc = { 0 };

	indexBufferDesc.ByteWidth = sizeof(short)* MaxBatchSize;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	auto indexValues = createIndexValues();

	D3D11_SUBRESOURCE_DATA indexDataDesc = { 0 };

	indexDataDesc.pSysMem = &indexValues.front();

	m_d3dDevice->CreateBuffer(&indexBufferDesc, &indexDataDesc, &indexbuffer);
}

// Helper for populating the SpriteBatch index buffer.
std::vector<short> Direct3DSpriteBatcher::createIndexValues()
{
	std::vector<short> indices;

	indices.reserve(MaxBatchSize * IndicesPerSprite);

	short j = 0;
	for (int i = 0; i < MaxBatchSize * IndicesPerSprite; i += IndicesPerSprite, j += VerticesPerSprite)
	{
		indices.push_back(j);
		indices.push_back(j + 1);
		indices.push_back(j + 2);
		indices.push_back(j + 2);
		indices.push_back(j + 3);
		indices.push_back(j + 0);
	}

	return indices;
}