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
	int i;
	cin >> i;
	cout << "test return from dll = " << b << endl;
}