//
//  RectangleBatcher.h
//  battlebombs
//
//  Created by Stephen Gowen on 9/25/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battlebombs__RectangleBatcher__
#define __battlebombs__RectangleBatcher__

#include "Color.h"

class Rectangle;

class RectangleBatcher
{
public:
    RectangleBatcher(bool isFill);
    
    virtual void beginBatch() = 0;
    
    virtual void endBatch() = 0;
    
    void renderRectangle(Rectangle &rectangle, Color &color);
    
    virtual void renderRectangle(float leftX, float bottomY, float rightX, float topY, Color &color) = 0;
    
protected:
    int m_iNumRectangles;
    bool m_isFill;
};

#endif /* defined(__battlebombs__RectangleBatcher__) */
