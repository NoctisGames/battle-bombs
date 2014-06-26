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

void DynamicGridGameObject::updateGrid()
{
    float boundsLeftX = m_bounds->getLowerLeft().getX();
    float boundsRightX = m_bounds->getLowerLeft().getX() + m_bounds->getWidth();
    float boundsBottomY = m_bounds->getLowerLeft().getY();
    float boundsTopY = m_bounds->getLowerLeft().getY() + m_bounds->getHeight();
    
    for (int i = 0; i < GRID_CELL_NUM_ROWS; i++)
    {
        for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
        {
            float leftX = GAME_X + GRID_CELL_WIDTH * j;
            float rightX = GAME_X + GRID_CELL_WIDTH * (j + 1);
            float bottomY = GAME_Y + GRID_CELL_HEIGHT * i;
            float topY = GAME_Y + GRID_CELL_HEIGHT * (i + 1);
            
            if (boundsLeftX > leftX && boundsRightX < rightX && boundsBottomY > bottomY && boundsTopY < topY)
            {
                m_gridX = j;
                m_gridY = i;
                return;
            }
        }
    }
}

int DynamicGridGameObject::getGridX()
{
    return m_gridX;
}

int DynamicGridGameObject::getGridY()
{
    return m_gridY;
}