
#define	NVIC_INT_CTRL			0xE000Ed04 	// �жϿ��Ƽ�״̬�Ĵ���
#define	NVIC_PENDSVSET		0x10000000 	// ��������жϵ�ֵ
#define	NVIC_SYSPRI2			0xE000ED22 	// ϵͳ���ȼ��Ĵ���
#define	NVIC_PENDSV_PRI		0x000000FF	// �������ȼ�

#define MEM32(addr) (*(volatile unsigned long *)(addr))
#define MEM8(addr)  (*(volatile unsigned char *)(addr))
	
typedef struct _BlockType_t
{
	unsigned long *stackPtr; // ��ջָ��

}BlockType_t;

// ����PendSV�쳣
void triggerPendSVC(void)
{
	MEM8(NVIC_SYSPRI2) = NVIC_PENDSV_PRI;
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;
}

// ����ʱ
void delay(int count)
{
	while (--count);
}

// ȫ�ֱ���
int flag;
BlockType_t block;
BlockType_t *blockPtr;
unsigned long stackBuffer[1024]; // ��ջ������

// ������
int main()
{
	block.stackPtr = &stackBuffer[1024];
	blockPtr = &block;
	for (;;)
	{
		flag = 0; 
		delay(100);
		flag = 1;
		delay(100);
		
		triggerPendSVC();
	}
}
