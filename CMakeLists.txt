cmake_minimum_required(VERSION 3.11)

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

# openmp
find_package(OpenMP)
if (OPENMP_FOUND)
	target_link_libraries(${app} PUBLIC OpenMP::OpenMP_CXX)
endif()
