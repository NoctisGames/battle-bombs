//
//  Direct3DGameScreen.h
//  bomberparty
//
//  Created by Stephen Gowen on 2/22/14.
//  Copyright (c) 2014 Techne Games. All rights reserved.
//

#pragma once

#include "GameScreen.h"
#include "Helpers\MediaEnginePlayer.h"

class Direct3DGameScreen : public GameScreen
{
public:
	Direct3DGameScreen(const char *username, int deviceScreenWidth, int deviceScreenHeight);

	void load(ID3D11Device1 *d3dDevice, ID3D11DeviceContext1 *d3dContext, ID3D11RenderTargetView *renderTargetView, ID3D11DepthStencilView *depthStencilView);

	virtual void platformInit();

	virtual void platformResume();

	virtual void platformPause();

	virtual void touchToWorld(TouchEvent &touchEvent);

	void handleSound();

	void handleMusic();

	void unload();

	virtual bool handleOnBackPressed();

private:
	std::unique_ptr<MediaEnginePlayer> m_mediaPlayer;

	float m_fGameScreenToDeviceScreenWidthRatio;
	float m_fGameScreenToDeviceScreenHeightRatio;
	float m_fDipToPixelRatio;
};