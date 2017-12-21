/************************************************************
FileName:     main.c
Author:       hewangxing
Date:         2017/12/21
Description:  ��ӽ����ٽ����Ĵ��룬��Ч�����˹�����Դ�ķ��ʳ�ͻ
Version:      c3.01
***********************************************************/

#include "tinyOS.h"
#include "ARMCM3.h"

// ȫ�ֱ���
tTask *currentTask;     // ָ��ǰ����
tTask *nextTask;        // ָ����һ������
tTask *tTaskTable[3];   // �����
uint32_t tickCounter;

// �����ʼ��
void tTaskInit(tTask *task, void (*entry)(void *), void *param, tTaskStack *stack)
{
	*(--stack) = (unsigned long)(1 << 24);   // xPSR, Thumb״̬
	*(--stack) = (unsigned long)entry;       // PC, �������
	*(--stack) = (unsigned long)0x14;        // LR
	*(--stack) = (unsigned long)0x12;           
	*(--stack) = (unsigned long)0x3;
	*(--stack) = (unsigned long)0x2;
	*(--stack) = (unsigned long)0x1;
	*(--stack) = (unsigned long)param;       // R0, ��������
	
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

// �������
void tTaskSched(void)
{
	  
	  uint32_t status = tEnterCritical();		  // �����ٽ���
		if (currentTask == tTaskTable[1])       // ��ǰ����Ϊ����1
		{
			if (tTaskTable[2]->taskDelay == 0)
			{
				nextTask = tTaskTable[2];
			}
			else
			{
				if (currentTask->taskDelay != 0)
				{			
					nextTask = tTaskTable[0];
				}
				else
				{
					
					tExitCritical(status);						// �˳��ٽ���
					return;
				}
			}
		}
		else if (currentTask == tTaskTable[2])	// ��ǰ����Ϊ����2
		{
			if (tTaskTable[1]->taskDelay == 0)
			{
				nextTask = tTaskTable[1];
			}
			else
			{
				if (currentTask->taskDelay != 0)
				{
					nextTask = tTaskTable[0];					
				}
				else
				{
					
					tExitCritical(status);						// �˳��ٽ���
					return;
				}
			}
		}
		else 																 	  // ��ǰ����Ϊ��������
		{
			if (tTaskTable[1]->taskDelay == 0)
			{
				nextTask = tTaskTable[1];
			}
			else if (tTaskTable[2]->taskDelay == 0)
			{
				nextTask = tTaskTable[2];
			}
			else
			{
				
	      tExitCritical(status);							// �˳��ٽ���
				return;
			}			
		}
		
	  tExitCritical(status);									// �˳��ٽ���
		tTaskSwitch();
}

// ͨ��SysTick������os��ʱ��Ƭ
void tSetSysTickPeriod(uint32_t ms)
{
	SysTick->LOAD = ms * SystemCoreClock / 1000 -1;
	NVIC_SetPriority(SysTick_IRQn, (1 << __NVIC_PRIO_BITS) - 1);
	SysTick->VAL = 0;
	SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk |
									SysTick_CTRL_TICKINT_Msk |
									SysTick_CTRL_ENABLE_Msk;
}

// ������ʱ����
void tTaskDelay(uint32_t tick)
{

	uint32_t status = tEnterCritical();			 // �����ٽ���
	currentTask->taskDelay = tick;	
	tExitCritical(status);									 // �˳��ٽ���
	
	tTaskSched();
}

// ��SysTick�ж��д���������ʱ
void tTaskSystemTickHandler(void)
{
	uint8_t i = 0;
	
	
	uint32_t status = tEnterCritical();			 // �����ٽ���
	for (i = 0; i < 3; i++)
	{
		if (tTaskTable[i]->taskDelay > 0)
		{
			tTaskTable[i]->taskDelay--;
		}
	}
	tickCounter++;
	tExitCritical(status);		               // �˳��ٽ���
	
	tTaskSched();
}

// SysTick�쳣������
void SysTick_Handler(void)
{
	tTaskSystemTickHandler();
}

// �����ٽ���
uint32_t tEnterCritical(void)
{
	uint32_t primask = __get_PRIMASK();
	__disable_irq();
	
	return primask;
}

// �˳��ٽ���
void tExitCritical(uint32_t status)
{
	__set_PRIMASK(status);
}

// ����ʱ
void delay(int count)
{
	while (--count);
}

int task1flag;
// ����1
void task1(void *param)
{
	for (;;)
	{
		task1flag = 0;
		tTaskDelay(10);
		task1flag = 1;
		tTaskDelay(10);
	}
}

int task2flag;
// ����2
void task2(void *param)
{	
	for (;;)
	{
		uint32_t i = 0;
		
		
		uint32_t status = tEnterCritical();		 // �����ٽ���
		uint32_t counter = tickCounter;
		for (i = 0; i < 0xFFFF; i++) {}
		tickCounter = counter + 1;				
		tExitCritical(status);                 // �˳��ٽ���
			
		task2flag = 0;
		tTaskDelay(10);
		task2flag = 1;
		tTaskDelay(10);
	}
}

tTask tIdleTask;
tTaskStack idleTaskEnv[1024];
// ��������
void idleTask(void *param)
{	
	tSetSysTickPeriod(10);
	for (;;)
	{
		
	}
}

// ȫ�ֱ���
tTask tTask1;
tTask tTask2;

tTaskStack task1Env[1024];
tTaskStack task2Env[1024];

// ������
int main()
{
	tTaskInit(&tIdleTask, idleTask, (void *)0x11111111, &idleTaskEnv[1024]);
	tTaskInit(&tTask1, task1, (void *)0x22222222, &task1Env[1024]);
	tTaskInit(&tTask2, task2, (void *)0x33333333, &task2Env[1024]);
		
	tTaskTable[0] = &tIdleTask;
	tTaskTable[1] = &tTask1;
	tTaskTable[2] = &tTask2;
	nextTask = tTaskTable[0];
	
	tTaskRunFirst();
	
	return 0;
}
