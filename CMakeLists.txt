cmake_minimum_required(VERSION 3.16)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(myRemoteDevice)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/project_config.h.in
               ${CMAKE_CURRENT_BINARY_DIR}/project_config.h)
idf_build_set_property(INCLUDE_DIRECTORIES ${CMAKE_CURRENT_BINARY_DIR} APPEND)

