cmake_minimum_required(VERSION 3.7)

set(app OneWeek)

project(${app})

set(src 
	src/main.cpp 
	src/stb_image_write.c
)

add_executable(${app} ${src})
set_property(TARGET ${app} PROPERTY CXX_STANDARD 17)