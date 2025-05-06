cmake_minimum_required(VERSION 3.29)
project(ProjectionConverter)

set(CMAKE_CXX_STANDARD 20)

add_executable(ProjectionConverter main.cpp
        include/geo_coord.h
        include/geo_projection.h
        include/geo_gcs.h
)
