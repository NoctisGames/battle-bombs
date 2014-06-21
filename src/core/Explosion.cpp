//
//  Explosion.cpp
//  bomberparty
//
//  Created by Stephen Gowen on 4/30/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "Explosion.h"
#include "OverlapTester.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "InsideBlock.h"
#include "BreakableBlock.h"
#include "Fire.h"

Explosion::Explosion(short power, int gridX, int gridY, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks, float width, float height) : GridGameObject(gridX, gridY, width, height, 0)
{
    m_fStateTime = 0;
    m_fExplosionTime = 0.4f + power * 0.1f;
    m_sPower = power;
    m_sPowerRemaining = power;
    m_sFrames = 1;
    m_isComplete = false;
    
    m_fireParts.push_back(std::unique_ptr<Fire>(new Fire(Fire_Type::CORE_PART_1, gridX, gridY, 0)));
    
    m_travelingRight = true;
    m_travelingUp = true;
    m_travelingLeft = true;
    m_travelingDown= true;
    
    m_iRightGridX= gridX + 1;
    m_iUpGridY = gridY + 1;
    m_iLeftGridX = gridX - 1;
    m_iDownGridY = gridY - 1;
    
    runBlockLogic(insideBlocks, breakableBlocks);
    
    m_fireParts.push_back(std::unique_ptr<Fire>(new Fire(m_travelingRight ? HEAD_PART_1 : EDGE_FROM_CORE_PART_1, m_iRightGridX, gridY, 0)));
    m_fireParts.push_back(std::unique_ptr<Fire>(new Fire(m_travelingUp ? HEAD_PART_1 : EDGE_FROM_CORE_PART_1, gridX, m_iUpGridY, 90)));
    m_fireParts.push_back(std::unique_ptr<Fire>(new Fire(m_travelingLeft ? HEAD_PART_1 : EDGE_FROM_CORE_PART_1, m_iLeftGridX, gridY, 180)));
    m_fireParts.push_back(std::unique_ptr<Fire>(new Fire(m_travelingDown ? HEAD_PART_1 : EDGE_FROM_CORE_PART_1, gridX, m_iDownGridY, 270)));
}

void Explosion::update(float deltaTime, std::vector<std::unique_ptr<InsideBlock >> &insideBlocks, std::vector<std::unique_ptr<BreakableBlock >> &breakableBlocks)
{
    m_fStateTime += deltaTime;
    m_fTravelTime += deltaTime;
    
    static const float TIME_TO_TRAVEL = 0.1f;
    if(m_fTravelTime > TIME_TO_TRAVEL)
    {
        m_sFrames++;
        m_fTravelTime = 0;
        
        for (std::vector<std::unique_ptr<Fire>>::iterator itr = m_fireParts.begin(); itr != m_fireParts.end(); )
        {
            (*itr)->nextFrame(m_sPower);
            
            if ((*itr)->isExhausted())
            {
                itr = m_fireParts.erase(itr);
            }
            else
            {
                itr++;
            }
        }
        
        if(m_sFrames >= 4)
        {
            m_sPowerRemaining--;
        }
        
        if(m_sPowerRemaining == 0 && m_sPower >= 2 && m_sFrames >= 7)
        {
            m_fireParts.clear();
            m_fireParts.push_back(std::unique_ptr<Fire>(new Fire(Fire_Type::CORE_END, m_gridX, m_gridY, 0)));
        }
        
        if((m_sFrames == 4 && m_sPower == 2) || (m_sFrames >= 5 && m_sPower >= 3))
        {
            m_iRightGridX++;
            m_iUpGridY++;
            m_iLeftGridX--;
            m_iDownGridY--;
            
            runBlockLogic(insideBlocks, breakableBlocks);
            
            Fire_Type travelingFireType = m_sFrames == 4 ? Fire_Type::HEAD_PART_4_POW_2 : Fire_Type::HEAD_PART_5_POW_2;
            Fire_Type blockingFireType = m_sFrames == 4 ? Fire_Type::EDGE_FROM_FAT_NECK : Fire_Type::EDGE_FROM_THIN_NECK;
            
            m_fireParts.push_back(std::unique_ptr<Fire>(new Fire(m_travelingRight ? travelingFireType : blockingFireType, m_iRightGridX, m_gridY, 0)));
            m_fireParts.push_back(std::unique_ptr<Fire>(new Fire(m_travelingUp ? travelingFireType : blockingFireType, m_gridX, m_iUpGridY, 90)));
            m_fireParts.push_back(std::unique_ptr<Fire>(new Fire(m_travelingLeft ? travelingFireType : blockingFireType, m_iLeftGridX, m_gridY, 180)));
            m_fireParts.push_back(std::unique_ptr<Fire>(new Fire(m_travelingDown ? travelingFireType : blockingFireType, m_gridX, m_iDownGridY, 270)));
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

float Explosion::isComplete()
{
    return m_isComplete;
}

// Private

void Explosion::runBlockLogic(std::vector<std::unique_ptr<InsideBlock> > &insideBlocks, std::vector<std::unique_ptr<BreakableBlock> > &breakableBlocks)
{
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
    
    for (std::vector < std::unique_ptr < BreakableBlock >> ::iterator itr = breakableBlocks.begin(); itr != breakableBlocks.end(); itr++)
    {
        if((*itr)->getGridX() == m_iRightGridX && (*itr)->getGridY() == m_gridY)
        {
            (*itr)->onDestroy();
        }
        else if((*itr)->getGridX() == m_gridX && (*itr)->getGridY() == m_iUpGridY)
        {
            (*itr)->onDestroy();
        }
        else if((*itr)->getGridX() == m_iLeftGridX && (*itr)->getGridY() == m_gridY)
        {
            (*itr)->onDestroy();
        }
        else if((*itr)->getGridX() == m_gridX && (*itr)->getGridY() == m_iDownGridY)
        {
            (*itr)->onDestroy();
        }
    }
}