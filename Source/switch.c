#include "tinyOS.h"
#include <ARMCM3.h>

#define	NVIC_INT_CTRL			0xE000Ed04 	// 中断控制及状态寄存器
#define	NVIC_PENDSVSET		0x10000000 	// 触发软件中断的值
#define	NVIC_SYSPRI2			0xE000ED22 	// 系统优先级寄存器
#define	NVIC_PENDSV_PRI		0x000000FF	// 配置优先级

#define MEM32(addr) (*(volatile unsigned long *)(addr))
#define MEM8(addr)  (*(volatile unsigned char *)(addr))

/*************************************************
  Function:       PendSV_Handler
  Description:    PendSVC异常处理函数
  Input:          无
  Output:         无
  Return:         无
  Others:         
*************************************************/
__asm void PendSV_Handler(void)
{
	IMPORT currentTask
	IMPORT nextTask
	
	MRS R0, PSP							;判断PSP是否为0
	CBZ R0, PendSVHandler_nosave
	STMDB R0!, {R4-R11} 		;保存上一个任务的R4-R11   
	 
	LDR R1, =currentTask		;更新currentTask->stack
	LDR R1, [R1]
	STR R0, [R1]
PendSVHandler_nosave
	LDR R0, =currentTask		;将下一个任务作为当前任务
	LDR R1, =nextTask
	LDR R2, [R1]
	STR R2, [R0]
	
	LDR R0, [R2]
	LDMIA R0!, {R4-R11} 		;恢复下一个任务的R4-R11
	
	MSR PSP, R0
	;ORR LR, LR, #0x04
	MOV LR, #0xFFFFFFFD
	BX LR
	
}

// 运行第一个任务
void tTaskRunFirst(void)
{
	__set_PSP(0);
	MEM8(NVIC_SYSPRI2) = NVIC_PENDSV_PRI;
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;	
}

// 任务切换
void tTaskSwitch(void)
{
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;	
}
