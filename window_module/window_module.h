#ifndef __WINDOW_MODULE_H_
#define __WINDOW_MODULE_H_
#include <Windows.h>

class window_module {
public:
	window_module() = default;
	bool create_window(const int w, const int h);
	HWND get_handle();
	~window_module();
private:
	HWND wnd_{ nullptr };
};

#endif//__WINDOW_MODULE_H_