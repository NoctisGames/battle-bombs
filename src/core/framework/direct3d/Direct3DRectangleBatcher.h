//
//  Direct3DRectangleBatcher.h
//  insectoid-defense
//
//  Created by Stephen Gowen on 9/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __insectoiddefense__Direct3DRectangleBatcher__
#define __insectoiddefense__Direct3DRectangleBatcher__

struct COLOR_VERTEX
{
	float X, Y, Z; // vertex position
	float R, G, B, A; // vertex color
};

#include "RectangleBatcher.h"
#include <d3d11_1.h>
#include <memory>
#include <vector>

class Direct3DRectangleBatcher : public RectangleBatcher
{
public:
	Direct3DRectangleBatcher(ID3D11Device1 *d3dDevice, ID3D11DeviceContext1 *d3dContext, bool isFill);

	virtual void beginBatch();

	virtual void endBatch();
    
    void renderRectangle(Rectangle &rectangle, Color &color);
    
	virtual void renderRectangle(float x1, float y1, float x2, float y2, Color &color);
    
private:
    void addVertexCoordinate(float x, float y, float z, float r, float g, float b, float a, float u, float v);

	void createIndexBuffer();

	std::vector<short> createIndexValues();
};

#endif /* defined(__insectoiddefense__Direct3DRectangleBatcher__) */
