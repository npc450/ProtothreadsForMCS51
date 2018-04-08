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

void osTimerInit(void)	//50����@11.0592MHz
{
	AUXR &= 0x7F;	//��ʱ��ʱ��12Tģʽ
	TMOD &= 0xF0;	//���ö�ʱ��ģʽ
	TL0 = 0x00;		//���ö�ʱ��ֵ
	TH0 = 0x4C;		//���ö�ʱ��ֵ
	TF0 = 0;			//���TF0��־
	TR0 = 0;			//��ʱ��0�رռ�ʱ
	ET0	=	1;			//��ʱ��0�жϿ���
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
	appUserInit();		//�û���ʼ��
	osTimerInit();		//��ʼ��ʱ�ӽ���
	TR0 = 1;			//����ʱ�ӽ��Ķ�ʱ��
	EA = 1;				//�������ж�
	while (1)
  {
		for(index=0; index<TASK_MAX; index++)
		{//��ѯ��������
			if((OSTasks[index].flag == RUN) && (OSTasks[index].tick == 0))
			{//�������������̬
				(OSTasks[index].task)(index);
			}
		}
  }
}