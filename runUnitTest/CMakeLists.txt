cmake_minimum_required (VERSION 3.2)


include_directories(${renderer_SOURCE_DIR})

if(MSVC)
  set(variables 
    CMAKE_CXX_FLAGS_DEBUG
    CMAKE_CXX_FLAGS_RELEASE
	CMAKE_CXX_FLAGS_RELWITHDEBINFO
	CMAKE_CXX_FLAGS_MINSIZEREL
  )
  foreach(variable ${variables})
	if(${variable} MATCHES "/MD")
	  string(REGEX REPLACE "/MD" "/MT" ${variable} "${${variable}}")
	endif()
  endforeach()
endif()

################################
# Testing
################################
if (test)
 
  enable_testing()
   # This is so you can do 'make test' to see all your tests run, instead of
  # manually running the executable runUnitTests to see those specific tests.
  #add_test(NAME that-test-I-made COMMAND runUnitTests)
  add_test(my_test runUnitTests)

  # You can also omit NAME and COMMAND. The second argument could be some other
  # test executable.
  #add_test(that-other-test-I-made runUnitTests)

  # Include the gtest library. gtest_SOURCE_DIR is available due to
  # 'project(gtest)' above.
  include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
  
  ##############
  # Unit Tests
  ##############
  add_executable(runUnitTests 
  test_project1.cpp
  ${renderer_SOURCE_DIR}/utility/array_helper.h
  DirectX11_1_test.cxx
  )

  # Standard linking to gtest stuff.
  target_link_libraries(runUnitTests gtest gtest_main)

  target_link_libraries(runUnitTests 
	d3d11
	DXGI
	)
 
endif()