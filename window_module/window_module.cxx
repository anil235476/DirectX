#include "window_module.h"
#include <string>
#include <cassert>

LRESULT CALLBACK WinProc(HWND handle, UINT msg, WPARAM wparam, LPARAM lparam) {
	if (msg == WM_DESTROY ||
		msg == WM_CLOSE) {
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(handle, msg, wparam, lparam);
}

window_module::~window_module() {
	assert(wnd_);
	const auto r1 = DestroyWindow(wnd_);
	assert(r1);
	const auto r = UnregisterClass("DirectX_Rendering", nullptr);
	assert(r);
}

bool window_module::create_window(const int w, const int h) {
	assert(wnd_ == nullptr);
	// Define window style
	WNDCLASS wc = { 0 };
	wc.style = CS_OWNDC;
	wc.lpfnWndProc = WinProc;
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wc.lpszClassName = "DirectX_Rendering";
	auto r = RegisterClass(&wc);
	if (r == 0) {
		const auto error = GetLastError();
		MessageBox(nullptr, "Error registring", std::to_string(error).c_str(), MB_OK);
		return false;
	}

	// Create the window
	wnd_ = CreateWindow("DirectX_Rendering", "DirectX Rendering",
		WS_POPUP | WS_CAPTION | WS_SYSMENU | WS_VISIBLE,
		100, 100, w, h,
		nullptr, nullptr, nullptr, nullptr);
	assert(wnd_ != nullptr);
	return wnd_ != nullptr;

}

HWND window_module::get_handle() {
	return wnd_;
}

