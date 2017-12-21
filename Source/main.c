
#define	NVIC_INT_CTRL			0xE000Ed04 	// 中断控制及状态寄存器
#define	NVIC_PENDSVSET		0x10000000 	// 触发软件中断的值
#define	NVIC_SYSPRI2			0xE000ED22 	// 系统优先级寄存器
#define	NVIC_PENDSV_PRI		0x000000FF	// 配置优先级

#define MEM32(addr) (*(volatile unsigned long *)(addr))
#define MEM8(addr)  (*(volatile unsigned char *)(addr))
	
typedef struct _BlockType_t
{
	unsigned long *stackPtr; // 堆栈指针

}BlockType_t;

// 触发PendSV异常
void triggerPendSVC(void)
{
	MEM8(NVIC_SYSPRI2) = NVIC_PENDSV_PRI;
	MEM32(NVIC_INT_CTRL) = NVIC_PENDSVSET;
}

// 软延时
void delay(int count)
{
	while (--count);
}

// 全局变量
int flag;
BlockType_t block;
BlockType_t *blockPtr;
unsigned long stackBuffer[1024]; // 堆栈缓冲区

// 主函数
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
