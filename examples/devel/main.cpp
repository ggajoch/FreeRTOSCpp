#include <cstdio>

#include "inc/++FreeRTOS.h"

extern "C" {
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include <FreeRTOS/include/semphr.h>
}

xQueueHandle queue;

void TxTask(void *pvParameters) {

    float x = 0;
    while (true) {
        printf("sending %f...\n", x);
        xQueueSend(queue, &x, portMAX_DELAY);
        x += 0.1;
        vTaskDelay(1000); // 1 second
    }
}


void RxTask(void *pvParameters) {
    while (true) {
        float received;
        if (xQueueReceive(queue, &received, portMAX_DELAY) == pdTRUE) {
            printf("received: %f\n", received);
        }
    }
}

xSemaphoreHandle smphr;
struct data {
    int a, b;
};

void TaskSend(data * val) {
    using namespace FreeRTOS::context;
    int i = 0;
    while(true) {
        i++;
        val->a = i;
        val->b = i+1;
        xSemaphoreGive(smphr);
        delay(100);
    }
}

void TaskGet(data * val) {
    while(true) {
        xSemaphoreTake(smphr, portMAX_DELAY);
        printf("got: %d %d\n", val->a, val->b);
    }
}

int main() {
    using namespace FreeRTOS;
    using namespace control;

    queue = xQueueCreate(1000, sizeof(float));
    smphr = xSemaphoreCreateBinary();

    Task first = Task::create<void>(RxTask, "Rx", 128, 1);
    Task::create(TxTask, "Tx", 128, 1);

    data d;
    Task::create(TaskSend, "s", 128, 1, &d);
    Task::create(TaskGet, "r", 128, 1, &d);

    startScheduler();
    return 0;
}