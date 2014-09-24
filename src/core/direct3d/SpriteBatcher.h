//
//  SpriteBatcher.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battle_bombs__SpriteBatcher__
#define __battle_bombs__SpriteBatcher__

#include <d3d11_1.h>
#include <memory>
#include <vector>
#include "Color.h"

class TextureRegion;
class Vertices2D;

using namespace Microsoft::WRL;

class SpriteBatcher
{
public:
	SpriteBatcher(ID3D11Device1 *d3dDevice, ID3D11DeviceContext1 *d3dContext, int maxSprites, bool useColors);
    
    void beginBatch();

	void endBatchWithTexture(ComPtr<ID3D11ShaderResourceView> texture);
    
    void drawSprite(float x, float y, float width, float height, float angle, TextureRegion tr);
    
    void drawSprite(float x, float y, float width, float height, float angle, Color &color, TextureRegion tr);
    
private:
	ID3D11Device1 *dev;                      // the device interface
	ID3D11DeviceContext1 *devcon;            // the device context interface
	ComPtr<ID3D11BlendState> blendstate;            // the blend state interface
	ComPtr<ID3D11SamplerState> samplerstate;        // the sampler state interfaces
	ComPtr<ID3D11Buffer> indexbuffer;               // the index buffer interface
	std::unique_ptr<Vertices2D> m_vertices;
	std::vector<short> m_indices;
	int m_iNumSprites;
    
    void drawSprite(float x, float y, float width, float height, TextureRegion tr);
    
    void drawSprite(float x, float y, float width, float height, Color &color, TextureRegion tr);
    
    void generateIndices(int maxSprites);
};

#endif /* defined(__battle_bombs__SpriteBatcher__) */