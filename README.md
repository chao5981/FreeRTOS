  在任务工作中，总会有一些任务需要和另外的任务进行数据传输和交互，或者是任务和中断进行任务交换之前我们是用全局变量进行工作，但是全局变量有可能会被中途打断导致数据不一致，并且当不需要的时候还不能删除，
一直占用着本就不大的栈空间，因此，消息队列应运而生。

  消息队列，顾名思义，就是一个传递消息的队列，用于任务之间的信息传递。你可以理解为不称职的邮递员，一个任务需要传递一个参数，就把信件给邮递员，但是它不会把任务送到指定的任务，需要任务自己去消息队列取消息。

  并且消息还不能随便取，还有特定的取顺序，消息队列遵循先进先出(FIFO)和后进先出(LIFO)俩种模式，一般我们采用的是先进先出模式。

  既然是把信件给邮递员，信件被存放到消息队列中，那消息队列不可能存储所有的信息吧，它总有一定空间(内存)，那如果消息队列满了，我还想存入消息怎么办呢？那就只能等咯，这个就涉及到消息队列的阻塞机制。

  当消息队列满了时，任务会根据用户指定的阻塞超时时间进行阻塞，在这段时间中，如果队列一直不允许入队，该任务将保持阻塞状态以等待队列允许入队。当其它任务从其等待的队列中读取入了数据（队列未满），该任务将
自动由阻塞态转移为就绪态。当等待的时间超过了指定的阻塞时间，即使队列中还不允许入队，任务也会自动从阻塞态转移为就绪态，此时发送消息的任务或者中断程序会收到一个错误码 errQUEUE_FULL。任务继续执行之前的函数内容，
数据也没有被传输出去

  发送紧急消息的过程与发送消息几乎一样，唯一的不同是，当发送紧急消息时，发送的位置是消息队列队头而非队尾，这样，接收者就能够优先接收到紧急消息，从而及时进行消息处理。

  那竟然发送方会有这种苦恼，接收方也肯定有吧，如果消息队列一直为空，任务也可以设置为阻塞态

  假设有一个任务 A 对某个队列进行读操作的时候（也就是我们所说的出队），发现它没有消息，那么此时任务 A 有 3 个选择：第一个选择，任务 A 扭头就走，既然队列没有消息，那我也不等了，干其它事情去，这样子
任务 A 不会进入阻塞态；第二个选择，任务 A 还是在这里等等吧，可能过一会队列就有消息，此时任务 A 会进入阻塞状态，在等待着消息的道来，而任务 A 的等待时间就由我们自己定义，比如设置 1000 个系统时钟节拍 tick 的
等待，在这 1000 个 tick 到来之前任务 A 都是处于阻塞态，当阻塞的这段时间任务 A 等到了队列的消息，那么任务 A 就会从阻塞态变成就绪态，如果此时任务 A 比当前运行的任务优先级还高，那么，任务 A 就会得到消息并
且运行；假如 1000 个 tick 都过去了，队列还没消息，那任务 A 就不等了，从阻塞态中唤醒，返回一个没等到消息的错误代码，然后继续执行任务 A 的其他代码；第三个选择，任务 A 死等，不等到消息就不走了，这样子任务 A 
就会进入阻塞态，直到完成读取队列的消息。

  当然，只有在任务中发送消息才允许进行阻塞状态，而在中断中发送消息不允许带有阻塞机制的，需要调用在中断中发送消息的 API 函数接口，因为发送消息的上下文环境是在中断中，不允许有阻塞的情况。

  假如有多个任务阻塞在一个消息队列中，那么这些阻塞的任务将按照任务优先级进行排序，优先级高的任务将优先获得队列的访问权。

  <img width="907" height="383" alt="image" src="https://github.com/user-attachments/assets/0e56da48-6d0a-43a0-9eba-782b5869a1c6" />

  下面介绍消息队列的相关函数：

  1.消息队列创建函数 xQueueCreate()。
    
    传入参数：
    uxQueueLength 队列能够存储的最大消息单元数目，即队列长度。
    uxItemSize 队列中消息单元的大小，以字节为单位。
    
    如果创建成功则返回一个队列句柄，用于访问创建的队列。如果创建不成功则返回NULL

  2.消息队列静态创建函数 xQueueCreateStatic()。

    传入参数：
    uxQueueLength 队列能够存储的最大单元数目，即队列深度
    uxItemSize 队列中数据单元的长度，以字节为单位。
    pucQueueStorageBuffer 指针，指向一个 uint8_t 类型的数组，数组的大小至少有uxQueueLength* uxItemSize 个字节。当 uxItemSize 为 0 时，pucQueueStorageBuffer 可以为 NULL。
    pxQueueBuffer 指针，指向 StaticQueue_t 类型的变量，该变量用于存储队列的数据结构。

    如果创建成功则返回一个队列句柄，用于访问创建的队列。如果创建不成功则返回NULL

  3.消息队列删除函数 vQueueDelete()

    传入参数：
    消息队列句柄

  4.向消息队列的队尾发送消息函数 xQueueSend() 

    传入参数:
    xQueue 队列句柄。
    pvItemToQueue 指针，指向要发送到队列尾部的队列消息。
    xTicksToWait 队列满时，等待队列空闲的最大超时时间。如果队列满并 且xTicksToWait 被设置成 0，函数立刻返回。超时时间的单位为系统
    节拍周期，常量 portTICK_PERIOD_MS 用于辅助计算真实的时间，单位为 ms。如果 INCLUDE_vTaskSuspend 设置成 1，并且指定延时
    为 portMAX_DELAY 将导致任务挂起（没有超时）。

    发送消息成功返回 pdTRUE，否则返回 errQUEUE_FULL。

    
5.在中断中向消息队列的队尾发送消息函数 xQueueSendFromISR()

    传入参数：
    xQueue 队列句柄
    pvItemToQueue 指针，指向要发送到队列尾部的消息。
    pxHigherPriorityTaskWoken 如果入队导致一个任务解锁，并且解锁的任务优先级高于当前被中断的任务，则将*pxHigherPriorityTaskWoken
    设置成 pdTRUE，然后在中断退出前需要进行一次上下文切换， 去 执 行 被 唤醒 的 优 先 级 更高 的 任 务 。从
    FreeRTOS V7.3.0 起，pxHigherPriorityTaskWoken 作为一个可选参数，可以设置为 NULL。

    发送消息成功返回 pdTRUE，否则返回 errQUEUE_FULL。
    
这里给出xQueueSendFromISR()函数的使用示例：

    void USART1_IRQHandler(void) {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        
        if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) {
            uint8_t c = USART_ReceiveData(USART1);
            
            // 将接收到的字节发送到队列
            if(xQueueSendFromISR(xUartQueue, &c, &xHigherPriorityTaskWoken) != pdPASS) {
                // 队列已满，可在此处理错误
                USART_SendData(USART1, 'F'); // 发送'F'表示队列满
            }
        }
        
        // 如果有更高优先级任务就绪，触发上下文切换
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

 6.向消息队列的队首发送消息函数 xQueueSendToFront() 

    传入参数：
    xQueue 队列句柄。
    pvItemToQueue 指针，指向要发送到队首的消息。
    xTicksToWait 队列满时，等待队列空闲的最大超时时间。如果队列满并 且xTicksToWait 被设置成 0，函数立刻返回。超时时间的单位为系统
    节拍周期，常量 portTICK_PERIOD_MS 用于辅助计算真实的时间，单位为 ms。如果 INCLUDE_vTaskSuspend 设置成 1，并且指定延时
    为 portMAX_DELAY 将导致任务无限阻塞（没有超时）。

    发送消息成功返回 pdTRUE，否则返回 errQUEUE_FULL。

7.在中断服务程序中向消息队列队首发送一个消息 xQueueSendToFrontFromISR()

    传入参数：
    xQueue 队列句柄。
    pvItemToQueue 指针，指向要发送到队首的消息。
    pxHigherPriorityTaskWoken 如果入队导致一个任务解锁，并且解锁的任务优先级高当前被中断的任务，则将*pxHigherPriorityTaskWoken
    设置成 pdTRUE，然后在中断退出前需要进行一次上下文切换， 去 执 行 被 唤醒 的 优 先 级 更高 的 任 务 。从
    FreeRTOS V7.3.0 起，pxHigherPriorityTaskWoken 作为一个可选参数，可以设置为 NULL。

    队列项投递成功返回 pdTRUE，否则返回 errQUEUE_FULL。

使用示例：

    // 定时器中断处理程序(高优先级消息插队)
    void TIM2_IRQHandler(void)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        EmergencyMsg_t urgentMsg;
        
        if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
        {
            // 构造紧急消息
            urgentMsg.type = BATTERY_LOW;
            urgentMsg.timestamp = xTaskGetTickCountFromISR();
            
            // 将紧急消息插入队列头部(LIFO方式)
            if(xQueueSendToFrontFromISR(xEmergencyQueue, &urgentMsg, &xHigherPriorityTaskWoken) != pdPASS)
            {
                // 队列已满时的处理
                LogError_FROM_ISR(ERROR_QUEUE_FULL);
            }
            
            // 清除中断标志
            TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
        }

        // 如果有更高优先级任务就绪，触发上下文切换
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

8.用于从一个队列中接收消息，并把接收的消息从队列中删除 xQueueReceive()

    传入参数：
    xQueue 队列句柄。
    pvBuffer 指针，指向接收到要保存的数据。
    xTicksToWait 队列空时，阻塞超时的最大时间。如果该参数设置为 0，函数立刻返回。超时时间的单位为系统节拍周期，常量 portTICK_PERIOD_MS 用
    于辅助计算真实的时间，单位为 ms。如果 INCLUDE_vTaskSuspend 设 置成 1，并且指定延时为 portMAX_DELAY 将导致任务无限阻塞（没有超时）。

    队列项接收成功返回 pdTRUE，否则返回 pdFALSE。

9.用于从一个队列中接收消息，但不把接收的消息从队列中删除 xQueuePeek()

    传入参数：
    xQueue 队列句柄。
    pvBuffer 指针，指向接收到要保存的数据。
    xTicksToWait 队列空时，阻塞超时的最大时间。如果该参数设置为 0，函数立刻返回。超时时间的单位为系统节拍周期，常量 portTICK_PERIOD_MS 用
    于辅助计算真实的时间，单位为 ms。如果 INCLUDE_vTaskSuspend 设 置成 1，并且指定延时为 portMAX_DELAY 将导致任务无限阻塞（没有超时）。

    队列项接收成功返回 pdTRUE，否则返回 pdFALSE。
    
10.在中断中从一个队列中接收消息，并从队列中删除该消息。xQueueReceiveFromISR()

    传入参数：
    xQueue 队列句柄。
    pvBuffer 指针，指向接收到要保存的数据。
    pxHigherPriorityTaskWoken 任务在往队列投递信息时，如果队列满，则任务将阻塞在该队列上。如果 xQueueReceiveFromISR()到账了一个
    任 务 解 锁 了 则将 *pxHigherPriorityTaskWoken 设 置为pdTRUE ， 否 则 *pxHigherPriorityTaskWoken 的 值将不
    变。从 FreeRTOS V7.3.0 起，pxHigherPriorityTaskWoken作为一个可选参数，可以设置为 NULL。

    队列项接收成功返回 pdTRUE，否则返回 pdFALSE。

使用示例：


      // 接收ADC采样数据的中断服务程序
      void ADC_IRQHandler(void)
      {
          BaseType_t xHigherPriorityTaskWoken = pdFALSE;
          uint16_t adcValue;
          
          if(ADC_GetITStatus(ADC_IT_EOC) != RESET)
          {
              // 从队列接收数据(会移除数据)
              if(xQueueReceiveFromISR(xAdcQueue, &adcValue, &xHigherPriorityTaskWoken) == pdPASS)
              {
                  // 成功接收到数据
                  ADC_ValueProcess(adcValue); // 处理ADC值
              }
              else
              {
                  // 队列为空时的处理
                  ADC_StartConversion(); // 重新启动转换
              }
              
              // 清除中断标志
              ADC_ClearITPendingBit(ADC_IT_EOC);
          }
          
          // 如果有更高优先级任务就绪，触发上下文切换
          portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
      }



11.在中断中从一个队列中接收消息，但并不会把消息从该队列中移除。 xQueuePeekFromISR()

    传入参数：
    xQueue 队列句柄。
    pvBuffer 指针，指向接收到要保存的数据。

    队列项接收(peek)成功返回 pdTRUE，否则返回 pdFALSE。


使用示例：

    // UART接收中断查看队列头部的紧急命令
    void USART1_IRQHandler(void)
    {
        BaseType_t xHigherPriorityTaskWoken = pdFALSE;
        uint8_t nextCmd;
        
        if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
        {
            uint8_t receivedByte = USART_ReceiveData(USART1);
            
            // 查看队列头部数据(不移除)
            if(xQueuePeekFromISR(xUartCmdQueue, &nextCmd) == pdPASS)
            {
                if(nextCmd == 0xFF) // 检查是否是紧急命令
                {
                    // 立即处理紧急命令
                    ProcessEmergencyCmd(nextCmd);
                    
                    // 然后移除该命令
                    xQueueReceiveFromISR(xUartCmdQueue, &nextCmd, &xHigherPriorityTaskWoken);
                }
            }
            
            // 将新接收的字节存入队列
            xQueueSendToBackFromISR(xUartCmdQueue, &receivedByte, &xHigherPriorityTaskWoken);
        }

        // 如果有更高优先级任务就绪，触发上下文切换
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }


    
