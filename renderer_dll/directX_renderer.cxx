#include "directX_renderer.h"
#include <cassert>
#include <d3d11.h>

class render_interface {
public:
	render_interface() {
		// Define our swap chain
		DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
		swapChainDesc.BufferCount = 1;
		swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapChainDesc.OutputWindow = nullptr;
		swapChainDesc.SampleDesc.Count = 1;
		swapChainDesc.Windowed = true;

		// Device stuff
		IDXGISwapChain* m_swapChain = nullptr;
		ID3D11Device* m_device = nullptr;
		ID3D11DeviceContext* m_deviceContext = nullptr;
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
	}
private:

};

bool test_init() {
	render_interface a{};
	return true;
}