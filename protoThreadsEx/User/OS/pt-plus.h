/* 
 * ��ԭ��pt�ϵĹ��ܸĽ�
 *
*/
#ifndef __PT_PLUS_H__
#define __PT_PLUS_H__

#include "pt.h"

#ifndef NULL
#define NULL 0
#endif

#define TRUE	1
#define FALSE	0

#define	RUN		TRUE
#define	STOP	FALSE

extern ClassTask idata OSTasks[TASK_MAX];

void PT_TaskInit(unsigned char id, int (*taskname)(unsigned char ));
//----------------------------------------------------------------------
//����ʼ���������
#define PT_CBEGIN(id) { char PT_YIELD_FLAG = 1;	\
												LC_RESUME(OSTasks[id].lc)
//----------------------------------------------------------------------
//�������ǰ�������
#define PT_CEND(id) LC_END(OSTasks[id].lc); PT_YIELD_FLAG = 0; \
                   OSTasks[id].lc=NULL; return PT_ENDED; }
//----------------------------------------------------------------------
//ֹͣid�����������
#define PT_STOP(id)														\
	do{																					\
		OSTasks[id].flag = STOP;									\
		LC_SET(OSTasks[id].lc);										\
		if(OSTasks[id].flag	==	STOP)							\
			return PT_WAITING;											\
	}while(0)
	//----------------------------------------------------------------------
//����id�����������
#define PT_START(id)	OSTasks[id].flag = RUN;OSTasks[id].tick =	0
//----------------------------------------------------------------------
//�����������,��ticks��ʱ�ӽ����ڵȴ���Ϣ
#define PT_PEND(ticks)   						\
  do {															\
		PT_START(idx);									\
		OSTasks[idx].tick = (ticks);		\
		LC_SET(OSTasks[idx].lc);				\
		if(PT_GET(idx)!=0){							\
			OSTasks[idx].tick =	0;				\
		}else if(OSTasks[idx].tick) {		\
			return	PT_WAITING;						\
		}																\
  }while(0)
//----------------------------------------------------------------------
//��id����������Ϣ
#define PT_POST(id, msg)					\
	do{															\
		PT_START(id);									\
		OSTasks[id].value = (msg);		\
	}while(0)
//----------------------------------------------------------------------
//��ȡ���͸����������Ϣ
#define PT_GET(id)	(OSTasks[id].value)
//----------------------------------------------------------------------
//��ձ�������Ϣ��ʾ�������
#define PT_CLR(id)	(OSTasks[id].value	=	0)
//----------------------------------------------------------------------
//������ʱ�����������������ticks��ʱ�ӽ���
#define PT_DELAY(ticks)	      								\
  do {																				\
		PT_START(idx);														\
		OSTasks[idx].tick = (ticks);							\
    LC_SET(OSTasks[idx].lc);									\
    if(OSTasks[idx].tick) {										\
      return PT_WAITING;											\
    }																					\
  }while(0)
//----------------------------------------------------------------------
//�жϺ���ʹ��
#define ISR_START(id)				OSTasks[id].flag = RUN
#define ISR_STOP(id)				OSTasks[id].flag = STOP
#define ISR_PEND(id, ticks)	PT_START(id);OSTasks[id].tick = (ticks)
#define ISR_POST(id, msg)		PT_POST(id, msg)

#endif