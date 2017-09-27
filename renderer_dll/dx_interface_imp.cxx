#include "dx_interface_imp.h"
#include <cassert>

namespace renderer
{
	bool dx_interface_imp::initialize(HWND wnd) {
		if (create_device(wnd)) {
			return create_render_target();
		}

		assert(false);
		return false;
	}

	bool dx_interface_imp::create_device(HWND wnd) {
		// Define our swap chain
		DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = wnd;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.Windowed = true;


		// Create the swap chain, device and device context
		auto result = D3D11CreateDeviceAndSwapChain(
			nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0,
			nullptr, 0, D3D11_SDK_VERSION,
			&swapChainDesc, swap_chain_.ref(),
			device_.ref(), nullptr, device_context_.ref());

		// Check for error
		if (result != S_OK) {
			MessageBox(nullptr, "Error creating DX11", "Error", MB_OK);
			assert(false);
			exit(0);
		}

		return result == S_OK && create_render_target();
	}

	bool dx_interface_imp::create_render_target() {
		assert(swap_chain_);
		util::release_helper
			<
			ID3D11Texture2D
			>
			backBuffer;
		{
			const auto hr = swap_chain_->GetBuffer(
				0,
				__uuidof(ID3D11Texture2D),
				reinterpret_cast<void**>(backBuffer.ref())
			);
			assert(!FAILED(hr));
		}
		{
			const auto hr = device_->CreateRenderTargetView(
				backBuffer.get(), nullptr, render_target_view_.ref());
			assert(!FAILED(hr));
		}

		return render_target_view_.get() != nullptr;
	}

	void dx_interface_imp::update() {
		begin_frame();
		end_frame();

	}

	void dx_interface_imp::begin_frame() {
		// Set the background color
		assert(device_context_);
		float clearColor[] = { .25f, .5f, 1, 1 };
		device_context_->ClearRenderTargetView(render_target_view_.get(), clearColor);
	}

	void dx_interface_imp::end_frame() {
		swap_chain_->Present(
			0/*1*/,//syncInterval--> 
			0//DXGI_PRESENT flag
		);
	}
}