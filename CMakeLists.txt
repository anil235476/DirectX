cmake_minimum_required (VERSION 8.8)
project (DirectX_Renderer)



#add the binary tree to the search path for include files
#so that we will find tutorialConfig.h

include_directories("${PROJECT_BINARY_DIR}")

#add the executable

add_library(directX_renderer directX_renderer.cxx)