cmake_minimum_required(VERSION 3.3)
project(basic)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(GLOB FreeRTOSSrc
        FreeRTOS/*.c
        FreeRTOS/include/*.h
        FreeRTOS/portable/*.c
        FreeRTOS/portable/*.h
)

file(GLOB Project
        examples/devel/*.c
        examples/devel/*.cpp
        examples/devel/*.h
)


set(SOURCE_FILES ${Project} ${FreeRTOSSrc})
include_directories(. FreeRTOS/include FreeRTOS/portable ${Project})


add_executable(basic ${SOURCE_FILES})


find_library(LIBWMM winmm)
target_link_libraries(basic ${LIBWMM})