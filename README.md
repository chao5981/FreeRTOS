  在上一节中，我们用消息队列传递了一个消息，LED任务接受到消息就会执行对应的操作。实际上，准确来说的话，我们这叫信号，而不是消息。如果是传递信号的话，Freertos有专门的变量--信号量

  信号量分为以下几种类型：

  1.二值信号量：只能用于俩个任务之间的通信，且只有0（不可用）或 1（可用）俩种状态。例如：像公共厕所的钥匙：只有一把钥匙（信号量=1），谁拿到钥匙谁用，用完挂回去（释放），其他人才能用。
  
  2.计数信号量：允许N个线程同时访问资源（N>1），信号量的值=当前可用资源数。像停车场的剩余车位显示牌：共有5个车位（信号量=5），来一辆车减1，满员时新车需等待。

  3.互斥信号量：本质是二值信号量，但强调所有权（谁加锁，谁解锁），并且具有优先级继承机制。例如：办公室的门禁卡：只有持卡人（线程）能刷卡进入（加锁），离开时必须自己刷卡解锁（防止别人误操作）。

  4.递归(互斥)信号量：可以不断的加锁，并需对应次数的解锁。例如：家门的多重锁：你可以用同一把钥匙反复锁门（加锁），但必须开锁相同次数才能打开门。

  在这四个信号量类型中，最常用的是二值信号量，计数信号量和递归信号量我想不到有什么裸机开发的应用场景，互斥量在下一节会有讲解。

  所以此处重点讲解二值信号量的使用场景。比如，某个任务需要等待一个标记，那么任务可以在轮询中查询这个标记有没有被置位，但是这样子做，就会很消耗 CPU资源并且妨碍其它任务执行，更好的做法是任务的大部分时间处于
阻塞状态（允许其它任务执行），直到某些事件发生该任务才被唤醒去执行。可以使用二进制信号量实现这种同步，当任务取信号量时，因为此时尚未发生特定事件，信号量为空，任务会进入阻塞状态；当事件的条件满足后，
任务/中断便会释放信号量，告知任务这个事件发生了，任务取得信号量便被唤醒去执行对应的操作，任务执行完毕并不需要归还信号量，这样子的 CPU 的效率可以大大提高，而且实时响应也是最快的。

  再比如某个任务使用信号量在等中断的标记的发生，在这之前任务已经进入了阻塞态，在等待着中断的发生，当在中断发生之后，释放一个信号量，也就是我们常说的标记，当它退出中断之后，操作系统会进行任务的调度，如果这
个任务能够运行，系统就会把等待这个任务运行起来，这样子就大大提高了我们的效率。

  举个实际的例子：假设我们有一个温湿度的传感器，假设是 1s 采集一次数据，那么我们让他在液晶屏中显示数据出来，这个周期也是要 1s 一次的，如果液晶屏刷新的周期是 100ms 更新一次，那么此时的温湿度的数据还没更新，
液晶屏根本无需刷新，只需要在 1s 后温湿度数据更新的时候刷新即可，否则 CPU 就是白白做了多次的无效数据更新，CPU的资源就被刷新数据这个任务占用了大半，造成 CPU 资源浪费，所以，还是需要同步协调工作，在温
湿度采集完毕之后，进行液晶屏数据的刷新，这样子，才是最准确的，并且不会浪费 CPU的资源。

  再举个例子：我们在串口接收中，我们不知道啥时候有数据发送过来，有一个任务是做接收这些数据处理，总不能在任务中每时每刻都在任务查询有没有数据到来，那样会浪费 CPU 资源，所以在这种情况下使用二值信号量是很
好的办法，当没有数据到来的时候，任务就进入阻塞态，不参与任务的调度，等到数据到来了，释放一个二值信号量，任务就立即从阻塞态中解除，进入就绪态，然后运行的时候处理数据，这样子系统的资源就会很好的被利用起来。

  二值信号量的工作示意图如下:

  <img width="774" height="208" alt="image" src="https://github.com/user-attachments/assets/50f0b000-7521-4e6a-9522-a21b13eaa4b5" />

  <img width="451" height="181" alt="image" src="https://github.com/user-attachments/assets/dfeb5ce2-76df-4ec9-a3ed-d9dfdb2dff34" />

  <img width="819" height="223" alt="image" src="https://github.com/user-attachments/assets/570b3467-82a8-45a0-844f-b4755afbb45e" />


这里简单介绍以下其他信号量可能的应用领域：

  1.计数信号量：
  
（1）资源池管理：管理有限数量的资源（如内存块、网络连接、硬件外设等）。

  示例：数据库连接池（最多 10 个连接）。动态内存分配器（管理空闲内存块）。

（2）生产者-消费者问题：控制缓冲区（队列）的读写访问。

  示例：生产者任务增加信号量（表示数据可消费）。消费者任务减少信号量（表示数据被取出）。

（3）限制并发任务数：防止系统过载（如最多允许 3 个任务同时执行某段代码）



  2.互斥信息量：
  
（1）保护临界区（共享资源）：确保同一时间只有一个任务访问共享数据或硬件。

  示例：修改全局变量（如计数器、状态机）。访问 SPI/I2C 等硬件外设。

（2）需要严格互斥的场景：避免数据竞争（Data Race）导致的不一致问题。



  3.递归(互斥)信号量：

（1）递归函数保护共享资源：函数可能直接或间接调用自身。

  示例：递归算法（如二叉树遍历）。嵌套调用的模块化代码。

（2）复杂代码结构的互斥：确保多层函数调用中资源的一致性。
  


  下面介绍信号量的相关函数：


  1.创建二值信号量：SemaphoreHandle_t xSemaphoreCreateBinary(void);

    返回值：成功返回信号量句柄，失败返回 NULL。

  2.创建计数信号量：SemaphoreHandle_t xSemaphoreCreateCounting(UBaseType_t uxMaxCount, UBaseType_t uxInitialCount);

      uxMaxCount：信号量最大值（资源总数）。
      uxInitialCount：初始可用资源数。
      
      返回值：成功获取返回 pdTRUE，超时返回 pdFALSE。

  3.创建互斥信号量：SemaphoreHandle_t xSemaphoreCreateMutex(void);

      返回值：成功获取返回 pdTRUE，超时返回 pdFALSE。

  4.释放信号量:BaseType_t xSemaphoreGive(SemaphoreHandle_t xSemaphore);(适用于二值信号量，计数信号量，互斥信号量，不适用于递归信号量)

    xSemaphore:二值信号量的句柄
    
    返回值：成功获取返回 pdTRUE，超时返回 pdFALSE。

  5.获取信号量:BaseType_t xSemaphoreTake(SemaphoreHandle_t xSemaphore, TickType_t xTicksToWait);(适用于二值信号量，计数信号量，互斥信号量，不适用于递归信号量)

    xSemaphore:二值信号量的句柄
    xTicksToWait：等待时间（单位：Tick），portMAX_DELAY 表示无限等待。
    
    返回值：成功获取返回 pdTRUE，超时返回 pdFALSE。

  6.中断版本释放信号量：xSemaphoreGiveFromISR();(只适用于二值信号量和计数信号量，不适用于互斥信号量和递归信号量)

    xSemaphore：信号量句柄。
    pxHigherPriorityTaskWoken：若释放信号量唤醒了更高优先级的任务，此参数会被设为 pdTRUE。

    最后需要portYIELD_FROM_ISR(pxHigherPriorityTaskWoken)决定释放退出中断后进行上下文切换

  使用例子如下：

  <img width="843" height="425" alt="image" src="https://github.com/user-attachments/assets/baada5e3-4025-41f3-9309-e82f98ec0929" />

  7.中断版本获取信号量：xSemaphoreTakeFromISR();(非常少用，因为一般都是到任务中处理数据，中断要求快进快出;只适用于二值信号量和计数信号量，不适用于互斥信号量和递归信号量)

    xSemaphore 信号量句柄。
    pxHigherPriorityTaskWoken：若释放信号量唤醒了更高优先级的任务，此参数会被设为 pdTRUE。

    最后需要portYIELD_FROM_ISR(pxHigherPriorityTaskWoken)决定释放退出中断后进行上下文切换

  8.创建递归互斥信号量:SemaphoreHandle_t xSemaphoreCreateRecursiveMutex(void);

  9.递归信号量释放信号量：xSemaphoreGiveRecursive(xRecursiveMutex);                 

  10.递归信号量获取信号量：xSemaphoreTakeRecursive(xRecursiveMutex, portMAX_DELAY); 


  为了未来可能会用到除了二值信号量的其他信号量，这里给出其他的代码示例：

  1.计数信号量：

      SemaphoreHandle_t xCountingSemaphore = xSemaphoreCreateCounting(5, 5); // 最大5，初始5

      // 任务：申请资源
      void Task(void *pvParameters) {
          xSemaphoreTake(xCountingSemaphore, portMAX_DELAY); // 占用1个资源
          printf("资源占用中...\n");
          xSemaphoreGive(xCountingSemaphore); // 释放资源
      }

  2.互斥信号量：

    SemaphoreHandle_t xMutex = xSemaphoreCreateMutex(); // 初始为1（可用）
    
    void TaskA(void *pvParameters) {
        xSemaphoreTake(xMutex, portMAX_DELAY); // 加锁
        printf("TaskA 进入临界区\n");
        xSemaphoreGive(xMutex); // 解锁
    }
    
    void TaskB(void *pvParameters) {
        xSemaphoreTake(xMutex, portMAX_DELAY); // 加锁
        printf("TaskB 进入临界区\n");
        xSemaphoreGive(xMutex); // 解锁
    }
    

  3.递归信号量：

    SemaphoreHandle_t xRecursiveMutex = xSemaphoreCreateRecursiveMutex();

    void RecursiveFunc(int level) {
        xSemaphoreTakeRecursive(xRecursiveMutex, portMAX_DELAY); // 递归加锁
        printf("层级 %d\n", level);
        if (level > 0) RecursiveFunc(level - 1); // 递归调用
        xSemaphoreGiveRecursive(xRecursiveMutex); // 递归解锁
    }
  
    

  

  
