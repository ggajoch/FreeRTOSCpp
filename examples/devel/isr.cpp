//
// Created by bambucha on 19.12.15.
//
#include <cstdio>

#include "inc/++FreeRTOS.h"

using namespace FreeRTOS;

Queue<int> exampleQueue(10);

void isr_handler(){
    static Queue<int>::IntertupInterface queue = exampleQueue.interruptView();
}