#include "tinyOS.h"
#include <ARMCM3.h>

#define	NVIC_INT_CTRL			0xE000Ed04 	// �жϿ��Ƽ�״̬�Ĵ���
#define	NVIC_PENDSVSET		0x10000000 	// ��������жϵ�ֵ
#define	NVIC_SYSPRI2			0xE000ED22 	// ϵͳ���ȼ��Ĵ���
#define	NVIC_PENDSV_PRI		0x000000FF	// �������ȼ�

#define MEM32(addr) (*(volatile unsigned long *)(addr))
#define MEM8(addr)  (*(volatile unsigned char *)(addr))

/*************************************************
  Function:       PendSV_Handler
  Description:    PendSVC�쳣������
  Input:          ��
  Output:         ��
  Return:         ��
  Others:         
*************************************************/
__asm void PendSV_Handler(void)
{
	IMPORT currentTask
	IMPORT nextTask
	
	MRS R0, PSP							;�ж�PSP�Ƿ�Ϊ0
	CBZ R0, PendSVHandler_nosave
	STMDB R0!, {R4-R11} 		;������һ�������R4-R11   
	 
	LDR R1, =currentTask		;����currentTask->stack
	LDR R1, [R1]
	STR R0, [R1]
PendSVHandler_nosave
	LDR R0, =currentTask		;����һ��������Ϊ��ǰ����
	LDR R1, =nextTask
	LDR R2, [R1]
	STR R2, [R0]
	
	LDR R0, [R2]
	LDMIA R0!, {R4-R11} 		;�ָ���һ�������R4-R11
	
	MSR PSP, R0
	;ORR LR, LR, #0x04
	MOV LR, #0xFFFFFFFD
	BX LR
	
}

// ���е�һ������
void tTaskRunFirst(void)
{
	__set_PSP(0);
	MEM8(NVIC_SYSPRI2) = NVIC_PENDSV_PRI;
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;	
}

// �����л�
void tTaskSwitch(void)
{
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;	
}
