在FreeRTOS中，创建任务有俩种方式，一种是创建动态任务，一种是创建静态任务

静态任务适用于:无动态内存的系统（如资源受限的裸机环境），对内存分配有严格要求的场景（如汽车电子、医疗设备），需完全控制任务内存布局时（如配合 MPU 实现内存保护）

这一节讲解创建静态任务的步骤。

1.创建任务句柄
<img width="360" height="76" alt="image" src="https://github.com/user-attachments/assets/fb775670-b3dc-45fb-8560-38cb1305e118" />

2.创建任务栈
<img width="668" height="185" alt="image" src="https://github.com/user-attachments/assets/5fe96511-e2eb-4806-bcd9-76891438f431" />

3.创建任务控制块
<img width="530" height="100" alt="image" src="https://github.com/user-attachments/assets/9c2ad4c8-62a2-4ce9-8bd2-190f6f96094a" />

4.必须定义vApplicationGetIdleTaskMemory函数完成空闲任务的内存分配
<img width="1250" height="227" alt="image" src="https://github.com/user-attachments/assets/9ecbb287-a16c-4360-97bc-e2af4d4a12e5" />

函数原型如下，可直接复制

    /**
     * @brief 	获取定时器任务的任务堆栈和任务控制块内存
     * @param 	ppxTimerTaskTCBBuffer:任务控制块内存
    						ppxTimerTaskStackBuffer:任务堆栈的内存
    						pulTimerTaskStackSize:任务堆栈的大小
     * @retval 
     */
    void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer, 
    																		StackType_t **ppxTimerTaskStackBuffer, 
    																		uint32_t *pulTimerTaskStackSize)
    {
    	*ppxTimerTaskTCBBuffer=&Timer_Task_TCB;
    	*ppxTimerTaskStackBuffer=Timer_Task_Stack;
    	*pulTimerTaskStackSize=configTIMER_TASK_STACK_DEPTH;
    }
    
    
    /**
     * @brief 	获取空闲任务的任务堆栈和任务控制块内存
     * @param 	ppxIdleTaskTCBBuffer:任务控制块内存
    						ppxIdleTaskStackBuffer:任务堆栈的内存
    						pulIdleTaskStackSize:任务堆栈的大小
     * @retval 
     */
    void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer, 
    																	 StackType_t **ppxIdleTaskStackBuffer, 
    																	 uint32_t *pulIdleTaskStackSize)
    {
    	*ppxIdleTaskTCBBuffer=&Idle_Task_TCB;
    	*ppxIdleTaskStackBuffer=Idle_Task_Stack;
    	*pulIdleTaskStackSize=configMINIMAL_STACK_SIZE;
    }


5.创建静态任务，这里我统一到apptaskcreate函数中创建，在创建任务的过程中，要开启临界段保护，最后删除apptaskcreate任务并关闭临界段保护
<img width="868" height="552" alt="image" src="https://github.com/user-attachments/assets/20fae1c0-3f4b-46ad-ad90-ab65fed93550" />

6.main函数中创建apptaskcreate任务，如果成功，则启动任务调度。
<img width="791" height="422" alt="image" src="https://github.com/user-attachments/assets/be29aa5e-cf20-43d2-91bb-823728b3519d" />


