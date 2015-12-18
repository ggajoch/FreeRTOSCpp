#ifndef BASIC_FREERTOS_H
#define BASIC_FREERTOS_H



extern "C" {
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "queue.h"
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


    class SemaphoreGeneric {
    protected:
        xSemaphoreHandle handle;
        SemaphoreGeneric() {}
    public:
        void take(uint32_t timeToWait = portMAX_DELAY) {
            xSemaphoreTake(this->handle, timeToWait);
        }

        void give() {
            xSemaphoreGive(this->handle);
        }

        ~SemaphoreGeneric() {
            vSemaphoreDelete(handle);
        }
    };

    class Semaphore : public SemaphoreGeneric {
    public:
        Semaphore() {
            handle = xSemaphoreCreateBinary();
        }
    };

    class CountingSemaphore : SemaphoreGeneric {
    public:
        CountingSemaphore(uint32_t maxCount = portMAX_DELAY, uint32_t initialCount = 0) {
            handle = xSemaphoreCreateCounting(maxCount, initialCount);
        }
    };

    class Mutex : SemaphoreGeneric {
    public:
        Mutex() {
            handle = xSemaphoreCreateMutex();
        }
    };

    class RecursiveMutex : SemaphoreGeneric {
    public:
        RecursiveMutex() {
            handle = xSemaphoreCreateRecursiveMutex();
        }
    };


    template<typename T>
    class Queue {
        xQueueHandle handle;
    public:
        Queue(uint32_t length) {
            handle = xQueueCreate(length, sizeof(T));
        }
        void send(const T& element, uint32_t timeToWait = portMAX_DELAY) {
            xQueueSend(handle, &element, timeToWait);
        }
        void sendToBack(const T& element, uint32_t timeToWait = portMAX_DELAY) {
            xQueueSendToBack(handle, &element, timeToWait);
        }

        void sendToFront(const T& element, uint32_t timeToWait = portMAX_DELAY) {
            xQueueSendToFront(handle, &element, timeToWait);
        }
        bool peek(T * element, uint32_t timeToWait = portMAX_DELAY) {
            return xQueuePeek(handle, element, timeToWait);
        }

        bool receive(T * element, uint32_t timeToWait = portMAX_DELAY) {
            return xQueueReceive(handle, element, timeToWait);
        }
    };

    namespace context {
        void delay(uint32_t ticks) {
            vTaskDelay(ticks);
        }
    }

    namespace control {
        void startScheduler() {
            vTaskStartScheduler();
        }
    }
}
#endif //BASIC_FREERTOS_H
