//
//  GameSession.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 3/21/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#include "GameSession.h"
#include "GameListener.h"
#include "Vector2D.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "OverlapTester.h"
#include "MapBorder.h"
#include "SpaceTile.h"
#include "InsideBlock.h"
#include "BreakableBlock.h"
#include "GameEvent.h"
#include "BombGameObject.h"
#include "Explosion.h"
#include "PowerUp.h"
#include "PlayerDynamicGameObject.h"
#include "Fire.h"
#include "PathFinder.h"
#include "Crater.h"
#include "FireBall.h"
#include "IceBall.h"
#include "IcePatch.h"
#include "FallingObjectShadow.h"

#include <iostream>

#ifdef TECHNE_GAMES_DIRECT_3D
#define strdup _strdup
#endif

GameSession::GameSession()
{
    m_gameState = WAITING;
    m_iMapType = MAP_SPACE;
    m_iNumBreakableBlocksAtSpawnTime = 0;
    m_isSuddenDeath = false;
}

int GameSession::getNumPlayers()
{
    return (int) m_players.size();
}

bool GameSession::isPlayerBotAtIndex(short playerIndex)
{
    return m_players.at(playerIndex).get()->isBot();
}

float GameSession::getPlayerXAtIndex(short playerIndex)
{
    return m_players.at(playerIndex).get()->getPosition().getX();
}

float GameSession::getPlayerYAtIndex(short playerIndex)
{
    return m_players.at(playerIndex).get()->getPosition().getY();
}

int GameSession::getPlayerDirectionAtIndex(short playerIndex)
{
    return m_players.at(playerIndex).get()->getDirection();
}

bool GameSession::isPlayerAliveAtIndex(short playerIndex)
{
    return m_players.at(playerIndex).get()->getPlayerState() == Player_State::ALIVE;
}

#pragma mark <Protected>

void GameSession::initializeInsideBlocksAndMapBordersForMapType(int mapType)
{
    m_iMapType = mapType;
    m_isSuddenDeath = false;
    
    m_mapBorders.clear();
    m_insideBlocks.clear();
    
    // BEGIN MAP BORDER FAR
    m_mapBorders.push_back(std::unique_ptr<MapBorder>(new MapBorder(BORDER_TOP, SCREEN_WIDTH / 2, WORLD_HEIGHT - 1.45522388023028f, SCREEN_WIDTH, 2.91044776046056f)));
    m_mapBorders.push_back(std::unique_ptr<MapBorder>(new MapBorder(BORDER_LEFT, 0.7388059701492f, 15.76119403157902f, 1.4776119402984f, 20.0597014947369f)));
    m_mapBorders.push_back(std::unique_ptr<MapBorder>(new MapBorder(BORDER_RIGHT, 23.44029850746264f, 15.76119403157902f, 1.11940298507472f, 20.0597014947369f)));
    m_mapBorders.push_back(std::unique_ptr<MapBorder>(new MapBorder(BORDER_BOTTOM_LEFT, 2.865671641791f, 3.58208955263162f, 5.731343283582f, 4.29850746315789f)));
    m_mapBorders.push_back(std::unique_ptr<MapBorder>(new MapBorder(BORDER_BOTTOM_RIGHT, 21.31343283582084f, 3.58208955263162f, 5.37313432835832f, 4.29850746315789f)));
    m_mapBorders.push_back(std::unique_ptr<MapBorder>(new MapBorder(BORDER_BOTTOM, SCREEN_WIDTH / 2, 0.71641791052634f, SCREEN_WIDTH, 1.43283582105267f)));
    
    if(mapType == MAP_MOUNTAINS)
    {
        m_mapBorders.push_back(std::unique_ptr<MapBorder>(new MapBorder(MOUNTAINS_DOOR, GAME_X + GRID_CELL_WIDTH * 7 + GRID_CELL_WIDTH / 2.0f, GAME_Y + GRID_CELL_HEIGHT * GRID_CELL_NUM_ROWS + GRID_CELL_HEIGHT / 2, GRID_CELL_WIDTH * 3, GRID_CELL_HEIGHT * 3)));
    }
    else if(mapType == MAP_SPACE)
    {
        // BEGIN TOP
        int fallingIndex = 118;
        for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
        {
            int tempFallingIndex = j < 7 ? fallingIndex + j : j > 7 ? fallingIndex + (14 - j) : fallingIndex + 7;
            m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, GRID_CELL_NUM_ROWS - 1, tempFallingIndex, m_gameListener.get())));
        }
        
        fallingIndex = 114;
        for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j += 2)
        {
            int tempFallingIndex = j < 7 ? fallingIndex + (j / 2) : fallingIndex + (7 - j / 2);
            m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, GRID_CELL_NUM_ROWS - 2, tempFallingIndex, m_gameListener.get())));
        }
        
        fallingIndex = 106;
        for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
        {
            int tempFallingIndex = j < 7 ? fallingIndex + j : j > 7 ? fallingIndex + (14 - j) : fallingIndex + 7;
            m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, GRID_CELL_NUM_ROWS - 3, tempFallingIndex, m_gameListener.get())));
        }
        
        fallingIndex = 102;
        for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j += 2)
        {
            int tempFallingIndex = j < 7 ? fallingIndex + (j / 2) : fallingIndex + (7 - j / 2);
            m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, GRID_CELL_NUM_ROWS - 4, tempFallingIndex, m_gameListener.get())));
        }
        
        fallingIndex = 94;
        for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
        {
            int tempFallingIndex = j < 7 ? fallingIndex + j : j > 7 ? fallingIndex + (14 - j) : fallingIndex + 7;
            m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, GRID_CELL_NUM_ROWS - 5, tempFallingIndex, m_gameListener.get())));
        }
        
        fallingIndex = 90;
        for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j += 2)
        {
            int tempFallingIndex = j < 7 ? fallingIndex + (j / 2) : fallingIndex + (7 - j / 2);
            m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, GRID_CELL_NUM_ROWS - 6, tempFallingIndex, m_gameListener.get())));
        }
        
        fallingIndex = 82;
        for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
        {
            int tempFallingIndex = j < 7 ? fallingIndex + j : j > 7 ? fallingIndex + (14 - j) : fallingIndex + 7;
            m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, GRID_CELL_NUM_ROWS - 7, tempFallingIndex, m_gameListener.get())));
        }
        
        // BEGIN MIDDLE
        fallingIndex = 80;
        for (int j = 2; j < (NUM_GRID_CELLS_PER_ROW - 2); j += 2)
        {
            m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, TOP_HALF_BOTTOM_GRID_Y - 1, fallingIndex + 1, m_gameListener.get())));
            m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, BOTTOM_HALF_TOP_GRID_Y + 1, fallingIndex, m_gameListener.get())));
        }
        
        // BEGIN BOTTOM
        fallingIndex = 79;
        for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
        {
            m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, BOTTOM_HALF_TOP_GRID_Y, fallingIndex--, m_gameListener.get())));
        }
        
        fallingIndex = 57;
        for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j += 2)
        {
            m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, BOTTOM_HALF_TOP_GRID_Y - 1, fallingIndex++, m_gameListener.get())));
        }
        
        fallingIndex = 56;
        for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
        {
            m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, BOTTOM_HALF_TOP_GRID_Y - 2, fallingIndex--, m_gameListener.get())));
        }
        
        fallingIndex = 35;
        for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j += 2)
        {
            m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, BOTTOM_HALF_TOP_GRID_Y - 3, fallingIndex++, m_gameListener.get())));
        }
        
        fallingIndex = 34;
        for (int j = 0; j < NUM_GRID_CELLS_PER_ROW; j++)
        {
            m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(j, BOTTOM_HALF_TOP_GRID_Y - 4, fallingIndex--, m_gameListener.get())));
        }
        
        // BEGIN BOTTOM CENTER
        m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(3, 2, 15, m_gameListener.get())));
        m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(5, 2, 16, m_gameListener.get())));
        m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(7, 2, 17, m_gameListener.get())));
        m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(9, 2, 18, m_gameListener.get())));
        m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(11, 2, 19, m_gameListener.get())));
        
        m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(3, 1, 14, m_gameListener.get())));
        m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(4, 1, 13, m_gameListener.get())));
        m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(5, 1, 12, m_gameListener.get())));
        m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(6, 1, 11, m_gameListener.get())));
        m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(7, 1, 10, m_gameListener.get())));
        m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(8, 1, 9, m_gameListener.get())));
        m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(9, 1, 8, m_gameListener.get())));
        m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(10, 1, 7, m_gameListener.get())));
        m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(11, 1, 6, m_gameListener.get())));
        
        m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(3, 0, 0, m_gameListener.get())));
        m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(4, 0, 1, m_gameListener.get())));
        m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(6, 0, 2, m_gameListener.get())));
        m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(8, 0, 3, m_gameListener.get())));
        m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(10, 0, 4, m_gameListener.get())));
        m_spaceTiles.push_back(std::unique_ptr<SpaceTile>(new SpaceTile(11, 0, 5, m_gameListener.get())));
    }
    
    // BEGIN BOTTOM CENTER
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock( 5, 0)));
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock( 7, 0)));
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock( 9, 0)));
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock( 4, 2)));
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock( 6, 2)));
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock( 8, 2)));
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(10, 2)));
    
    // BEGIN BOTTOM
    for (int i = 4; i < BOTTOM_HALF_TOP_GRID_Y; i += 2)
    {
        for (int j = 1; j < NUM_GRID_CELLS_PER_ROW; j += 2)
        {
            m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(j, i)));
        }
    }
    
    // BEGIN MIDDLE
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(0, BOTTOM_HALF_TOP_GRID_Y + 1)));
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(0, TOP_HALF_BOTTOM_GRID_Y - 1)));
    
    for (int j = 1; j < NUM_GRID_CELLS_PER_ROW; j += 2)
    {
        m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(j, BOTTOM_HALF_TOP_GRID_Y + 1)));
        m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(j, TOP_HALF_BOTTOM_GRID_Y - 1)));
    }
    
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(NUM_GRID_CELLS_PER_ROW - 1, BOTTOM_HALF_TOP_GRID_Y + 1)));
    m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(NUM_GRID_CELLS_PER_ROW - 1, TOP_HALF_BOTTOM_GRID_Y - 1)));
    
    // BEGIN TOP
    for (int i = TOP_HALF_BOTTOM_GRID_Y + 1; i < GRID_CELL_NUM_ROWS; i += 2)
    {
        for (int j = 1; j < NUM_GRID_CELLS_PER_ROW; j += 2)
        {
            if(mapType == MAP_MOUNTAINS)
            {
                if(i >= GRID_CELL_NUM_ROWS - 2 && j >= 5 && j <= 9)
                {
                    continue;
                }
            }
            
            m_insideBlocks.push_back(std::unique_ptr<InsideBlock>(new InsideBlock(j, i)));
        }
    }
}

void GameSession::updateCommon(float deltaTime)
{
    for (std::vector < std::unique_ptr < BombGameObject >> ::iterator itr = m_bombs.begin(); itr != m_bombs.end();)
    {
        (**itr).update(deltaTime, m_explosions, m_mapBorders, m_insideBlocks, m_breakableBlocks, m_players, m_bombs);

        if ((**itr).isDestroyed())
        {
            itr = m_bombs.erase(itr);
        }
        else
        {
            itr++;
        }
    }
    
    for (std::vector < std::unique_ptr < InsideBlock >> ::iterator itr = m_insideBlocks.begin(); itr != m_insideBlocks.end(); )
    {
        (**itr).update(deltaTime);
        
        if((*itr)->getInsideBlockState() == IB_GONE)
        {
            itr = m_insideBlocks.erase(itr);
        }
        else
        {
            itr++;
        }
    }

    for (std::vector < std::unique_ptr < BreakableBlock >> ::iterator itr = m_breakableBlocks.begin(); itr != m_breakableBlocks.end(); )
    {
        (**itr).update(deltaTime);
        
        if ((**itr).getBreakableBlockState() == DESTROYED)
        {
            if ((**itr).hasPowerUp())
            {
                m_powerUps.push_back(std::unique_ptr<PowerUp>(new PowerUp((**itr).getGridX(), (**itr).getGridY(), (**itr).getPowerUpFlag())));
            }
            
            if (!(**itr).wasHitByFireBall())
            {
                // Only free the cell for traversal if the breakable block wasn't destroyed by a fire ball
                // This is necessary because in this case, a crater is in its place
                PathFinder::getInstance().freeGameGridCell((*itr)->getGridX(), (*itr)->getGridY());
                
                onBreakableBlockDestroyed(**itr);
            }

            itr = m_breakableBlocks.erase(itr);
        }
        else
        {
            itr++;
        }
    }

    for (std::vector < std::unique_ptr < Explosion >> ::iterator itr = m_explosions.begin(); itr != m_explosions.end();)
    {
        (**itr).update(deltaTime, m_insideBlocks, m_breakableBlocks, m_players);

        if ((**itr).isComplete())
        {
            itr = m_explosions.erase(itr);
        }
        else
        {
            itr++;
        }
    }

    for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = m_players.begin(); itr != m_players.end(); itr++)
    {
        (**itr).update(deltaTime, m_mapBorders, m_spaceTiles, m_insideBlocks, m_breakableBlocks, m_craters, m_powerUps, m_explosions, m_players, m_bombs);
    }

    for (std::vector < std::unique_ptr < PowerUp >> ::iterator itr = m_powerUps.begin(); itr != m_powerUps.end();)
    {
        (**itr).update(deltaTime);
        
        if ((**itr).isPickedUp())
        {
            onPowerUpPickedUp(**itr);
            
            itr = m_powerUps.erase(itr);
        }
        else
        {
            itr++;
        }
    }
    
    if(m_isSuddenDeath)
    {
        switch (m_iMapType)
        {
            case MAP_SPACE:
                for (std::vector < std::unique_ptr < SpaceTile >> ::iterator itr = m_spaceTiles.begin(); itr != m_spaceTiles.end(); itr++)
                {
                    (*itr)->update(deltaTime, m_isSuddenDeath, m_players, m_bombs, m_insideBlocks, m_breakableBlocks, m_powerUps);
                }
                break;
            case MAP_GRASSLANDS:
                for (std::vector < std::unique_ptr < FireBall >> ::iterator itr = m_fireBalls.begin(); itr != m_fireBalls.end(); )
                {
                    (*itr)->update(deltaTime, m_breakableBlocks);
                    
                    bool continueIteration = true;
                    
                    if ((*itr)->isDescending() && (*itr)->isTargetReached())
                    {
                        if((*itr)->getShadow().isTargetOccupiedByInsideBlock())
                        {
                            InsideBlock *insideBlock = (*itr)->getShadow().getTargetInsideBlock();
                            insideBlock->onDestroy();
                        }
                        else if((*itr)->getShadow().isTargetOccupiedByBreakableBlock())
                        {
                            BreakableBlock *breakableBlock = (*itr)->getShadow().getTargetBreakableBlock();
                            breakableBlock->onHitByFireBall();
                        }
                        
                        m_craters.push_back(std::unique_ptr<Crater>(new Crater((*itr)->getGridX(), (*itr)->getGridY())));
                    }
                    
                    if((*itr)->isExplosionCompleted())
                    {
                        itr = m_fireBalls.erase(itr);
                        continueIteration = false;
                    }
                    
                    if (continueIteration)
                    {
                        itr++;
                    }
                }
                break;
            case MAP_MOUNTAINS:
                for (std::vector < std::unique_ptr < IceBall >> ::iterator itr = m_iceBalls.begin(); itr != m_iceBalls.end(); )
                {
                    (*itr)->update(deltaTime, m_breakableBlocks);
                    
                    if ((*itr)->isTargetReached())
                    {
                        if((*itr)->getShadow().isTargetOccupiedByInsideBlock())
                        {
                            InsideBlock *insideBlock = (*itr)->getShadow().getTargetInsideBlock();
                            insideBlock->onHitByIceBall();
                        }
                        else if((*itr)->getShadow().isTargetOccupiedByBreakableBlock())
                        {
                            BreakableBlock *breakableBlock = (*itr)->getShadow().getTargetBreakableBlock();
                            for (std::vector < std::unique_ptr < BreakableBlock >> ::iterator itr = m_breakableBlocks.begin(); itr != m_breakableBlocks.end(); itr++)
                            {
                                if(breakableBlock == (*itr).get())
                                {
                                    // Test to see if breakable block still exists
                                    breakableBlock->onHitByIceBall();
                                    break;
                                }
                            }
                        }
                        else
                        {
                            m_icePatches.push_back(std::unique_ptr<IcePatch>(new IcePatch((*itr)->getGridX(), (*itr)->getGridY())));
                        }
                        
                        PathFinder::getInstance().occupyGameGridCell((*itr)->getGridX(), (*itr)->getGridY());
                        
                        itr = m_iceBalls.erase(itr);
                    }
                    else
                    {
                        itr++;
                    }
                }
                
                for (std::vector < std::unique_ptr < IcePatch >> ::iterator itr = m_icePatches.begin(); itr != m_icePatches.end(); itr++)
                {
                    (**itr).update(deltaTime, m_bombs);
                }
                break;
            case MAP_BASE:
                // TODO
                break;
        }
    }
}

void GameSession::updateBots()
{
    for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = m_players.begin(); itr != m_players.end(); itr++)
    {
        if ((*itr)->isBot())
        {
            (*itr)->handlePowerUps(m_powerUps);
            
            if ((*itr)->isHitByExplosion(m_explosions, m_bombs))
            {
                m_gameListener->addLocalEventForPlayer(PLAYER_DEATH, (**itr));
            }
            
            switch(m_iMapType)
            {
                case MAP_SPACE:
                    if((*itr)->isTrappedOnFallingSpaceTile(m_spaceTiles))
                    {
                        m_gameListener->addLocalEventForPlayer(PLAYER_ABOUT_TO_FALL, (**itr));
                    }
                    else if((*itr)->isFallingDueToSpaceTile(m_spaceTiles))
                    {
                        m_gameListener->addLocalEventForPlayer(PLAYER_FALL, (**itr));
                    }
                    break;
                case MAP_GRASSLANDS:
                    if((*itr)->isHitByFireBall(m_craters))
                    {
                        m_gameListener->addLocalEventForPlayer(PLAYER_DEATH, (**itr));
                    }
                    break;
                case MAP_MOUNTAINS:
                    if((*itr)->isHitByIce(m_icePatches))
                    {
                        m_gameListener->addLocalEventForPlayer(PLAYER_FREEZE, (**itr));
                    }
                    break;
            }
        }
    }
}

void GameSession::clientUpdate(rapidjson::Document &d, bool isBeginGame)
{
    static const char *playerIndex0Key = "playerIndex0";
    static const char *playerIndex1Key = "playerIndex1";
    static const char *playerIndex2Key = "playerIndex2";
    static const char *playerIndex3Key = "playerIndex3";
    static const char *playerIndex4Key = "playerIndex4";
    static const char *playerIndex5Key = "playerIndex5";
    static const char *playerIndex6Key = "playerIndex6";
    static const char *playerIndex7Key = "playerIndex7";
    
    static const char *playerIndex0IsBotKey = "playerIndex0IsBot";
    static const char *playerIndex1IsBotKey = "playerIndex1IsBot";
    static const char *playerIndex2IsBotKey = "playerIndex2IsBot";
    static const char *playerIndex3IsBotKey = "playerIndex3IsBot";
    static const char *playerIndex4IsBotKey = "playerIndex4IsBot";
    static const char *playerIndex5IsBotKey = "playerIndex5IsBot";
    static const char *playerIndex6IsBotKey = "playerIndex6IsBot";
    static const char *playerIndex7IsBotKey = "playerIndex7IsBot";

    static const char *playerIndex0XKey = "playerIndex0X";
    static const char *playerIndex1XKey = "playerIndex1X";
    static const char *playerIndex2XKey = "playerIndex2X";
    static const char *playerIndex3XKey = "playerIndex3X";
    static const char *playerIndex4XKey = "playerIndex4X";
    static const char *playerIndex5XKey = "playerIndex5X";
    static const char *playerIndex6XKey = "playerIndex6X";
    static const char *playerIndex7XKey = "playerIndex7X";

    static const char *playerIndex0YKey = "playerIndex0Y";
    static const char *playerIndex1YKey = "playerIndex1Y";
    static const char *playerIndex2YKey = "playerIndex2Y";
    static const char *playerIndex3YKey = "playerIndex3Y";
    static const char *playerIndex4YKey = "playerIndex4Y";
    static const char *playerIndex5YKey = "playerIndex5Y";
    static const char *playerIndex6YKey = "playerIndex6Y";
    static const char *playerIndex7YKey = "playerIndex7Y";

    static const char *playerIndex0DirectionKey = "playerIndex0Direction";
    static const char *playerIndex1DirectionKey = "playerIndex1Direction";
    static const char *playerIndex2DirectionKey = "playerIndex2Direction";
    static const char *playerIndex3DirectionKey = "playerIndex3Direction";
    static const char *playerIndex4DirectionKey = "playerIndex4Direction";
    static const char *playerIndex5DirectionKey = "playerIndex5Direction";
    static const char *playerIndex6DirectionKey = "playerIndex6Direction";
    static const char *playerIndex7DirectionKey = "playerIndex7Direction";
    
    static const char *playerIndex0AliveKey = "playerIndex0Alive";
    static const char *playerIndex1AliveKey = "playerIndex1Alive";
    static const char *playerIndex2AliveKey = "playerIndex2Alive";
    static const char *playerIndex3AliveKey = "playerIndex3Alive";
    static const char *playerIndex4AliveKey = "playerIndex4Alive";
    static const char *playerIndex5AliveKey = "playerIndex5Alive";
    static const char *playerIndex6AliveKey = "playerIndex6Alive";
    static const char *playerIndex7AliveKey = "playerIndex7Alive";

    clientUpdateForPlayerIndex(d, playerIndex0Key, playerIndex0IsBotKey, playerIndex0XKey, playerIndex0YKey, playerIndex0DirectionKey, playerIndex0AliveKey, 0, isBeginGame);
    clientUpdateForPlayerIndex(d, playerIndex1Key, playerIndex1IsBotKey, playerIndex1XKey, playerIndex1YKey, playerIndex1DirectionKey, playerIndex1AliveKey, 1, isBeginGame);
    clientUpdateForPlayerIndex(d, playerIndex2Key, playerIndex2IsBotKey, playerIndex2XKey, playerIndex2YKey, playerIndex2DirectionKey, playerIndex2AliveKey, 2, isBeginGame);
    clientUpdateForPlayerIndex(d, playerIndex3Key, playerIndex3IsBotKey, playerIndex3XKey, playerIndex3YKey, playerIndex3DirectionKey, playerIndex3AliveKey, 3, isBeginGame);
    clientUpdateForPlayerIndex(d, playerIndex4Key, playerIndex4IsBotKey, playerIndex4XKey, playerIndex4YKey, playerIndex4DirectionKey, playerIndex4AliveKey, 4, isBeginGame);
    clientUpdateForPlayerIndex(d, playerIndex5Key, playerIndex5IsBotKey, playerIndex5XKey, playerIndex5YKey, playerIndex5DirectionKey, playerIndex5AliveKey, 5, isBeginGame);
    clientUpdateForPlayerIndex(d, playerIndex6Key, playerIndex6IsBotKey, playerIndex6XKey, playerIndex6YKey, playerIndex6DirectionKey, playerIndex6AliveKey, 6, isBeginGame);
    clientUpdateForPlayerIndex(d, playerIndex7Key, playerIndex7IsBotKey, playerIndex7XKey, playerIndex7YKey, playerIndex7DirectionKey, playerIndex7AliveKey, 7, isBeginGame);
    
    handleClientEventsArrayInDocument(d);
}

void GameSession::hardUpdate(rapidjson::Document &d)
{
    static const char *numDeletedBreakableBlocksKey = "numDeletedBreakableBlocks";
    
    if(d.HasMember(numDeletedBreakableBlocksKey))
    {
        static const char *deletedBreakableBlockXValuesKey = "deletedBreakableBlockXValues";
        static const char *deletedBreakableBlockYValuesKey = "deletedBreakableBlockYValues";
        
        std::vector<int> deletedBreakableBlockXValues;
        std::vector<int> deletedBreakableBlockYValues;
        
        handleIntArrayInDocument(d, deletedBreakableBlockXValuesKey, deletedBreakableBlockXValues, -1);
        handleIntArrayInDocument(d, deletedBreakableBlockYValuesKey, deletedBreakableBlockYValues, -1);
        
        int numDeletedBreakableBlocks = d[numDeletedBreakableBlocksKey].GetInt();
        for(int i = 0; i < numDeletedBreakableBlocks; i++)
        {
            for (std::vector < std::unique_ptr < BreakableBlock >> ::iterator itr = m_breakableBlocks.begin(); itr != m_breakableBlocks.end(); )
            {
                bool isMatch = (*itr)->getGridX() == deletedBreakableBlockXValues.at(i) && (*itr)->getGridY() == deletedBreakableBlockYValues.at(i);
                if (isMatch && (**itr).getBreakableBlockState() == BB_NORMAL)
                {
                    if ((**itr).hasPowerUp())
                    {
                        m_powerUps.push_back(std::unique_ptr<PowerUp>(new PowerUp((**itr).getGridX(), (**itr).getGridY(), (**itr).getPowerUpFlag())));
                    }
                    
                    PathFinder::getInstance().freeGameGridCell((*itr)->getGridX(), (*itr)->getGridY());
                    
                    itr = m_breakableBlocks.erase(itr);
                }
                else
                {
                    itr++;
                }
            }
        }
    }
    
    static const char *numDeletedPowerUpsKey = "numDeletedPowerUps";
    
    if(d.HasMember(numDeletedPowerUpsKey))
    {
        static const char *deletedPowerUpsXValuesKey = "deletedPowerUpsXValues";
        static const char *deletedPowerUpsYValuesKey = "deletedPowerUpsYValues";
        
        std::vector<int> deletedPowerUpsXValues;
        std::vector<int> deletedPowerUpsYValues;
        
        handleIntArrayInDocument(d, deletedPowerUpsXValuesKey, deletedPowerUpsXValues, -1);
        handleIntArrayInDocument(d, deletedPowerUpsYValuesKey, deletedPowerUpsYValues, -1);
        
        int numDeletedPowerUps = d[numDeletedPowerUpsKey].GetInt();
        for(int i = 0; i < numDeletedPowerUps; i++)
        {
            for (std::vector < std::unique_ptr < PowerUp >> ::iterator itr = m_powerUps.begin(); itr != m_powerUps.end();)
            {
                bool isMatch = (*itr)->getGridX() == deletedPowerUpsXValues.at(i) && (*itr)->getGridY() == deletedPowerUpsYValues.at(i);
                
                if (isMatch)
                {
                    itr = m_powerUps.erase(itr);
                }
                else
                {
                    itr++;
                }
            }
        }
    }
}

void GameSession::suddenDeath()
{
    m_isSuddenDeath = true;
    
    if (m_iMapType == MAP_MOUNTAINS)
    {
        m_iceBalls.clear();
        
        static const char *iceBallXValuesChar = "0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,14,14,14,14,14,14,14,14,14,14,14,14,14,11,10,9,8,7,6,5,4,3,0,0,0,0,0,0,0,0,0,0,0,0,0,1,2,3,4,5,6,7,8,9,10,11,12,13,13,13,13,13,13,13,13,13,13,13,13,13,11,10,9,8,7,6,5,4,3,1,1,1,1,1,1,1,1,1,1,1,1,2,3,4,5,6,7,8,9,10,11,12,12,12,12,12,12,12,12,12,12,12,12,11,10,9,8,7,6,5,4,3,2,2,2,2,2,2,2,2,2,2,2,3,4,5,6,7,8,9,10,11,11,11,11,11,11,11,11,11,11,11,10,9,8,7,6,5,4,3,3,3,3,3,3,3,3,3,3,4,5,6,7,8,9,10,10,10,10,10,10,10,10,10,9,8,7,6,5,4,4,4,4,4,4,4,4,5,6,7,8,9,9,9,9,9,9,9,8,7,6,5,5,5,5,5,5,6,7,8,8,8,8,8,7,6,6,6,6,7,7,7,7,-1";
        static const char *iceBallYValuesChar = "16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,15,14,13,12,11,10,9,8,7,6,5,4,3,0,0,0,0,0,0,0,0,0,3,4,5,6,7,8,9,10,11,12,13,14,15,15,15,15,15,15,15,15,15,15,15,15,15,15,14,13,12,11,10,9,8,7,6,5,4,3,1,1,1,1,1,1,1,1,1,3,4,5,6,7,8,9,10,11,12,13,14,14,14,14,14,14,14,14,14,14,14,14,13,12,11,10,9,8,7,6,5,4,3,2,2,2,2,2,2,2,2,2,3,4,5,6,7,8,9,10,11,12,13,13,13,13,13,13,13,13,13,13,12,11,10,9,8,7,6,5,4,3,3,3,3,3,3,3,3,3,4,5,6,7,8,9,10,11,12,12,12,12,12,12,12,12,11,10,9,8,7,6,5,4,4,4,4,4,4,4,5,6,7,8,9,10,11,11,11,11,11,11,10,9,8,7,6,5,5,5,5,5,6,7,8,9,10,10,10,10,9,8,7,6,6,6,7,8,9,9,8,7,8,-1";
        
        std::vector<int> iceBallXValues;
        std::vector<int> iceBallYValues;
        
        readCharArrayIntoIntArray(iceBallXValuesChar, iceBallXValues, -1);
        readCharArrayIntoIntArray(iceBallYValuesChar, iceBallYValues, -1);
        
        static int numIceBalls = 238;
        for(int i = 0; i < numIceBalls; i++)
        {
            m_iceBalls.push_back(std::unique_ptr<IceBall>(new IceBall(iceBallXValues.at(i), iceBallYValues.at(i), i, m_gameListener.get(), m_insideBlocks, m_breakableBlocks)));
        }
    }
    else if(m_iMapType == MAP_GRASSLANDS)
    {
        m_fireBalls.clear();
        
        static const char *fireBallXValuesChar1 = "4,1,12,10,9,7,5,4,13,14,12,13,8,8,8,0,10,12,13,1,10,3,14,9,0,6,6,9,7,8,13,4,2,0,1,9,1,0,7,2,13,2,3,2,11,10,13,13,10,6,0,11,8,2,7,3,14,12,11,2,5,1,12,6,9,7,7,13,6,5,9,10,2,5,12,10,9,13,4,6,8,5,5,6,7,0,9,7,10,13,13,10,1,0,7,8,6,4,11,5,5,11,8,12,9,7,0,7,4,8,2,7,11,8,12,13,1,11,2,6,11,0,3,0,14,3,5,8,3,1,14,5,14,4,3,6,11,4,5,9,12,4,11,4,3,10,12,11,14,9,11,7,7,13,11,1,3,11,12,0,6,9,8,6,0,6,4,1,14,2,10,5,2,9,3,3,7,10,14,4,11,9,1,5,13,4,3,8,10,11,10,12,3,4,9,14,10,1,5,8,3,9,7,10,11,8,3,0,14,4,7,3,2,14,6,8,1,12,10,9,2,4,14,2,12,0,3,6,5,14,6,8,4,5,1,5,6,-1";
        static const char *fireBallYValuesChar1 = "16,16,5,1,12,8,5,0,12,14,12,11,3,0,10,5,15,3,14,4,13,2,16,6,4,7,13,10,1,15,13,14,4,11,3,2,8,13,11,7,15,3,4,16,14,7,8,16,6,11,8,4,4,8,6,8,5,4,7,14,4,10,7,0,11,13,16,5,16,12,16,9,13,13,8,12,13,7,15,8,12,15,16,9,3,6,0,15,11,9,6,14,6,9,7,5,6,9,16,1,0,10,8,13,5,5,16,12,11,13,6,0,1,7,9,4,11,6,15,2,2,7,13,12,15,5,10,6,3,13,6,9,9,2,14,15,0,12,6,8,10,7,12,8,15,0,6,13,4,1,9,2,14,10,3,9,9,8,16,15,10,9,11,5,14,14,4,7,7,10,5,2,12,4,10,12,10,4,12,6,5,14,5,7,3,10,1,9,2,11,8,14,16,13,3,3,10,15,3,14,0,7,4,16,15,1,11,3,10,1,9,6,5,8,4,2,14,11,3,15,9,5,11,11,15,10,7,1,8,13,12,16,3,14,12,11,3,-1";
        
        static const char *fireBallXValuesChar2 = "9,4,6,14,1,10,8,5,8,5,4,6,11,13,7,6,12,7,3,12,11,1,8,1,14,0,11,12,1,2,0,6,0,9,11,12,8,12,7,4,7,12,14,6,5,14,2,13,5,10,6,14,13,5,0,10,10,3,4,10,11,0,8,12,3,3,6,4,13,1,10,3,8,3,2,4,11,2,10,10,13,5,6,9,14,4,0,13,2,14,2,1,5,8,6,14,2,9,10,5,2,3,3,3,0,13,9,6,13,6,8,11,1,2,9,11,8,8,2,8,1,7,9,6,8,9,14,9,11,10,7,11,5,14,13,9,9,1,7,9,5,5,7,2,6,13,5,2,12,4,11,11,5,6,10,4,10,5,5,12,8,11,0,4,2,7,1,14,13,7,10,10,1,11,3,10,10,8,5,0,13,7,9,12,14,4,8,6,7,13,0,0,2,4,8,11,4,3,10,11,12,11,14,3,7,3,9,4,7,0,5,7,3,7,0,12,3,4,0,3,9,14,4,4,7,6,9,1,9,12,1,1,13,12,8,3,6,-1";
        static const char *fireBallYValuesChar2 = "15,15,1,8,13,1,0,6,5,1,1,12,6,12,0,7,13,7,4,8,3,8,10,6,4,8,7,14,16,7,6,11,12,1,8,15,8,11,5,16,11,3,3,16,13,6,15,9,3,14,0,16,14,8,15,15,13,5,6,2,15,10,6,5,6,10,6,13,7,11,4,8,12,9,12,3,2,6,8,12,6,9,15,0,13,7,4,3,3,11,10,14,10,2,3,15,14,11,9,4,8,1,3,16,14,4,10,2,15,13,3,4,7,13,12,1,1,9,4,15,9,6,5,14,4,7,9,13,9,3,2,11,5,14,11,3,4,4,13,9,16,14,14,9,9,8,11,16,10,0,10,0,2,4,11,5,16,0,12,12,11,12,9,11,5,1,15,10,13,9,6,5,3,13,12,10,7,13,15,7,5,4,16,9,12,12,7,8,10,16,13,3,11,9,14,14,4,14,0,16,4,5,7,15,12,2,14,8,3,5,7,15,13,16,11,16,0,2,16,7,6,5,10,14,8,5,8,5,2,6,10,12,10,7,16,11,10,-1";
        
        std::vector<int> fireBallXValues;
        std::vector<int> fireBallYValues;
        
        bool isNumBreakableBlocksAtSpawnTimeEven = m_iNumBreakableBlocksAtSpawnTime % 2 == 0;
        
        readCharArrayIntoIntArray(isNumBreakableBlocksAtSpawnTimeEven ? fireBallXValuesChar1 : fireBallXValuesChar2, fireBallXValues, -1);
        readCharArrayIntoIntArray(isNumBreakableBlocksAtSpawnTimeEven ? fireBallYValuesChar1 : fireBallYValuesChar2, fireBallYValues, -1);
        
        static int numFireBalls = 237;
        for(int i = 0; i < numFireBalls; i++)
        {
            m_fireBalls.push_back(std::unique_ptr<FireBall>(new FireBall(fireBallXValues.at(i), fireBallYValues.at(i), i, m_gameListener.get(), m_insideBlocks, m_breakableBlocks)));
        }
    }
}

void GameSession::handlePlayerDataUpdate(rapidjson::Document& d, const char *keyIsBot, const char *keyX, const char *keyY, const char *keyDirection, const char *keyAlive, short playerIndex, bool isBeginGame)
{
    if (d.HasMember(keyX) && d.HasMember(keyY) && d.HasMember(keyDirection))
    {
        float playerX = d[keyX].GetDouble();
        m_players.at(playerIndex).get()->getPosition().setX(playerX);

        float playerY = d[keyY].GetDouble();
        m_players.at(playerIndex).get()->getPosition().setY(playerY);

        int playerDirection = d[keyDirection].GetInt();
        m_players.at(playerIndex).get()->setDirection(playerDirection);
        
        m_players.at(playerIndex).get()->updateBounds();
        m_players.at(playerIndex).get()->updateGrid();
    }
    
    if(d.HasMember(keyAlive))
    {
        bool isPlayerAlive = d[keyAlive].GetBool();
        Player_State playerState = m_players.at(playerIndex).get()->getPlayerState();
        if(playerState == ALIVE && !isPlayerAlive)
        {
            m_players.at(playerIndex).get()->setPlayerState(DEAD);
        }
    }
    
    if(d.HasMember(keyIsBot))
    {
        bool isBot = d[keyIsBot].GetBool();
        m_players.at(playerIndex).get()->setIsBot(isBot);
    }
}

void GameSession::handleClientEventsArrayInDocument(rapidjson::Document &d)
{
    static const char *eventsKey = "events";

    handleIntArrayInDocument(d, eventsKey, m_sEventIds, 0);
}

void GameSession::handleIntArrayInDocument(rapidjson::Document &d, const char *intArrayKey, std::vector<int> &intArray, int sentinelValue)
{
    if (d.HasMember(intArrayKey))
    {
        const char *charArray = d[intArrayKey].GetString();

        readCharArrayIntoIntArray(charArray, intArray, sentinelValue);
    }
}

void GameSession::readCharArrayIntoIntArray(const char *charArray, std::vector<int> &intArray, int sentinelValue)
{
    char *copy = strdup(charArray);
    
    char *value = std::strtok(copy, ",");
    
    int i = 0;
    
    while (value != NULL)
    {
        int intValue = atoi(value);
        if (intValue != sentinelValue)
        {
            intArray.push_back(intValue);
        }
        
        value = strtok(NULL, ","); // Get next event
        i++;
    }
    
    free(copy);
    free(value);
}

void GameSession::handlePlayerEvent(int event)
{
    int playerIndex = 0;
    
    while(event >= PLAYER_EVENT_BASE)
    {
        event -= PLAYER_EVENT_BASE;
        playerIndex++;
    }
    
    int direction = 0;
    while (event >= PLAYER_EVENT_DIRECTION_BASE)
    {
        event -= PLAYER_EVENT_DIRECTION_BASE;
        direction++;
    }
    m_players.at(playerIndex).get()->setDirection(direction);
    
    int gridX = 0;
    while (event >= PLAYER_EVENT_GRID_X_BASE)
    {
        event -= PLAYER_EVENT_GRID_X_BASE;
        gridX++;
    }
    m_players.at(playerIndex).get()->setGridX(gridX);
    
    int gridY = 0;
    while (event >= PLAYER_EVENT_GRID_Y_BASE)
    {
        event -= PLAYER_EVENT_GRID_Y_BASE;
        gridY++;
    }
    m_players.at(playerIndex).get()->setGridY(gridY);
    
    int eventMod = 0;
    while (event >= PLAYER_EVENT_MOD_BASE)
    {
        event -= PLAYER_EVENT_MOD_BASE;
        eventMod++;
    }
    
    switch (event)
    {
        case PLAYER_MOVE_RIGHT:
            m_players.at(playerIndex).get()->moveInDirection(DIRECTION_RIGHT);
            break;
        case PLAYER_MOVE_UP:
            m_players.at(playerIndex).get()->moveInDirection(DIRECTION_UP);
            break;
        case PLAYER_MOVE_LEFT:
            m_players.at(playerIndex).get()->moveInDirection(DIRECTION_LEFT);
            break;
        case PLAYER_MOVE_DOWN:
            m_players.at(playerIndex).get()->moveInDirection(DIRECTION_DOWN);
            break;
        case PLAYER_MOVE_STOP:
            m_players.at(playerIndex).get()->moveInDirection(-1);
            break;
        case PLAYER_PLANT_BOMB:
            // eventMod reflects the number shown on the power up bar, hence the + 1 here (fire power is 1 by default)
            layBombForPlayer(m_players.at(playerIndex).get(), eventMod + 1);
            break;
        case PLAYER_PUSH_BOMB:
            pushBombForPlayer(m_players.at(playerIndex).get());
            break;
        case PLAYER_RAISE_SHIELD:
            m_players.at(playerIndex).get()->raiseShield();
            break;
        case PLAYER_LOWER_SHIELD:
            m_players.at(playerIndex).get()->lowerShield();
            break;
        case PLAYER_FORCE_FIELD_HIT:
            m_players.at(playerIndex).get()->onForceFieldHit();
            break;
        case PLAYER_DEATH:
            m_players.at(playerIndex).get()->onDeath();
            break;
        case PLAYER_ABOUT_TO_FALL:
            m_players.at(playerIndex).get()->onTrappedOnFallingSpaceTile(m_spaceTiles);
            break;
        case PLAYER_FALL:
            m_players.at(playerIndex).get()->onFall();
            break;
        case PLAYER_FREEZE:
            m_players.at(playerIndex).get()->onFreeze();
            break;
        case PLAYER_PU_BOMB:
            m_players.at(playerIndex).get()->collectPowerUp(POWER_UP_TYPE_BOMB);
            break;
        case PLAYER_PU_FIRE:
            m_players.at(playerIndex).get()->collectPowerUp(POWER_UP_TYPE_FIRE);
            break;
        case PLAYER_PU_FORCE_FIELD:
            m_players.at(playerIndex).get()->collectPowerUp(POWER_UP_TYPE_FORCE_FIELD);
            break;
        case PLAYER_PU_SPEED:
            m_players.at(playerIndex).get()->collectPowerUp(POWER_UP_TYPE_SPEED);
            break;
        case PLAYER_PU_PUSH:
            m_players.at(playerIndex).get()->collectPowerUp(POWER_UP_TYPE_PUSH);
            break;
        case PLAYER_PU_SHIELD:
            m_players.at(playerIndex).get()->collectPowerUp(POWER_UP_TYPE_SHIELD);
            break;
        default:
            break;
    }
    
    if(event >= PLAYER_PU_BOMB)
    {
        for (std::vector < std::unique_ptr < PowerUp >> ::iterator itr = m_powerUps.begin(); itr != m_powerUps.end();)
        {
            if((*itr)->getGridX() == gridX && (*itr)->getGridY() == gridY)
            {
                itr = m_powerUps.erase(itr);
            }
            else
            {
                itr++;
            }
        }
    }
}

#pragma mark <For ServerGameSession to override>

void GameSession::onBreakableBlockDestroyed(BreakableBlock &breakableBlock)
{
    // Empty
}

void GameSession::onPowerUpPickedUp(PowerUp &powerUp)
{
    // Empty
}

#pragma mark <Private>

void GameSession::layBombForPlayer(PlayerDynamicGameObject *player, int firePower)
{
    BombGameObject *bomb = new BombGameObject(player, firePower, player->getGridX(), player->getGridY());
    m_bombs.push_back(std::unique_ptr<BombGameObject>(bomb));
    
    for (std::vector < std::unique_ptr < PlayerDynamicGameObject >> ::iterator itr = m_players.begin(); itr != m_players.end(); itr++)
    {
        if(OverlapTester::doRectanglesOverlap(bomb->getBounds(), (*itr)->getBounds()))
        {
            (*itr)->setGridX(bomb->getGridX());
            (*itr)->setGridY(bomb->getGridY());
        }
    }
}

void GameSession::pushBombForPlayer(PlayerDynamicGameObject *player)
{
    for(std::vector<std::unique_ptr<BombGameObject>>::iterator itr = m_bombs.begin(); itr != m_bombs.end(); itr++)
    {
        if(player->isBombInFrontOfPlayer(**itr))
        {
            (*itr)->pushed(player->getDirection());
            player->onBombPushed((*itr).get());
        }
    }
}