//
//  OpenGLESGameScreen.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#include "pch.h"
#include "OpenGLESGameScreen.h"
#include "Vector2D.h"
#include "TouchEvent.h"
#include "Vector2D.h"
#include "Rectangle.h"
#include "Assets.h"
#include "OverlapTester.h"
#include "GameEvent.h"
#include "BombGameObject.h"
#include "Explosion.h"
#include "PowerUp.h"
#include "PlayerDynamicGameObject.h"
#include "BotPlayerDynamicGameObject.h"
#include "MapSearchNode.h"
#include "GameListener.h"
#include "Renderer.h"
#include "Fire.h"
#include "Triangle.h"
#include "MapBorder.h"
#include "InsideBlock.h"
#include "BreakableBlock.h"
#include "PathFinder.h"
#include "WaitingForServerInterface.h"
#include "WaitingForLocalSettingsInterface.h"
#include "InterfaceOverlay.h"
#include "BombButton.h"
#include "PowerUpBarItem.h"
#include "PlayerAvatar.h"
#include "Font.h"
#include "SpectatorControls.h"
#include "CountDownNumberGameObject.h"
#include "DisplayBattleGameObject.h"
#include "DisplayGameOverGameObject.h"
#include "OpenGLESRenderer.h"
#include "PlayerRow.h"
#include "PlayerRowAvatar.h"
#include "OpenGLESRectangleBatcher.h"
#include "PlayerRowPlatformAvatar.h"
#include "SpriteBatcher.h"
#include "Vertices2D.h"

OpenGLESGameScreen::OpenGLESGameScreen(const char *username, bool isOffline) : GameScreen(username, isOffline)
{
    // No further setup
}

void OpenGLESGameScreen::onSurfaceCreated(int width, int height)
{
	m_iDeviceScreenWidth = width;
	m_iDeviceScreenHeight = height;
    
    glViewport(0, 0, width, height);
	glScissor(0, 0, width, height);
    
	glLoadIdentity();
    
	glMatrixMode(GL_PROJECTION);
	glOrthof(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, -1, 1);
	glMatrixMode(GL_MODELVIEW);
    
	glLoadIdentity();
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    
    m_renderer = std::unique_ptr<OpenGLESRenderer>(new OpenGLESRenderer(m_iDeviceScreenWidth, m_iDeviceScreenHeight));
}

void OpenGLESGameScreen::onSurfaceChanged(int width, int height, int dpWidth, int dpHeight)
{
	m_iDeviceScreenWidth = width;
	m_iDeviceScreenHeight = height;
    m_iDeviceScreenDpWidth = dpWidth;
	m_iDeviceScreenDpHeight = dpHeight;
}

void OpenGLESGameScreen::touchToWorld(TouchEvent &touchEvent)
{
#ifdef TECHNE_GAMES_OPENGL_ANDROID
    m_touchPoint->set((touchEvent.getX() / (float) m_iDeviceScreenWidth) * SCREEN_WIDTH, (1 - touchEvent.getY() / (float) m_iDeviceScreenHeight) * SCREEN_HEIGHT);
#elif defined TECHNE_GAMES_IOS
    m_touchPoint->set((touchEvent.getY() / (float) m_iDeviceScreenDpHeight) * SCREEN_WIDTH, (touchEvent.getX() / (float) m_iDeviceScreenDpWidth) * SCREEN_HEIGHT);
#endif
}

void OpenGLESGameScreen::platformResume()
{
    // Empty
}

void OpenGLESGameScreen::platformPause()
{
    // Empty
}

bool OpenGLESGameScreen::handleOnBackPressed()
{
    return false;
}