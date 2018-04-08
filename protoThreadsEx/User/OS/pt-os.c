#include "all.h"

ClassTask idata OSTasks[TASK_MAX];
static unsigned char index;

void PT_TaskInit(	unsigned char id,
									int (*taskname)(unsigned char ))
{
	OSTasks[id].task	= taskname;
	OSTasks[id].lc		= NULL;
	OSTasks[id].tick	= 0;
	OSTasks[id].flag	= RUN;
	OSTasks[id].id		= id;
	OSTasks[id].value	= NULL;
}

void osTimerInit(void)	//50毫秒@11.0592MHz
{
	AUXR &= 0x7F;	//定时器时钟12T模式
	TMOD &= 0xF0;	//设置定时器模式
	TL0 = 0x00;		//设置定时初值
	TH0 = 0x4C;		//设置定时初值
	TF0 = 0;			//清除TF0标志
	TR0 = 0;			//定时器0关闭计时
	ET0	=	1;			//定时器0中断开启
}
void osTimerTicks(void) interrupt 1
{
#if OS_TICKS_RELOAD == 1
	TL0 = 0x00;
	TH0 = 0x4C;
#endif
	unsigned char i;
	for(i=0; i<TASK_MAX; i++)
	{
		if(OSTasks[i].tick)
		{
			(OSTasks[i].tick)--;
		}
	}
}
void main(void)
{
	appUserInit();		//用户初始化
	osTimerInit();		//初始化时钟节拍
	TR0 = 1;			//启动时钟节拍定时器
	EA = 1;				//启动总中断
	while (1)
  {
		for(index=0; index<TASK_MAX; index++)
		{//轮询任务链表
			if((OSTasks[index].flag == RUN) && (OSTasks[index].tick == 0))
			{//如果任务处于运行态
				(OSTasks[index].task)(index);
			}
		}
  }
}