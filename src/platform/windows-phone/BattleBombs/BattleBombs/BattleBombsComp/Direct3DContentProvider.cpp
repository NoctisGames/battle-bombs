#include "pch.h"
#include "Direct3DContentProvider.h"
#include "Direct3DGameScreen.h"
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
#include "PlayerRow.h"
#include "PlayerRowPlatformAvatar.h"
#include "Direct3DRenderer.h"
#include "SpriteBatcher.h"
#include "RectangleBatcher.h"
#include "Global.h"
#include "GameSound.h"
#include "PlayerRowAvatar.h"
#include "Crater.h"
#include "FireBall.h"
#include "IceBall.h"
#include "IcePatch.h"
#include "FallingObjectShadow.h"
#include "SpaceTile.h"
#include "Map.h"
#include "StartButton.h"
#include "EnableBotButton.h"
#include "EnablePowerUpButton.h"
#include "RegeneratingDoor.h"
#include "BaseTile.h"
#include "Landmine.h"
#include "RemoteBomb.h"
#include "DetonateButton.h"

using namespace BattleBombsComp;

Direct3DContentProvider::Direct3DContentProvider(Direct3DInterop^ controller) : m_controller(controller)
{
	m_controller->RequestAdditionalFrame += ref new RequestAdditionalFrameHandler([=] ()
		{
			if (m_host)
			{
				m_host->RequestAdditionalFrame();
			}
		});
	
	m_controller->RecreateSynchronizedTexture += ref new RecreateSynchronizedTextureHandler([=] ()
		{
			if (m_host)
			{
				m_host->CreateSynchronizedTexture(m_controller->GetTexture(), &m_synchronizedTexture);
			}
		});
}

// IDrawingSurfaceContentProviderNative interface
HRESULT Direct3DContentProvider::Connect(_In_ IDrawingSurfaceRuntimeHostNative* host)
{
	m_host = host;

	return m_controller->Connect(host);
}

void Direct3DContentProvider::Disconnect()
{
	m_controller->Disconnect();
	m_host = nullptr;
	m_synchronizedTexture = nullptr;
}

HRESULT Direct3DContentProvider::PrepareResources(_In_ const LARGE_INTEGER* presentTargetTime, _Out_ BOOL* contentDirty)
{
	return m_controller->PrepareResources(presentTargetTime, contentDirty);
}

HRESULT Direct3DContentProvider::GetTexture(_In_ const DrawingSurfaceSizeF* size, _Out_ IDrawingSurfaceSynchronizedTextureNative** synchronizedTexture, _Out_ DrawingSurfaceRectF* textureSubRectangle)
{
	HRESULT hr = S_OK;

	if (!m_synchronizedTexture)
	{
		hr = m_host->CreateSynchronizedTexture(m_controller->GetTexture(), &m_synchronizedTexture);
	}

	// Set output parameters.
	textureSubRectangle->left = 0.0f;
	textureSubRectangle->top = 0.0f;
	textureSubRectangle->right = static_cast<FLOAT>(size->width);
	textureSubRectangle->bottom = static_cast<FLOAT>(size->height);

	m_synchronizedTexture.CopyTo(synchronizedTexture);

	// Draw to the texture.
	if (SUCCEEDED(hr))
	{
		hr = m_synchronizedTexture->BeginDraw();
		
		if (SUCCEEDED(hr))
		{
			hr = m_controller->GetTexture(size, synchronizedTexture, textureSubRectangle);
		}

		m_synchronizedTexture->EndDraw();
	}

	return hr;
}