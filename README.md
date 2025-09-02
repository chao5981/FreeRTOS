  在实际设计一个产品时，为了降低使用成本，我们会争取最大程度地提高一个芯片CPU的利用率，以致达到一个不造成浪费，并且还能留有一定空间以方便以后进行更新和改良

  那么如何得知一个CPU的利用率呢?在FreeRTOS中，官方给我们提供了一些函数，以便于我们在调试时查看CPU的占用率

  若要查看CPU的利用率，需要按照以下要求进行配置:

    1.初始化基本TIM和update中断，注意NVIC的分组
    2.在FreeRTOSConfig.h头文件中把configGENERATE_RUN_TIME_STATS和configUSE_TRACE_FACILITY置1
    3.加上 extern volatile uint32_t CPU_RunTime;    #define portCONFIGURE_TIMER_FOR_RUN_TIME_STATS() (CPU_RunTime = 0ul)   #define portGET_RUN_TIME_COUNTER_VALUE() 					CPU_RunTime  这三行，
      前者的宏定义用于初始化运行时统计的定时器，后者用于获取当前运行时计数器的值。
    4.注释掉#include "trcRecorder.h" #define INCLUDE_xTaskGetCurrentTaskHandle 这俩行
    5.编写中断服务函数，也就是当基本TIM触发一次数完的中断后，CPU_RunTime++。
    6.编写一个CPU的任务，打印任务的基本信息和各个任务的CPU使用率

  中断服务函数代码如下:

    volatile uint32_t CPU_RunTime=0UL;

    void BASIC_TIM_IRQHandler(void)
    {
      if ( TIM_GetITStatus( BASIC_TIM, TIM_IT_Update) != RESET ) 
      {
        CPU_RunTime++; 
        TIM_ClearITPendingBit(BASIC_TIM , TIM_FLAG_Update);
      }
    }


  CPU任务的模板如下:
  
    static void CPU_Task(void)
    {
    	char CPU_RunInfo[400];
    	while (1)
    	{
    		memset(CPU_RunInfo,0,sizeof(CPU_RunInfo));
    		vTaskList(CPU_RunInfo);
    		printf("---------------------------------------------\r\n");
    		printf("任务名 任务状态 优先级 剩余栈 任务序号\r\n");
    		printf("%s", CPU_RunInfo);
    		printf("---------------------------------------------\r\n");
    		memset(CPU_RunInfo,0,sizeof(CPU_RunInfo));
    		
    		vTaskGetRunTimeStats(CPU_RunInfo);
     		printf("任务名 运行计数 使用率\r\n");
    		printf("%s", CPU_RunInfo);
    		printf("---------------------------------------------\r\n\n");
    		vTaskDelay(1000);
    	}
    	
    }

  在这个任务中，主要是调用了vTaskList()和vTaskGetRunTimeStats()函数，vTaskList()函数生成一个详细的文本表格，显示所有任务的当前状态信息;vTaskGetRunTimeStats() 生成每个任务的 CPU 使用率统计信息，显示每个任务的运行时间占
比。这俩个参数都是只需要传入字符串类型的数组即可。这俩个函数依赖sprintf()函数，把数据存储到一个char数组里面。虽然非常容易看懂且方便，但是由于依赖于sprintf，官方说明这可能会使代码体积增大、占用大量堆栈，并且在不同平台上
可能会产生不同的结果。若是在调试时用这个还可以，实际生产时就把这个任务删去;若本意就是要求可以打印出CPU的利用率的话，官方建议直接调用最底层的uxTaskGetSystemState () 来获取原始统计数据，这个函数可以涵盖前面俩者，只是
编写起来略微麻烦了点

  这里给出uxTaskGetSystemState () 的使用示例:

      void printDetailedTaskInfo(void) {
        // 获取任务数量
        UBaseType_t taskCount = uxTaskGetNumberOfTasks();
        
        // 分配状态数组
        TaskStatus_t *taskStatusArray = pvPortMalloc(taskCount * sizeof(TaskStatus_t));
        
        if (taskStatusArray != NULL) {
            unsigned long totalRunTime;
            
            // 获取系统状态
            UBaseType_t filledCount = uxTaskGetSystemState(
                taskStatusArray, 
                taskCount, 
                &totalRunTime
            );
            
            // 处理任务信息
            for (UBaseType_t i = 0; i < filledCount; i++) {
                printf("Task: %s, State: %d, Runtime: %lu\n",
                       taskStatusArray[i].pcTaskName,
                       taskStatusArray[i].eCurrentState,
                       taskStatusArray[i].ulRunTimeCounter);
            }
            
            vPortFree(taskStatusArray);
        }
    }


  也稍微讲解以下这个函数:UBaseType_t uxTaskGetSystemState(TaskStatus_t * const pxTaskStatusArray,const UBaseType_t uxArraySize,unsigned long * const pulTotalRunTime);

    参数类型:
    pxTaskStatusArray:任务状态数组指针
    uxArraySize:数组大小
    pulTotalRunTime:返回总运行时间（可选）

  而任务状态数组的参数类型非常丰富，有：

      typedef struct xTASK_STATUS {
        TaskHandle_t xHandle;           // 任务句柄
        const char *pcTaskName;         // 任务名称
        UBaseType_t xTaskNumber;        // 任务编号
        eTaskState eCurrentState;       // 当前状态（枚举值）
        UBaseType_t uxCurrentPriority;  // 当前优先级
        UBaseType_t uxBasePriority;     // 基础优先级
        unsigned long ulRunTimeCounter; // 运行时间计数器
        unsigned short usStackHighWaterMark; // 栈高水位标记
    } TaskStatus_t;

  对于当前任务状态，其枚举类型为：

      typedef enum {
        eRunning = 0,    // 正在运行
        eReady,          // 就绪态
        eBlocked,        // 阻塞态
        eSuspended,      // 挂起态
        eDeleted,        // 已删除
        eInvalid         // 无效（不会出现在数组中）
    } eTaskState;


  只需要像示例那样打印出来你感兴趣的任务情况即可
