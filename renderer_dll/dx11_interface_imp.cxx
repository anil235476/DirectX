#include "dx11_interface_imp.h"
#include <cassert>
#include <Dxgi1_3.h>
#include <array>
#include <tuple>

using namespace std;

struct dxgi_factory2_handler
{
	dxgi_factory2_handler();
	IDXGIFactory2* operator->() {
		return dxgiFactory2.get();
	}
	util::release_helper<IDXGIFactory2> dxgiFactory2;
};

namespace {
	constexpr array<D3D_DRIVER_TYPE, 3> driverTypes{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_REFERENCE,
	};
	constexpr array<D3D_FEATURE_LEVEL, 4>featureLevels{
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
	};
	struct dimension
	{
		const UINT width;
		const UINT height;
	};
	dimension get_width_height(HWND wnd) {
		RECT rc;
		auto const r = GetClientRect(wnd, &rc);
		assert(r);
		const UINT width = rc.right - rc.left;
		const UINT height = rc.bottom - rc.top;
		return{ width, height };
	}
};//anonymous namespace

dxgi_factory2_handler::dxgi_factory2_handler() {
#ifdef _DEBUG
	const int flag = DXGI_CREATE_FACTORY_DEBUG;
#else
	const int flag = 0;
#endif
	const HRESULT hr = CreateDXGIFactory2(
		flag,
		__uuidof(IDXGIFactory2),
		(void**)dxgiFactory2.ref());
	assert(SUCCEEDED(hr));
}

namespace renderer {
	using namespace util;

	dx11_interface_imp::dx11_interface_imp() {
		const auto r = create_device();
		assert(r);
	}

	bool dx11_interface_imp::initialize(HWND wnd) {
		if (create_swap_chain2(wnd)) {
			if (create_swap_chain1()) {
				const auto r = create_render_target();
				assert(r);
				return r;
			}
		}
		assert(false);
		return false;
	}

	bool dx11_interface_imp::create_device() {
		
		for (const auto& driverType : driverTypes) {
		
			UINT createDeviceFlags = 0;
#ifdef _DEBUG
			createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif
			D3D_FEATURE_LEVEL featureLevel{ D3D_FEATURE_LEVEL_11_0 };
			auto hr = D3D11CreateDevice(
				nullptr,
				driverType,
				nullptr,
				createDeviceFlags,
				&featureLevels[0],
				static_cast<UINT>(featureLevels.size()),
				D3D11_SDK_VERSION,
				pd3dDevice.ref(),
				&featureLevel,
				pImmediateContext.ref()
			);

			if (hr == E_INVALIDARG) {
				// DirectX 11.0 platforms will not recognize 
				//D3D_FEATURE_LEVEL_11_1 so we need to retry without it
				hr = D3D11CreateDevice(
					nullptr,
					driverType,
					nullptr, createDeviceFlags,
					&featureLevels[1],
					static_cast<UINT>(featureLevels.size() - 1),
					D3D11_SDK_VERSION,
					pd3dDevice.ref(),
					&featureLevel,
					pImmediateContext.ref()
				);
			}

			if (SUCCEEDED(hr))
				return true;
		}
		return false;
	}

	bool dx11_interface_imp::create_swap_chain2(HWND wnd) {
		assert(pd3dDevice);
		
		const auto width_height = get_width_height(wnd);
		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Width = width_height.width;
		sd.Height = width_height.height;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		dxgi_factory2_handler dxg_factory;
		
		auto hr = dxg_factory->CreateSwapChainForHwnd(
			pd3dDevice.get(),
			wnd,
			&sd,
			nullptr,
			nullptr,
			swap_chain1_.ref()
		);

		return hr == S_OK;
	}

	bool dx11_interface_imp::create_swap_chain1() {
		assert(swap_chain1_);
		assert(!swap_chain_);
		const auto hr = swap_chain1_->QueryInterface(
			__uuidof(IDXGISwapChain),
			reinterpret_cast<void**>(swap_chain_.ref())
		);
		return hr == S_OK;
	}

	bool dx11_interface_imp::create_render_target() {
		assert(pd3dDevice);
		assert(swap_chain_);
		assert(!render_target_view_);
		release_helper<ID3D11Texture2D> backBuffer;
		{
			const auto hr = swap_chain_->GetBuffer(
				0, __uuidof(ID3D11Texture2D),
				reinterpret_cast<void**>(backBuffer.ref())
			);
			assert(SUCCEEDED(hr));
		}
		{
			const auto hr = pd3dDevice->CreateRenderTargetView(
				backBuffer.get(), nullptr, render_target_view_.ref());
			assert(SUCCEEDED(hr));
		}
		return render_target_view_.get() != nullptr;
	}

	void dx11_interface_imp::update() {
		begin_frame();
		end_frame();
	}

	void dx11_interface_imp::begin_frame() {
		// Set the background color
		assert(pImmediateContext);
		assert(render_target_view_);
		const float clearColor[] = { .25f, .5f, 1, 1 };
		pImmediateContext->ClearRenderTargetView(
			render_target_view_.get(), clearColor);
	}

	void dx11_interface_imp::end_frame() {
		swap_chain_->Present(
			0/*1*/,//syncInterval--> 
			0//DXGI_PRESENT flag
		);
	}
} //namespace renderer