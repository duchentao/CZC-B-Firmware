/**********************************************************
**	本文件为开放给用户的调试接口，采用modbus通信协议，
**	可存储配置，配置存储在外置eeprom内
****************  uC/OS II 系统宏定义  ********************/
#include "main.h"
#include "io.h"
#include "uart.h"
#include "modbus.h"
#include "task_setup.h"

/**********************************************************
**	
**
****************  uC/OS II 系统宏定义  ********************/
OS_STK		TASK_SETUP_STACK[TASK_SETUP_STACK_SIZE];


/****************************************************************************
**	任务名称:
**	任务功能：	RS232通讯接口，采用MODBUS-RTU协议，
				作为从站，地址0x24(36),03功能码
****************************************************************************/
void TASK_SETUP(void *pvData)
{
	uint8  listen_ok = 0;
    uint8  fun = 0;         //返回的功能码
    uint16 sub_addr = 0;    //返回的寄存器子地址
    uint16 rw_num = 0;      //返回的需要读写的数量（16位）
    uint8  err_code = 0;	//modbus超限错误代码
    
    memset(modbus0_buf, 0, MAX_BUFFER_SIZE0);
    
	while (1)
	{
		listen_ok = modbus0_listen(SLAVE_ADDR0, &fun, &sub_addr, &rw_num, modbus0_buf, 0, &err_code);
        if (1 == listen_ok)
        {
            modbus0_reply(SLAVE_ADDR0, fun, sub_addr, rw_num, modbus1_buf);
        }
        else
        { 
        	if (err_code != MODBUS_NO_ERR)
        	{
        		modbus0_err_reply(SLAVE_ADDR0, fun, err_code);
        	}
        }
	}		
}

