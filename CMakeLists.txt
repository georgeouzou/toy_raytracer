cmake_minimum_required(VERSION 3.7)

set(app OneWeek)

project(${app})

set(EXTERN_DIR "${CMAKE_CURRENT_SOURCE_DIR}/extern")

set(src 
	src/main.cpp 
	src/stb_image_write.c
)

add_executable(${app} ${src})
set_property(TARGET ${app} PROPERTY CXX_STANDARD 17)

# glm
set(GLM_DIR "${EXTERN_DIR}/glm")
target_include_directories(${app} PRIVATE ${GLM_DIR})