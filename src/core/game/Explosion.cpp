//
//  Explosion.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 4/30/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "Explosion.h"
#include "OverlapTester.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "InsideBlock.h"
#include "BreakableBlock.h"
#include "PlayerDynamicGameObject.h"
#include "Fire.h"

Explosion::Explosion(short power, int gridX, int gridY, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players, float width, float height) : GridGameObject(gridX, gridY, width, height, 0)
{
    m_fStateTime = 0;
    m_fTravelTime = 0;
    m_fExplosionTime = power == 2 ? 0.2f + power * 0.05f + 0.05f : 0.2f + power * 0.05f;
    m_sPower = power;
    m_sPowerRemaining = power - 1;
    m_sFrames = 1;
    m_isComplete = false;
    
    m_fireParts.push_back(std::unique_ptr<Fire>(new Fire(Fire_Type::CORE_PART_1, gridX, gridY, DIRECTION_RIGHT)));
    
    m_travelingRight = true;
    m_travelingUp = true;
    m_travelingLeft = true;
    m_travelingDown = true;
    m_destroyedBlockRight = false;
    m_destroyedBlockUp = false;
    m_destroyedBlockLeft = false;
    m_destroyedBlockDown = false;
    
    m_iRightGridX= gridX + 1;
    m_iUpGridY = gridY + 1;
    m_iLeftGridX = gridX - 1;
    m_iDownGridY = gridY - 1;
    
    runBlockLogic(insideBlocks, breakableBlocks, players);
    
    m_fireParts.push_back(std::unique_ptr<Fire>(new Fire(m_travelingRight ? HEAD_PART_1 : EDGE_FROM_CORE_PART_1, m_iRightGridX, gridY, DIRECTION_RIGHT)));
    m_fireParts.push_back(std::unique_ptr<Fire>(new Fire(m_travelingUp ? HEAD_PART_1 : EDGE_FROM_CORE_PART_1, gridX, m_iUpGridY, DIRECTION_UP)));
    m_fireParts.push_back(std::unique_ptr<Fire>(new Fire(m_travelingLeft ? HEAD_PART_1 : EDGE_FROM_CORE_PART_1, m_iLeftGridX, gridY, DIRECTION_LEFT)));
    m_fireParts.push_back(std::unique_ptr<Fire>(new Fire(m_travelingDown ? HEAD_PART_1 : EDGE_FROM_CORE_PART_1, gridX, m_iDownGridY, DIRECTION_DOWN)));
    
    runPostBlockLogic();
}

void Explosion::update(float deltaTime, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players)
{
    m_fStateTime += deltaTime;
    m_fTravelTime += deltaTime;
    
    static const float TIME_TO_TRAVEL = 0.05f;
    if(m_fTravelTime > TIME_TO_TRAVEL)
    {
        m_sFrames++;
        m_fTravelTime = 0;
        
        for (std::vector<std::unique_ptr<Fire>>::iterator itr = m_fireParts.begin(); itr != m_fireParts.end(); )
        {
            (*itr)->nextFrame(m_sPowerRemaining);
            
            if ((*itr)->isExhausted())
            {
                itr = m_fireParts.erase(itr);
            }
            else
            {
                itr++;
            }
        }
        
        if(m_sFrames >= 4 && m_sPowerRemaining > 0)
        {
            m_iRightGridX++;
            m_iUpGridY++;
            m_iLeftGridX--;
            m_iDownGridY--;
            
            bool canExpandRight = m_travelingRight;
            bool canExpandUp = m_travelingUp;
            bool canExpandLeft = m_travelingLeft;
            bool canExpandDown = m_travelingDown;
            
            runBlockLogic(insideBlocks, breakableBlocks, players);
            
            Fire_Type travelingFireType = m_sFrames == 4 ? Fire_Type::HEAD_PART_4_POW_2 : Fire_Type::HEAD_PART_5_POW_2;
            Fire_Type blockingFireType = m_sFrames == 4 ? Fire_Type::EDGE_FROM_FAT_NECK : Fire_Type::EDGE_FROM_THIN_NECK;
            
            if(canExpandRight)
            {
                m_fireParts.push_back(std::unique_ptr<Fire>(new Fire(m_travelingRight ? travelingFireType : blockingFireType, m_iRightGridX, m_gridY, DIRECTION_RIGHT)));
            }
            
            if(canExpandUp)
            {
                m_fireParts.push_back(std::unique_ptr<Fire>(new Fire(m_travelingUp ? travelingFireType : blockingFireType, m_gridX, m_iUpGridY, DIRECTION_UP)));
            }
            
            if(canExpandLeft)
            {
                m_fireParts.push_back(std::unique_ptr<Fire>(new Fire(m_travelingLeft ? travelingFireType : blockingFireType, m_iLeftGridX, m_gridY, DIRECTION_LEFT)));
            }
            
            if(canExpandDown)
            {
                m_fireParts.push_back(std::unique_ptr<Fire>(new Fire(m_travelingDown ? travelingFireType : blockingFireType, m_gridX, m_iDownGridY, DIRECTION_DOWN)));
            }
            
            runPostBlockLogic();
        }
        
        if(m_sFrames >= 4 && m_sPower >= 2 && m_sPowerRemaining > 0)
        {
            m_sPowerRemaining--;
        }
        
        if(m_sPowerRemaining == 0 && m_sPower >= 3 && m_sFrames >= 4 + m_sPower)
        {
            m_fireParts.clear();
            m_fireParts.push_back(std::unique_ptr<Fire>(new Fire(Fire_Type::CORE_END, m_gridX, m_gridY, DIRECTION_RIGHT)));
        }
    }
    
    if(m_fStateTime > m_fExplosionTime)
    {
        m_isComplete = true;
    }
}

std::vector<std::unique_ptr<Fire >> & Explosion::getFireParts()
{
    return m_fireParts;
}

float Explosion::getStateTime()
{
    return m_fStateTime;
}

short Explosion::getPower()
{
    return m_sPower;
}

bool Explosion::isComplete()
{
    return m_isComplete;
}

// Private

void Explosion::runBlockLogic(std::vector<std::unique_ptr<InsideBlock> > &insideBlocks, std::vector<std::unique_ptr<BreakableBlock> > &breakableBlocks, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players)
{
    for (std::vector < std::unique_ptr < BreakableBlock >> ::iterator itr = breakableBlocks.begin(); itr != breakableBlocks.end(); itr++)
    {
        if((*itr)->getBreakableBlockState() == Breakable_Block_State::BB_NORMAL)
        {
            if(m_travelingRight && (*itr)->getGridX() == m_iRightGridX && (*itr)->getGridY() == m_gridY)
            {
                (*itr)->onDestroy();
                m_destroyedBlockRight = true;
            }
            else if(m_travelingUp && (*itr)->getGridX() == m_gridX && (*itr)->getGridY() == m_iUpGridY)
            {
                (*itr)->onDestroy();
                m_destroyedBlockUp = true;
            }
            else if(m_travelingLeft && (*itr)->getGridX() == m_iLeftGridX && (*itr)->getGridY() == m_gridY)
            {
                (*itr)->onDestroy();
                m_destroyedBlockLeft = true;
            }
            else if(m_travelingDown && (*itr)->getGridX() == m_gridX && (*itr)->getGridY() == m_iDownGridY)
            {
                (*itr)->onDestroy();
                m_destroyedBlockDown = true;
            }
        }
    }
    
    for (std::vector < std::unique_ptr < InsideBlock >> ::iterator itr = insideBlocks.begin(); itr != insideBlocks.end(); itr++)
    {
        if((*itr)->getGridX() == m_iRightGridX && (*itr)->getGridY() == m_gridY)
        {
            m_travelingRight = false;
        }
        else if((*itr)->getGridX() == m_gridX && (*itr)->getGridY() == m_iUpGridY)
        {
            m_travelingUp = false;
        }
        else if((*itr)->getGridX() == m_iLeftGridX && (*itr)->getGridY() == m_gridY)
        {
            m_travelingLeft = false;
        }
        else if((*itr)->getGridX() == m_gridX && (*itr)->getGridY() == m_iDownGridY)
        {
            m_travelingDown = false;
        }
    }
    
    for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = players.begin(); itr != players.end(); itr++)
    {
        if((*itr)->getPlayerActionState() == SHIELD_RAISED)
        {
            if((*itr)->getGridX() == m_iRightGridX && (*itr)->getGridY() == m_gridY && (*itr)->getDirection() == DIRECTION_LEFT)
            {
                m_travelingRight = false;
            }
            else if((*itr)->getGridX() == m_gridX && (*itr)->getGridY() == m_iUpGridY && (*itr)->getDirection() == DIRECTION_DOWN)
            {
                m_travelingUp = false;
            }
            else if((*itr)->getGridX() == m_iLeftGridX && (*itr)->getGridY() == m_gridY && (*itr)->getDirection() == DIRECTION_RIGHT)
            {
                m_travelingLeft = false;
            }
            else if((*itr)->getGridX() == m_gridX && (*itr)->getGridY() == m_iDownGridY && (*itr)->getDirection() == DIRECTION_UP)
            {
                m_travelingDown = false;
            }
        }
    }
    
    if(m_iRightGridX >= NUM_GRID_CELLS_PER_ROW)
    {
        m_travelingRight = false;
    }
    
    if(m_iUpGridY >= GRID_CELL_NUM_ROWS)
    {
        m_travelingUp = false;
    }
    
    if(m_iLeftGridX < 0)
    {
        m_travelingLeft = false;
    }
    
    if(m_iDownGridY < 0)
    {
        m_travelingDown = false;
    }
    
    if(m_gridY <= 2 && m_iRightGridX >= NUM_GRID_CELLS_PER_ROW - 3)
    {
        m_travelingRight = false;
    }
    
    if(m_iDownGridY <= 2 && (m_gridX <= 2 || m_gridX >= NUM_GRID_CELLS_PER_ROW - 3))
    {
        m_travelingDown = false;
    }
    
    if(m_gridY <= 2 && m_iLeftGridX <= 2)
    {
        m_travelingLeft = false;
    }
}

void Explosion::runPostBlockLogic()
{
    if(m_destroyedBlockRight)
    {
        m_travelingRight = false;
    }
    if(m_destroyedBlockUp)
    {
        m_travelingUp = false;
    }
    if(m_destroyedBlockLeft)
    {
        m_travelingLeft = false;
    }
    if(m_destroyedBlockDown)
    {
        m_travelingDown = false;
    }
}