#include "all.h"

void itrUartInit(void)//@18.432MHz
{
#if UART1 == 1
	//9600bps,n,8,1
	PCON &= 0x7F;//�����ʲ�����
	SCON = 0x50; //8λ����,�ɱ䲨����
	AUXR |= 0x40;//��ʱ��1ʱ��ΪFosc,��1T
	AUXR &= 0xFE;//����1ѡ��ʱ��1Ϊ�����ʷ�����
	TMOD &= 0x0F;//�����ʱ��1ģʽλ
	TMOD |= 0x20;//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
	TL1 = 0xC4;	 //�趨��ʱ��ֵ
	TH1 = 0xC4;	 //�趨��ʱ����װֵ
	ET1 = 0;		 //��ֹ��ʱ��1�ж�
	TR1 = 1;		 //������ʱ��1
#endif
#if	UART2 == 1
	//������2:115200
	S2CON = 0x50;//8λ����,�ɱ䲨����
	AUXR |= 0x04;//��ʱ��2ʱ��ΪFosc,��1T
	T2L = 0xD8;	 //�趨��ʱ��ֵ
	T2H = 0xFF;	 //�趨��ʱ��ֵ
	AUXR |= 0x10;//������ʱ��2
#endif
#if	UART3 == 1
	//������3:9600
	S3CON = 0x10;	//8λ����,�ɱ䲨����
	S3CON |= 0x40;//8λ����,�ɱ䲨����
	T4T3M |= 0x02;//��ʱ��3Ϊ�����ʼ�����
	T3L = 0x20;	  //�趨��ʱ��ֵ
	T3H = 0xFE;		//�趨��ʱ��ֵ
	T4T3M |= 0x08;//������ʱ��3
#endif
#if	UART4 == 1
	//������4:9600
	S4CON = 0x10;	//8λ����,�ɱ䲨����
	S4CON |= 0x40;//8λ����,�ɱ䲨����
	T4T3M |= 0x20;//��ʱ��4Ϊ�����ʼ�����
	T4L = 0x20;	  //�趨��ʱ��ֵ
	T4H = 0xFE;		//�趨��ʱ��ֵ
	T4T3M |= 0x80;//������ʱ��4
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