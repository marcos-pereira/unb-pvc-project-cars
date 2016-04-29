#============================================================================
#  Universidade de Brasilia
#  Departamento de Ciencia da Computacao
#  Principios de Visao Computacional
#  Trabalho Final: Contador de Carros
#  Integrantes: Marcos, Felipe Dalosto, Maximillian, Felipe Spinola, Pedro, Bruno Donnici, Gustavo
#============================================================================

cmake_minimum_required(VERSION 2.8.4 FATAL_ERROR)
project (groundtruth_final_project)

include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
if(COMPILER_SUPPORTS_CXX11)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
endif()


# OpenCV required things to build project
find_package( OpenCV REQUIRED )

# add the include path to find the .h files and also OpenCV_INCLUDE_DIRS
include_directories (${CMAKE_BINARY_DIR}
		     ${CMAKE_SOURCE_DIR}
		     ${OpenCV_INCLUDE_DIRS})

set (SRCS ${SRCS} 
	generate_negative_samples.cpp
	draw_rectangles.cpp
	)

add_library(GroundtruthLibs ${SRCS})

add_executable (generate_negative_samples generate_negative_samples.cpp)
add_executable (draw_rectangles draw_rectangles.cpp)

target_link_libraries (generate_negative_samples GroundtruthLibs ${OpenCV_LIBS})
target_link_libraries (draw_rectangles GroundtruthLibs ${OpenCV_LIBS})

# add the install targets
install (TARGETS generate_negative_samples DESTINATION bin)
