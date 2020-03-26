cmake_minimum_required(VERSION 3.0.0)
set (CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "" FORCE)
project(RayTracer)

set(DEPENDENCIES_DIR ${PROJECT_SOURCE_DIR}/dependencies)

add_subdirectory(
	src
)