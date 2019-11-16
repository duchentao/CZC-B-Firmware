/*
 * @Descripttion: 
 * @version: 
 * @Author: 
 * @Date: 2019-10-12 21:13:11
 * @LastEditors: 
 * @LastEditTime: 2019-11-11 19:42:36
 */
#include "main.h"
#include "io.h"
#include "uart.h"
#include "modbus.h"
#include "task_modbus.h"
/**********************************************************
**				Task ID, PRIO, STACK_SIZE
**
****************  uC/OS II ϵͳ�궨��  ********************/
OS_STK		TASK_TIME_STACK[TASK_TIME_STACK_SIZE];
OS_STK		TASK_MODBUS1_STACK[TASK_MODBUS1_STACK_SIZE];

void modbus1_updata(uint8 func,uint8 *buf);
extern uint8 SLAVE_ADDR1 ;						// ��վ��ַ
extern uint8 tog_sw;
/****************************************************************************
**	��������:	TASK_MODBUS1_SLAVE
**	�����ܣ�	RS485ͨѶ�ӿڣ�����MODBUS-RTUЭ�飬
		��Ϊ��վ����ַ0x24(36),03������
****************************************************************************/
void TASK_MODBUS1_SLAVE(void *pvData)
{
	uint8  listen_ok = 0;
    uint8  fun = 0;         //���صĹ�����
    uint16 sub_addr = 0;    //���صļĴ����ӵ�ַ
    uint16 rw_num = 0;      //���ص���Ҫ��д��������16λ��
    uint8  err_code = 0;	//modbus���޴������
    uint8 i=0;

    memset(modbus1_buf, 0, MAX_BUFFER_SIZE1);
    
	while (1)
	{
		SET_UART2_IN;
		listen_ok = modbus2_listen(SLAVE_ADDR1, &fun, &sub_addr, &rw_num, modbus1_buf, 0, &err_code);
		if (1 == listen_ok)
        {
			modbus1_updata(fun, modbus1_buf);
            SET_UART2_OUT;
            modbus2_reply(SLAVE_ADDR1, fun, sub_addr, rw_num, modbus1_buf);
            
        }
        else
        { 
        	if (err_code != MODBUS_NO_ERR)
        	{
        		SET_UART2_OUT;        		
        		modbus2_err_reply(SLAVE_ADDR1, fun, err_code);
 
        	}
        }
	}		
}
//****************************************************************************
//**	��������:	TASK_TIME
//**	�����ܣ�	TASK_TIME
//****************************************************************************
void TASK_TIME(void *pvData)
{
	TIME_PROCESS();
}

