//
//  Direct3DRectangleBatcher.h
//  gowengamedev-framework
//
//  Created by Stephen Gowen on 9/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __insectoiddefense__Direct3DRectangleBatcher__
#define __insectoiddefense__Direct3DRectangleBatcher__

#include "RectangleBatcher.h"
#include <d3d11_1.h>
#include <memory>
#include <vector>

class Direct3DRectangleBatcher : public RectangleBatcher
{
public:
	Direct3DRectangleBatcher(bool isFill);

	virtual void beginBatch();

	virtual void endBatch();
    
    void renderRectangle(Rectangle &rectangle, Color &color);
    
	virtual void renderRectangle(float x1, float y1, float x2, float y2, Color &color);
    
private:
    void addVertexCoordinate(float x, float y, float z, float r, float g, float b, float a, float u, float v);
};

#endif /* defined(__insectoiddefense__Direct3DRectangleBatcher__) */
