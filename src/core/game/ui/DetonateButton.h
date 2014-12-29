//
//  DetonateButton.h
//  battlebombs
//
//  Created by Stephen Gowen on 12/28/14.
//  Copyright (c) 2014 Gowen Game Dev. All rights reserved.
//

#ifndef __battlebombs__DetonateButton__
#define __battlebombs__DetonateButton__

#include "GameObject.h"
#include "DetonateButtonState.h"

class DetonateButton : public GameObject
{
public:
    DetonateButton(float x, float y, float width, float height);
    
    void update(float deltaTime);
    
    float getStateTime();
    
    void setNumRemoteBombsDeployed(int numRemoteBombsDeployed);
    
    int getNumRemoteBombsDeployed();
    
    Detonate_Button_State getState();
    
private:
    float m_fStateTime;
    int m_iNumRemoteBombsDeployed;
    Detonate_Button_State m_state;
    bool m_isActivating;
    bool m_isDeactivating;
};

#endif /* defined(__battlebombs__DetonateButton__) */