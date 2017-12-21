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

void tTaskRunFirst(void);
void tTaskSwitch(void);

#endif
