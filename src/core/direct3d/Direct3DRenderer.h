//
//  Direct3DRenderer.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/1/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battle_bombs__Direct3DRenderer__
#define __battle_bombs__Direct3DRenderer__

#include "Renderer.h"
#include <d3d11_1.h>

class Direct3DRenderer : public Renderer
{
public:
	Direct3DRenderer(ID3D11Device1 *d3dDevice, ID3D11DeviceContext1 *d3dContext, ID3D11RenderTargetView *rendertarget);

	virtual void loadMapType(int mapType, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players);

	virtual void clearScreenWithColor(float r, float g, float b, float a);

	virtual void beginFrame();

	virtual void endFrame();

	virtual void cleanUp();
};

#endif /* defined(__battle_bombs__Direct3DRenderer__) */