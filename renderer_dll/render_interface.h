#ifndef _RENDER_INTERFACE_H__
#define _RENDER_INTERFACE_H__
#include <Windows.h>
#include <d3d11.h>

class render_interface {
public:

	bool create_device(HWND wnd);
	bool create_render_target();
	void upate();
private:
	// Device stuff
	IDXGISwapChain* m_swapChain = nullptr;
	ID3D11Device* m_device = nullptr;
	ID3D11DeviceContext* m_deviceContext = nullptr;

	//render target
	// Render target
	ID3D11RenderTargetView* m_renderTargetView = nullptr;
};


#endif//_RENDER_INTERFACE_H__
