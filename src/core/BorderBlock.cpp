//
//  BorderBlock.cpp
//  bomberparty
//
//  Created by Stephen Gowen on 3/9/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "BorderBlock.h"
#include "Vector2D.h"
#include "Rectangle.h"

BorderBlock::BorderBlock(float x, float y, float width, float height, Border_Block_Type type) : GameObject(x, y, width, height, 0)
{
    m_type = type;
}

Border_Block_Type BorderBlock::getBorderBlockType()
{
    return m_type;
}
