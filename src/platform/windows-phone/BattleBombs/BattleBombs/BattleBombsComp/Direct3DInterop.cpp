#include "pch.h"
#include "Direct3DInterop.h"
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
#include "Global.h"
#include "SpriteBatcher.h"
#include "RectangleBatcher.h"
#include "GameSound.h"
#include "PlayerRowAvatar.h"
#include "Crater.h"
#include "FireBall.h"
#include "IceBall.h"
#include "IcePatch.h"
#include "FallingObjectShadow.h"
#include "SpaceTile.h"
#include "ScreenState.h"
#include "Map.h"
#include "StartButton.h"
#include "EnableBotButton.h"
#include "EnablePowerUpButton.h"
#include "RegeneratingDoor.h"
#include "BaseTile.h"
#include "Landmine.h"
#include "RemoteBomb.h"
#include "DetonateButton.h"

#include <string.h>
#include <sstream>

// For Randomness
#include <stdlib.h>
#include <time.h>

using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Microsoft::WRL;
using namespace Windows::Phone::Graphics::Interop;
using namespace Windows::Phone::Input::Interop;

namespace BattleBombsComp
{
	Direct3DInterop::Direct3DInterop() : m_timer(ref new BasicTimer())
	{
		m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
		m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
		m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
		m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
		m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
		m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
		m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
		m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
		m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
		m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
		m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
		m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
		m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
		m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
		m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
		m_touchEventsPool.push_back(TouchEvent(0, 0, Touch_Type::DOWN));
	}

	IDrawingSurfaceContentProvider^ Direct3DInterop::CreateContentProvider(Platform::String^ username, bool isOffline)
	{
		std::wstring fooW(username->Begin());
		std::string fooA(fooW.begin(), fooW.end());
		const char *usernameCharArray = fooA.c_str();

		m_gameScreen = std::unique_ptr<Direct3DGameScreen>(new Direct3DGameScreen(usernameCharArray, isOffline));
		m_isOffline = isOffline;

		ComPtr<Direct3DContentProvider> provider = Make<Direct3DContentProvider>(this);
		return reinterpret_cast<IDrawingSurfaceContentProvider^>(provider.Get());
	}

	// IDrawingSurfaceManipulationHandler
	void Direct3DInterop::SetManipulationHost(DrawingSurfaceManipulationHost^ manipulationHost)
	{
		manipulationHost->PointerPressed += ref new TypedEventHandler<DrawingSurfaceManipulationHost^, PointerEventArgs^>(this, &Direct3DInterop::OnPointerPressed);
		manipulationHost->PointerMoved += ref new TypedEventHandler<DrawingSurfaceManipulationHost^, PointerEventArgs^>(this, &Direct3DInterop::OnPointerMoved);
		manipulationHost->PointerReleased += ref new TypedEventHandler<DrawingSurfaceManipulationHost^, PointerEventArgs^>(this, &Direct3DInterop::OnPointerReleased);
	}

	bool Direct3DInterop::onBackPressed()
	{
		return m_gameScreen->handleOnBackPressed();
	}

	void Direct3DInterop::onChatReceived(Platform::String^ message)
	{
		std::wstring fooW(message->Begin());
		std::string fooA(fooW.begin(), fooW.end());
		const char *messageCharArray = fooA.c_str();

		m_gameScreen->handleServerUpdate(messageCharArray);
	}

	void Direct3DInterop::setWinRtCallback(WinRtCallback^ callback)
	{
		m_winRtCallback = callback;
	}

	void Direct3DInterop::RenderResolution::set(Windows::Foundation::Size renderResolution)
	{
		if (renderResolution.Width != m_renderResolution.Width || renderResolution.Height != m_renderResolution.Height)
		{
			m_renderResolution = renderResolution;

			if (m_gameScreen)
			{
				m_gameScreen->updateForRenderResolutionChange(m_renderResolution.Width, m_renderResolution.Height);
				RecreateSynchronizedTexture();
			}
		}
	}

	// Event Handlers
	void Direct3DInterop::OnPointerPressed(DrawingSurfaceManipulationHost^ sender, PointerEventArgs^ args)
	{
		addTouchEventForType(DOWN, args->CurrentPoint->RawPosition.X, args->CurrentPoint->RawPosition.Y);
	}

	void Direct3DInterop::OnPointerMoved(DrawingSurfaceManipulationHost^ sender, PointerEventArgs^ args)
	{
		if (m_touchEventsBuffer.size() < 3)
		{
			addTouchEventForType(DRAGGED, args->CurrentPoint->RawPosition.X, args->CurrentPoint->RawPosition.Y);
		}
	}

	void Direct3DInterop::OnPointerReleased(DrawingSurfaceManipulationHost^ sender, PointerEventArgs^ args)
	{
		addTouchEventForType(UP, args->CurrentPoint->RawPosition.X, args->CurrentPoint->RawPosition.Y);
	}

	// Interface With Direct3DContentProvider
	HRESULT Direct3DInterop::Connect(_In_ IDrawingSurfaceRuntimeHostNative* host)
	{
		m_gameScreen->load(RenderResolution.Width, RenderResolution.Height, (int)WindowBounds.Width, (int)WindowBounds.Height);
		m_gameScreen->onResume();

		// Restart timer after renderer has finished initializing.
		m_timer->Reset();

		return S_OK;
	}

	void Direct3DInterop::Disconnect()
	{
		m_gameScreen->unload();
	}

	HRESULT Direct3DInterop::PrepareResources(_In_ const LARGE_INTEGER* presentTargetTime, _Out_ BOOL* contentDirty)
	{
		*contentDirty = true;

		return S_OK;
	}

	HRESULT Direct3DInterop::GetTexture(_In_ const DrawingSurfaceSizeF* size, _Out_ IDrawingSurfaceSynchronizedTextureNative** synchronizedTexture, _Out_ DrawingSurfaceRectF* textureSubRectangle)
	{
		static std::wstring toastMessage = L"You are now in Spectator Mode"; // I know this is stupid, but it works
		
		int screenState = m_gameScreen->getScreenState();
		switch (screenState)
		{
		case SCREEN_STATE_ENTERED_SPECTATOR_MODE:
			m_winRtCallback->Invoke("DISPLAY_TOAST", ref new Platform::String(toastMessage.c_str()));

			m_gameScreen->clearState();
		case SCREEN_STATE_NORMAL:
			for (std::vector<TouchEvent>::iterator itr = m_touchEvents.begin(); itr != m_touchEvents.end(); itr++)
			{
				if (m_touchEventsPool.size() < 50)
				{
					m_touchEventsPool.push_back(*itr);
				}
			}

			m_touchEvents.clear();
			m_touchEvents.swap(m_touchEventsBuffer);
			m_touchEventsBuffer.clear();

			m_timer->Update();
			m_gameScreen->update(m_timer->Delta, m_touchEvents);

			if (!m_isOffline)
			{
				pushEvents();
			}
			break;
		case SCREEN_STATE_CONNECTION_ERROR:
			m_winRtCallback->Invoke("CONNECTION_ERROR", "NULL");
			break;
		case SCREEN_STATE_EXIT:
			m_winRtCallback->Invoke("EXIT", "NULL");
			break;
		default:
			break;
		}

		m_gameScreen->present();
		m_gameScreen->handleSound();
		m_gameScreen->handleMusic();

		RequestAdditionalFrame();

		return S_OK;
	}

	ID3D11Texture2D* Direct3DInterop::GetTexture()
	{
		return m_gameScreen->getTexture();
	}

	void Direct3DInterop::pushEvents()
	{
		int eventId = m_gameScreen->popOldestEventId();
		if (eventId > 0)
		{
			Platform::String^ eventsMessage = eventId + ",";
			while ((eventId = m_gameScreen->popOldestEventId()) > 0)
			{
				eventsMessage += eventId + ",";
			}

			eventsMessage += "0"; // Terminate with 0

			static Platform::String^ EVENT_TYPE = "eventType";
			static Platform::String^ EVENTS = "events";

			static Platform::String^ PLAYER_INDEX = "playerIndex";
			static Platform::String^ X = "X";
			static Platform::String^ Y = "Y";
			static Platform::String^ DIRECTION = "Direction";

			int playerIndex = m_gameScreen->getPlayerIndex();
			Platform::String^ clientUpdate = "{";
			clientUpdate += "\"" + EVENT_TYPE + "\":" + CLIENT_UPDATE;
			clientUpdate += ",\"" + EVENTS + "\":\"" + eventsMessage + "\"";
			clientUpdate += ",\"" + PLAYER_INDEX + playerIndex + X + "\":" + m_gameScreen->getPlayerX();
			clientUpdate += ",\"" + PLAYER_INDEX + playerIndex + Y + "\":" + m_gameScreen->getPlayerY();
			clientUpdate += ",\"" + PLAYER_INDEX + playerIndex + DIRECTION + "\":" + m_gameScreen->getPlayerDirection();
			clientUpdate += "}";

			m_gameScreen->resetTimeSinceLastClientEvent();

			m_winRtCallback->Invoke("SEND_CHAT", clientUpdate);
		}
		else if (m_gameScreen->isTimeToSendKeepAlive())
		{
			m_gameScreen->resetTimeSinceLastClientEvent();

			m_winRtCallback->Invoke("SEND_CHAT", "KEEP_ALIVE,3");
		}
	}

	void Direct3DInterop::addTouchEventForType(Touch_Type touchType, float x, float y)
	{
		TouchEvent touchEvent = newTouchEvent();
		touchEvent.setTouchType(touchType);
		touchEvent.setX(x);
		touchEvent.setY(y);

		m_touchEventsBuffer.push_back(touchEvent);
	}

	TouchEvent Direct3DInterop::newTouchEvent()
	{
		if (m_touchEventsPool.size() == 0)
		{
			return TouchEvent(0, 0, Touch_Type::DOWN);
		}
		else
		{
			TouchEvent touchEvent = m_touchEventsPool.back();
			m_touchEventsPool.pop_back();
			return touchEvent;
		}
	}
}