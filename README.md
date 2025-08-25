  首先先声明，此定时器非彼定时器

  为什么这样说呢，之前我们的定时器就是TIMx，这个属于硬件定时器，一旦到达时间时，它就会触发中断。

  正因为硬件定时器的特性，我们习惯于认为定时器是充当于一个功能。但是在freertos中，软件定时器更加像是"一个任务"，只是在一定时间后，他会返回来进行执行。

  软件定时器在被创建之后，当经过设定的时钟计数值后会触发用户定义的回调函数(也就是定时器)。但是相比于任务而言，定时器还是有些不同的，相比于任务可以用vTaskDelay()这种阻塞函数，软件定时器不能有任何阻塞任务运行
的情况

  相比于TIMx定时器随便设置时间，软件定时器的定时又一定的限制，系统节拍配置为configTICK_RATE_HZ，该宏在 FreeRTOSConfig.h 中有定义，默认是 1000。那么系统的时钟节拍周期就为 1ms（1s跳动 1000 下，每一下就为
1ms）。软件定时器的所定时数值必须是这个节拍周期的整数倍。而不能是1.5ms。我们完全可以修改宏定义，系统节拍周期的值越小，精度越高，但是系统开销也将越大，因为这代表在 1 秒中系统进入时钟中断的次数也就越多。

  软件定时器是如何工作的呢?例如：系统当前时间 xTimeNow 值为 0，注意：xTimeNow 其实是一个局部变量，是根据 xTaskGetTickCount()函数获取的，实际它的值就是全局变量 xTickCount 的值，下文都采用它表示当前系统时间。
在当前系统中已经创建并启动了 1 个定时器 Timer1；系统继续运行，当系统的时间 xTimeNow 为 20 的时候，用户创建并且启动一个定时时间为 100 的定时器 Timer2，此 时 Timer2 的 溢出 时间 xTicksToWait 就 为
定时时间 +系统 当前时 间（100+20=120），然后将 Timer2 按 xTicksToWait 升序插入软件定时器列表中；假设当前系统时间 xTimeNow 为 40 的时候，用户创建并且启动了一个定时时间为 50 的定时器Timer3 ，
那 么 此 时 Timer3 的 溢 出 时 间 xTicksToWait 就 为 40+50=90 ， 同 样 安 装xTicksToWait 的数值升序插入软件定时器列表中，在定时器链表中插入过程具体见图 21-2。
同理创建并且启动在已有的两个定时器中间的定时器也是一样的，具体见图21-3。

<img width="805" height="608" alt="image" src="https://github.com/user-attachments/assets/b083faab-2306-4d9f-a990-c5186b44d1ac" />

<img width="881" height="496" alt="image" src="https://github.com/user-attachments/assets/1c842aff-990f-42bc-9c10-4fe34bf0c40c" />

那么系统如何管理定时器列表呢？如果一个定时器被触发，当定时器为单次模式时，那么执行过后则会被删除;如果是循环模式时，那么执行后会根据下一次唤醒时间重新将定时器添加到软件定时器列表中，并按升序排列

那么定时器如果和任务发生了冲突，那么CPU要先执行哪一个呢?

这个涉及到定时器的优先级， 但是定时器不能设置单个的优先级，而是只能设置定时器列表的优先级，这是和任务不同的地方。而定时器和定时器之间就是同等的优先级。

每个任务都存在任务栈的东西，那么定时器会发生溢出吗?

定时器也是有栈的，但是不是单个定时器的栈，而是所有定时器都共同使用一个栈，如果总量大于栈的大小，则会爆栈。

综上，用软件定时器时，需要注意以下几点:

  1.软件定时器的回调函数中应快进快出，绝对不允许使用任何可能引软件定时器起任务挂起或者阻塞的 API 接口，在回调函数中也绝对不允许出现死循环

  2.软件定时器使用了系统的一个队列和一个任务资源，软件定时器任务的优先级默认为 configTIMER_TASK_PRIORITY，为了更好响应，该优先级应设置为所有任务中最高的优先级。

  3.创建单次软件定时器，该定时器超时执行完回调函数后，系统会自动删除该软件定时器，并回收资源。

  4.定时器任务的堆栈大小默认为 configTIMER_TASK_STACK_DEPTH 个字节。

  在很多应用中，我们需要一些定时器任务，硬件定时器受硬件的限制，数量上不足以满足用户的实际需求，无法提供更多的定时器，那么可以采用软件定时器来完成，由软件定时器代替硬件定时器任务。但需要注意的是软件定时器的
精度是无法和硬件定时器相比的，而且在软件定时器的定时过程中是极有可能被其它中断所打断，因为软件定时器的执行上下文环境是任务。所以，软件定时器更适用于对时间精度要求不高的任务，一些辅助型的任务。

  如果需要用到软件定时器，则需要引用event_groups.h函数，并且将宏 configUSE_TIMERS 和configSUPPORT_DYNAMIC_ALLOCATION 均 定义为 1（configSUPPORT_DYNAMIC_ALLOCATION 在 FreeRTOS.h 中默认定义为 1）

  下面介绍软件定时器相关函数。

  1.定时器创建函数TimerHandle_t xTimerCreate(const char * const pcTimerName,const TickType_t xTimerPeriodInTicks, const UBaseType_t uxAutoReload, void * const pvTimerID, TimerCallbackFunction_t pxCallbackFunction )

    相关参数:
    pcTimerName:定时器的文本名称
    
    xTimerPeriodInTicks:定时器的周期，单位是系统节拍 (ticks,默认为1ms)。
    
    uxAutoReload:定时器模式。
    
    传入 pdTRUE -> 自动重载（周期）定时器。超时后自动重启，周期性地执行回调函数。
    
    传入 pdFALSE -> 单次定时器。超时执行一次回调函数后，自身进入休眠态，需要手动重启。
    
    pvTimerID:
    定时器标识符（ID）。一个用户定义的整数值或指针，用于区分共享同一个回调函数的多个定时器。但由于参数为void *,传入参数的格式为(void*)1
    
    pxCallbackFunction:定时器回调函数指针。

    返回值：

    成功：返回所创建的定时器的句柄 (TimerHandle_t)。
    失败：返回 NULL（通常是因为内存不足，无法为定时器对象分配 RAM）。

  但是，定时器在创建后处于休眠状态，若调用的话，则需要唤醒它。
      
  2. 启动定时器：BaseType_t xTimerStart( TimerHandle_t xTimer, TickType_t xTicksToWait )

    相关参数：
    xTimer: 要启动的定时器的句柄。
    
    xTicksToWait: 命令发送超时时间。因为启动命令是发送到定时器服务任务的队列中的，如果队列已满，调用任务可能会被阻塞。

    返回值：
    pdPASS: 命令成功发送到定时器命令队列。
    pdFAIL: 命令在指定的 xTicksToWait 时间内未能发送到队列（通常是因为队列已满）


  3.启动定时器的中断版本:xTimerStartFromISR( xTimer, pxHigherPriorityTaskWoken )

    xTimer 软件定时器句柄。
    pxHigherPriorityTaskWoken 定时器守护任务的大部分时间都在阻塞态等待定时器命令队列的命令
    如 果 调用函数 xTimerStartFromISR()让定时器任务脱离阻塞态，且定时器守护任务的优先级大于或者等于当前被中断的任务的
    优先级，那么 pxHigherPriorityTaskWoken 的值会在函数xTimerStartFromISR()内部设置为 pdTRUE，然后在中断退出之前执行一次上下文切换。

    返回值:
    如果启动命令无法成功地发送到定时器命令队列则返回 pdFAILE，成功发送则返回pdPASS。

  使用示例:
  <img width="786" height="675" alt="image" src="https://github.com/user-attachments/assets/e5c65a8d-5be6-4d7c-a1a7-3ad323433bfa" />

  4.停止定时器：BaseType_t xTimerStop( TimerHandle_t xTimer, TickType_t xTicksToWait )
  
    参数和返回值：与 xTimerStart() 完全相同

  5.停止定时器的中断版本:BaseType_t xTimerStopFromISR(TimerHandle_t xTimer,BaseType_t *pxHigherPriorityTaskWoken)

    参数和返回值：与 xTimerStartFromISR() 完全相同

  6.重置定时器：BaseType_t xTimerReset( TimerHandle_t xTimer, TickType_t xTicksToWait )

     参数和返回值：与 xTimerStart() 完全相同

  备注:重置一个定时器。如果定时器正在运行，它会从调用此函数的时刻重新开始计时；如果定时器处于休眠态，则效果等同于 xTimerStart()。用于软件看门狗或超时检测。在主循环中定期“喂狗”，即重置定时器，防止其超时。

  7.查询定时器状态：BaseType_t xTimerIsTimerActive( TimerHandle_t xTimer )

    参数：xTimer，要查询的定时器句柄。

    返回值：

    pdFALSE: 定时器处于休眠态（未运行）。
    非零值 (pdTRUE)：定时器处于运行态（正在计时）。

  8.修改定时器周期：BaseType_t xTimerChangePeriod( TimerHandle_t xTimer,TickType_t xNewPeriod,TickType_t xTicksToWait )

    参数：

    xTimer: 要修改的定时器句柄。
    
    xNewPeriod: 新的周期值（单位: ticks）。
    
    xTicksToWait: 命令发送超时时间。
    
    返回值：与 xTimerStart() 相同。


  9.软件定时器删除函数:xTimerDelete( xTimer, xTicksToWait )

    参数:
    
    xTimer: 软件定时器句柄。
    xBlockTime :用户指定的超时时间，单位为系统节拍周期(即 tick)，如果在 FreeRTOS调度器开启之前调用 xTimerStart()，该形参将不起作用。

    返回值:
    如果删除命令在超时时间之前无法成功地发送到定时器命令队列则返回 pdFAILE，成功发送则返回 pdPASS。

    
