#ifndef _TASK_MODBUS_H_
#define _TASK_MODBUS_H_
#include "main.h"

/**********************************************************
**				Task ID, PRIO, STACK_SIZE
**
****************  uC/OS II 系统宏定义  ********************/
#define		TASK_TIME_ID							5
#define		TASK_MODBUS1_ID							6


#define		TASK_MODBUS1_PRIO				    	16
#define		TASK_TIME_PRIO					    	18

#define		TASK_TIME_STACK_SIZE					128
#define		TASK_MODBUS1_STACK_SIZE					400

extern OS_STK		TASK_TIME_STACK[TASK_TIME_STACK_SIZE];
extern OS_STK		TASK_MODBUS1_STACK[TASK_MODBUS1_STACK_SIZE];


/****************************************************************************
**	任务名称:	TASK_MODBUS1_SLAVE
**	任务功能：	RS485通讯接口，采用MODBUS-RTU协议，
		作为从站，地址0x24(36),03功能码
****************************************************************************/
void TASK_MODBUS1_SLAVE(void *pvData);

//****************************************************************************
//**	任务名称:	TASK_TIME
//**	任务功能：	TASK_TIME
//****************************************************************************
void TASK_TIME(void *pvData);

#endif

