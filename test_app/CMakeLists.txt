cmake_minimum_required (VERSION 3.2)

#subdirs(${renderer_SOURCE_DIR}/window_module/)
#link_directories(${renderer_SOURCE_DIR}/window_module/)
include_directories(${renderer_SOURCE_DIR}/renderer_dll)

set(wnd_source
    ${renderer_SOURCE_DIR}/window_module/window_module.cxx
	${renderer_SOURCE_DIR}/window_module/window_module.h
  )

set(exe_src 
	test.cxx 
	${wnd_source}
)

add_executable(test_app ${exe_src})

#add library linkage to tester
target_link_libraries(test_app directX_renderer)
