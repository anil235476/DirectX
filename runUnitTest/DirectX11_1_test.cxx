#include "gtest/gtest.h"
#include "renderer_dll/dx11_interface_imp.cxx"
#include "window_module/window_module.cxx"
using namespace renderer;

TEST(protoype, test) {
	EXPECT_TRUE(true);
}

TEST(directX_11_1, contruction) {
	dx11_interface_imp temp{};
	EXPECT_TRUE(true);
}

TEST(diretX_11_1, intialization) {
	dx11_interface_imp temp{};
	window_module wnd;
	{
		auto r = wnd.create_window(800, 600);
		ASSERT_TRUE(r);
	}
	{
		const auto r = temp.initialize(wnd.get_handle());
		EXPECT_TRUE(r);
	}
}

TEST(diretX_11_1, update) {
	dx11_interface_imp temp{};
	window_module wnd;
	{
		auto r = wnd.create_window(800, 600);
		ASSERT_TRUE(r);
	}
	{
		const auto r = temp.initialize(wnd.get_handle());
		EXPECT_TRUE(r);
	}

	temp.update();
	MSG msg = { 0 };
	int i = 0;
	while (++i < 100) {
		if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) {
				break;
			}
			temp.update();
		}


	}
}
