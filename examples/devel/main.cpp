#include <cstdio>

#include "inc/++FreeRTOS.h"
using namespace FreeRTOS;

Queue<float> Q1(100);

void TxTask(void *pvParameters) {

    float x = 0;
    while (true) {
        printf("sending %f...\n", x);
        Q1.send(x);
        x += 0.1;
        vTaskDelay(1000); // 1 second
    }
}


void RxTask(void *pvParameters) {
    while (true) {
        float received;
        if ( Q1.receive(&received) ) {
            printf("received: %f\n", received);
        }
    }
}

FreeRTOS::Semaphore smphr;
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
        smphr.give();
        delay(100);
    }
}

void TaskGet(data * val) {
    while(true) {
        smphr.take();
        printf("got: %d %d\n", val->a, val->b);
    }
}

int main() {
    using namespace FreeRTOS;
    using namespace control;

//    queue = xQueueCreate(1000, sizeof(float));
//    Semaphore handle = Semaphore::createBinary();
//    smphr = &handle;

    Task first = Task::create<void>(RxTask, "Rx", 128, 1);
    Task::create(TxTask, "Tx", 128, 1);

    data d;
    Task::create(TaskSend, "s", 128, 1, &d);
    Task::create(TaskGet, "r", 128, 1, &d);

    startScheduler();
    return 0;
}