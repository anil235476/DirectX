#include "render_interface.h"
#include <cassert>

bool render_interface::create_device(HWND wnd) {
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
		&swapChainDesc, &m_swapChain,
		&m_device, nullptr, &m_deviceContext);

	// Check for error
	if (result != S_OK) {
		MessageBox(nullptr, "Error creating DX11", "Error", MB_OK);
		exit(0);
	}

	return result == S_OK && create_render_target();
}

bool render_interface::create_render_target() {
	ID3D11Texture2D* backBuffer;
	m_swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&backBuffer);
	m_device->CreateRenderTargetView(backBuffer, nullptr, &m_renderTargetView);;
	backBuffer->Release();

	return m_renderTargetView != nullptr;
}

void render_interface::upate() {
	// Set the background color
	assert(m_deviceContext);
	float clearColor[] = { .25f, .5f, 1, 1 };
	m_deviceContext->ClearRenderTargetView(m_renderTargetView, clearColor);

	m_swapChain->Present(1, 0);
}