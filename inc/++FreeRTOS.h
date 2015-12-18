#ifndef BASIC_FREERTOS_H
#define BASIC_FREERTOS_H

extern "C" {
#include "FreeRTOS.h"
#include "task.h"
}

namespace FreeRTOS {

    class Task {
        TaskHandle_t rtosTask;
        Task(TaskHandle_t rtosTask) : rtosTask(rtosTask) {
        }

    public:
        template<typename T>
        static Task create(void (*task)(T *), char * name, uint32_t stackDepth, uint8_t priority, T * param = NULL) {
            void (*castedTask)(void *) = reinterpret_cast<void (*)(void *)>(task);
            void (*castedParamter) = static_cast<void *>(&param);
            TaskHandle_t handler;
            xTaskCreate(castedTask, name, stackDepth, castedParamter, priority, &handler);
            return Task(handler);
        }
    };
}
#endif //BASIC_FREERTOS_H
