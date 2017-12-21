/************************************************************
FileName:     main.c
Author:       hewangxing
Date:         2017/12/21
Description:  �����������ۼ���ʼ��
Version:      c2.01
***********************************************************/

#include "tinyOS.h"

#define	NVIC_INT_CTRL			0xE000Ed04 	// �жϿ��Ƽ�״̬�Ĵ���
#define	NVIC_PENDSVSET		0x10000000 	// ��������жϵ�ֵ
#define	NVIC_SYSPRI2			0xE000ED22 	// ϵͳ���ȼ��Ĵ���
#define	NVIC_PENDSV_PRI		0x000000FF	// �������ȼ�

#define MEM32(addr) (*(volatile unsigned long *)(addr))
#define MEM8(addr)  (*(volatile unsigned char *)(addr))

// ����PendSV�쳣
void triggerPendSVC(void)
{
	MEM8(NVIC_SYSPRI2) = NVIC_PENDSV_PRI;
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;
}

// �����ʼ��
void tTaskInit(tTask *task, void (*entry)(void *), void *param, tTaskStack *stack)
{
	task->stack = stack;
}

// ����ʱ
void delay(int count)
{
	while (--count);
}

// ����1
void task1(void *param)
{
	for (;;)
	{
		
	}
}

//����2
void task2(void *param)
{
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
	tTaskInit(&tTask1, task1, (void *)0x11111111, &task1Env[1024]);
	tTaskInit(&tTask2, task2, (void *)0x22222222, &task2Env[1024]);
	
	return 0;
}
