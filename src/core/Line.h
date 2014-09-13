//
//  Line.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battle_bombs__Line__
#define __battle_bombs__Line__

#include <memory>
#include "Vector2D.h"

class Line
{
public:
    Line(float x1, float y1, float x2, float y2);
    
    Vector2D& getOrigin();
    
    Vector2D& getEnd();
    
private:
    std::unique_ptr<Vector2D> m_origin;
    std::unique_ptr<Vector2D> m_end;
};

#endif /* defined(__battle_bombs__Line__) */
