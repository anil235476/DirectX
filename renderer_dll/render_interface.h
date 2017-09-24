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
	IDXGISwapChain* swap_chain_{ nullptr };
	ID3D11Device* device_{ nullptr };
	ID3D11DeviceContext* device_context_{ nullptr };

	//render target
	// Render target
	ID3D11RenderTargetView* render_target_view_{ nullptr };
};


#endif//_RENDER_INTERFACE_H__
