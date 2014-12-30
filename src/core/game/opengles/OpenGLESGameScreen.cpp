//
//  OpenGLESGameScreen.cpp
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

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
#include "DisplayXMovingGameObject.h"
#include "DisplayGameOverGameObject.h"
#include "OpenGLESRenderer.h"
#include "PlayerRow.h"
#include "PlayerRowAvatar.h"
#include "OpenGLESRectangleBatcher.h"
#include "PlayerRowPlatformAvatar.h"
#include "SpriteBatcher.h"
#include "Crater.h"
#include "FireBall.h"
#include "IceBall.h"
#include "IcePatch.h"
#include "FallingObjectShadow.h"
#include "SpaceTile.h"
#include "OpenGLESManager.h"
#include "Map.h"
#include "StartButton.h"
#include "EnableBotButton.h"
#include "EnablePowerUpButton.h"
#include "RegeneratingDoor.h"
#include "BaseTile.h"
#include "Landmine.h"
#include "RemoteBomb.h"
#include "DetonateButton.h"

OpenGLESGameScreen::OpenGLESGameScreen(const char *username, bool isOffline, bool isRunningIOS8) : GameScreen(username, isOffline)
{
    m_isRunningIOS8 = isRunningIOS8;
}

void OpenGLESGameScreen::onSurfaceCreated(int width, int height)
{
	m_iDeviceScreenWidth = width;
	m_iDeviceScreenHeight = height;
    
    OGLESManager->init(width, height);
    
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
    if(m_isRunningIOS8)
    {
        m_touchPoint->set((touchEvent.getX() / (float) m_iDeviceScreenDpWidth) * SCREEN_WIDTH, (1 - touchEvent.getY() / (float) m_iDeviceScreenDpHeight) * SCREEN_HEIGHT);
    }
    else
    {
        m_touchPoint->set((touchEvent.getY() / (float) m_iDeviceScreenDpHeight) * SCREEN_WIDTH, (touchEvent.getX() / (float) m_iDeviceScreenDpWidth) * SCREEN_HEIGHT);
    }
#endif
}

bool OpenGLESGameScreen::handleOnBackPressed()
{
    return false;
}