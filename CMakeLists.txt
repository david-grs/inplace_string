cmake_minimum_required(VERSION 3.8)

project(inplace_string)
include(gtest.cmake)

set(COVERAGE OFF CACHE BOOL "Coverage")
set(CMAKE_CXX_STANDARD 17)

add_executable(tests unit_tests.cpp)

find_package (Threads)
target_link_libraries(tests gtest ${CMAKE_THREAD_LIBS_INIT})

if (MSVC)
	target_compile_options(tests PRIVATE /W4 /WX)

	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ox")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Od")

	# VS 2017 removed tr1
	add_definitions(-DGTEST_HAS_TR1_TUPLE=0)

	# unit tests are using std::tmpnam
	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
	target_compile_options(tests PRIVATE -g -Wall -Werror -Wextra -Wpedantic -Wconversion -Wswitch-default -Wswitch-enum -Wunreachable-code -Wwrite-strings -Wcast-align -Wshadow -Wundef)

	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0")

	if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
		target_compile_options(tests PRIVATE -Weverything -Wno-c++98-compat -Wno-global-constructors)
	endif()

	if (COVERAGE)
		target_compile_options(tests PRIVATE --coverage)
		target_link_libraries(tests PRIVATE --coverage)
	endif()
endif()

