//
//  RegeneratingDoor.h
//  battlebombs
//
//  Created by Stephen Gowen on 12/28/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__RegeneratingDoor__
#define __battlebombs__RegeneratingDoor__

#include "GridGameObject.h"

#include <vector>

class BombGameObject;
class Explosion;
class PlayerDynamicGameObject;

class RegeneratingDoor : public GridGameObject
{
public:
    RegeneratingDoor(int gridX, int gridY);
    
    void update(float deltaTime, std::vector<std::unique_ptr<BombGameObject>> &bombs, std::vector<std::unique_ptr<Explosion>> &explosions, std::vector<std::unique_ptr<PlayerDynamicGameObject>> &players);
    
    void onDestroy();
    
    float getStateTime();
    
    bool isExploding();
    
    bool isDestroyed();
    
private:
    float m_fStateTime;
    bool m_isExploding;
    bool m_isDestroyed;
};

#endif /* defined(__battlebombs__RegeneratingDoor__) */