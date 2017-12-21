//  延时
void delay(int count)
{
	while (--count);
}

int flag;
// 主函数
int main()
{
	for (;;)
	{
		flag = 0; 
		delay(100);
		flag = 1;
		delay(100);
	}
}
