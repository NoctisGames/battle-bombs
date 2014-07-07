#include "pch.h"
#include "Direct3DInterop.h"
#include "Direct3DContentProvider.h"
#include "GameButton.h"
#include <string.h>

using namespace Windows::Foundation;
using namespace Windows::UI::Core;
using namespace Microsoft::WRL;
using namespace Windows::Phone::Graphics::Interop;
using namespace Windows::Phone::Input::Interop;

namespace BomberPartyComp
{
	Direct3DInterop::Direct3DInterop() : m_timer(ref new BasicTimer())
	{
	}

	IDrawingSurfaceContentProvider^ Direct3DInterop::CreateContentProvider(Platform::String^ username)
	{
		std::wstring fooW(username->Begin());
		std::string fooA(fooW.begin(), fooW.end());
		const char *usernameCharArray = fooA.c_str();

		m_gameScreen = std::unique_ptr<Direct3DGameScreen>(new Direct3DGameScreen(usernameCharArray, (int)NativeResolution.Width, (int)NativeResolution.Height));
		m_isOnline = !username->Equals("Player_Offline");

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
			//m_gameScreen = std::unique_ptr<Direct3DGameScreen>(new Direct3DGameScreen((int)NativeResolution.Width, (int)NativeResolution.Height));
			//m_direct3DBase->loadScreen(*m_gameScreen);
		}
		else if (gameState == 2)
		{
			// Exiting Status
		}
		else if (gameState == 3)
		{
			m_gameScreen->clearState();
			m_winRtCallback->Invoke("GAME_OVER", "NULL");
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
		if (m_isOnline)
		{
			short eventId = m_gameScreen->popOldestEventId();
			if (eventId > 0)
			{
				Platform::String^ eventsMessage = eventId + ",";
				while ((eventId = m_gameScreen->popOldestEventId()) > 0)
				{
					eventsMessage += eventId + ",";
				}

				eventsMessage += "0"; // Terminate with 0

				static Platform::String^ EVENT_TYPE = "eventType";
				static int CLIENT_UPDATE = 1338;
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
}