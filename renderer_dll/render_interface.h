#ifndef _RENDER_INTERFACE_H__
#define _RENDER_INTERFACE_H__
#include <Windows.h>

namespace renderer
{
	class render_interface 
	{
	public:
		virtual bool initialize(HWND wnd) = 0;
		virtual void update() = 0;	
	};
} //namespace renderer

#endif//_RENDER_INTERFACE_H__
