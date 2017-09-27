#include "dx11_interface_imp.h"
#include <cassert>
#include <Dxgi1_3.h>



namespace renderer {

	dx11_interface_imp::dx11_interface_imp() {
#ifdef _DEBUG
		const int flag = DXGI_CREATE_FACTORY_DEBUG;
#else
		const int flag = 0;
#endif
		const HRESULT hr = CreateDXGIFactory2(
			flag,
			__uuidof(IDXGIFactory2),
			(void**)dxgiFactory2.ref());
		assert(!FAILED(hr));
		
		UINT createDeviceFlags = 0;
#ifdef _DEBUG
		createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

		D3D_DRIVER_TYPE driverTypes[] =
		{
			D3D_DRIVER_TYPE_HARDWARE,
			D3D_DRIVER_TYPE_WARP,
			D3D_DRIVER_TYPE_REFERENCE,
		};
		UINT numDriverTypes = ARRAYSIZE(driverTypes);

		D3D_FEATURE_LEVEL featureLevels[] =
		{
			D3D_FEATURE_LEVEL_11_1,
			D3D_FEATURE_LEVEL_11_0,
			D3D_FEATURE_LEVEL_10_1,
			D3D_FEATURE_LEVEL_10_0,
		};
		const UINT numFeatureLevels = ARRAYSIZE(featureLevels);
		
		D3D_FEATURE_LEVEL  g_featureLevel = D3D_FEATURE_LEVEL_11_0;
		
		for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; driverTypeIndex++)
		{
			D3D_DRIVER_TYPE driverType = driverTypes[driverTypeIndex];
			auto hr = D3D11CreateDevice(
				nullptr, 
				driverType,
				nullptr, 
				createDeviceFlags, 
				featureLevels, 
				numFeatureLevels,
				D3D11_SDK_VERSION, 
				pd3dDevice.ref(),
				&g_featureLevel,
				pImmediateContext.ref()
			);

			if (hr == E_INVALIDARG)
			{
				// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it
				hr = D3D11CreateDevice(
					nullptr,
					driverType, 
					nullptr, createDeviceFlags,
					&featureLevels[1], 
					numFeatureLevels - 1,
					D3D11_SDK_VERSION,
					pd3dDevice.ref(),
					&g_featureLevel, 
					pImmediateContext.ref()
				);
			}

			if (SUCCEEDED(hr))
				break;
		}

	}

	bool dx11_interface_imp::initialize(HWND wnd) {
		assert(pd3dDevice);
		RECT rc;
		auto const r = GetClientRect(wnd, &rc);
		assert(r);
		const UINT width = rc.right - rc.left;
		const UINT height = rc.bottom - rc.top;
		DXGI_SWAP_CHAIN_DESC1 sd;
		ZeroMemory(&sd, sizeof(sd));
		sd.Width = width;
		sd.Height = height;
		sd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		sd.SampleDesc.Count = 1;
		sd.SampleDesc.Quality = 0;
		sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		sd.BufferCount = 1;

		assert(dxgiFactory2);
		auto hr = dxgiFactory2->CreateSwapChainForHwnd(
			pd3dDevice.get(),
			wnd, 
			&sd,
			nullptr, 
			nullptr, 
			swap_chain1_.ref()
		);
		

		//dxgiFactory2->Release();
		return hr == S_OK;
	}


	bool dx11_interface_imp::create_render_target() {
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
			const auto hr = pd3dDevice->CreateRenderTargetView(
				backBuffer.get(), nullptr, render_target_view_.ref());
			assert(!FAILED(hr));
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
		float clearColor[] = { .25f, .5f, 1, 1 };
		pImmediateContext->ClearRenderTargetView(render_target_view_.get(), clearColor);
	}

	void dx11_interface_imp::end_frame() {
		swap_chain_->Present(
			0/*1*/,//syncInterval--> 
			0//DXGI_PRESENT flag
		);
	}
} //namespace renderer