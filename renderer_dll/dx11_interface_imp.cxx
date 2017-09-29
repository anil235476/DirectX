#include "dx11_interface_imp.h"
#include <cassert>
#include "dxgi_factory.h"
#include <array>
#include <tuple>
#include "utility/array_helper.h"

using namespace std;
using namespace experiment;

//#define _DXGI_FACT_2 1 //enable this to enable factory
namespace
{
	namespace detail
	{
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
	}


	DXGI_SWAP_CHAIN_DESC1 get_swap_chain1_desc(HWND wnd) {
		const auto width_height = detail::get_width_height(wnd);
		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Width = width_height.width;
		sd.Height = width_height.height;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;
		return sd;
	}

};//anonymous namespace


namespace renderer
{
	using namespace util;

	dx11_interface_imp::dx11_interface_imp() {
#if _DXGI_FACT_2
		const auto r = create_device();
		assert(r);
#endif
	}

	bool dx11_interface_imp::initialize(HWND wnd) {
		if (create_swap_chain2(wnd)) {
#if _DXGI_FACT_2
			if (create_swap_chain1()) {
#endif
				const auto r = create_render_target();
				assert(r);
				return r;
#if _DXGI_FACT_2
			}
#endif

		}
		assert(false);
		return false;
	}

	bool dx11_interface_imp::create_device() {
		const auto driverTypes = make_array(D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE);

		const auto featureLevels = make_array(D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0);

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
		assert(false);
		return false;
	}

	bool dx11_interface_imp::create_swap_chain2(HWND wnd) {
#if _DXGI_FACT_2
		assert(pd3dDevice);

		const DXGI_SWAP_CHAIN_DESC1 sd = get_swap_chain1_desc(wnd);

		dxgi_factory2_handler dxg_factory;
		const auto hr = dxg_factory->CreateSwapChainForHwnd(
			pd3dDevice.get(),
			wnd,
			&sd,
			nullptr,
			nullptr,
			swap_chain1_.ref()
		);
		assert(hr == S_OK);
		return hr == S_OK;
#else
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
			pd3dDevice.ref(), nullptr, pImmediateContext.ref());

		// Check for error
		if (result != S_OK) {
			MessageBox(nullptr, "Error creating DX11", "Error", MB_OK);
			assert(false);
			exit(0);
		}
		return true;
#endif
		return false;
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

		assert(pImmediateContext);

		pImmediateContext->OMSetRenderTargets(1, render_target_view_.ref(), nullptr);
		// Setup the viewport
		D3D11_VIEWPORT vp;
		vp.Width = (FLOAT)300;
		vp.Height = (FLOAT)200;
		vp.MinDepth = 0.0f;
		vp.MaxDepth = 1.0f;
		vp.TopLeftX = 0;
		vp.TopLeftY = 0;
		pImmediateContext->RSSetViewports(1, &vp);

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