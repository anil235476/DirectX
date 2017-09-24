cmake_minimum_required (VERSION 3.2)

#project name 
project (renderer)

#subdirs(renderer_dll test_app window_module)

subdirs(renderer_dll test_app)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin/)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_HOME_DIRECTORY}/bin/)

#add the binary tree to the search path for include files
#so that we will find tutorialConfig.h

include_directories("${PROJECT_BINARY_DIR}")

#adding definition
#add_definitions(-DTEST)

#add the executable

#set(dll_src directX_renderer.cxx)

#add_library(directX_renderer SHARED ${dll_src})
#target_link_libraries(directX_renderer, D3D11.lib); here it is an error
#target_link_libraries(directX_renderer d3d11)

#set(exe_src test.cxx)

#add_executable(tester ${exe_src})

#add library linkage to tester
#target_link_libraries(tester directX_renderer)

#add_executable(window_module window_module/window_module.cxx)
