//
//  OpenGLESGameScreen.h
//  battlebombs
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#ifndef __battle_bombs__OpenGLESGameScreen__
#define __battle_bombs__OpenGLESGameScreen__

#include "GameScreen.h"

class OpenGLESRenderer;

class OpenGLESGameScreen : public GameScreen
{
public:
    OpenGLESGameScreen(const char *username, bool isOffline = false);
    
    virtual void touchToWorld(TouchEvent &touchEvent);
    
    virtual void platformResume();
    
    virtual void platformPause();
    
    virtual bool handleOnBackPressed();
    
    void onSurfaceCreated(int deviceScreenWidth, int deviceScreenHeight);
    
	void onSurfaceChanged(int deviceScreenWidth, int deviceScreenHeight);
    
	void setDpDimensions(int dpWidth, int dpHeight);

private:
    int m_iDeviceScreenDpWidth;
    int m_iDeviceScreenDpHeight;
};

#endif /* defined(__battle_bombs__OpenGLESGameScreen__) */
