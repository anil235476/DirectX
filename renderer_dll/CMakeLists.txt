cmake_minimum_required (VERSION 3.2)

include_directories(${renderer_SOURCE_DIR})

set(dll_src 
	directX_renderer.h
	directX_renderer.cxx
	render_interface.h
	render_interface.cxx
	dx_interface_imp.h
	dx_interface_imp.cxx
	dx11_interface_imp.h
	dx11_interface_imp.cxx
	dxgi_factory.h
	dxgi_factory.cxx
)

add_library(directX_renderer SHARED 
	${dll_src}
)

target_link_libraries(directX_renderer 
	d3d11
	DXGI
	)

