//
//  RECTUtils.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2013 Techne Games. All rights reserved.
//

#pragma once

#include "windef.h"
#include "GameObject.h"

class RECTUtils
{
public:
	static RECTUtils * getInstance()
	{
		static RECTUtils *instance = new RECTUtils();
		return instance;
	}

	void setGameScreenToDeviceScreenRatio(float gameScreenToDeviceScreenWidthRatio, float gameScreenToDeviceScreenHeightRatio);

	RECT getRECTForGameObject(GameObject &gameObject, bool hasRotation);

	RECT getRECTForCoordinates(float x, float y, float w, float h, bool hasRotation);

	float convertGameXToScreenX(float x);

	float convertGameYToScreenY(float y);

private:
	float m_fGameScreenToDeviceScreenWidthRatio;
	float m_fGameScreenToDeviceScreenHeightRatio;
};