/* 
 * 在原有pt上的功能改进
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
//任务开始处必须调用
#define PT_CBEGIN(id) { char PT_YIELD_FLAG = 1;	\
												LC_RESUME(OSTasks[id].lc)
//----------------------------------------------------------------------
//任务结束前必须调用
#define PT_CEND(id) LC_END(OSTasks[id].lc); PT_YIELD_FLAG = 0; \
                   OSTasks[id].lc=NULL; return PT_ENDED; }
//----------------------------------------------------------------------
//停止id号任务的运行
#define PT_STOP(id)														\
	do{																					\
		OSTasks[id].flag = STOP;									\
		LC_SET(OSTasks[id].lc);										\
		if(OSTasks[id].flag	==	STOP)							\
			return PT_WAITING;											\
	}while(0)
	//----------------------------------------------------------------------
//运行id号任务的运行
#define PT_START(id)	OSTasks[id].flag = RUN;OSTasks[id].tick =	0
//----------------------------------------------------------------------
//将本任务挂起,在ticks个时钟节拍内等待消息
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
//给id号任务发送消息
#define PT_POST(id, msg)					\
	do{															\
		PT_START(id);									\
		OSTasks[id].value = (msg);		\
	}while(0)
//----------------------------------------------------------------------
//获取发送给本任务的消息
#define PT_GET(id)	(OSTasks[id].value)
//----------------------------------------------------------------------
//清空本任务消息表示处理完毕
#define PT_CLR(id)	(OSTasks[id].value	=	0)
//----------------------------------------------------------------------
//任务延时函数，将此任务挂起ticks个时钟节拍
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
//中断函数使用
#define ISR_START(id)				OSTasks[id].flag = RUN
#define ISR_STOP(id)				OSTasks[id].flag = STOP
#define ISR_PEND(id, ticks)	PT_START(id);OSTasks[id].tick = (ticks)
#define ISR_POST(id, msg)		PT_POST(id, msg)

#endif