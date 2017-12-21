#ifndef TINYOS_H
#define TINYOS_H

#include <stdint.h>

typedef uint32_t tTaskStack; 

typedef struct _tTask
{
	tTaskStack *stack;   // ��ջָ��
	uint32_t taskDelay;	 // ��ʱ��
	
}tTask;

extern tTask *currentTask;
extern tTask *nextTask;


uint32_t tEnterCritical(void);        // �����ٽ���
void tExitCritical(uint32_t status);  // �˳��ٽ���

void tTaskRunFirst(void);
void tTaskSwitch(void);

#endif
