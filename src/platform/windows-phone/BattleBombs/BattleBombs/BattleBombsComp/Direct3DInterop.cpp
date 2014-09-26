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
#include "DisplayBattleGameObject.h"
#include "DisplayGameOverGameObject.h"
#include "PlayerRow.h"
#include "PlayerRowPlatformAvatar.h"
#include "Direct3DRenderer.h"
#include "Global.h"
#include "SpriteBatcher.h"
#include "RectangleBatcher.h"
#include "Direct3DAssets.h"
#include "GameSound.h"
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

		int usernameLength = (int)strlen(usernameCharArray);

		m_username = new char[usernameLength];

		std::strncpy(m_username, usernameCharArray, usernameLength);
		m_username[usernameLength] = '\0';

		m_isOffline = isOffline;
		m_gameScreen = std::unique_ptr<Direct3DGameScreen>(new Direct3DGameScreen(usernameCharArray, isOffline));

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

				m_winRtCallback->Invoke("SEND_CHAT", "KEEP_ALIVE,3");
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

		srand((int)time(NULL));

		int _beginGameMessagesIndex = rand() % 12;
		
		if (_beginGameMessagesIndex == 0)
		{
			std::stringstream ss;
			ss << "{\"breakableBlockPowerUpFlags\": \"3,3,1,5,3,0,1,0,0,0,0,0,0,0,0,0,0,3,0,0,0,0,0,0,0,1,5,0,0,0,0,3,0,4,0,0,0,0,3,0,0,5,3,4,0,0,0,0,0,0,2,0,0,4,0,1,0,3,2,4,0,0,0,1,2,0,4,0,0,0,0,1,2,0,0,0,0,0,3,4,0,0,0,0,0,0,0,1,3,0,0,3,0,0,0,1,0,2,0,0,0,0,0,0\", \"breakableBlockXValues\": \"6,8,4,5,7,9,10,5,7,11,0,2,3,4,5,6,7,8,10,13,14,2,4,8,10,12,2,3,4,5,6,7,9,10,11,12,14,6,10,4,5,8,9,11,2,6,8,10,2,4,6,8,12,2,4,7,8,9,10,12,2,8,10,12,0,1,2,4,5,6,7,8,9,10,12,14,0,2,4,6,12,14,2,3,4,6,7,9,11,12,14,2,4,6,8,10,12,3,4,5,6,8,9,11\", \"breakableBlockYValues\": \"0,0,1,1,1,1,1,2,2,2,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,6,6,7,7,7,7,7,8,8,8,8,9,9,9,9,9,10,10,10,10,10,10,10,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,16,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 0, \"numBreakableBlocks\": 104, \"numClientBots\": 7, \"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"";
			ss << m_username;
			ss << "\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 22.208955764770508, \"playerIndex0Y\": 25.074626922607422, \"playerIndex1\": \"Norman Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 22.208955764770508, \"playerIndex1Y\": 12.179104804992676, \"playerIndex2\": \"Jay Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 2.1492538452148438, \"playerIndex2Y\": 12.179104804992676, \"playerIndex3\": \"Dave Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 2.1492538452148438, \"playerIndex3Y\": 25.074626922607422, \"playerIndex4\": \"Mikel Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 17.91044807434082, \"playerIndex4Y\": 2.1492538452148438, \"playerIndex5\": \"Don Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 16.477611541748047, \"playerIndex6\": \"Brandi Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 6.447761058807373, \"playerIndex6Y\": 2.1492538452148438, \"playerIndex7\": \"Kathy Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 22.208955764770508, \"playerIndex7Y\": 16.477611541748047}";
			std::string beginGameMessageString = ss.str();
			const char *beginGameMessage = beginGameMessageString.c_str();

			m_gameScreen->handleServerUpdate(beginGameMessage);
		}
		else if (_beginGameMessagesIndex == 1)
		{
			std::stringstream ss;
			ss << "{\"breakableBlockPowerUpFlags\": \"0,5,0,4,0,0,4,0,0,0,0,0,5,4,3,0,2,0,1,0,0,0,3,0,0,5,1,0,0,0,0,1,0,0,0,0,0,0,0,3,0,1,0,0,4,2,0,0,0,0,0,0,0,2,1,0,0,4,0,0,3,0,0,0,0,0,3,0,0,0,0,0,0,2,2,0,0,4,0,0,2,0,2,0,0,0,0,0,4,0,4,3,1,0,0,5,0,3,0,0,0,0,3,0,1,0\", \"breakableBlockXValues\": \"6,4,6,8,9,10,3,5,7,9,11,0,2,4,6,7,9,11,13,0,2,4,10,14,0,1,2,5,6,7,8,9,11,12,4,6,8,12,2,3,5,6,9,10,11,12,2,4,6,10,12,2,4,6,10,2,4,5,6,7,10,11,12,8,10,12,0,1,3,5,6,9,11,12,14,2,4,6,8,10,12,14,2,5,6,7,9,10,11,13,14,2,6,8,10,12,2,3,4,5,7,8,9,10,11,12\", \"breakableBlockYValues\": \"0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,3,3,3,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,7,7,7,7,7,7,7,7,8,8,8,8,8,9,9,9,9,10,10,10,10,10,10,10,10,11,11,11,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,15,15,15,15,15,16,16,16,16,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 1, \"numBreakableBlocks\": 106, \"numClientBots\": 7, \"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"";
			ss << m_username;
			ss << "\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 2.1492538452148438, \"playerIndex0Y\": 25.074626922607422, \"playerIndex1\": \"Norman Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 17.91044807434082, \"playerIndex1Y\": 2.1492538452148438, \"playerIndex2\": \"Kenny Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 2.1492538452148438, \"playerIndex2Y\": 16.477611541748047, \"playerIndex3\": \"Adam Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 22.208955764770508, \"playerIndex3Y\": 12.179104804992676, \"playerIndex4\": \"Don Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 22.208955764770508, \"playerIndex4Y\": 25.074626922607422, \"playerIndex5\": \"Dexter Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 22.208955764770508, \"playerIndex5Y\": 16.477611541748047, \"playerIndex6\": \"Tyler Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 2.1492538452148438, \"playerIndex6Y\": 12.179104804992676, \"playerIndex7\": \"Peter Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 6.447761058807373, \"playerIndex7Y\": 2.1492538452148438}";
			std::string beginGameMessageString = ss.str();
			const char *beginGameMessage = beginGameMessageString.c_str();

			m_gameScreen->handleServerUpdate(beginGameMessage);
		}
		else if (_beginGameMessagesIndex == 2)
		{
			std::stringstream ss;
			ss << "{\"breakableBlockPowerUpFlags\": \"0,0,3,0,3,0,0,0,4,0,0,2,0,4,5,0,5,0,0,0,0,3,0,0,0,1,0,3,0,3,2,0,0,5,4,0,1,0,0,0,1,0,0,0,0,2,0,0,3,4,1,0,1,0,2,0,0,0,0,4,1,2,0,4,3,1,3,0,0,0,3,0,1,0,0,3,0,0,0,4,2,5,0,0,0,0,0,0,0,0,1,0,4,0,0,1,0,4,2,0,0,3,1,3,0,0,2,0,5\", \"breakableBlockXValues\": \"8,4,5,7,8,9,3,5,7,9,11,0,2,3,5,6,7,8,9,14,0,2,4,8,10,14,0,2,3,4,6,7,8,9,10,11,2,4,6,8,10,12,3,4,5,7,9,10,11,2,4,8,10,12,4,8,10,12,2,5,6,7,8,9,11,12,2,4,6,8,10,12,0,1,2,4,6,7,8,2,4,10,12,1,2,3,5,6,7,9,10,11,12,13,14,2,4,5,6,7,10,12,2,3,4,5,9,10,11\", \"breakableBlockYValues\": \"0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,7,7,7,7,7,7,7,8,8,8,8,8,9,9,9,9,10,10,10,10,10,10,10,10,11,11,11,11,11,11,12,12,12,12,12,12,12,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,16,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 2, \"numBreakableBlocks\": 109, \"numClientBots\": 7, \"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"";
			ss << m_username;
			ss << "\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 2.1492538452148438, \"playerIndex0Y\": 25.074626922607422, \"playerIndex1\": \"Bobby Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 17.91044807434082, \"playerIndex1Y\": 2.1492538452148438, \"playerIndex2\": \"Aaron Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 2.1492538452148438, \"playerIndex2Y\": 12.179104804992676, \"playerIndex3\": \"Matt Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 22.208955764770508, \"playerIndex3Y\": 25.074626922607422, \"playerIndex4\": \"Don Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 22.208955764770508, \"playerIndex4Y\": 12.179104804992676, \"playerIndex5\": \"Kenny Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 16.477611541748047, \"playerIndex6\": \"Dylan Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 6.447761058807373, \"playerIndex6Y\": 2.1492538452148438, \"playerIndex7\": \"Dan Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 22.208955764770508, \"playerIndex7Y\": 16.477611541748047}";
			std::string beginGameMessageString = ss.str();
			const char *beginGameMessage = beginGameMessageString.c_str();

			m_gameScreen->handleServerUpdate(beginGameMessage);
		}
		else if (_beginGameMessagesIndex == 3)
		{
			std::stringstream ss;
			ss << "{\"breakableBlockPowerUpFlags\": \"0,0,0,0,5,0,2,3,0,2,2,2,0,3,0,0,5,0,4,2,3,2,0,0,2,0,3,0,3,2,0,0,0,4,2,0,0,2,0,0,0,0,0,3,0,2,0,0,0,0,0,0,0,0,2,0,0,0,0,0,2,4,2,0,0,2,0,0,2,1,5,2,0,0,0,0,0,0,4,3,5,0,0,0,0,0,0,0,2,0,0,3,0,0,3,3,0,0,0,0,0,5,5,0,0,0,0,0,0,4,0,1\", \"breakableBlockXValues\": \"6,4,5,7,8,9,10,3,7,2,3,6,7,8,9,11,12,13,0,2,4,6,8,10,12,14,1,4,7,8,12,13,14,2,6,8,10,12,2,3,4,6,8,9,10,12,2,4,6,8,10,12,2,6,10,12,2,3,4,5,6,7,8,10,11,2,4,6,8,10,12,1,3,4,5,6,7,8,9,11,13,14,0,4,6,8,10,12,14,0,2,3,4,5,7,8,9,12,13,2,4,6,8,12,3,4,6,7,8,9,11,12\", \"breakableBlockYValues\": \"0,1,1,1,1,1,1,2,2,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,4,5,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,7,7,7,7,8,8,8,8,8,8,9,9,9,9,10,10,10,10,10,10,10,10,10,11,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,16,16,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 0, \"numBreakableBlocks\": 112, \"numClientBots\": 7, \"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"";
			ss << m_username;
			ss << "\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 17.91044807434082, \"playerIndex0Y\": 2.1492538452148438, \"playerIndex1\": \"Ryan Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 2.1492538452148438, \"playerIndex1Y\": 25.074626922607422, \"playerIndex2\": \"Wesley Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 6.447761058807373, \"playerIndex2Y\": 2.1492538452148438, \"playerIndex3\": \"Dean Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 22.208955764770508, \"playerIndex3Y\": 25.074626922607422, \"playerIndex4\": \"Kurt Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 2.1492538452148438, \"playerIndex4Y\": 16.477611541748047, \"playerIndex5\": \"Jay Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 12.179104804992676, \"playerIndex6\": \"Kyle Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 22.208955764770508, \"playerIndex6Y\": 16.477611541748047, \"playerIndex7\": \"Peter Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 22.208955764770508, \"playerIndex7Y\": 12.179104804992676}";
			std::string beginGameMessageString = ss.str();
			const char *beginGameMessage = beginGameMessageString.c_str();

			m_gameScreen->handleServerUpdate(beginGameMessage);
		}
		else if (_beginGameMessagesIndex == 4)
		{
			std::stringstream ss;
			ss << "{\"breakableBlockPowerUpFlags\": \"0,3,2,0,0,0,2,3,0,0,0,0,0,5,2,1,0,5,4,1,1,2,0,2,1,0,5,5,0,0,0,4,2,0,0,0,0,0,0,4,0,0,4,4,5,0,0,0,0,0,2,0,2,0,0,3,0,0,2,4,0,1,0,1,1,0,4,0,0,0,5,0,0,0,0,0,0,0,0,2,0,0,5,0,5,0,1,2,0,0,0,0,1,0,0,0,0,0,0,3,1,0,0,0,0,3,5,0,0,1,0,1,0,2\", \"breakableBlockXValues\": \"8,6,7,10,3,5,9,11,0,1,2,3,4,5,6,7,8,11,13,14,2,4,10,12,0,1,2,4,6,7,8,9,10,11,12,14,2,4,6,8,12,4,5,6,7,8,9,2,4,8,12,2,4,8,10,2,3,4,5,7,8,10,11,12,2,4,6,8,10,0,1,2,3,4,5,6,8,10,11,13,14,0,4,6,8,10,12,14,0,1,2,3,4,5,6,7,8,11,12,13,14,2,4,6,8,3,4,5,7,8,9,10,11,12\", \"breakableBlockYValues\": \"0,1,1,1,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,7,7,8,8,8,8,9,9,9,9,10,10,10,10,10,10,10,10,10,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,16,16,16,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 1, \"numBreakableBlocks\": 114, \"numClientBots\": 7, \"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"";
			ss << m_username;
			ss << "\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 22.208955764770508, \"playerIndex0Y\": 12.179104804992676, \"playerIndex1\": \"Heather Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 2.1492538452148438, \"playerIndex1Y\": 12.179104804992676, \"playerIndex2\": \"Adam Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 22.208955764770508, \"playerIndex2Y\": 16.477611541748047, \"playerIndex3\": \"Morgan Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 2.1492538452148438, \"playerIndex3Y\": 16.477611541748047, \"playerIndex4\": \"Mikel Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 22.208955764770508, \"playerIndex4Y\": 25.074626922607422, \"playerIndex5\": \"Jay Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 17.91044807434082, \"playerIndex5Y\": 2.1492538452148438, \"playerIndex6\": \"Ryan Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 2.1492538452148438, \"playerIndex6Y\": 25.074626922607422, \"playerIndex7\": \"Dick Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 6.447761058807373, \"playerIndex7Y\": 2.1492538452148438}";
			std::string beginGameMessageString = ss.str();
			const char *beginGameMessage = beginGameMessageString.c_str();

			m_gameScreen->handleServerUpdate(beginGameMessage);
		}
		else if (_beginGameMessagesIndex == 5)
		{
			std::stringstream ss;
			ss << "{\"breakableBlockPowerUpFlags\": \"0,1,0,0,0,0,0,3,0,2,0,1,2,0,2,0,0,0,0,2,5,0,0,0,0,0,0,5,2,0,0,0,0,0,0,0,0,0,3,0,0,0,0,3,0,0,0,0,0,0,0,4,4,5,0,4,1,0,2,0,0,0,0,0,0,0,5,0,0,0,2,0,0,0,0,5,0,0,3,0,0,4,0,3,2,0,3,3,4,1,2,4,0,0,0,3,4,0,0,0,0,0,0,0,0,5,0,0,0,0,0,0,3,0,0,0,3,0\", \"breakableBlockXValues\": \"6,8,4,5,6,7,8,9,10,3,5,7,9,11,0,2,3,6,7,8,9,10,11,12,14,0,2,6,8,10,14,0,2,3,4,6,7,8,9,10,11,12,13,14,2,6,8,12,3,5,6,7,8,10,11,2,6,8,12,2,4,6,8,10,12,2,3,4,5,6,7,8,9,10,11,4,8,10,0,1,2,3,4,5,6,9,13,14,0,2,4,6,8,10,14,0,1,2,4,5,6,7,8,9,10,12,14,2,4,5,6,7,8,12,4,5,10,12\", \"breakableBlockYValues\": \"0,0,1,1,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,7,7,7,7,7,7,7,8,8,8,8,9,9,9,9,9,9,10,10,10,10,10,10,10,10,10,10,11,11,11,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,16,16,16,16\", \"eventType\": 1337, \"mapType\": 2, \"numBreakableBlocks\": 118, \"numClientBots\": 7, \"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"";
			ss << m_username;
			ss << "\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 17.91044807434082, \"playerIndex0Y\": 2.1492538452148438, \"playerIndex1\": \"Dean Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 2.1492538452148438, \"playerIndex1Y\": 16.477611541748047, \"playerIndex2\": \"Bradlee Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 6.447761058807373, \"playerIndex2Y\": 2.1492538452148438, \"playerIndex3\": \"Ryan Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 2.1492538452148438, \"playerIndex3Y\": 12.179104804992676, \"playerIndex4\": \"Jason Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 22.208955764770508, \"playerIndex4Y\": 16.477611541748047, \"playerIndex5\": \"Aaron Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 22.208955764770508, \"playerIndex5Y\": 12.179104804992676, \"playerIndex6\": \"Marshall Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 22.208955764770508, \"playerIndex6Y\": 25.074626922607422, \"playerIndex7\": \"Travis Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 2.1492538452148438, \"playerIndex7Y\": 25.074626922607422}";
			std::string beginGameMessageString = ss.str();
			const char *beginGameMessage = beginGameMessageString.c_str();

			m_gameScreen->handleServerUpdate(beginGameMessage);
		}
		else if (_beginGameMessagesIndex == 6)
		{
			std::stringstream ss;
			ss << "{\"breakableBlockPowerUpFlags\": \"0,0,0,0,0,0,1,0,1,3,3,0,5,0,0,3,0,0,0,0,0,0,3,0,0,2,4,0,0,1,0,5,1,0,0,0,0,0,0,0,3,0,1,1,0,0,0,0,1,0,0,0,1,0,3,2,1,2,0,4,0,5,0,5,0,0,1,4,0,0,4,0,0,2,0,0,2,0,0,2,2,2,1,0,0,3,2,0,0,0,0,4,3,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,3,0,4\", \"breakableBlockXValues\": \"6,8,5,7,8,9,10,3,5,7,9,11,0,2,3,4,5,6,7,9,10,11,12,13,0,4,8,10,12,1,2,6,7,8,9,10,14,2,4,6,8,12,3,4,5,6,7,8,9,11,12,2,4,8,10,12,2,4,6,2,3,4,6,7,8,9,10,11,12,2,4,6,8,10,12,1,2,6,8,9,10,11,14,2,6,8,10,12,1,3,4,5,6,7,8,9,10,11,12,13,14,2,4,6,10,12,3,6,9,10,12\", \"breakableBlockYValues\": \"0,0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,5,5,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,7,7,7,7,7,8,8,8,8,8,9,9,9,10,10,10,10,10,10,10,10,10,10,11,11,11,11,11,11,12,12,12,12,12,12,12,12,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 0, \"numBreakableBlocks\": 111, \"numClientBots\": 7, \"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"";
			ss << m_username;
			ss << "\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 22.208955764770508, \"playerIndex0Y\": 25.074626922607422, \"playerIndex1\": \"Roy Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 2.1492538452148438, \"playerIndex1Y\": 25.074626922607422, \"playerIndex2\": \"Bobby Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 22.208955764770508, \"playerIndex2Y\": 16.477611541748047, \"playerIndex3\": \"Todd Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 22.208955764770508, \"playerIndex3Y\": 12.179104804992676, \"playerIndex4\": \"Debra Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 6.447761058807373, \"playerIndex4Y\": 2.1492538452148438, \"playerIndex5\": \"Mariel Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 16.477611541748047, \"playerIndex6\": \"Kenny Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 17.91044807434082, \"playerIndex6Y\": 2.1492538452148438, \"playerIndex7\": \"Dante Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 2.1492538452148438, \"playerIndex7Y\": 12.179104804992676}";
			std::string beginGameMessageString = ss.str();
			const char *beginGameMessage = beginGameMessageString.c_str();

			m_gameScreen->handleServerUpdate(beginGameMessage);
		}
		else if (_beginGameMessagesIndex == 7)
		{
			std::stringstream ss;
			ss << "{\"breakableBlockPowerUpFlags\": \"0,0,0,3,3,0,0,0,0,0,0,4,2,0,0,0,0,4,0,0,2,0,2,0,2,0,3,0,0,4,2,0,1,0,2,0,0,1,1,1,0,0,3,0,4,0,0,0,1,0,3,0,3,0,5,2,0,0,0,5,0,0,0,0,2,2,0,1,3,0,0,0,0,0,0,0,2,0,0,0,0,5,0,0,0,4,0,0,2,4,3,0,0,0,0,3,5,0,2,0,0,0,3,0,0\", \"breakableBlockXValues\": \"8,5,6,8,9,10,5,7,0,1,3,5,6,7,8,11,14,0,2,4,6,8,10,12,1,2,3,4,5,6,7,8,10,11,14,2,4,6,8,10,12,2,3,4,5,6,7,8,9,10,11,12,4,10,2,6,12,2,3,4,6,7,8,9,12,4,6,8,10,0,1,2,4,7,9,10,11,12,13,14,0,2,4,6,8,14,2,4,5,9,10,12,2,8,10,2,3,4,6,7,8,9,10,11,12\", \"breakableBlockYValues\": \"0,1,1,1,1,1,2,2,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,7,7,7,7,7,7,7,7,7,7,7,8,8,9,9,9,10,10,10,10,10,10,10,10,11,11,11,11,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,14,14,14,14,14,14,15,15,15,16,16,16,16,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 1, \"numBreakableBlocks\": 105, \"numClientBots\": 7, \"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"";
			ss << m_username;
			ss << "\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 2.1492538452148438, \"playerIndex0Y\": 16.477611541748047, \"playerIndex1\": \"Bill Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 22.208955764770508, \"playerIndex1Y\": 25.074626922607422, \"playerIndex2\": \"Stephen Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 2.1492538452148438, \"playerIndex2Y\": 25.074626922607422, \"playerIndex3\": \"Morgan Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 22.208955764770508, \"playerIndex3Y\": 16.477611541748047, \"playerIndex4\": \"Kurt Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 22.208955764770508, \"playerIndex4Y\": 12.179104804992676, \"playerIndex5\": \"Robin Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 12.179104804992676, \"playerIndex6\": \"Sam Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 6.447761058807373, \"playerIndex6Y\": 2.1492538452148438, \"playerIndex7\": \"Tyler Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 17.91044807434082, \"playerIndex7Y\": 2.1492538452148438}";
			std::string beginGameMessageString = ss.str();
			const char *beginGameMessage = beginGameMessageString.c_str();

			m_gameScreen->handleServerUpdate(beginGameMessage);
		}
		else if (_beginGameMessagesIndex == 8)
		{
			std::stringstream ss;
			ss << "{\"breakableBlockPowerUpFlags\": \"0,0,4,0,4,0,0,0,4,0,5,4,1,0,1,0,1,0,0,0,0,0,0,2,2,0,0,0,0,0,0,0,3,2,0,2,3,0,0,1,0,0,1,0,5,0,0,0,0,0,1,0,0,0,3,0,1,0,0,2,0,0,0,0,3,0,2,0,4,1,0,0,0,0,0,0,0,0,0,2,0,0,0,2,0,5,2,5,0,1,0,4,1,0,5,0,0,5,5,0,0,0,0\", \"breakableBlockXValues\": \"6,8,4,5,7,9,10,3,5,7,9,11,0,2,3,4,10,11,13,0,4,10,14,1,5,6,7,8,9,12,13,14,2,8,10,2,3,5,6,7,9,10,11,12,2,4,6,10,8,2,3,4,5,6,7,8,10,11,12,4,6,0,2,3,4,6,7,8,10,12,13,14,0,4,6,10,12,1,2,3,4,5,8,9,10,11,12,13,14,2,4,5,7,8,9,10,12,2,3,5,10,11,12\", \"breakableBlockYValues\": \"0,0,1,1,1,1,1,2,2,2,2,2,3,3,3,3,3,3,3,4,4,4,4,5,5,5,5,5,5,5,5,5,6,6,6,7,7,7,7,7,7,7,7,7,8,8,8,8,9,10,10,10,10,10,10,10,10,10,10,11,11,12,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 2, \"numBreakableBlocks\": 103, \"numClientBots\": 7, \"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"";
			ss << m_username;
			ss << "\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 2.1492538452148438, \"playerIndex0Y\": 16.477611541748047, \"playerIndex1\": \"Matt Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 17.91044807434082, \"playerIndex1Y\": 2.1492538452148438, \"playerIndex2\": \"Stan Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 6.447761058807373, \"playerIndex2Y\": 2.1492538452148438, \"playerIndex3\": \"Wesley Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 22.208955764770508, \"playerIndex3Y\": 12.179104804992676, \"playerIndex4\": \"Jason Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 22.208955764770508, \"playerIndex4Y\": 16.477611541748047, \"playerIndex5\": \"Bruce Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 2.1492538452148438, \"playerIndex5Y\": 12.179104804992676, \"playerIndex6\": \"Pier Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 2.1492538452148438, \"playerIndex6Y\": 25.074626922607422, \"playerIndex7\": \"Don Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 22.208955764770508, \"playerIndex7Y\": 25.074626922607422}";
			std::string beginGameMessageString = ss.str();
			const char *beginGameMessage = beginGameMessageString.c_str();

			m_gameScreen->handleServerUpdate(beginGameMessage);
		}
		else if (_beginGameMessagesIndex == 9)
		{
			std::stringstream ss;
			ss << "{\"breakableBlockPowerUpFlags\": \"0,1,0,0,0,0,0,0,0,0,0,0,0,4,0,0,0,2,0,0,2,0,0,0,0,0,3,2,0,0,0,0,5,4,5,4,0,2,4,0,0,0,1,0,0,2,0,2,0,0,3,0,3,3,0,5,0,3,0,0,0,3,2,0,4,0,0,0,3,0,3,2,5,0,1,0,0,2,0,1,0,3,0,0,4,2,0,0,3,2,0,2,0,0,0,2,0,1\", \"breakableBlockXValues\": \"6,8,4,5,6,3,7,0,2,3,6,7,8,9,11,12,13,4,6,8,10,12,0,1,2,4,5,6,7,9,11,12,4,6,10,2,4,5,9,11,12,2,4,6,2,4,6,10,2,4,5,6,7,9,2,4,6,8,10,0,3,4,5,6,7,8,11,13,14,0,2,4,6,8,10,12,14,0,1,3,4,5,6,8,9,10,11,12,13,6,8,10,2,3,5,6,7,11\", \"breakableBlockYValues\": \"0,0,1,1,1,2,2,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,7,7,7,7,7,7,8,8,8,9,9,9,9,10,10,10,10,10,10,11,11,11,11,11,12,12,12,12,12,12,12,12,12,12,13,13,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,14,15,15,15,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 0, \"numBreakableBlocks\": 98, \"numClientBots\": 7, \"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"";
			ss << m_username;
			ss << "\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 22.208955764770508, \"playerIndex0Y\": 16.477611541748047, \"playerIndex1\": \"Todd Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 2.1492538452148438, \"playerIndex1Y\": 16.477611541748047, \"playerIndex2\": \"Kathy Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 6.447761058807373, \"playerIndex2Y\": 2.1492538452148438, \"playerIndex3\": \"James Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 2.1492538452148438, \"playerIndex3Y\": 25.074626922607422, \"playerIndex4\": \"Kenny Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 2.1492538452148438, \"playerIndex4Y\": 12.179104804992676, \"playerIndex5\": \"Mariel Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 22.208955764770508, \"playerIndex5Y\": 12.179104804992676, \"playerIndex6\": \"Sam Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 22.208955764770508, \"playerIndex6Y\": 25.074626922607422, \"playerIndex7\": \"Mary Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 17.91044807434082, \"playerIndex7Y\": 2.1492538452148438}";
			std::string beginGameMessageString = ss.str();
			const char *beginGameMessage = beginGameMessageString.c_str();

			m_gameScreen->handleServerUpdate(beginGameMessage);
		}
		else if (_beginGameMessagesIndex == 10)
		{
			std::stringstream ss;
			ss << "{\"breakableBlockPowerUpFlags\": \"5,0,0,0,0,0,5,0,0,0,0,0,0,0,0,0,3,4,1,5,0,3,1,5,0,0,1,1,3,0,0,0,3,3,0,0,4,0,0,0,0,3,0,0,5,1,0,0,3,0,0,0,1,1,2,0,0,0,4,0,2,4,1,0,0,0,4,0,0,0,0,0,3,0,0,5,0,0,2,0,0,3,0,0,0,3,0,3,0,0,1,0,0,0,0,0,0,0,0,2,0,3,0,2,0,0,0,0,1\", \"breakableBlockXValues\": \"6,8,5,6,8,9,10,3,5,0,1,2,4,5,7,8,11,13,14,2,10,14,0,3,4,7,8,9,10,12,13,14,2,4,6,8,10,12,2,4,6,7,8,10,11,6,8,10,12,2,4,6,8,10,2,3,5,6,7,8,10,11,12,2,4,6,8,10,12,0,2,5,7,8,9,11,13,14,4,6,8,10,14,1,2,3,4,5,6,7,9,10,11,13,2,4,6,8,10,12,2,5,6,7,8,9,10,11,12\", \"breakableBlockYValues\": \"0,0,1,1,1,1,1,2,2,3,3,3,3,3,3,3,3,3,3,4,4,4,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,6,7,7,7,7,7,7,7,8,8,8,8,9,9,9,9,9,10,10,10,10,10,10,10,10,10,11,11,11,11,11,11,12,12,12,12,12,12,12,12,12,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,16,16,16,16,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 1, \"numBreakableBlocks\": 109, \"numClientBots\": 7, \"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"";
			ss << m_username;
			ss << "\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 22.208955764770508, \"playerIndex0Y\": 16.477611541748047, \"playerIndex1\": \"Morgan Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 22.208955764770508, \"playerIndex1Y\": 12.179104804992676, \"playerIndex2\": \"Dick Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 6.447761058807373, \"playerIndex2Y\": 2.1492538452148438, \"playerIndex3\": \"Mike Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 2.1492538452148438, \"playerIndex3Y\": 25.074626922607422, \"playerIndex4\": \"Joe Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 17.91044807434082, \"playerIndex4Y\": 2.1492538452148438, \"playerIndex5\": \"Brandi Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 22.208955764770508, \"playerIndex5Y\": 25.074626922607422, \"playerIndex6\": \"Jay Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 2.1492538452148438, \"playerIndex6Y\": 16.477611541748047, \"playerIndex7\": \"Roy Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 2.1492538452148438, \"playerIndex7Y\": 12.179104804992676}";
			std::string beginGameMessageString = ss.str();
			const char *beginGameMessage = beginGameMessageString.c_str();

			m_gameScreen->handleServerUpdate(beginGameMessage);
		}
		else if (_beginGameMessagesIndex == 11)
		{
			std::stringstream ss;
			ss << "{\"breakableBlockPowerUpFlags\": \"2,3,1,0,0,0,0,1,5,2,4,0,0,0,0,3,0,0,0,2,0,3,0,0,0,4,1,0,4,0,0,1,0,3,0,0,0,2,0,0,0,3,0,4,2,0,0,3,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,0,5,2,0,0,0,1,5,0,1,2,0,0,0,4,0,0,0,0,0,3,4,1,0,5,0,0,0,5,3,5,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,1,1,0,3\", \"breakableBlockXValues\": \"8,5,6,7,8,9,10,3,7,9,11,1,2,3,5,6,7,8,9,10,11,13,14,0,2,4,6,12,14,0,1,2,4,5,6,7,9,10,11,12,13,14,4,6,8,10,12,2,3,4,5,7,8,12,2,4,8,10,12,2,4,6,8,10,12,2,3,4,5,6,7,8,9,10,12,2,4,0,3,4,6,8,9,13,14,2,4,6,10,12,14,2,3,4,5,7,8,9,11,12,13,2,4,6,7,8,9,10,12,4,9,10,11,12\", \"breakableBlockYValues\": \"0,1,1,1,1,1,1,2,2,2,2,3,3,3,3,3,3,3,3,3,3,3,3,4,4,4,4,4,4,5,5,5,5,5,5,5,5,5,5,5,5,5,6,6,6,6,6,7,7,7,7,7,7,7,8,8,8,8,8,9,9,9,9,9,9,10,10,10,10,10,10,10,10,10,10,11,11,12,12,12,12,12,12,12,12,13,13,13,13,13,13,14,14,14,14,14,14,14,14,14,14,15,15,15,15,15,15,15,15,16,16,16,16,16\", \"eventType\": 1337, \"mapType\": 2, \"numBreakableBlocks\": 114, \"numClientBots\": 7, \"numPlayers\": 1, \"numSecondsLeftForRound\": 180, \"playerIndex0\": \"";
			ss << m_username;
			ss << "\", \"playerIndex0Alive\": true, \"playerIndex0Direction\": 0, \"playerIndex0IsBot\": false, \"playerIndex0X\": 2.1492538452148438, \"playerIndex0Y\": 16.477611541748047, \"playerIndex1\": \"Ruby Bot\", \"playerIndex1Alive\": true, \"playerIndex1Direction\": 2, \"playerIndex1IsBot\": true, \"playerIndex1X\": 6.447761058807373, \"playerIndex1Y\": 2.1492538452148438, \"playerIndex2\": \"Stephen Bot\", \"playerIndex2Alive\": true, \"playerIndex2Direction\": 0, \"playerIndex2IsBot\": true, \"playerIndex2X\": 2.1492538452148438, \"playerIndex2Y\": 12.179104804992676, \"playerIndex3\": \"Joe Bot\", \"playerIndex3Alive\": true, \"playerIndex3Direction\": 2, \"playerIndex3IsBot\": true, \"playerIndex3X\": 17.91044807434082, \"playerIndex3Y\": 2.1492538452148438, \"playerIndex4\": \"Wesley Bot\", \"playerIndex4Alive\": true, \"playerIndex4Direction\": 1, \"playerIndex4IsBot\": true, \"playerIndex4X\": 22.208955764770508, \"playerIndex4Y\": 12.179104804992676, \"playerIndex5\": \"Eric Bot\", \"playerIndex5Alive\": true, \"playerIndex5Direction\": 3, \"playerIndex5IsBot\": true, \"playerIndex5X\": 22.208955764770508, \"playerIndex5Y\": 16.477611541748047, \"playerIndex6\": \"Jay Bot\", \"playerIndex6Alive\": true, \"playerIndex6Direction\": 1, \"playerIndex6IsBot\": true, \"playerIndex6X\": 2.1492538452148438, \"playerIndex6Y\": 25.074626922607422, \"playerIndex7\": \"Kenny Bot\", \"playerIndex7Alive\": true, \"playerIndex7Direction\": 3, \"playerIndex7IsBot\": true, \"playerIndex7X\": 22.208955764770508, \"playerIndex7Y\": 25.074626922607422}";
			std::string beginGameMessageString = ss.str();
			const char *beginGameMessage = beginGameMessageString.c_str();

			m_gameScreen->handleServerUpdate(beginGameMessage);
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