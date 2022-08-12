# CMakeList.txt : CMake project for ImageFlippingHttpServer, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

project ("ImageFlippingHttpServer")
set(CMAKE_CXX_STANDARD 17)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/boost_1_80_0)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/opencv/)
link_directories(${CMAKE_CURRENT_SOURCE_DIR}/opencv/lib)

add_executable (ImageFlippingHttpServer "main.cpp" "core.h" "core.cpp" )

target_link_libraries(ImageFlippingHttpServer libopencv_core.so)
target_link_libraries(ImageFlippingHttpServer libopencv_imgcodecs.so)
