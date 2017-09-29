#include "dxgi_factory.h"
#include <cassert>
#include <Dxgi1_3.h>
#if _DXGI_FACT_2
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
#endif