//
//  Direct3DRenderer.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/1/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battle_bombs__Direct3DRenderer__
#define __battle_bombs__Direct3DRenderer__

#include "Renderer.h"

class Direct3DRenderer : public Renderer
{
public:
	Direct3DRenderer();

	virtual void loadMapType(int mapType, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players);

	virtual void clearScreenWithColor(float r, float g, float b, float a);

	virtual void beginFrame();

	virtual void endFrame();

	virtual void cleanUp();
};

#endif /* defined(__battle_bombs__Direct3DRenderer__) */