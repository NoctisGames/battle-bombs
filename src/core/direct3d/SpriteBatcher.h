//
//  SpriteBatcher.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battle_bombs__SpriteBatcher__
#define __battle_bombs__SpriteBatcher__

struct TEXTURE_VERTEX
{
	float X, Y, Z; // vertex position
	float R, G, B, A; // vertex color
	float U, V;    // texture coordinates
};

#include <d3d11_1.h>
#include <memory>
#include <vector>
#include "Color.h"

class TextureRegion;

using namespace Microsoft::WRL;

class SpriteBatcher
{
public:
	SpriteBatcher(ID3D11Device1 *d3dDevice, ID3D11DeviceContext1 *d3dContext);
    
    void beginBatch();

	void endBatchWithTexture(ID3D11ShaderResourceView *texture);
    
    void drawSprite(float x, float y, float width, float height, float angle, TextureRegion tr);
    
    void drawSprite(float x, float y, float width, float height, float angle, Color &color, TextureRegion tr);
    
private:
	std::vector<TEXTURE_VERTEX> m_textureVertices;
	int m_iNumSprites;
    
    void drawSprite(float x, float y, float width, float height, TextureRegion tr);
    
    void drawSprite(float x, float y, float width, float height, Color &color, TextureRegion tr);
    
	void addVertexCoordinate(float x, float y, float z, float r, float g, float b, float a, float u, float v);

	void createVertexBuffer();
	void createIndexBuffer();

	std::vector<short> createIndexValues();
};

#endif /* defined(__battle_bombs__SpriteBatcher__) */