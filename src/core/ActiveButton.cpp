//
//  ActiveButton.cpp
//  bomberparty
//
//  Created by Mikel Adkins on 5/18/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "ActiveButton.h"
#include "OverlapTester.h"
#include "Vector2D.h"

ActiveButton::ActiveButton(float x, float y, float width, float height) : GameObject(x, y, width, height, 0)
{
	m_activeButtonBounds = std::unique_ptr<Rectangle>(new Rectangle(x, y, width, height));
}

bool ActiveButton::isPointInBounds(Vector2D &touchPoint)
{
	if(OverlapTester::isPointInRectangle(touchPoint, *m_activeButtonBounds))
	{
		return true;
	}
	return false;
}
