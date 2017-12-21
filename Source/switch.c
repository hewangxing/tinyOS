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
	IMPORT blockPtr
	
	LDR R0, =blockPtr 
	LDR R0, [R0]
	LDR R0, [R0]
	
	STMDB R0!, {R4-R11} ;������һ�������R4-R11
	
	LDR R1, =blockPtr   ;����blockPtr->stackPtr
	LDR R1, [R1]
	STR R0, [R1]
	
	ADD R4, R4, #1
	ADD R5, R5, #1
	
	LDMIA R0!, {R4-R11} ;�ָ���һ�������R4-R11
	
	BX LR
	NOP
}
