//
//  SpriteBatcher.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 9/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "macros.h"
#include "SpriteBatcher.h"
#include <stdlib.h>
#include "Vertices2D.h"
#include "TextureRegion.h"
#include "Color.h"

SpriteBatcher::SpriteBatcher(ID3D11Device1 *d3dDevice, ID3D11DeviceContext1 *d3dContext, int maxSprites, bool useColors)
{
	dev = d3dDevice;
	devcon = d3dContext;
	m_vertices = std::unique_ptr<Vertices2D>(new Vertices2D(d3dDevice, d3dContext, maxSprites * 4, true, useColors));
    m_iNumSprites = 0;
    
    generateIndices(maxSprites);

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

	dev->CreateBlendState(&bd, &blendstate);

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

	dev->CreateSamplerState(&sd, &samplerstate);    // create the linear blur sampler
}

void SpriteBatcher::beginBatch()
{
    m_vertices->resetIndex();
    m_iNumSprites = 0;
}

void SpriteBatcher::endBatchWithTexture(ComPtr<ID3D11ShaderResourceView> texture)
{
    if(m_iNumSprites > 0)
    {
		m_vertices->bind();

		// set the blend state
		devcon->OMSetBlendState(blendstate.Get(), 0, 0xffffffff);

		devcon->IASetIndexBuffer(indexbuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

		// set the primitive topology
		devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// tell the GPU which texture to use
		devcon->PSSetShaderResources(0, 1, texture.GetAddressOf());

		// set the appropriate sampler state
		devcon->PSSetSamplers(0, 1, samplerstate.GetAddressOf());

		devcon->DrawIndexed(m_iNumSprites * 6, 0, 0);
    }
}

void SpriteBatcher::drawSprite(float x, float y, float width, float height, float angle, TextureRegion tr)
{
    if(angle > 0)
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

		m_vertices->addVertexCoordinate(x1, y1, 0, 1, 1, 1, 1, tr.u1, tr.v2);
		m_vertices->addVertexCoordinate(x4, y4, 0, 1, 1, 1, 1, tr.u1, tr.v1);
		m_vertices->addVertexCoordinate(x3, y3, 0, 1, 1, 1, 1, tr.u2, tr.v1);
		m_vertices->addVertexCoordinate(x2, y2, 0, 1, 1, 1, 1, tr.u2, tr.v2);
    }
    else
    {
        drawSprite(x, y, width, height, tr);
    }
    
    m_iNumSprites++;
}

void SpriteBatcher::drawSprite(float x, float y, float width, float height, float angle, Color &color, TextureRegion tr)
{
    if(angle > 0)
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

		m_vertices->addVertexCoordinate(x1, y1, 0, color.red, color.green, color.blue, color.alpha, tr.u1, tr.v2);
		m_vertices->addVertexCoordinate(x4, y4, 0, color.red, color.green, color.blue, color.alpha, tr.u1, tr.v1); 
		m_vertices->addVertexCoordinate(x3, y3, 0, color.red, color.green, color.blue, color.alpha, tr.u2, tr.v1); 
		m_vertices->addVertexCoordinate(x2, y2, 0, color.red, color.green, color.blue, color.alpha, tr.u2, tr.v2);
    }
    else
    {
        drawSprite(x, y, width, height, color, tr);
    }
    
    m_iNumSprites++;
}

#pragma <Private>

void SpriteBatcher::drawSprite(float x, float y, float width, float height, TextureRegion tr)
{
	float halfWidth = width / 2;
	float halfHeight = height / 2;
	float x1 = x - halfWidth;
	float y1 = y - halfHeight;
	float x2 = x + halfWidth;
	float y2 = y + halfHeight;

	m_vertices->addVertexCoordinate(x1, y1, 0, 1, 1, 1, 1, tr.u1, tr.v2);
	m_vertices->addVertexCoordinate(x1, y2, 0, 1, 1, 1, 1, tr.u1, tr.v1); 
	m_vertices->addVertexCoordinate(x2, y2, 0, 1, 1, 1, 1, tr.u2, tr.v1); 
	m_vertices->addVertexCoordinate(x2, y1, 0, 1, 1, 1, 1, tr.u2, tr.v2);
}

void SpriteBatcher::drawSprite(float x, float y, float width, float height, Color &color, TextureRegion tr)
{
	float halfWidth = width / 2;
	float halfHeight = height / 2;
	float x1 = x - halfWidth;
	float y1 = y - halfHeight;
	float x2 = x + halfWidth;
	float y2 = y + halfHeight;

	m_vertices->addVertexCoordinate(x1, y1, 0, color.red, color.green, color.blue, color.alpha, tr.u1, tr.v2);
	m_vertices->addVertexCoordinate(x1, y2, 0, color.red, color.green, color.blue, color.alpha, tr.u1, tr.v1);
	m_vertices->addVertexCoordinate(x2, y2, 0, color.red, color.green, color.blue, color.alpha, tr.u2, tr.v1);
	m_vertices->addVertexCoordinate(x2, y1, 0, color.red, color.green, color.blue, color.alpha, tr.u2, tr.v2);
}

void SpriteBatcher::generateIndices(int maxSprites)
{
    int numIndices = maxSprites * 6;
	m_indices = std::unique_ptr<short>(new short[numIndices]);
    
	short j = 0;
    
    for (int i = 0; i < numIndices; i += 6, j += 4)
    {
		m_indices.get()[i + 0] = j + 0;
		m_indices.get()[i + 1] = j + 1;
		m_indices.get()[i + 2] = j + 2;
		m_indices.get()[i + 3] = j + 2;
		m_indices.get()[i + 4] = j + 3;
		m_indices.get()[i + 5] = j + 0;
    }

	// create the index buffer
	D3D11_BUFFER_DESC ibd = { 0 };
	ibd.ByteWidth = sizeof(short) * numIndices;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA isrd = { m_indices.get(), 0, 0 };

	dev->CreateBuffer(&ibd, &isrd, &indexbuffer);
}