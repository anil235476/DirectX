#include "directX_renderer.h"
#include <cassert>
#include "render_interface.h"

render_interface* get_renderer_handle() {
	static render_interface render{};
	return &render;
}


bool test_init() {
	auto p = get_renderer_handle();
	return p != nullptr;
}

bool test_renderer(HWND wnd) {
	auto p = get_renderer_handle();
	return p->create_device(wnd);
}

void update() {
	auto p = get_renderer_handle();
	p->upate();
}