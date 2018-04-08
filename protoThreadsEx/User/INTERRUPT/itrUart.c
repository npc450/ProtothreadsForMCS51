#include "all.h"

void itrUartInit(void)//@18.432MHz
{
#if UART1 == 1
	//9600bps,n,8,1
	PCON &= 0x7F;//波特率不倍速
	SCON = 0x50; //8位数据,可变波特率
	AUXR |= 0x40;//定时器1时钟为Fosc,即1T
	AUXR &= 0xFE;//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;//清除定时器1模式位
	TMOD |= 0x20;//设定定时器1为8位自动重装方式
	TL1 = 0xC4;	 //设定定时初值
	TH1 = 0xC4;	 //设定定时器重装值
	ET1 = 0;		 //禁止定时器1中断
	TR1 = 1;		 //启动定时器1
#endif
#if	UART2 == 1
	//开串口2:115200
	S2CON = 0x50;//8位数据,可变波特率
	AUXR |= 0x04;//定时器2时钟为Fosc,即1T
	T2L = 0xD8;	 //设定定时初值
	T2H = 0xFF;	 //设定定时初值
	AUXR |= 0x10;//启动定时器2
#endif
#if	UART3 == 1
	//开串口3:9600
	S3CON = 0x10;	//8位数据,可变波特率
	S3CON |= 0x40;//8位数据,可变波特率
	T4T3M |= 0x02;//定时器3为波特率计算器
	T3L = 0x20;	  //设定定时初值
	T3H = 0xFE;		//设定定时初值
	T4T3M |= 0x08;//启动定时器3
#endif
#if	UART4 == 1
	//开串口4:9600
	S4CON = 0x10;	//8位数据,可变波特率
	S4CON |= 0x40;//8位数据,可变波特率
	T4T3M |= 0x20;//定时器4为波特率计算器
	T4L = 0x20;	  //设定定时初值
	T4H = 0xFE;		//设定定时初值
	T4T3M |= 0x80;//启动定时器4
#endif
}
#if UART1 == 1
void uartPrintChar1(unsigned char ch)
{
    SBUF = ch;
		while(TI == 0);
		TI = 0;
}
void uartPrintStr1(char *s)
{
    while (*s)
    {
        uartPrintChar1(*s++);
    }
}
void Uart1() interrupt 4
{
	if(RI)
  {
		RI = 0;
		// = SBUF;
  }
  if (TI)
  {
    TI = 0;
  }
}
#endif
#if UART2 == 1
void uartPrintChar2(unsigned char ch)
{
    S2BUF = ch;
		while(!(S2CON&S2TI));    
    S2CON&=~S2TI;
}
void uartPrintStr2(char *s)
{
    while (*s)
    {
        uartPrintChar2(*s++);
    }
}
void Uart2() interrupt 8
{
	if (S2CON & S2RI)
  {
		S2CON &= ~S2RI;
		// = S2BUF;
  }
  if (S2CON & S2TI)
  {
    S2CON &= ~S2TI;
  }
}
#endif
#if UART3 == 1
void uartPrintChar3(unsigned char ch)
{
    S3BUF = ch;
		while(!(S3CON&S3TI));    
    S3CON&=~S3TI;
}
void uartPrintStr3(char *s)
{
    while (*s)
    {
        uartPrintChar3(*s++);
    }
}
void Uart3() interrupt 17
{
	if (S3CON & S3RI)
	{
		S3CON &= ~S3RI;
		// = S3BUF;
	}
  if (S3CON & S3TI)
  {
    S3CON &= ~S3TI;
  }
}
#endif
#if UART4 == 1
void uartPrintChar4(unsigned char ch)
{
    S4BUF = ch;
		while(!(S4CON&S4TI));    
    S4CON&=~S4TI;
}
void uartPrintStr4(char *s)
{
    while (*s)
    {
        uartPrintChar4(*s++);
    }
}
void Uart4() interrupt 18
{
	if (S4CON & S4RI)
  {
		S4CON &= ~S4RI;
		// = S4BUF;
  }
  if (S4CON & S4TI)
  {
		S4CON &= ~S4TI;
  }
}
#endif