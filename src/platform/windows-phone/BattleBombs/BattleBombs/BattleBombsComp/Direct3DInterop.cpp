#include "pch.h"
#include "Direct3DInterop.h"
#include "Direct3DContentProvider.h"
#include "GameButton.h"
#include "GameEvent.h"
#include <string.h>
#include <sstream>

using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Microsoft::WRL;
using namespace Windows::Phone::Graphics::Interop;
using namespace Windows::Phone::Input::Interop;

namespace BattleBombsComp
{
	Direct3DInterop::Direct3DInterop() : m_timer(ref new BasicTimer())
	{
		m_mapType = 0;
	}

	IDrawingSurfaceContentProvider^ Direct3DInterop::CreateContentProvider(Platform::String^ username, bool isOffline)
	{
		std::wstring fooW(username->Begin());
		std::string fooA(fooW.begin(), fooW.end());
		const char *usernameCharArray = fooA.c_str();

		int usernameLength = (int)strlen(usernameCharArray);

		m_username = new char[usernameLength];

		std::strncpy(m_username, usernameCharArray, usernameLength);
		m_username[usernameLength] = '\0';

		m_isOffline = isOffline;
		m_gameScreen = std::unique_ptr<Direct3DGameScreen>(new Direct3DGameScreen(usernameCharArray, (int)NativeResolution.Width, (int)NativeResolution.Height, isOffline));

		ComPtr<Direct3DContentProvider> provider = Make<Direct3DContentProvider>(this);
		return reinterpret_cast<IDrawingSurfaceContentProvider^>(provider.Get());
	}

	// IDrawingSurfaceManipulationHandler
	void Direct3DInterop::SetManipulationHost(DrawingSurfaceManipulationHost^ manipulationHost)
	{
		manipulationHost->PointerPressed +=
			ref new TypedEventHandler<DrawingSurfaceManipulationHost^, PointerEventArgs^>(this, &Direct3DInterop::OnPointerPressed);

		manipulationHost->PointerMoved +=
			ref new TypedEventHandler<DrawingSurfaceManipulationHost^, PointerEventArgs^>(this, &Direct3DInterop::OnPointerMoved);

		manipulationHost->PointerReleased +=
			ref new TypedEventHandler<DrawingSurfaceManipulationHost^, PointerEventArgs^>(this, &Direct3DInterop::OnPointerReleased);
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

			if (m_direct3DBase)
			{
				m_direct3DBase->UpdateForRenderResolutionChange(m_renderResolution.Width, m_renderResolution.Height);
				m_direct3DBase->loadScreen(*m_gameScreen);
				RecreateSynchronizedTexture();
			}
		}
	}

	// Event Handlers
	void Direct3DInterop::OnPointerPressed(DrawingSurfaceManipulationHost^ sender, PointerEventArgs^ args)
	{
		m_direct3DBase->touchDown(args->CurrentPoint->RawPosition.X, args->CurrentPoint->RawPosition.Y);
	}

	void Direct3DInterop::OnPointerMoved(DrawingSurfaceManipulationHost^ sender, PointerEventArgs^ args)
	{
		m_direct3DBase->touchDragged(args->CurrentPoint->RawPosition.X, args->CurrentPoint->RawPosition.Y);
	}

	void Direct3DInterop::OnPointerReleased(DrawingSurfaceManipulationHost^ sender, PointerEventArgs^ args)
	{
		m_direct3DBase->touchUp(args->CurrentPoint->RawPosition.X, args->CurrentPoint->RawPosition.Y);
	}

	// Interface With Direct3DContentProvider
	HRESULT Direct3DInterop::Connect(_In_ IDrawingSurfaceRuntimeHostNative* host)
	{
		m_direct3DBase = ref new Direct3DBase();
		m_direct3DBase->Initialize();
		m_direct3DBase->UpdateForWindowSizeChange(WindowBounds.Width, WindowBounds.Height);
		m_direct3DBase->UpdateForRenderResolutionChange(m_renderResolution.Width, m_renderResolution.Height);
		m_direct3DBase->loadScreen(*m_gameScreen);

		if (m_isOffline)
		{
			handleGameStateOne();
		}

		// Restart timer after renderer has finished initializing.
		m_timer->Reset();

		return S_OK;
	}

	void Direct3DInterop::Disconnect()
	{
		m_gameScreen->unload();
		m_direct3DBase = nullptr;
	}

	HRESULT Direct3DInterop::PrepareResources(_In_ const LARGE_INTEGER* presentTargetTime, _Out_ BOOL* contentDirty)
	{
		*contentDirty = true;

		return S_OK;
	}

	HRESULT Direct3DInterop::GetTexture(_In_ const DrawingSurfaceSizeF* size, _Out_ IDrawingSurfaceSynchronizedTextureNative** synchronizedTexture, _Out_ DrawingSurfaceRectF* textureSubRectangle)
	{
		int gameState = m_gameScreen->getState();
		if (gameState == 0)
		{
			m_timer->Update();
			m_direct3DBase->Update(*m_gameScreen, m_timer->Delta);
			pushEvents();
		}
		else if (gameState == 1)
		{
			if (m_isOffline)
			{
				handleGameStateOne();
			}
			else
			{
				m_winRtCallback->Invoke("EXIT", "NULL");
			}
		}

		m_direct3DBase->Render(*m_gameScreen);

		RequestAdditionalFrame();

		return S_OK;
	}

	ID3D11Texture2D* Direct3DInterop::GetTexture()
	{
		return m_direct3DBase->GetTexture();
	}

	void Direct3DInterop::pushEvents()
	{
		if (m_isOffline)
		{
			int event;
			while ((event = m_gameScreen->popOldestEventId()) > 0)
			{
				int playerIndex = 0;
				while (event >= PLAYER_EVENT_BASE)
				{
					event -= PLAYER_EVENT_BASE;
					playerIndex++;
				}

				while (event >= PLAYER_EVENT_DIRECTION_BASE)
				{
					event -= PLAYER_EVENT_DIRECTION_BASE;
				}

				while (event >= PLAYER_EVENT_GRID_X_BASE)
				{
					event -= PLAYER_EVENT_GRID_X_BASE;
				}

				while (event >= PLAYER_EVENT_GRID_Y_BASE)
				{
					event -= PLAYER_EVENT_GRID_Y_BASE;
				}

				while (event >= PLAYER_EVENT_MOD_BASE)
				{
					event -= PLAYER_EVENT_MOD_BASE;
				}

				switch (event)
				{
				case PLAYER_DEATH:
					handleDeathForPlayerIndex(playerIndex);
					continue;
				default:
					continue;
				}
			}

			if (m_gameScreen->getNumSecondsLeft() <= 0)
			{
				std::stringstream ss;
				ss << "{\"eventType\": ";
				ss << GAME_OVER;
				ss << ", \"hasWinner\": false, \"winningPlayerIndex\": -1}";
				std::string gameOverMessageString = ss.str();
				const char *gameOverMessage = gameOverMessageString.c_str();

				m_gameScreen->handleServerUpdate(gameOverMessage);
			}
		}
		else
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

				m_winRtCallback->Invoke("SEND_CHAT", "KEEP_ALIVE");
			}
		}
	}

	void Direct3DInterop::handleGameStateOne()
	{
		m_playersAlive[0] = true;
		m_playersAlive[1] = true;
		m_playersAlive[2] = true;
		m_playersAlive[3] = true;
		m_playersAlive[4] = true;
		m_playersAlive[5] = true;
		m_playersAlive[6] = true;
		m_playersAlive[7] = true;

		m_gameScreen->clearState();

		std::stringstream ss;
		ss << "{\"breakableBlockPowerUpFlags\": \"0,1,2,3,4,5\", \"breakableBlockXValues\": \"8,4,5,7,8,9\", \"breakableBlockYValues\": \"0,1,2,3,4,5\", \"eventType\": 1337, \"mapType\": ";
		ss << m_mapType;
		ss << ", \"numBreakableBlocks\": 5, \"numClientBots\": 7, \"numPlayers\": 1, \"playerIndex0\": \"";
		ss << m_username;
		ss << "\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0X\": 22.208955764770508, \"playerIndex0Y\": 12.179104804992676, \"playerIndex1\": \"Bot 1\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1X\": 2.1492538452148438, \"playerIndex1Y\": 12.179104804992676, \"playerIndex2\": \"Bot 2\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2X\": 17.91044807434082, \"playerIndex2Y\": 2.1492538452148438, \"playerIndex3\": \"Bot 3\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3X\": 6.447761058807373, \"playerIndex3Y\": 2.1492538452148438, \"playerIndex4\": \"Bot 4\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4X\": 2.1492538452148438, \"playerIndex4Y\": 25.074626922607422, \"playerIndex5\": \"Bot 5\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 16.477611541748047, \"playerIndex6\": \"Bot 6\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6X\": 22.208955764770508, \"playerIndex6Y\": 25.074626922607422, \"playerIndex7\": \"Bot 7\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7X\": 22.208955764770508, \"playerIndex7Y\": 16.477611541748047}";
		std::string beginGameMessageString = ss.str();
		const char *beginGameMessage = beginGameMessageString.c_str();

		m_gameScreen->handleServerUpdate(beginGameMessage);

		m_mapType++;
		if (m_mapType >= 3)
		{
			m_mapType = 0;
		}
	}

	void Direct3DInterop::handleDeathForPlayerIndex(int playerIndex)
	{
		m_playersAlive[playerIndex] = false;

		int numAlive = 0;
		int winningPlayerIndex = -1;
		for (int i = 0; i < 8; i++)
		{
			if (m_playersAlive[i])
			{
				winningPlayerIndex = i;
				numAlive++;
			}
		}

		if (numAlive <= 1)
		{
			bool hasWinner = numAlive == 1;

			std::stringstream ss;
			ss << "{\"eventType\": ";
			ss << GAME_OVER;
			ss << ", \"hasWinner\": ";
			ss << (hasWinner ? "true" : "false");
			ss << ", \"winningPlayerIndex\": ";
			ss << winningPlayerIndex;
			ss << "}";
			std::string gameOverMessageString = ss.str();
			const char *gameOverMessage = gameOverMessageString.c_str();

			m_gameScreen->handleServerUpdate(gameOverMessage);
		}
	}
}