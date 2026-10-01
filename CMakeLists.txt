cmake_minimum_required(VERSION 4.4)

project(LAB1)

add_executable(LAB1)

target_sources(LAB1
	PRIVATE
	src/main.cpp)

add_library(mylib)

target_sources(mylib
	PRIVATE
		src/tbitfield.cpp
		src/tset.cpp
	PUBLIC
	FILE_SET HEADERS
	BASE_DIRS
		include
	FILES
		include/tbitfield.h
		include/tset.h)

