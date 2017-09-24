#ifndef _DIRECT_X_RENDERER_H_
#define _DIRECT_X_RENDERER_H_


#if defined (_WIN32) 
#include <Windows.h>

#if defined(directX_renderer_EXPORTS)
#define  RENDERER_EXPORT __declspec(dllexport)
#else
#define  RENDERER_EXPORT __declspec(dllimport)
#endif /* directX_renderer_EXPORTS */
#else /* defined (_WIN32) */
#define RENDERER_EXPORT
#endif

RENDERER_EXPORT
bool test_init();

RENDERER_EXPORT
bool test_renderer(HWND wnd);

#endif//_DIRECT_X_RENDERER_H_
