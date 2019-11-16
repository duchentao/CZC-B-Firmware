/**********************************************************
**	���ļ�Ϊ���Ÿ��û��ĵ��Խӿڣ�����modbusͨ��Э�飬
**	�ɴ洢���ã����ô洢������eeprom��
****************  uC/OS II ϵͳ�궨��  ********************/
#include "main.h"
#include "io.h"
#include "uart.h"
#include "modbus.h"
#include "task_setup.h"

/**********************************************************
**	
**
****************  uC/OS II ϵͳ�궨��  ********************/
OS_STK		TASK_SETUP_STACK[TASK_SETUP_STACK_SIZE];


/****************************************************************************
**	��������:
**	�����ܣ�	RS232ͨѶ�ӿڣ�����MODBUS-RTUЭ�飬
				��Ϊ��վ����ַ0x24(36),03������
****************************************************************************/
void TASK_SETUP(void *pvData)
{
	uint8  listen_ok = 0;
    uint8  fun = 0;         //���صĹ�����
    uint16 sub_addr = 0;    //���صļĴ����ӵ�ַ
    uint16 rw_num = 0;      //���ص���Ҫ��д��������16λ��
    uint8  err_code = 0;	//modbus���޴������
    
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

