  前面我们讲过如何创建静态任务，这期我讲解如何创建动态任务

  动态任务适用于:通用嵌入式系统（有堆内存管理）;任务需频繁创建/删除（如动态加载模块）;快速原型开发（代码更简洁）.但需要配置FreeRTOS的堆内存

  1.在FreeRTOSConfig.h头文件中修改configSUPPORT_DYNAMIC_ALLOCATION宏定义，将其配置为1

  2.创建任务句柄
  <img width="354" height="84" alt="image" src="https://github.com/user-attachments/assets/bd342997-b164-49c8-8454-9298d991b293" />

  3.创建任务,进入临界段创建任务
  <img width="861" height="567" alt="image" src="https://github.com/user-attachments/assets/94f7a9f2-9a1f-48cc-982f-0c0a0fd3fa73" />

  在main函数的调用如图所示
  <img width="836" height="277" alt="image" src="https://github.com/user-attachments/assets/2ccede9d-3469-4261-8af3-a237e5b4d751" />


