#include "directX_renderer.h"
#include <cassert>
#include "dx_interface_imp.h"

using namespace renderer;

render_interface* get_renderer_handle() {
	static dx_interface_imp render{};
	return &render;
}


bool test_init() {
	auto p = get_renderer_handle();
	return p != nullptr;
}

bool test_renderer(HWND wnd) {
	auto p = get_renderer_handle();
	return p->initialize(wnd);
}

void update() {
	auto p = get_renderer_handle();
	p->update();
}