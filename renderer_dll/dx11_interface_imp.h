#ifndef __DX11_INTERFACE_IMP_H_
#define __DX11_INTERFACE_IMP_H_
#include "render_interface.h"
#include "utility/release_helper.h"
#include <dxgi1_2.h>
#include <D3D11.h>

namespace renderer {
	class dx11_interface_imp : public render_interface {
	public:
		dx11_interface_imp();
		bool initialize(HWND wnd) override;
		void update() override;
	private:
		bool create_render_target();
		void begin_frame();
		void end_frame();
		util::release_helper<IDXGISwapChain> swap_chain_;
		util::release_helper<IDXGISwapChain1> swap_chain1_;
		util::release_helper<IDXGIFactory2> dxgiFactory2;
		util::release_helper<ID3D11Device> pd3dDevice;
		util::release_helper<ID3D11DeviceContext> pImmediateContext;
		//render target
		// Render target
		util::release_helper<ID3D11RenderTargetView> render_target_view_;
	};

}//namespace renderer

#endif//__DX11_INTERFACE_IMP_H_