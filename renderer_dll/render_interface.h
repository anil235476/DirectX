#ifndef _RENDER_INTERFACE_H__
#define _RENDER_INTERFACE_H__
#include <Windows.h>
#include <d3d11.h>
#include "utility/release_helper.h"

class render_interface {
public:

	bool create_device(HWND wnd);
	bool create_render_target();
	void upate();
private:
	// Device stuff
	util::release_helper<IDXGISwapChain> swap_chain_;
	util::release_helper<ID3D11Device> device_;
	util::release_helper<ID3D11DeviceContext> device_context_;
	//render target
	// Render target
	util::release_helper<ID3D11RenderTargetView> render_target_view_;
};


#endif//_RENDER_INTERFACE_H__
