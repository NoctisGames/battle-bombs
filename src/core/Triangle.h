//
//  Triangle.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battle_bombs__Triangle__
#define __battle_bombs__Triangle__

#include <memory>
#include "Line.h"
#include "Vector2D.h"

class Triangle
{
public:
    Triangle(float x1, float y1, float x2, float y2, float x3, float y3);
    
    void update(float x1, float y1, float x2, float y2, float x3, float y3);
    
    Vector2D& getPointA();
    
    Vector2D& getPointB();
    
    Vector2D& getPointC();
    
    Line& getSideA();
    
    Line& getSideB();
    
    Line& getSideC();
    
private:
    std::unique_ptr<Line> m_sideA;
    std::unique_ptr<Line> m_sideB;
    std::unique_ptr<Line> m_sideC;
};

#endif /* defined(__battle_bombs__Triangle__) */
