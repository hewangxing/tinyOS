/************************************************************
FileName:     main.c
Author:       hewangxing
Date:         2017/12/21
Description:  定义任务的外观及初始化
Version:      c2.01
***********************************************************/

#include "tinyOS.h"

#define	NVIC_INT_CTRL			0xE000Ed04 	// 中断控制及状态寄存器
#define	NVIC_PENDSVSET		0x10000000 	// 触发软件中断的值
#define	NVIC_SYSPRI2			0xE000ED22 	// 系统优先级寄存器
#define	NVIC_PENDSV_PRI		0x000000FF	// 配置优先级

#define MEM32(addr) (*(volatile unsigned long *)(addr))
#define MEM8(addr)  (*(volatile unsigned char *)(addr))

// 触发PendSV异常
void triggerPendSVC(void)
{
	MEM8(NVIC_SYSPRI2) = NVIC_PENDSV_PRI;
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;
}

// 任务初始化
void tTaskInit(tTask *task, void (*entry)(void *), void *param, tTaskStack *stack)
{
	task->stack = stack;
}

// 软延时
void delay(int count)
{
	while (--count);
}

// 任务1
void task1(void *param)
{
	for (;;)
	{
		
	}
}

//任务2
void task2(void *param)
{
	for (;;)
	{

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
	
	return 0;
}
