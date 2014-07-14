//
//  OpenGLESGameScreen.cpp
//  bomber-party
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "OpenGLESGameScreen.h"
#include "Vector2D.h"
#include "TouchEvent.h"
#include "SpriteBatcher.h"
#include "Rectangle.h"
#include "GameButton.h"
#include "OpenGLESRenderer.h"
#include "GameConstants.h"
#include "Font.h"

OpenGLESGameScreen::OpenGLESGameScreen(const char *username) : GameScreen(username)
{
    // TODO
}

void OpenGLESGameScreen::onSurfaceCreated(int width, int height)
{
	m_iDeviceScreenWidth = width;
	m_iDeviceScreenHeight = height;
    
    m_renderer = std::unique_ptr<OpenGLESRenderer>(new OpenGLESRenderer(width, height));
}

void OpenGLESGameScreen::onSurfaceChanged(int width, int height)
{
	m_iDeviceScreenWidth = width;
	m_iDeviceScreenHeight = height;
    
    glViewport(0, 0, width, height);
	glScissor(0, 0, width, height);
}

void OpenGLESGameScreen::setDpDimensions(int dpWidth, int dpHeight)
{
	m_iDeviceScreenDpWidth = dpWidth;
	m_iDeviceScreenDpHeight = dpHeight;
}

void OpenGLESGameScreen::platformResume()
{
    // TODO
}

void OpenGLESGameScreen::platformPause()
{
    // TODO
}

bool OpenGLESGameScreen::handleOnBackPressed()
{
    return false;
}

void OpenGLESGameScreen::touchToWorld(TouchEvent &touchEvent)
{
#ifdef TECHNE_GAMES_OPENGL_ANDROID
    m_touchPoint->set((touchEvent.getX() / (float) m_iDeviceScreenWidth) * SCREEN_WIDTH, (1 - touchEvent.getY() / (float) m_iDeviceScreenHeight) * SCREEN_HEIGHT);
#elif defined TECHNE_GAMES_IOS
    m_touchPoint->set((touchEvent.getY() / (float) m_iDeviceScreenDpHeight) * SCREEN_WIDTH, (touchEvent.getX() / (float) m_iDeviceScreenDpWidth) * SCREEN_HEIGHT);
#endif
}