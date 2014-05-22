//
//  ActiveButton.h
//  bomberparty
//
//  Created by Mikel Adkins on 5/18/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __bomber_party__ActiveButton__
#define __bomber_party__ActiveButton__

#include "GameObject.h"
#include "Rectangle.h"
#include "PowerUpType.h"

class ActiveButton : public GameObject
{
public:
	ActiveButton(float x, float y, float width, float height);

	bool isPointInBounds(Vector2D &touchPoint);

private:
	std::unique_ptr<Rectangle> m_activeButtonBounds;
};

#endif /* defined (__ActiveButton__) */