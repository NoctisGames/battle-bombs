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
#include "OpenGLESRenderer.h"
#include "SpriteBatcher.h"
#include "TextureRegion.h"
#include "Rectangle.h"
#include "GameButton.h"
#include "OpenGLESRenderer.h"
#include "GameConstants.h"

extern "C"
{
#include "platform_gl.h"
#include "platform_log.h"
}

OpenGLESGameScreen::OpenGLESGameScreen(const char *username) : GameScreen(username)
{
    
}

void OpenGLESGameScreen::platformInit()
{
    
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
    
    m_renderer = std::unique_ptr<OpenGLESRenderer>(new OpenGLESRenderer());
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

void OpenGLESGameScreen::present()
{
    glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    
    switch (m_gameState)
    {
        case WAITING_FOR_SERVER:
            glEnable(GL_TEXTURE_2D);
            
            m_renderer->renderWorldBackground();
            
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            m_renderer->renderWorldForeground(m_insideBlocks, m_breakableBlocks, m_powerUps);
            m_renderer->renderInterface();
            
            glDisable(GL_BLEND);
            
            glDisable(GL_TEXTURE_2D);
            break;
        case RUNNING:
        case SPECTATING:
            m_renderer->calcScrollYForPlayer(*m_player);
            
            glEnable(GL_TEXTURE_2D);
            
            m_renderer->renderWorldBackground();
            
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            
            m_renderer->renderWorldForeground(m_insideBlocks, m_breakableBlocks, m_powerUps);
            m_renderer->renderBombs(m_bombs);
            m_renderer->renderExplosions(m_explosions);
            m_renderer->renderPlayers(m_players);
            m_renderer->renderInterface();
            m_renderer->renderControls(*m_dPad, *m_activeButton);
            
            glDisable(GL_BLEND);
            
            glDisable(GL_TEXTURE_2D);
            break;
        default:
            break;
    }
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