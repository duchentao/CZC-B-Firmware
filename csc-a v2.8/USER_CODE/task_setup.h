#ifndef _TASK_SETUP_H_
#define _TASK_SETUP_H_
#include "main.h"

/**********************************************************
**				Task ID, PRIO, STACK_SIZE
**
****************  uC/OS II 系统宏定义  ********************/
#define				TASK_SETUP_ID							5
#define				TASK_SETUP_PRIO					    	13
#define				TASK_SETUP_STACK_SIZE					256
extern OS_STK		TASK_SETUP_STACK[TASK_SETUP_STACK_SIZE];


/****************************************************************************
**	任务名称:	TASK_MODBUS1_SLAVE
**	任务功能：	RS485通讯接口，采用MODBUS-RTU协议，
		作为从站，地址0x24(36),03功能码
****************************************************************************/
void TASK_SETUP(void *pvData);


#endif

