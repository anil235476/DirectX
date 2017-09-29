#ifndef _DXGI_FACTORY_H__
#define _DXGI_FACTORY_H__

#include "utility/release_helper.h"
struct IDXGIFactory2;

struct dxgi_factory2_handler
{
	dxgi_factory2_handler();
	IDXGIFactory2* operator->() {
		return dxgiFactory2.get();
	}
	util::release_helper<IDXGIFactory2> dxgiFactory2;
};



#endif//_DXGI_FACTORY_H__
