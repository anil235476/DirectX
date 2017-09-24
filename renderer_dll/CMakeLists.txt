cmake_minimum_required (VERSION 3.2)

set(dll_src directX_renderer.cxx)

add_library(directX_renderer SHARED ${dll_src})
target_link_libraries(directX_renderer d3d11)
