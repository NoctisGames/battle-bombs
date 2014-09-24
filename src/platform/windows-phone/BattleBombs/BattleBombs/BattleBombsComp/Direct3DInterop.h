#pragma once

#include "pch.h"
#include "BasicTimer.h"
#include "Direct3DGameScreen.h"
#include "TouchEvent.h"
#include <DrawingSurfaceNative.h>

namespace BattleBombsComp
{
	public delegate void RequestAdditionalFrameHandler();
	public delegate void RecreateSynchronizedTextureHandler();
	public delegate void WinRtCallback(Platform::String^ command, Platform::String^ param);

	[Windows::Foundation::Metadata::WebHostHidden]
	public ref class Direct3DInterop sealed : public Windows::Phone::Input::Interop::IDrawingSurfaceManipulationHandler
	{
	public:
		Direct3DInterop();

		Windows::Phone::Graphics::Interop::IDrawingSurfaceContentProvider^ CreateContentProvider(Platform::String^ username, bool isOffline);

		// IDrawingSurfaceManipulationHandler
		virtual void SetManipulationHost(Windows::Phone::Input::Interop::DrawingSurfaceManipulationHost^ manipulationHost);

		event RequestAdditionalFrameHandler^ RequestAdditionalFrame;
		event RecreateSynchronizedTextureHandler^ RecreateSynchronizedTexture;

		bool onBackPressed();

		void onChatReceived(Platform::String^ message);

		// Add callback method here to be invoked when a chat message needs to be sent
		void setWinRtCallback(WinRtCallback^ callback);

		property Windows::Foundation::Size WindowBounds;
		property Windows::Foundation::Size NativeResolution;
		property Windows::Foundation::Size RenderResolution;

	protected:
		// Event Handlers
		void OnPointerPressed(Windows::Phone::Input::Interop::DrawingSurfaceManipulationHost^ sender, Windows::UI::Core::PointerEventArgs^ args);
		void OnPointerMoved(Windows::Phone::Input::Interop::DrawingSurfaceManipulationHost^ sender, Windows::UI::Core::PointerEventArgs^ args);
		void OnPointerReleased(Windows::Phone::Input::Interop::DrawingSurfaceManipulationHost^ sender, Windows::UI::Core::PointerEventArgs^ args);

	internal:
		HRESULT STDMETHODCALLTYPE Connect(_In_ IDrawingSurfaceRuntimeHostNative* host);
		void STDMETHODCALLTYPE Disconnect();
		HRESULT STDMETHODCALLTYPE PrepareResources(_In_ const LARGE_INTEGER* presentTargetTime, _Out_ BOOL* contentDirty);
		HRESULT STDMETHODCALLTYPE GetTexture(_In_ const DrawingSurfaceSizeF* size, _Out_ IDrawingSurfaceSynchronizedTextureNative** synchronizedTexture, _Out_ DrawingSurfaceRectF* textureSubRectangle);
		ID3D11Texture2D* GetTexture();

	private:
		ComPtr<ID3D11Device1> dev;                      // the device interface
		ComPtr<ID3D11DeviceContext1> devcon;            // the device context interface
		ComPtr<ID3D11Texture2D> m_renderTarget;         // the render target texture
		ComPtr<ID3D11RenderTargetView> rendertarget;    // the render target interface
		WinRtCallback^ m_winRtCallback;
		std::unique_ptr<Direct3DGameScreen> m_gameScreen;
		BasicTimer^ m_timer;
		Windows::Foundation::Size m_renderResolution;
		bool m_isOffline;
		char *m_username;
		const char *m_gameOverMessage;
		bool m_playersAlive[8];
		std::vector<TouchEvent> m_touchEvents;
		std::vector<TouchEvent> m_touchEventsPool;
		std::vector<TouchEvent> m_touchEventsBuffer;

		void pushEvents();

		void handleGameStateOne();

		void handleDeathForPlayerIndex(int playerIndex);

		void addTouchEventForType(Touch_Type touchType, float x, float y);

		TouchEvent newTouchEvent();
	};
}