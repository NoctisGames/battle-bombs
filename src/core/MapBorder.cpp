//
//  MapBorder.cpp
//  bomberparty
//
//  Created by Stephen Gowen on 6/23/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "MapBorder.h"
#include "Vector2D.h"
#include "Rectangle.h"

MapBorder::MapBorder(Map_Border_Type type, float x, float y, float width, float height) : GameObject(x, y, width, height, 0)
{
    m_type = type;

    switch (type)
    {
        case BORDER_TOP:
            m_bounds->setHeight(height * 6);
            break;
        case BORDER_LEFT:
            resetBounds(width * 6, height);
            break;
        case BORDER_RIGHT:
            m_bounds->setWidth(width * 6);
            break;
        case BORDER_BOTTOM:
            resetBounds(width, height * 6);
        default:
            break;
    }
}

Map_Border_Type MapBorder::getType()
{
    return m_type;
}

bool MapBorder::isNearFront()
{
    return m_type == BORDER_BOTTOM_LEFT || m_type == BORDER_BOTTOM_RIGHT || m_type == BORDER_BOTTOM;
}