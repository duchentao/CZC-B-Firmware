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
****************  uC/OS II 系统宏定义  ********************/
OS_STK		TASK_TIME_STACK[TASK_TIME_STACK_SIZE];
OS_STK		TASK_MODBUS1_STACK[TASK_MODBUS1_STACK_SIZE];

void modbus1_updata(uint8 func,uint8 *buf);
extern uint8 SLAVE_ADDR1 ;						// 从站地址
extern uint8 tog_sw;
/****************************************************************************
**	任务名称:	TASK_MODBUS1_SLAVE
**	任务功能：	RS485通讯接口，采用MODBUS-RTU协议，
		作为从站，地址0x24(36),03功能码
****************************************************************************/
void TASK_MODBUS1_SLAVE(void *pvData)
{
	uint8  listen_ok = 0;
    uint8  fun = 0;         //返回的功能码
    uint16 sub_addr = 0;    //返回的寄存器子地址
    uint16 rw_num = 0;      //返回的需要读写的数量（16位）
    uint8  err_code = 0;	//modbus超限错误代码
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
//**	任务名称:	TASK_TIME
//**	任务功能：	TASK_TIME
//****************************************************************************
void TASK_TIME(void *pvData)
{
	TIME_PROCESS();
}

