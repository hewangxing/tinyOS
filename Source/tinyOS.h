#ifndef TINYOS_H
#define TINYOS_H

#include <stdint.h>

typedef uint32_t tTaskStack; 

typedef struct _tTask
{
	tTaskStack *stack;   // 堆栈指针
	uint32_t taskDelay;	 // 计时器
	
}tTask;

extern tTask *currentTask;
extern tTask *nextTask;
extern uint8_t schedLockCount;

uint32_t tEnterCritical(void);        // 进入临界区
void tExitCritical(uint32_t status);  // 退出临界区

void tTaskRunFirst(void);
void tTaskSwitch(void);

//调度器相关
void tTaskSched(void);                // 任务调度
void tTaskSchedInit(void);						// 调度器初始化
void tTaskSchedDisable(void);					// 调度锁上锁
void tTaskSchedEnable(void);					// 调度锁解锁

#endif
