  在之前的章节，我们是采用了信号量或者消息队列进行任务与任务之间的传递，消息队列可以实现多对多的通信，但是内存开销很大，毕竟你用一个消息队列,一个消息最小都得是uint8_t;而信息量的确是用来传递信息的，但是却只能
实现俩个任务之间的信息通信，于是，为了节约空间，又可以实现多对多的通信，事件应运而生。

  事件:一种实现任务间通信的机制，主要用于实现多任务间的同步，但事件通信只能是事件类型的通信，无数据传输。与信号量不同的是，它可以实现一对多，多对多的同步。即一个任务可以等待多个事件的发生：可以是任意一个事件
发生时唤醒任务进行事件处理；也可以是几个事件都发生后才唤醒任务进行事件处理。同样，也可以是多个任务同步多个事件。

  事件只需要用位来记录信息，每一个事件组只需要很少的 RAM 空间来保存事件组的状态。事件组存储在一个EventBits_t 类型的变量中如 果 宏configUSE_16_BIT_TICKS 定义为 1，那么变量 uxEventBits 就是 16 位的，其中有
8 个位用来存储事件组；而如果宏 configUSE_16_BIT_TICKS 定义为0，那么变量 uxEventBits 就是32的其中有24个位用来存储事件组。

  一般地在stm32中，我们我们一般将configUSE_16_BIT_TICKS 定义为 0，那么 uxEventBits是32位的，有24个位用来实现事件标志组。

  每一位代表一个事件，任务通过“逻辑与”或“逻辑或”与一个或多个事件建立关联，形成一个事件组。事件的“逻辑或”也被称作是独立型同步，指的是任务感兴趣的所有事件任一件发生即可被唤醒；事件“逻辑与”则被称为是关联型同步，
指的是任务感兴趣的若干事件都发生时才被唤醒，并且事件发生的时间可以不同步。


  下面是事件的工作原理:

  设置事件时，对指定事件写入指定的事件类型，设置事件集合的对应事件位为 1，可以一次同时写多个事件类型，设置事件成功可能会触发任务调度。清除事件时，根据入参数事件句柄和待清除的事件类型，对事件对应位进行清0操
作。事件不与任务相关联，事件相互独立，一个 32位的变量（事件集合，实际用于表示事件的只有 24 位），用于标识该任务发生的事件类型，其中每一位表示一种事件类型（0 表示该事件类型未发生、1表示该事件类型已经发生）

  <img width="1059" height="216" alt="image" src="https://github.com/user-attachments/assets/a81900b4-7f9e-4e96-a85a-8ee5aaf678f3" />

  事件唤醒机制，当任务因为等待某个或者多个事件发生而进入阻塞态，当事件发生的时候会被唤醒

  <img width="729" height="649" alt="image" src="https://github.com/user-attachments/assets/b7c79093-53bb-422e-aca5-c3c893fc6cbd" />

  任务 1 对事件 3 或事件 5 感兴趣（逻辑或），当发生其中的某一个事件都会被唤醒，并且执行相应操作。而任务 2 对事件 3 与事件 5 感兴趣（逻辑与），当且仅当事件 3 与事件 5 都发生的时候，任务 2 才会被唤醒，如果
只有一个其中一个事件发生，那么任务还是会继续等待事件发生。如果接在收事件函数中设置了清除事件位 xClearOnExit，那么当任务唤醒后将把事件 3 和事件 5 的事件标志清零，否则事件标志将依然存在

  下面介绍事件的相关函数:
  1.事件创建函数 xEventGroupCreate()

      创建成功返回事件句柄；创建失败返回 NULL

  2.事件删除函数 vEventGroupDelete(xEventGroup)

      xEventGroup:事件句柄

  3.事件组置位函数 xEventGroupSetBits(EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToSet)

    xEventGroup 事件句柄。
    uxBitsToSet 指定事件中的事件标志位。如设置 uxBitsToSet 为 0x08 则只置位位 3，如果设置 uxBitsToSet 为 0x09 则位 3和位 0都需要被置位。
    
    返回调用 xEventGroupSetBits() 时事件组中的值。

  xEventGroupSetBits()的运用很简单，举个例子，比如我们要记录一个事件的发生，这个事件在事件组的位置是 bit0，当它还未发生的时候，那么事件组 bit0 的值也是 0，当它发生的时候，我们往事件集合 bit0 中写入这个
事件，也就是 0x01，那这就表示事件已经发生了，为了便于理解，一般操作我们都是用宏定义来实现 #define EVENT (0x01 << x)，“<< x”表示写入事件集合的 bit x

  4.事件组置位函数 xEventGroupSetBitsFromISR(EventGroupHandle_t xEventGroup,const EventBits_t uxBitsToSet,BaseType_t *pxHigherPriorityTaskWoken)

    xEventGroup 事件句柄。
    uxBitsToSet 指定事件组中的哪些位需要置位。如设置 uxBitsToSet 为0x08 则只置位位 3，如果设置 uxBitsToSet 为 0x09 则位 3和位 0 都需要被置位。
    pxHigherPriorityTaskWoken pxHigherPriorityTaskWoken 在使 用之前必须初始化成pdFALSE。

    消息成功发送给守护任务之后则返回 pdTRUE，否则返回 pdFAIL。如果定时器服务队列满了将返回 pdFAIL。

  使用示例:
  <img width="1157" height="764" alt="image" src="https://github.com/user-attachments/assets/9c7cdcd5-4db7-44ce-bcda-07855a2079f6" />

  
  5.等待事件函数 xEventGroupWaitBits(const EventGroupHandle_t xEventGroup,const EventBits_t uxBitsToWaitFor,const BaseType_t xClearOnExitconst BaseType_t xWaitForAllBits,TickType_t xTicksToWait)

    xEventGroup 事件句柄。
    
    uxBitsToWaitFor 一个按位或的值，指定需要等待事件组中的哪些位置 1。如果需要等待 bit 0 and/or bit 2 那么 uxBitsToWaitFor 配置为 0x05(0101b)。如
    果需要等待 bits 0 and/or bit 1 and/or bit 2 那么 uxBitsToWaitFor 配置为 0x07(0111b)。  
    
    (！！！但是需要注意的是，任务的"与"还是"或"触发不由这个决定)
    
    xClearOnExit 有俩种选择
    pdTRUE：当 xEventGroupWaitBits()等待到满足任务唤醒的事件时，系统将清除由形参 uxBitsToWaitFor 指定的事件标志位。
    pdFALSE：不会清除由形参 uxBitsToWaitFor 指定的事件标志位。

    xEventGroupWaitBits 有俩种选择
    pdTRUE:当形参uxBitsToWaitFor指定的位都置位的时候 ，xEventGroupWaitBits()才满足任务唤醒的条件，这也是“逻辑与”等待事件，并且在没有超时的情况下返回对应的事件标志位的值。
    pdFALSE：当形参 uxBitsToWaitFor 指定的位有其中任意一个置位的时候，这也是常说的“逻辑或”等待事件，在没有超时的情况下函数返回对应的事件标志位的值。   
    
    (！！！任务的"与"还是"或"触发由这个参数决定)
    
    xTicksToWait 最大超时时间，单位为系统节拍周期，常量 portTICK_PERIOD_MS用于辅助把时间转换成 MS。

  6.事件清除函数EventBits_t xEventGroupClearBits(EventGroupHandle_t xEventGroup,const EventBits_t uxBitsToClear )

    xEventGroup 事件句柄。
    uxBitsToClear 指定事件组中的哪个位需要清除。如设置 uxBitsToSet 为0x08 则只清除位 3，如果设置 uxBitsToSet 为 0x09 则位 3和位 0 都需要被清除。

    返回事件在还没有清除指定位之前的值。

  7.事件清除函数BaseType_t xEventGroupClearBitsFromISR(EventGroupHandle_t xEventGroup, const EventBits_t uxBitsToClear )

    xEventGroup 事件句柄。
    uxBitsToClear 指定事件组中的哪个位需要清除。如设置 uxBitsToSet 为0x08 则只清除位 3，如果设置 uxBitsToSet 为 0x09 则位 3和位 0 都需要被清除。

    返回事件在还没有清除指定位之前的值。
