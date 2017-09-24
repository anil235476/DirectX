cmake_minimum_required (VERSION 3.2)
project (DirectX_Renderer)



#add the binary tree to the search path for include files
#so that we will find tutorialConfig.h

include_directories("${PROJECT_BINARY_DIR}")

#adding definition
#add_definitions(-DTEST)

#add the executable

add_library(directX_renderer SHARED directX_renderer.cxx)

add_executable(tester test.cxx)

#add library linkage to tester
target_link_libraries(tester directX_renderer)