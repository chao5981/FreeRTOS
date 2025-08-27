  FreeRTOS 从 V8.2.0 版本开始提供任务通知这个功能，每个任务都有一个 32 位的通知值，在大多数情况下，任务通知可以替代二值信号量、计数信号量、事件组，也可以替代长度为 1 的队列（可以保存一个 32位整数或指针值）。

  什么是任务通知呢?之前的通过消息队列，信息量，事件等实现任务之间的通信时，共同的特点是不是都倒了一手，先从A任务发出，然后经过事件，消息队列等内核，然后才倒腾到B任务。而任务通知可以直接越过这些中间量，实现
任务与任务之间的通信。

  由于越过了这些中间量，使用任务通知显然更灵活。按照 FreeRTOS 官方的说法，使用任务通知比通过信号量等 ICP 通信方式解除阻塞的任务要快 45%，并且更加省 RAM 内存空间（使用 GCC 编译器，-o2 优化级别）

  所以合理使用任务通知，可以在一定场合代替FreeRTOS的信号量。但是，凡是都有利弊，消息通知虽然快，RAM开销小，但是相比于FreeRTOS的信号量，也有一些限制:  

      1.只能有一个任务接收通知消息，因为必须指定接收通知的任务
      2.只有等待通知的任务可以被阻塞，发送通知的任务，在任何情况下都不会因为发送失败而进入阻塞态

  因此，任务通知适合用于俩个任务之间的通信(一对一/一对多/多对一)，而多个任务的通信就不行了(多对多)

  若要使用任务通知，则需要将 FreeRTOSConfig.h 中的宏定义 configUSE_TASK_NOTIFICATIONS 设置为 1

  下面介绍相关函数

  1.功能最全发送函数:xTaskNotify( TaskHandle_t xTaskToNotify,uint32_t ulValue,eNotifyAction eAction )   

      xTaskToNotify：目标任务的句柄。你要通知哪个任务。

      ulValue：要传递的数据。根据 eAction 的不同，它可以是一个数据、一个位掩码或一个指针。
      
      eAction：关键参数，指定如何更新目标任务的通知值（ulNotifiedValue）。它是一个枚举类型 eNotifyAction:
      
              eNoAction：仅更新任务状态为“已通知”，不修改 ulNotifiedValue。用于实现轻量级二值信号量。ulValue 参数被忽略。(用于二值信号量)
              
              eSetBits：按位或（OR）。将 ulValue 与目标任务当前的通知值进行按位或操作。用于实现事件组功能，每一位代表一个独立事件。ulValue 是位掩码。(用于事件)
              
              eIncrement：递增。将目标任务的通知值加 1。用于实现计数信号量。ulValue 参数被忽略。(用于计数信号量)
              
              eSetValueWithOverwrite：覆写。无条件地将目标任务的通知值设置为 ulValue。即使上一个值还未被取走，也会被覆盖。(用于消息队列)
              
              eSetValueWithoutOverwrite：不覆写（保留下）。如果目标任务之前的值还未被取走（其通知状态为 eNotified），
              则本次调用不执行任何操作并返回 pdFAIL。否则，将通知值设置为 ulValue 并返回 pdPASS。用于实现轻量级队列。(用于消息队列)

              返回值:
              对于 eSetValueWithoutOverwrite 动作：成功写入返回 pdPASS，未写入（因为值未被取走）返回 pdFAIL。

              对于所有其他动作：总是返回 pdPASS。

  2.中断的发送函数：xTaskNotifyFromISR( TaskHandle_t xTaskToNotify,uint32_t ulValue,eNotifyAction eAction,BaseType_t *pxHigherPriorityTaskWoken )

      大部分参数同xTaskNotify()函数的参数

      pxHigherPriorityTaskWoken（仅限ISR版本）：如果发送通知解除了一个更高优先级任务的阻塞，则该参数会被设为 pdTRUE。之后需要进行上下文切换（portYIELD_FROM_ISR()）。
            
  使用示例如下:
  <img width="1000" height="831" alt="image" src="https://github.com/user-attachments/assets/c3d78878-787c-4e1c-81c9-9a797d492c79" />

  3.简化的用于信号量的发送函数:xTaskNotifyGive( TaskHandle_t xTaskToNotify )

      本质上是xTaskNotify()的宏定义。

      xTaskToNotify：目标任务的句柄。

  4.简化的用于信号量的发送函数的中断版本:vTaskNotifyGiveFromISR( TaskHandle_t xTaskToNotify,BaseType_t *pxHigherPriorityTaskWoken )

    
       大部分参数同xTaskNotifyGive()函数的参数

      pxHigherPriorityTaskWoken（仅限ISR版本）：如果发送通知解除了一个更高优先级任务的阻塞，则该参数会被设为 pdTRUE。之后需要进行上下文切换（portYIELD_FROM_ISR()）。

  使用示例如下:

  <img width="750" height="331" alt="image" src="https://github.com/user-attachments/assets/93095995-af58-4a70-8f0b-2438e2225b0a" />

  5.不常用的发送函数:t xTaskNotifyAndQuery( TaskHandle_t xTaskToNotify,uint32_t ulValue,eNotifyAction eAction,uint32_t *pulPreviousNotifyValue )

    与xTaskNotify()函数差不多，就是多了一个个 附 加 的 参 数pulPreviousNotifyValue 用于回传接收任务的上一个通知值，可以设置一个参数进行接收

  使用示例如下:

  <img width="1334" height="317" alt="image" src="https://github.com/user-attachments/assets/38ea48dd-4def-4ca8-baa6-b3f21f484958" />

  

  6.功能最全的接收函数：xTaskNotifyWait( uint32_t ulBitsToClearOnEntry,uint32_t ulBitsToClearOnExit,uint32_t *pulNotificationValue,TickType_t xTicksToWait )  

    ulBitsToClearOnEntry：在进入等待状态之前，先将当前通知值的哪些位清零。将其设置为 ULONG_MAX 将清零所有位（即整个值设为0），设置为 0 则不清除任何位。

    ulBitsToClearOnExit：在成功等到通知并退出函数之前，将通知值的哪些位清零。同样，ULONG_MAX 清零所有位。注意：退出前清零是在从 pulNotificationValue 传出之前完成的。
    
    pulNotificationValue：一个指向 uint32_t 的指针。用于传出接收到的通知值。如果不需要这个值，可以设置为 NULL。
    
    xTicksToWait：等待通知的最大阻塞时间。

    返回值：

    pdTRUE：成功获取到了通知。
    
    pdFALSE：超时，在指定时间内未收到通知。


  7.简化的用于信号量的接收函数：ulTaskNotifyTake( BaseType_t xClearCountOnExit,TickType_t xTicksToWait )

      xClearCountOnExit：决定函数退出时如何操作通知值。

    设置为 pdTRUE：在函数退出时，将通知值清零。这模拟了二值信号量的行为。
    
    设置为 pdFALSE：在函数退出时，将通知值递减 1。这模拟了计数信号量的行为。
    
    xTicksToWait：等待通知的最大阻塞时间。可以是 portMAX_DELAY。

    返回值：通知值的数值。

    如果是清零模式（pdTRUE），返回值通常是 0（未收到通知）或 1（收到通知）。
    
    如果是递减模式（pdFALSE），返回值是递减前的计数值。
          
    
      
    
