//
//  Vertices2D.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 9/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "Vertices2D.h"
#include "BasicReaderWriter.h"
#include "DirectXMath.h"
#include "GameConstants.h"
#include <stdlib.h>

using namespace DirectX;

Vertices2D::Vertices2D(ID3D11Device1 &dev, ID3D11DeviceContext1 &devcon, int maxNumVertices, bool isUsingTextureCoordinates, bool isUsingColors)
{
	m_iMaxNumVertices = maxNumVertices;

	// Create a Basic Reader-Writer class to load data from disk.  This class is examined
	// in the Resource Loading sample.
	BasicReaderWriter^ reader = ref new BasicReaderWriter();
	Platform::Array<byte>^ vertexShaderBytecode;
	Platform::Array<byte>^ pixelShaderBytecode;

	if (isUsingTextureCoordinates)
	{
		m_textureVertices = std::unique_ptr<TEXTURE_VERTEX>(new TEXTURE_VERTEX[maxNumVertices]);

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
		dev.CreateInputLayout(ied, ARRAYSIZE(ied), vertexShaderBytecode->Data, vertexShaderBytecode->Length, &inputlayout);
	}
	else if (isUsingColors)
	{
		m_colorVertices = std::unique_ptr<COLOR_VERTEX>(new COLOR_VERTEX[maxNumVertices]);

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
		dev.CreateInputLayout(ied, ARRAYSIZE(ied), vertexShaderBytecode->Data, vertexShaderBytecode->Length, &inputlayout);
	}
	else
	{
		m_basicVertices = std::unique_ptr<BASIC_VERTEX>(new BASIC_VERTEX[maxNumVertices]);

		// Load the raw shader bytecode from disk and create shader objects with it.
		vertexShaderBytecode = reader->ReadData("BasicVertexShader.cso");
		pixelShaderBytecode = reader->ReadData("BasicPixelShader.cso");

		// initialize input layout
		D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 }
		};

		// create and set the input layout
		dev.CreateInputLayout(ied, ARRAYSIZE(ied), vertexShaderBytecode->Data, vertexShaderBytecode->Length, &inputlayout);
	}
	
	dev.CreateVertexShader(vertexShaderBytecode->Data, vertexShaderBytecode->Length, nullptr, &vertexshader);
	dev.CreatePixelShader(pixelShaderBytecode->Data, pixelShaderBytecode->Length, nullptr, &pixelshader);

	D3D11_BUFFER_DESC bd2 = { 0 };

	bd2.Usage = D3D11_USAGE_DEFAULT;
	bd2.ByteWidth = 64;
	bd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	dev.CreateBuffer(&bd2, nullptr, &constantbuffer);

	devcon.VSSetConstantBuffers(0, 1, constantbuffer.GetAddressOf());
    
    m_iVerticesIndex = 0;
    
    m_hasColor = isUsingColors;
    m_hasTextureCoordinates = isUsingTextureCoordinates;
}

void Vertices2D::resetIndex()
{
    m_iVerticesIndex = 0;
}

void Vertices2D::addVertexCoordinate(float x, float y, float z, float r, float g, float b, float a, float u, float v)
{
	if (m_hasTextureCoordinates)
	{
		TEXTURE_VERTEX tv = { x, y, z, r, g, b, a, u, v };
		m_textureVertices.get()[m_iVerticesIndex++] = tv;
	}
	else if (m_hasColor)
	{
		COLOR_VERTEX cv = { x, y, z, r, g, b, a };
		m_colorVertices.get()[m_iVerticesIndex++] = cv;
	}
	else
	{
		BASIC_VERTEX bv = { x, y, z };
		m_basicVertices.get()[m_iVerticesIndex++] = bv;
	}
}

void Vertices2D::bind(ID3D11Device1 &dev, ID3D11DeviceContext1 &devcon)
{
	// create the vertex buffer
	D3D11_BUFFER_DESC bd = { 0 };
	D3D11_SUBRESOURCE_DATA srd = { 0 };

	if (m_hasTextureCoordinates)
	{
		bd.ByteWidth = sizeof(TEXTURE_VERTEX) * m_iMaxNumVertices;
		srd.pSysMem = m_textureVertices.get();
	}
	else if (m_hasColor)
	{
		bd.ByteWidth = sizeof(COLOR_VERTEX) * m_iMaxNumVertices;
		srd.pSysMem = m_colorVertices.get();
	}
	else
	{
		bd.ByteWidth = sizeof(BASIC_VERTEX) * m_iMaxNumVertices;
		srd.pSysMem = m_basicVertices.get();
	}

	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	dev.CreateBuffer(&bd, &srd, &vertexbuffer);

	// set the shader objects as the active shaders
	devcon.VSSetShader(vertexshader.Get(), nullptr, 0);
	devcon.PSSetShader(pixelshader.Get(), nullptr, 0);

	devcon.IASetInputLayout(inputlayout.Get());

	// set the vertex buffer
	UINT stride;
	if (m_hasTextureCoordinates)
	{
		stride = sizeof(TEXTURE_VERTEX);
	}
	else if (m_hasColor)
	{
		stride = sizeof(COLOR_VERTEX);
	}
	else
	{
		stride = sizeof(BASIC_VERTEX);
	}
	UINT offset = 0;
	devcon.IASetVertexBuffers(0, 1, vertexbuffer.GetAddressOf(), &stride, &offset);

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
	devcon.UpdateSubresource(constantbuffer.Get(), 0, 0, &matFinal, 0, 0);
}