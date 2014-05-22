//
//  DynamicGridGameObject.cpp
//  bomberparty
//
//  Created by Stephen Gowen on 5/15/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "DynamicGridGameObject.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "GameConstants.h"

DynamicGridGameObject::DynamicGridGameObject(int gridX, int gridY, float width, float height, float angle) : DynamicGameObject(GAME_X + GRID_CELL_WIDTH * gridX + GRID_CELL_WIDTH / 2.0f, GAME_Y + GRID_CELL_HEIGHT * gridY + GRID_CELL_HEIGHT / 2.0f, width, height, angle)
{
    m_gridX = gridX;
    m_gridY = gridY;
}

int DynamicGridGameObject::getGridX()
{
    return m_gridX;
}

int DynamicGridGameObject::getGridY()
{
    return m_gridY;
}