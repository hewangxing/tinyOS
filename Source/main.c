/************************************************************
FileName:     main.c
Author:       hewangxing
Date:         2017/12/21
Description:  实现基于时间片的双任务的切换
Version:      c2.03
***********************************************************/

#include "tinyOS.h"
#include "ARMCM3.h"

// 全局变量
tTask *currentTask;     // 指向当前任务
tTask *nextTask;        // 指向下一个任务
tTask *tTaskTable[2];   // 任务表

// 任务初始化
void tTaskInit(tTask *task, void (*entry)(void *), void *param, tTaskStack *stack)
{
	*(--stack) = (unsigned long)(1 << 24);   // xPSR, Thumb状态
	*(--stack) = (unsigned long)entry;       // PC, 函数入口
	*(--stack) = (unsigned long)0x14;        // LR
	*(--stack) = (unsigned long)0x12;           
	*(--stack) = (unsigned long)0x3;
	*(--stack) = (unsigned long)0x2;
	*(--stack) = (unsigned long)0x1;
	*(--stack) = (unsigned long)param;       // R0, 函数传参
	
	*(--stack) = (unsigned long)0x11;
	*(--stack) = (unsigned long)0x10;
	*(--stack) = (unsigned long)0x9;
	*(--stack) = (unsigned long)0x8;
	*(--stack) = (unsigned long)0x7;
	*(--stack) = (unsigned long)0x6;
	*(--stack) = (unsigned long)0x5;
	*(--stack) = (unsigned long)0x4;
	
	task->stack = stack;
}

// 任务调度
void tTaskSched(void)
{
		if (currentTask == tTaskTable[0])
		{
			nextTask = tTaskTable[1];
		}
		else
		{
			nextTask = tTaskTable[0];
		}
		tTaskSwitch();
}

// 通过SysTick来设置os的时间片
void tSetSysTickPeriod(uint32_t ms)
{
	SysTick->LOAD = ms * SystemCoreClock / 1000 -1;
	NVIC_SetPriority(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
									SysTick_CTRL_TICKINT_Msk |
									SysTick_CTRL_ENABLE_Msk;
}

// SysTick异常处理函数
void SysTick_Handler(void)
{
	tTaskSched();
}

// 软延时
void delay(int count)
{
	while (--count);
}

int task1flag;
// 任务1
void task1(void *param)
{
	tSetSysTickPeriod(10);
	for (;;)
	{
		task1flag = 0;
		delay(100);
		task1flag = 1;
		delay(100);
	}
}

int task2flag;
// 任务2
void task2(void *param)
{	
	for (;;)
	{
		task2flag = 0;
		delay(100);
		task2flag = 1;
		delay(100);
	}
}

// 全局变量
tTask tTask1;
tTask tTask2;

tTaskStack task1Env[1024];
tTaskStack task2Env[1024];

// 主函数
int main()
{
	tTaskInit(&tTask1, task1, (void *)0x11111111, &task1Env[1024]);
	tTaskInit(&tTask2, task2, (void *)0x22222222, &task2Env[1024]);
	
	tTaskTable[0] = &tTask1;
	tTaskTable[1] = &tTask2;
	nextTask = tTaskTable[0];
	
	tTaskRunFirst();
	
	return 0;
}
