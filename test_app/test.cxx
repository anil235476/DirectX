#include "test.h"
#include "directX_renderer.h"
#include <iostream>
#include "window_module/window_module.h"
#include <cassert>

using namespace std;


int main() {
	window_module wnd;
	auto r = wnd.create_window( 800, 600 );
	//assert(r);
	auto const b = test_renderer(wnd.get_handle());


	MSG msg = { 0 };
	while (true) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) {
				break;
			}
			update();
		}

		
	}

	cout << "test return from dll = " << b << endl;
}