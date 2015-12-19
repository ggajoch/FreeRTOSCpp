cmake_minimum_required(VERSION 3.3)
project(basic)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c11")

file(GLOB FreeRTOSSrc
        FreeRTOS/*.c
        FreeRTOS/include/*.h
        FreeRTOS/portable/*
)

file(GLOB Project
        examples/devel/*.c
        examples/devel/*.cpp
        examples/devel/*.h
)


set(SOURCE_FILES  ${Project} ${FreeRTOSSrc} inc/++FreeRTOS.h examples/devel/isr.cpp)
include_directories(. FreeRTOS/include FreeRTOS/portable ${Project})


add_executable(basic ${SOURCE_FILES})
