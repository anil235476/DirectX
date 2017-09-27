#ifndef _DX_INTERFACE_IMP_H_
#define _DX_INTERFACE_IMP_H_
#include "render_interface.h"//for base class definition
#include <d3d11.h>
#include "utility/release_helper.h"

namespace renderer
{
	class dx_interface_imp : public render_interface
	{
	public:
		bool initialize(HWND wnd) override;
		void update() override;
	private:
		bool create_device(HWND wnd);
		bool create_render_target();
		void begin_frame();
		void end_frame();
		// Device stuff
		util::release_helper<IDXGISwapChain> swap_chain_;
		
		util::release_helper<ID3D11Device> device_;
		util::release_helper<ID3D11DeviceContext> device_context_;
		//render target
		// Render target
		util::release_helper<ID3D11RenderTargetView> render_target_view_;
	};
} //namespace renderer



#endif//_DX_INTERFACE_IMP_H_
