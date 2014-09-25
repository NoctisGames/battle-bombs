//
//  Direct3DRectangleRenderer.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__Direct3DRectangleRenderer__
#define __battlebombs__Direct3DRectangleRenderer__

struct COLOR_VERTEX
{
	float X, Y, Z; // vertex position
	float R, G, B, A; // vertex color
};

#include <d3d11_1.h>
#include <memory>
#include <vector>
#include "Color.h"

class Rectangle;

class Direct3DRectangleRenderer
{
public:
	Direct3DRectangleRenderer(ID3D11Device1 *d3dDevice, ID3D11DeviceContext1 *d3dContext, bool isFill);

	void beginBatch();

	void endBatch();
    
    void renderRectangle(Rectangle &rectangle, Color &color);
    
	void renderRectangle(float x1, float y1, float x2, float y2, Color &color);
    
private:
    bool m_isFill;
	int m_iNumRectangles;

	void addVertexCoordinate(float x, float y, float z, float r, float g, float b, float a, float u, float v);

	void createIndexBuffer();

	std::vector<short> createIndexValues();
};

#endif /* defined(__battlebombs__Direct3DRectangleRenderer__) */
