  在FreeRTOS中，有一系列的任务管理函数，这里主要讲解其函数功能和注意事项

  1.vTaskSuspend():挂起指定任务，任务将不再运行，直到被恢复。

  注意事项:   a.在任务调度器前调用无效    
              b.传入的任务句柄不能是无效的(传 NULL 表示挂起当前任务)

  2.vTaskSuspendAll():挂起所有任务（暂停调度器），但不会挂起空闲任务（Idle Task）.这个必须由xTaskResumeAll()恢复(因为是直接卡住了调度器)

  3.xTaskResumeFromISR():从中断服务程序（ISR）中恢复被挂起的任务。注意恢复后要进行上下文切换

  4.xTaskResumeAll():恢复被 vTaskSuspendAll 挂起的调度器。

  5.vTaskDelete():删除指定任务，释放其内存

  6.vTaskDelay():将当前任务延迟（阻塞）指定数量的时钟节拍（tick）

  7.vTaskDelayUntil():绝对延迟（固定周期延迟），适用于周期性任务,使用例子如下：

    void vPeriodicTask(void *pvParameters) {
      TickType_t xLastWakeTime = xTaskGetTickCount();
      const TickType_t xFrequency = pdMS_TO_TICKS(100); // 100ms周期
      while (1) {
          // 任务逻辑
          vTaskDelayUntil(&xLastWakeTime, xFrequency); // 严格100ms周期
      }
    }
