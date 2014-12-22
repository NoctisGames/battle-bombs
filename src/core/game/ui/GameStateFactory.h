//
//  GameStateFactory.h
//  battlebombs
//
//  Created by Stephen Gowen on 12/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__GameStateFactory__
#define __battlebombs__GameStateFactory__

#include <memory>

class GameState;

class GameStateFactory
{
public:
    static GameStateFactory& getInstance()
    {
        static GameStateFactory instance; // Guaranteed to be destroyed.
        return instance;
    }
    
    GameState * createGameState(int gameState, bool isOffline);
    
private:
    GameStateFactory() {}; // Constructor? (the {} brackets) are needed here.
    // Dont forget to declare these two. You want to make sure they
    // are unaccessable otherwise you may accidently get copies of
    // your singleton appearing.
    GameStateFactory(GameStateFactory const&); // Don't Implement
    void operator=(GameStateFactory const&); // Don't implement
};

#endif /* defined(__battlebombs__GameStateFactory__) */