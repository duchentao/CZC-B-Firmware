#ifndef _TASK_SETUP_H_
#define _TASK_SETUP_H_
#include "main.h"

/**********************************************************
**				Task ID, PRIO, STACK_SIZE
**
****************  uC/OS II ϵͳ�궨��  ********************/
#define				TASK_SETUP_ID							5
#define				TASK_SETUP_PRIO					    	13
#define				TASK_SETUP_STACK_SIZE					256
extern OS_STK		TASK_SETUP_STACK[TASK_SETUP_STACK_SIZE];


/****************************************************************************
**	��������:	TASK_MODBUS1_SLAVE
**	�����ܣ�	RS485ͨѶ�ӿڣ�����MODBUS-RTUЭ�飬
		��Ϊ��վ����ַ0x24(36),03������
****************************************************************************/
void TASK_SETUP(void *pvData);


#endif

