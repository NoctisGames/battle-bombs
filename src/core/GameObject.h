//
//  GameObject.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef GameObject_H
#define GameObject_H

#include <memory>

class Rectangle;
class Vector2D;

class GameObject
{
protected:
    std::unique_ptr<Vector2D> m_position;
    std::unique_ptr<Rectangle> m_bounds;
    float m_fWidth;
    float m_fHeight;
    float m_fAngle;

public:
    GameObject(float x, float y, float width, float height, float angle);

    virtual void resetBounds(float width, float height);

    virtual void updateBounds();

    Vector2D& getPosition();

    Rectangle& getBounds();

    const float& getWidth();

    const float& getHeight();

    // Should be overridden in cases where the object will only faces certain directions
    // This method could return 1 of several constant angles based on direction or something
    virtual float getAngle();
};

#endif /* GameObject_H */
