//
//  Direct3DRectangleBatcher.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 9/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "Direct3DRectangleBatcher.h"
#include "BasicReaderWriter.h"
#include "DirectXHelper.h"
#include "GameConstants.h"
#include "Rectangle.h"
#include "Vector2D.h"
#include <deque>

using namespace Microsoft::WRL;

ComPtr<ID3D11Device1> m_d3dDevice1; // the device interface
ComPtr<ID3D11DeviceContext1> m_d3dContext1; // the device context interface
ComPtr<ID3D11BlendState> blendstate1; // the blend state interface
ComPtr<ID3D11VertexShader> vertexshader1; // the vertex shader interface
ComPtr<ID3D11PixelShader> pixelshader1; // the pixel shader interface
ComPtr<ID3D11InputLayout> inputlayout1; // the input layout interface
ComPtr<ID3D11Buffer> constantbuffer1; // the constant buffer interface
ComPtr<ID3D11Buffer> vertexbuffer1; // the vertex buffer interface
ComPtr<ID3D11Buffer> indexbuffer1; // the index buffer interface

static const size_t MaxBatchSize = 256;
static const size_t MaxRectangleBatchSize = 128;
static const size_t VerticesPerRectangle = 4;
static const size_t IndicesPerRectangle = 6;

std::deque<COLOR_VERTEX> m_colorVertices;

Direct3DRectangleBatcher::Direct3DRectangleBatcher(ID3D11Device1 *d3dDevice, ID3D11DeviceContext1 *d3dContext, bool isFill) : RectangleBatcher(isFill)
{
	m_d3dDevice1 = ComPtr<ID3D11Device1>(d3dDevice);
	m_d3dContext1 = ComPtr<ID3D11DeviceContext1>(d3dContext);
    m_isFill = isFill;

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

	m_d3dDevice1->CreateBlendState(&bd, &blendstate1);

	// Create a Basic Reader - Writer class to load data from disk.This class is examined
	// in the Resource Loading sample.
	BasicReaderWriter^ reader = ref new BasicReaderWriter();
	Platform::Array<byte>^ vertexShaderBytecode;
	Platform::Array<byte>^ pixelShaderBytecode;

	// Load the raw shader bytecode from disk and create shader objects with it.
	vertexShaderBytecode = reader->ReadData("ColorVertexShader.cso");
	pixelShaderBytecode = reader->ReadData("ColorPixelShader.cso");

	// initialize input layout
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	// create and set the input layout
	m_d3dDevice1->CreateInputLayout(ied, ARRAYSIZE(ied), vertexShaderBytecode->Data, vertexShaderBytecode->Length, &inputlayout1);

	m_d3dDevice1->CreateVertexShader(vertexShaderBytecode->Data, vertexShaderBytecode->Length, nullptr, &vertexshader1);
	m_d3dDevice1->CreatePixelShader(pixelShaderBytecode->Data, pixelShaderBytecode->Length, nullptr, &pixelshader1);

	D3D11_BUFFER_DESC bd2 = { 0 };

	bd2.Usage = D3D11_USAGE_DEFAULT;
	bd2.ByteWidth = 64;
	bd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	m_d3dDevice1->CreateBuffer(&bd2, nullptr, &constantbuffer1);
}

void Direct3DRectangleBatcher::beginBatch()
{
	m_colorVertices.clear();
	m_iNumRectangles = 0;
}

void Direct3DRectangleBatcher::endBatch()
{
	if (m_iNumRectangles > 0)
	{
		// set the blend state
		m_d3dContext1->OMSetBlendState(blendstate1.Get(), 0, 0xffffffff);

		// set the primitive topology
		m_d3dContext1->IASetPrimitiveTopology(m_isFill ? D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST : D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

		m_d3dContext1->IASetInputLayout(inputlayout1.Get());

		// set the shader objects as the active shaders
		m_d3dContext1->VSSetShader(vertexshader1.Get(), nullptr, 0);
		m_d3dContext1->PSSetShader(pixelshader1.Get(), nullptr, 0);

		m_d3dContext1->IASetIndexBuffer(indexbuffer1.Get(), DXGI_FORMAT_R16_UINT, 0);

		using namespace DirectX;

		// calculate the view transformation
		XMVECTOR vecCamPosition = XMVectorSet(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 1, 0);
		XMVECTOR vecCamLookAt = XMVectorSet(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0, 0);
		XMVECTOR vecCamUp = XMVectorSet(0, 1, 0, 0);
		XMMATRIX matView = XMMatrixLookAtRH(vecCamPosition, vecCamLookAt, vecCamUp);

		// calculate the projection transformation
		XMMATRIX matProjection = XMMatrixOrthographicRH(SCREEN_WIDTH, SCREEN_HEIGHT, -1.0, 1.0);

		// calculate the final matrix
		XMMATRIX matFinal = matView * matProjection;

		// send the final matrix to video memory
		m_d3dContext1->UpdateSubresource(constantbuffer1.Get(), 0, 0, &matFinal, 0, 0);

		m_d3dContext1->VSSetConstantBuffers(0, 1, constantbuffer1.GetAddressOf());

		std::vector<COLOR_VERTEX> tempColorVertices;
		while (m_iNumRectangles > 0)
		{
			int len = m_colorVertices.size();
			for (int i = 0; i < len && i < MaxRectangleBatchSize * VerticesPerRectangle; i++)
			{
				tempColorVertices.push_back(m_colorVertices.front());
				m_colorVertices.pop_front();
			}

			// create the vertex buffer
			D3D11_BUFFER_DESC bd = { 0 };
			D3D11_SUBRESOURCE_DATA srd = { 0 };

			bd.ByteWidth = sizeof(COLOR_VERTEX)* tempColorVertices.size();
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.Usage = D3D11_USAGE_DEFAULT;
			srd.pSysMem = &tempColorVertices.front();

			DX::ThrowIfFailed(m_d3dDevice1->CreateBuffer(&bd, &srd, &vertexbuffer1));

			// Set the vertex and index buffer
			UINT stride = sizeof(COLOR_VERTEX);
			UINT offset = 0;
			m_d3dContext1->IASetVertexBuffers(0, 1, vertexbuffer1.GetAddressOf(), &stride, &offset);

			int indexCount = m_iNumRectangles > MaxRectangleBatchSize ? MaxRectangleBatchSize : m_iNumRectangles;
			m_d3dContext1->DrawIndexed(indexCount * IndicesPerRectangle, 0, 0);

			m_iNumRectangles -= MaxRectangleBatchSize;

			tempColorVertices.clear();
		}
	}
}

void Direct3DRectangleBatcher::renderRectangle(Rectangle &rectangle, Color &color)
{
    float x1 = rectangle.getLowerLeft().getX();
    float y1 = rectangle.getLowerLeft().getY();
    float x2 = x1 + rectangle.getWidth();
    float y2 = y1 + rectangle.getHeight();
    
    renderRectangle(x1, y1, x2, y2, color);
}

void Direct3DRectangleBatcher::renderRectangle(float x1, float y1, float x2, float y2, Color &color)
{
	addVertexCoordinate(x1, y1, 0, color.red, color.green, color.blue, color.alpha, 0, 0);
	addVertexCoordinate(x1, y2, 0, color.red, color.green, color.blue, color.alpha, 0, 0);
	addVertexCoordinate(x2, y2, 0, color.red, color.green, color.blue, color.alpha, 0, 0);
	addVertexCoordinate(x2, y1, 0, color.red, color.green, color.blue, color.alpha, 0, 0);

	m_iNumRectangles++;
}

void Direct3DRectangleBatcher::addVertexCoordinate(float x, float y, float z, float r, float g, float b, float a, float u, float v)
{
	COLOR_VERTEX cv = { x, y, z, r, g, b, a };
	m_colorVertices.push_back(cv);
}

// Creates the SpriteBatch index buffer.
void Direct3DRectangleBatcher::createIndexBuffer()
{
	D3D11_BUFFER_DESC indexBufferDesc = { 0 };

	indexBufferDesc.ByteWidth = sizeof(short)* MaxBatchSize * IndicesPerRectangle;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;

	auto indexValues = createIndexValues();

	D3D11_SUBRESOURCE_DATA indexDataDesc = { 0 };

	indexDataDesc.pSysMem = &indexValues.front();

	using namespace DirectX;
	DX::ThrowIfFailed(m_d3dDevice1->CreateBuffer(&indexBufferDesc, &indexDataDesc, &indexbuffer1));
}

// Helper for populating the SpriteBatch index buffer.
std::vector<short> Direct3DRectangleBatcher::createIndexValues()
{
	std::vector<short> indices;

	indices.reserve(MaxBatchSize * IndicesPerRectangle);

	short j = 0;
	for (int i = 0; i < MaxBatchSize * IndicesPerRectangle; i += IndicesPerRectangle, j += VerticesPerRectangle)
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