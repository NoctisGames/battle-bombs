//
//  Direct3DRectangleRenderer.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 9/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#define numPointsOnRectangle 6

#include "pch.h"
#include "Direct3DRectangleRenderer.h"
#include "Vertices2D.h"
#include "Rectangle.h"
#include "Vector2D.h"

Direct3DRectangleRenderer::Direct3DRectangleRenderer(ComPtr<ID3D11Device1> d3dDevice, ComPtr<ID3D11DeviceContext1> d3dContext, bool useColor, bool isFill)
{
	dev = d3dDevice;
	devcon = d3dContext;
	m_vertices = std::unique_ptr<Vertices2D>(new Vertices2D(d3dDevice, d3dContext, numPointsOnRectangle, false, useColor));
	m_iNumRectangles = 1;
	m_useColor = useColor;
    m_isFill = isFill;

	generateIndices(m_iNumRectangles);

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
}

void Direct3DRectangleRenderer::renderRectangle(Rectangle &rectangle, Color &color)
{
    float x1 = rectangle.getLowerLeft().getX();
    float y1 = rectangle.getLowerLeft().getY();
    float x2 = x1 + rectangle.getWidth();
    float y2 = y1 + rectangle.getHeight();
    
    renderRectangle(x1, y1, x2, y2, color);
}

void Direct3DRectangleRenderer::renderRectangle(float x1, float y1, float x2, float y2, Color &color)
{
    m_vertices->resetIndex();

	m_vertices->addVertexCoordinate(x1, y1, 0, color.red, color.green, color.blue, color.alpha, 0, 0);
	m_vertices->addVertexCoordinate(x1, y2, 0, color.red, color.green, color.blue, color.alpha, 0, 0);
	m_vertices->addVertexCoordinate(x2, y2, 0, color.red, color.green, color.blue, color.alpha, 0, 0);
	m_vertices->addVertexCoordinate(x2, y1, 0, color.red, color.green, color.blue, color.alpha, 0, 0);
    
	m_vertices->bind();

	// set the blend state
	devcon->OMSetBlendState(blendstate.Get(), 0, 0xffffffff);

	devcon->IASetIndexBuffer(indexbuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	// set the primitive topology
	devcon->IASetPrimitiveTopology(m_isFill ? D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST : D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	devcon->DrawIndexed(numPointsOnRectangle, 0, 0);
}

void Direct3DRectangleRenderer::generateIndices(int maxSprites)
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
	ibd.ByteWidth = sizeof(short)* numIndices;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA isrd = { m_indices.get(), 0, 0 };

	dev->CreateBuffer(&ibd, &isrd, &indexbuffer);
}
