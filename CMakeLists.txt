cmake_minimum_required(VERSION 3.26)
project(RayTracer)

set(CMAKE_CXX_STANDARD 17)

add_executable(RayTracer main.cpp
        vec3.h
        color.h
        ray.h)
