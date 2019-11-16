#ifndef _TASK_MODBUS_H_
#define _TASK_MODBUS_H_
#include "main.h"

/**********************************************************
**				Task ID, PRIO, STACK_SIZE
**
****************  uC/OS II ϵͳ�궨��  ********************/
#define		TASK_TIME_ID							5
#define		TASK_MODBUS1_ID							6


#define		TASK_MODBUS1_PRIO				    	16
#define		TASK_TIME_PRIO					    	18

#define		TASK_TIME_STACK_SIZE					128
#define		TASK_MODBUS1_STACK_SIZE					400

extern OS_STK		TASK_TIME_STACK[TASK_TIME_STACK_SIZE];
extern OS_STK		TASK_MODBUS1_STACK[TASK_MODBUS1_STACK_SIZE];


/****************************************************************************
**	��������:	TASK_MODBUS1_SLAVE
**	�����ܣ�	RS485ͨѶ�ӿڣ�����MODBUS-RTUЭ�飬
		��Ϊ��վ����ַ0x24(36),03������
****************************************************************************/
void TASK_MODBUS1_SLAVE(void *pvData);

//****************************************************************************
//**	��������:	TASK_TIME
//**	�����ܣ�	TASK_TIME
//****************************************************************************
void TASK_TIME(void *pvData);

#endif

