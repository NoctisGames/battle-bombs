//
//  Direct3DSpriteBatcher.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battle_bombs__Direct3DSpriteBatcher__
#define __battle_bombs__Direct3DSpriteBatcher__

struct TEXTURE_VERTEX
{
	float X, Y, Z; // vertex position
	float R, G, B, A; // vertex color
	float U, V;    // texture coordinates
};

#include "SpriteBatcher.h"
#include <d3d11_1.h>
#include <vector>

using namespace Microsoft::WRL;

class Direct3DSpriteBatcher : public SpriteBatcher
{
public:
	Direct3DSpriteBatcher(ID3D11Device1 *d3dDevice, ID3D11DeviceContext1 *d3dContext);
    
	virtual void beginBatch();

	virtual void endBatchWithTexture(TextureWrapper &textureWrapper);
    
	virtual void drawSprite(float x, float y, float width, float height, float angle, TextureRegion tr);
    
	virtual void drawSprite(float x, float y, float width, float height, float angle, Color &color, TextureRegion tr);
    
protected:
	virtual void drawSprite(float x, float y, float width, float height, TextureRegion tr);

	virtual void drawSprite(float x, float y, float width, float height, Color &color, TextureRegion tr);

private:
    void addVertexCoordinate(float x, float y, float z, float r, float g, float b, float a, float u, float v);

	void createIndexBuffer();

	std::vector<short> createIndexValues();
};

#endif /* defined(__battle_bombs__Direct3DSpriteBatcher__) */