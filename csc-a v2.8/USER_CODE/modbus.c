/****************************************************************************************************************/
//                    ��Ϊ��վ��ֻ��2�麯����
//                    0x03:    modbus_ask03    ��   modbus_read03
//                    0x05:    modbus_set05    ��   modbus_read05
//                    0x06:    modbus_set06    ��   modbus_read06
//                    0x10:    modbus_set16    ��   modbus_read16
//
//                    ��Ϊ��վ��ֻ��2������,ֻ�ʺ�0x03,0x05,0x06,0x10��
//                    modbus_listen   ��   modbus_reply   ��   modbus_err_reply
/****************************************************************************************************************/
#include "main.h"
#include "modbus.h"
#include "uart.h"
#include "IO.h"

const uint8  auchCRCHi[] = { 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 
0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 
0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 
}; 

const uint8  auchCRCLo[] = { 
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 
0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 
0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 
0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC, 0x14, 0xD4, 
0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 
0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 
0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 
0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF, 0x2D, 0xED, 
0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 
0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 
0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 
0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 
0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 
0x70, 0xB0, 0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 
0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 
0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89, 0x4B, 0x8B, 
0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 
0x43, 0x83, 0x41, 0x81, 0x80, 0x40 
};

//modbus�����ݴ洢��
uint8 modbus0_buf[MAX_BUFFER_SIZE0];		//2�ֽ�Ϊһ�Ĵ���ֵ����λ��ǰ������modbus_buffer[0]��ʾ�Ĵ���1�ĸ�λ��modbus_buffer[1]��ʾ�Ĵ���1�ĵ�λ
uint8 modbus1_buf[MAX_BUFFER_SIZE1];		//2�ֽ�Ϊһ�Ĵ���ֵ����λ��ǰ������modbus_buffer[0]��ʾ�Ĵ���1�ĸ�λ��modbus_buffer[1]��ʾ�Ĵ���1�ĵ�λ
uint8 modbus2_buf[MAX_BUFFER_SIZE2];		//2�ֽ�Ϊһ�Ĵ���ֵ����λ��ǰ������modbus_buffer[0]��ʾ�Ĵ���1�ĸ�λ��modbus_buffer[1]��ʾ�Ĵ���1�ĵ�λ
uint8 modbus3_buf[MAX_BUFFER_SIZE3];		//2�ֽ�Ϊһ�Ĵ���ֵ����λ��ǰ������modbus_buffer[0]��ʾ�Ĵ���1�ĸ�λ��modbus_buffer[1]��ʾ�Ĵ���1�ĵ�λ

extern OS_EVENT  *RecvUARTFlag0;  
extern OS_EVENT  *RecvUARTFlag1;  
extern OS_EVENT  *RecvUARTFlag2;  
extern OS_EVENT  *RecvUARTFlag3; 

extern uint16 uart0_receive_num;				    // ��Ч�Ľ��յ������ݸ������ѷ�����ʱ�жϣ�����Ҫ�����ź���
extern uint16 uart1_receive_num;				    // ��Ч�Ľ��յ������ݸ������ѷ�����ʱ�жϣ�����Ҫ�����ź���
extern uint16 uart2_receive_num;				    // ��Ч�Ľ��յ������ݸ������ѷ�����ʱ�жϣ�����Ҫ�����ź���
extern uint16 uart3_receive_num;				    // ��Ч�Ľ��յ������ݸ������ѷ�����ʱ�жϣ�����Ҫ�����ź���


uint16 crc16(uint8 *puchMsg, uint16 usDataLen) 
{ 
	uint8  uchCRCHi = 0xFF ; /* ��CRC�ֽڳ�ʼ�� */ 
	uint8  uchCRCLo = 0xFF ; /* ��CRC �ֽڳ�ʼ�� */ 
	uint32 uIndex ;          /* CRCѭ���е����� */ 
	while (usDataLen--)      /* ������Ϣ������ */ 
	{ 
		uIndex = uchCRCHi ^ *puchMsg++ ; /* ����CRC */ 
		uchCRCHi = uchCRCLo ^ auchCRCHi[uIndex] ; 
		uchCRCLo = auchCRCLo[uIndex] ; 
	} 
	return (((uint16)uchCRCHi << 8) | uchCRCLo) ; 
}

uint8 crc_detect(uint8 *crc_buf, uint16 len)    //����1�ɹ���0ʧ�ܡ���CRCУ��ΪMODBUS�ӻ��������ݣ�len���ֽڣ�ǰlen-2�������ݣ���2��CRCУ��
{
    uint8  *p_buf = crc_buf;
    uint8  hi, lo;
    uint16 crc;

    crc = crc16(p_buf, len - 2);	
    hi  = (uint8)(crc >> 8);	
    lo  = (uint8)(crc & 0xff);  	
    
    if ((hi == p_buf[len - 2]) && (lo == p_buf[len - 1]))
    {
        return 1;    
    }
    else
    {
        return 0;    
    }
}


//******************************************************************************************************
//*                                              MOBUS0                                                *
//*                                           ====��վ===                                              *
//******************************************************************************************************
//��վ������վ������1��ʾ�ɹ�������һ����Ч֡��0��ʾ���ݴ���û�и���,�ú��������Ѱ���OSDELAY
//slave_addr: ��վ��ַ
//func_code�� ���صĹ�����
//sub_addr��  ���صĴ�վ�ļĴ����ӵ�ַ(��ʼ��ַ)
//rw_num:     ���صĶ�д�Ĵ����ĸ���
//buf:        �������վд�����ɹ����ر�ʾ����һ��д��������
//out_time��  ��Ҫ�ȴ����ʱ��
//modbus_err��   modbus���޴������
uint8 modbus0_listen(uint8 slave_addr, uint8 *func_code, uint16 *sub_addr, uint16 *rw_num, uint8 *buf, uint16 out_time, uint8 *modbus_err)
{
    uint8  err = 0;
    uint8  i = 0;
    
    OSSemPend(RecvUARTFlag0, out_time, &err);
	if (err == OS_NO_ERR)
	{
		if (8 == uart0_receive_num)			// 8�ֽ��ж�
		{
			if ((modbus0_temp[0] == slave_addr) && crc_detect(modbus0_temp, uart0_receive_num))
			{
	    		*func_code = modbus0_temp[1];	// ������
	                
			    switch (modbus0_temp[1])
		        {
		            // Request
		            // Function code		[1 byte]: 0x03
		            // Starting address		[2 byte]: 0x0000 to 0xFFFF
		            // Quantity of registers[2 byte]: 0x0001 to 0x007D(125)
		            case 0x03:  // read holding registers
		                *sub_addr = ((uint16)modbus0_temp[2] << 8) + modbus0_temp[3];	// Starting address
	                    *rw_num   = ((uint16)modbus0_temp[4] << 8) + modbus0_temp[5];	// Quantity of registers
	
	                    if (((*sub_addr + *rw_num) * 2) > MAX_BUFFER_SIZE0)
	                    {    
	                        *modbus_err = EXCEPTION_CODE_2;
	                        
	                        return 0;
	                    }
	                    
	                    if ((0 == (*rw_num)) || ((*rw_num) > 0x007D))
	                    {
	                    	*modbus_err = EXCEPTION_CODE_3;
	                    	
	                    	return 0;
	                    }
	                    
	                    *modbus_err = MODBUS_NO_ERR;
	                    
	                    return 1;
		            
		            // Request
		            // Function code	[1 byte]: 0x05
		            // Output address	[2 byte]: 0x0000 to 0xFFFF
		            // Output value		[2 byte]: 0x0000 or 0xFF00
		            case 0x05:  // write single coil--OFF or ON
		                *sub_addr = modbus0_temp[3] + ((uint16)modbus0_temp[2] << 8);
		                
	                    if (*sub_addr * 2  > MAX_BUFFER_SIZE0) 
	                    {    
	                        *modbus_err = EXCEPTION_CODE_2;
	                        
	                        return 0;
	                    }
	                    
	                    if ((0xFF == modbus0_temp[4]) && (0x00 == modbus0_temp[5]))
	                    {
	                    	buf[0] = 0x01;
	                    }
	                    else if ((0x00 == modbus0_temp[4]) && (0x00 == modbus0_temp[5]))
	                    {
	                    	buf[0] = 0x00;
	                    }
	                    else
	                    {
	                    	*modbus_err = EXCEPTION_CODE_3;
	                    	
	                    	return 0;
	                    }
	                    
	                    *modbus_err = MODBUS_NO_ERR;
	                    
	                    return 1;
		            
		            // Request
		            // Function code	[1 byte]: 0x06
		            // Output address	[2 byte]: 0x0000 to 0xFFFF
		            // Output value		[2 byte]: 0x0000 to 0xFFFF
		            case 0x06:  // write single register
		                *sub_addr = modbus0_temp[3] + ((uint16)modbus0_temp[2] << 8);
		                
	                    if ((*sub_addr + 1) * 2  > MAX_BUFFER_SIZE0)
	                    {    
	                        *modbus_err = EXCEPTION_CODE_2;
	                        
	                        return 0;
	                    }
	                    
	                    buf[*sub_addr * 2]     = modbus0_temp[4];
	                    buf[*sub_addr * 2 + 1] = modbus0_temp[5];
						
						*modbus_err = MODBUS_NO_ERR;
						
	                    return 1;
		            
		            default:
		            	*modbus_err = EXCEPTION_CODE_1;
		            	
		                return 0;
		        }
	        }
	        else
	        {
	            return 0;		// ��ַ��CRCУ�����
	        }
		}
		else if(uart0_receive_num > 8)
		{
			if ((modbus0_temp[0] == slave_addr) && crc_detect(modbus0_temp, uart0_receive_num))
			{
	    		*func_code = modbus0_temp[1];	// ������
	                
			    switch (modbus0_temp[1])
		        {
		            // Request
		            // Function code		[1 byte]: 0x10
		            // Starting address		[2 byte]: 0x0000 to 0xFFFF
		            // Quantity of registers[2 byte]: 0x0001 to 0x007B(123)
		            // Byte count			[1 byte]: 2 x N
		            // Registers value	[N x 2 byte]: value
		            case 0x10:  // write multiple registers
		                *sub_addr = ((uint16)modbus0_temp[2] << 8) + modbus0_temp[3];	// Starting address
	                    *rw_num   = ((uint16)modbus0_temp[4] << 8) + modbus0_temp[5];	// Quantity of registers
	                    
	                    if (((*sub_addr + *rw_num) * 2) > MAX_BUFFER_SIZE0)
	                    {
	                        *modbus_err = EXCEPTION_CODE_2;
	                        
	                        return 0;
	                    }
	                    
	                    if ((0 == (*rw_num)) || ((*rw_num) > 0x007B) || (((*rw_num) * 2) != modbus0_temp[6]))
	                    {
	                    	*modbus_err = EXCEPTION_CODE_3;
	                    	
	                    	return 0;
	                    }
	                    
	                    if ((uart0_receive_num - 9) != (modbus0_temp[6]))
	                    {
	                    	*modbus_err = EXCEPTION_CODE_4;
	                    	
	                    	return 0;
	                    }
	                    
	                    for (i = 0; i < *rw_num; i++)
	                    {
	                    	buf[(*sub_addr + i) * 2]     = modbus0_temp[7 + (i * 2)];
	                    	buf[(*sub_addr + i) * 2 + 1] = modbus0_temp[8 + (i * 2)];
	                    }
	                    
	                    *modbus_err = MODBUS_NO_ERR;
	                    
	                    return 1;
		            
		            default: 
		                *modbus_err = EXCEPTION_CODE_1;
		                
		                return 0;
		        }
	        }
	        else
	        {
	            return 0;		// ��ַ��CRCУ�����
	        }
		}
		else
		{
			return 0;			// ���յ����� < 8���ֽ�
		}
    }
    else
    {
        return 0;			// UART���մ���
    }
}


//��վ�ظ���վ
//slave_addr: ��վ��ַ
//func_code:  ������
//sub_addr:   �ظ��ļĴ�����ַ
//rw_num:     �ظ��ļĴ����ĸ���
//buf:        ��Ҫ�ظ���վ�ļĴ���ֵ���ݿ��ַ
void modbus0_reply(uint8 slave_addr, uint8 func_code, uint16 sub_addr, uint16 rw_num, uint8 *buf)
{
    uint8  crc_temp_buf0[MAX_BUFFER_SIZE0 + 3] = {0};
    uint16 crcData = 0;
    uint8  crc_hi = 0, crc_lo = 0;
    uint16 i = 0;
    
    //slow_out(RS485DIR0, OUT);
    
    switch (func_code)
    {
        case 0x03:
            modbus0_send_byte(slave_addr);  // �ӵ�ַ
            modbus0_send_byte(0x03);        // ������
            modbus0_send_byte(rw_num * 2);  // �ֽ���

            crc_temp_buf0[0] = slave_addr;
            crc_temp_buf0[1] = func_code;
            crc_temp_buf0[2] = rw_num * 2;
                    
            for (i = 0; i < rw_num * 2; i++)   //�Ĵ�������
            {
                if (i + sub_addr * 2 > MAX_BUFFER_SIZE0)
                {
                    break;    
                }
                
                modbus0_send_byte(buf[i + sub_addr * 2]);
                crc_temp_buf0[3 + i] = buf[i + sub_addr * 2];
            }

            crcData = crc16(crc_temp_buf0, rw_num * 2 + 3);	
        	crc_hi = crcData >> 8;
        	crc_lo = crcData & 0xff;
        	
        	modbus0_send_byte(crc_hi);
        	modbus0_send_byte(crc_lo);
        	
            break;
            
        case 0x05:
            crc_temp_buf0[0] = slave_addr;
            crc_temp_buf0[1] = 0x05;
            crc_temp_buf0[2] = (uint8)(sub_addr >> 8);
            crc_temp_buf0[3] = (uint8)sub_addr;
            
            if (1 == buf[0])
            {
            	crc_temp_buf0[4] = 0xFF;
            	crc_temp_buf0[5] = 0x00;
            }
            else
            {
            	crc_temp_buf0[4] = 0x00;
            	crc_temp_buf0[5] = 0x00;
            }
            crcData = crc16(crc_temp_buf0, 6);
        	crc_hi = crcData >> 8;
        	crc_lo = crcData & 0xff;

            for (i = 0; i < 6; i++)         //�Ĵ�������
            {
                modbus0_send_byte(crc_temp_buf0[i]);
            }

        	modbus0_send_byte(crc_hi);
        	modbus0_send_byte(crc_lo);
        	
            break;
        
        case 0x06:
            crc_temp_buf0[0] = slave_addr;
            crc_temp_buf0[1] = 0x06;
            crc_temp_buf0[2] = (uint8)(sub_addr >> 8);
            crc_temp_buf0[3] = (uint8)sub_addr;
            crc_temp_buf0[4] = buf[sub_addr * 2];
            crc_temp_buf0[5] = buf[sub_addr * 2 + 1];

            crcData = crc16(crc_temp_buf0, 6);
        	crc_hi = crcData >> 8;
        	crc_lo = crcData & 0xff;

            for (i = 0; i < 6; i++)         //�Ĵ�������
            {
                modbus0_send_byte(crc_temp_buf0[i]);
            }

        	modbus0_send_byte(crc_hi);
        	modbus0_send_byte(crc_lo);
        	
            break;
            
        case 0x10:
            crc_temp_buf0[0] = slave_addr;
            crc_temp_buf0[1] = 0x10;
            crc_temp_buf0[2] = (uint8)(sub_addr >> 8);
            crc_temp_buf0[3] = (uint8)sub_addr;
            crc_temp_buf0[4] = (uint8)(rw_num >> 8);
            crc_temp_buf0[5] = (uint8)rw_num;

            crcData = crc16(crc_temp_buf0, 6);
        	crc_hi = crcData >> 8;
        	crc_lo = crcData & 0xff;

            for (i = 0; i < 6; i++)         //�Ĵ�������
            {
                modbus0_send_byte(crc_temp_buf0[i]);
            }

        	modbus0_send_byte(crc_hi);
        	modbus0_send_byte(crc_lo);
        	
            break;
        
        default:
            break;    
    }
}


//��վ���޴������ظ�
//slave_addr: ��վ��ַ
//func_code:  ������
//modbus_err: �޴������
void modbus0_err_reply(uint8 slave_addr, uint8 func_code, uint8 modbus_err)
{
	uint8  err_crc_temp_buf0[3] = {0};
	uint16 crcData = 0;
	uint8  crc_hi = 0, crc_lo = 0;
    uint16 i = 0;
	
	err_crc_temp_buf0[0] = slave_addr;
	err_crc_temp_buf0[1] = func_code + 0x80;
	err_crc_temp_buf0[2] = modbus_err;
	
	crcData = crc16(err_crc_temp_buf0, 3);
    crc_hi = crcData >> 8;
    crc_lo = crcData & 0xff;
	
	for (i = 0; i < 3; i++)         //�Ĵ�������
    {
        modbus0_send_byte(err_crc_temp_buf0[i]);
    }

	modbus0_send_byte(crc_hi);
	modbus0_send_byte(crc_lo);
}


//******************************************************************************************************
//*                                              MOBUS1                                                *
//*                                           ====��վ===                                              *
//******************************************************************************************************
//��վ������վ������1��ʾ�ɹ�������һ����Ч֡��0��ʾ���ݴ���û�и���,�ú��������Ѱ���OSDELAY
//slave_addr: ��վ��ַ
//func_code:  ���صĹ�����
//sub_addr:   ���صĴ�վ�ļĴ����ӵ�ַ
//rw_num:     ���صĶ�д�Ĵ����ĸ���
//buf:        �������վд�����ɹ����ر�ʾ����һ��д��������
//out_time:   ��Ҫ�ȴ����ʱ��
//mod_err:    modbus���޴������
uint8 modbus1_listen(uint8 slave_addr, uint8 *func_code, uint16 *sub_addr, uint16 *rw_num, uint8 *buf, uint16 out_time, uint8 *modbus_err)
{
    uint8  err = 0;
    uint8  i = 0;
        
    OSSemPend(RecvUARTFlag1, out_time, &err);
	if (err == OS_NO_ERR)
	{
		if (8 == uart1_receive_num)					// 8�ֽ��ж�
		{
			if ((modbus1_temp[0] == slave_addr) && crc_detect(modbus1_temp, uart1_receive_num))
			{
	    		*func_code = modbus1_temp[1];		// ������
	                
			    switch (modbus1_temp[1])
		        {
		            // Request
		            // Function code		[1 byte]: 0x03
		            // Starting address		[2 byte]: 0x0000 to 0xFFFF
		            // Quantity of registers[2 byte]: 0x0001 to 0x007D(125)
		            case 0x03:  // read holding registers
		                *sub_addr = ((uint16)modbus1_temp[2] << 8) + modbus1_temp[3];	// Starting address
	                    *rw_num   = ((uint16)modbus1_temp[4] << 8) + modbus1_temp[5];	// Quantity of registers
	
	                    if (((*sub_addr + *rw_num) * 2) > MAX_BUFFER_SIZE1)
	                    {    
	                        *modbus_err = EXCEPTION_CODE_2;
	                        
	                        return 0;
	                    }
	                    
	                    if ((0 == (*rw_num)) || ((*rw_num) > 0x007D))
	                    {
	                    	*modbus_err = EXCEPTION_CODE_3;
	                    	
	                    	return 0;
	                    }
	                    
	                    *modbus_err = MODBUS_NO_ERR;
	                    
	                    return 1;
		            
		            // Request
		            // Function code	[1 byte]: 0x05
		            // Output address	[2 byte]: 0x0000 to 0xFFFF
		            // Output value		[2 byte]: 0x0000 or 0xFF00
		            case 0x05:  // write single coil--OFF or ON
		                *sub_addr = modbus1_temp[3] + ((uint16)modbus1_temp[2] << 8);
		                
	                    if (*sub_addr * 2  > MAX_BUFFER_SIZE1) 
	                    {
	                        *modbus_err = EXCEPTION_CODE_2;
	                        
	                        return 0;
	                    }
	                    
	                    if ((0xFF == modbus1_temp[4]) && (0x00 == modbus1_temp[5]))
	                    {
	                    	buf[0] = 0x01;
	                    }
	                    else if ((0x00 == modbus1_temp[4]) && (0x00 == modbus1_temp[5]))
	                    {
	                    	buf[0] = 0x00;
	                    }
	                    else
	                    {
	                    	*modbus_err = EXCEPTION_CODE_3;
	                    	
	                    	return 0;
	                    }
	                    
	                    *modbus_err = MODBUS_NO_ERR;
	                    
	                    return 1;
		            
		            // Request
		            // Function code	[1 byte]: 0x06
		            // Output address	[2 byte]: 0x0000 to 0xFFFF
		            // Output value		[2 byte]: 0x0000 to 0xFFFF
		            case 0x06:  // write single register
		                *sub_addr = modbus1_temp[3] + ((uint16)modbus1_temp[2] << 8);
		                
	                    if ((*sub_addr + 1) * 2  > MAX_BUFFER_SIZE1)
	                    {    
	                        *modbus_err = EXCEPTION_CODE_2;
	                        
	                        return 0;
	                    }
	                    
	                    buf[*sub_addr * 2]     = modbus1_temp[4];
	                    buf[*sub_addr * 2 + 1] = modbus1_temp[5];
						
						*modbus_err = MODBUS_NO_ERR;
						
	                    return 1;
		            
		            default:
		            	*modbus_err = EXCEPTION_CODE_1;
		            	
		                return 0;
		        }
	        }
	        else
	        {
	            return 0;		// ��ַ��CRCУ�����
	        }
		}
		else if(uart1_receive_num > 8)
		{
			if ((modbus1_temp[0] == slave_addr) && crc_detect(modbus1_temp, uart1_receive_num))
			{
	    		*func_code = modbus1_temp[1];	// ������
	                
			    switch (modbus1_temp[1])
		        {
		            // Request
		            // Function code		[1 byte]: 0x10
		            // Starting address		[2 byte]: 0x0000 to 0xFFFF
		            // Quantity of registers[2 byte]: 0x0001 to 0x007B(123)
		            // Byte count			[1 byte]: 2 x N
		            // Registers value	[N x 2 byte]: value
		            case 0x10:  // write multiple registers
		                *sub_addr = ((uint16)modbus1_temp[2] << 8) + modbus1_temp[3];	// Starting address
	                    *rw_num   = ((uint16)modbus1_temp[4] << 8) + modbus1_temp[5];	// Quantity of registers
	                    
	                    if (((*sub_addr + *rw_num) * 2) > MAX_BUFFER_SIZE1)
	                    {    
	                        *modbus_err = EXCEPTION_CODE_2;
	                        
	                        return 0;
	                    }
	                    
	                    if ((0 == (*rw_num)) || ((*rw_num) > 0x007B) || (((*rw_num) * 2) != modbus1_temp[6]))
	                    {
	                    	*modbus_err = EXCEPTION_CODE_3;
	                    	
	                    	return 0;
	                    }
	                    
	                    if ((uart1_receive_num - 9) != (modbus1_temp[6]))
	                    {
	                    	*modbus_err = EXCEPTION_CODE_4;
	                    	
	                    	return 0;
	                    }
	                    
	                    for (i = 0; i < *rw_num; i++)
	                    {
	                    	buf[(*sub_addr + i) * 2]     = modbus1_temp[7 + (i * 2)];
	                    	buf[(*sub_addr + i) * 2 + 1] = modbus1_temp[8 + (i * 2)];
	                    }
	                    
	                    *modbus_err = MODBUS_NO_ERR;
	                    
	                    return 1;
		            
		            default: 
		                *modbus_err = EXCEPTION_CODE_1;
		                
		                return 0;
		        }
	        }
	        else
	        {
	            return 0;		// ��ַ��CRCУ�����
	        }
		}
		else
		{
			return 0;			// ���յ����� < 8���ֽ�
		}
    }
    else
    {
        return 0;				// UART���մ���
    }
}



//��վ�ظ���վ
//slave_addr: ��վ��ַ
//func_code:  ������
//sub_addr:   �ظ��ļĴ�����ַ
//rw_num:     �ظ��ļĴ����ĸ���
//buf:        ��Ҫ�ظ���վ�ļĴ���ֵ���ݿ��ַ
void modbus1_reply(uint8 slave_addr, uint8 func_code, uint16 sub_addr, uint16 rw_num, uint8 *buf)
{
    uint8  crc_temp_buf1[MAX_BUFFER_SIZE1 + 3] = {0};
    uint16 crcData = 0;
    uint8  crc_hi = 0, crc_lo = 0;
    uint16 i = 0;
    
    switch (func_code)
    {
        case 0x03:
            modbus1_send_byte(slave_addr);  	// �ӵ�ַ
            modbus1_send_byte(0x03);        	// ������
            modbus1_send_byte(rw_num * 2);  	// �ֽ���
			
            crc_temp_buf1[0] = slave_addr;
            crc_temp_buf1[1] = func_code;
            crc_temp_buf1[2] = rw_num * 2;
                    
            for (i = 0; i < rw_num * 2; i++)   // �Ĵ�������
            {
                if (i + sub_addr * 2 > MAX_BUFFER_SIZE1)
                {
                    break;
                }
                
                modbus1_send_byte(buf[i + sub_addr * 2]);			// ���͸��ºõ�����
                crc_temp_buf1[3 + i] = buf[i + sub_addr * 2];
            }
			
            crcData = crc16(crc_temp_buf1, rw_num * 2 + 3);	
        	crc_hi = crcData >> 8;
        	crc_lo = crcData & 0xff;
        	
        	modbus1_send_byte(crc_hi);
        	modbus1_send_byte(crc_lo);
        	
            break;
            
        case 0x05:
            crc_temp_buf1[0] = slave_addr;
            crc_temp_buf1[1] = 0x05;
            crc_temp_buf1[2] = (uint8)(sub_addr >> 8);
            crc_temp_buf1[3] = (uint8)sub_addr;
            
            if (1 == buf[0])
            {
            	crc_temp_buf1[4] = 0xFF;
            	crc_temp_buf1[5] = 0x00;
            }
            else
            {
            	crc_temp_buf1[4] = 0x00;
            	crc_temp_buf1[5] = 0x00;
            }
            crcData = crc16(crc_temp_buf1, 6);
        	crc_hi = crcData >> 8;
        	crc_lo = crcData & 0xff;

            for (i = 0; i < 6; i++)         //�Ĵ�������
            {
                modbus1_send_byte(crc_temp_buf1[i]);
            }

        	modbus1_send_byte(crc_hi);
        	modbus1_send_byte(crc_lo);
        	
            break;
        	
        case 0x06:
            crc_temp_buf1[0] = slave_addr;
            crc_temp_buf1[1] = 0x06;
            crc_temp_buf1[2] = (uint8)(sub_addr >> 8);
            crc_temp_buf1[3] = (uint8)sub_addr;
            crc_temp_buf1[4] = buf[sub_addr * 2];
            crc_temp_buf1[5] = buf[sub_addr * 2 + 1];

            crcData = crc16(crc_temp_buf1, 6);
        	crc_hi = crcData >> 8;
        	crc_lo = crcData & 0xff;

            for (i = 0; i < 6; i++)         //�Ĵ�������
            {
                modbus1_send_byte(crc_temp_buf1[i]);
            }

        	modbus1_send_byte(crc_hi);
        	modbus1_send_byte(crc_lo);
        	
            break;
            
        case 0x10:
            crc_temp_buf1[0] = slave_addr;
            crc_temp_buf1[1] = 0x10;
            crc_temp_buf1[2] = (uint8)(sub_addr >> 8);
            crc_temp_buf1[3] = (uint8)sub_addr;
            crc_temp_buf1[4] = (uint8)(rw_num >> 8);
            crc_temp_buf1[5] = (uint8)rw_num;

            crcData = crc16(crc_temp_buf1, 6);	
        	crc_hi = crcData >> 8;
        	crc_lo = crcData & 0xff;

            for (i = 0; i < 6; i++)         //�Ĵ�������
            {
                modbus1_send_byte(crc_temp_buf1[i]);
            }

        	modbus1_send_byte(crc_hi);
        	modbus1_send_byte(crc_lo);
        	
            break;
        
        default:
            break;
    }
}


//��վ���޴������ظ�
//slave_addr: ��վ��ַ
//func_code:  ������
//modbus_err: �޴������
void modbus1_err_reply(uint8 slave_addr, uint8 func_code, uint8 modbus_err)
{
	uint8  err_crc_temp_buf1[3] = {0};
	uint16 crcData = 0;
	uint8  crc_hi = 0, crc_lo = 0;
    uint16 i = 0;
	
	err_crc_temp_buf1[0] = slave_addr;
	err_crc_temp_buf1[1] = func_code + 0x80;
	err_crc_temp_buf1[2] = modbus_err;
	
	crcData = crc16(err_crc_temp_buf1, 3);
    crc_hi = crcData >> 8;
    crc_lo = crcData & 0xff;
	
	for (i = 0; i < 3; i++)         //�Ĵ�������
    {
        modbus1_send_byte(err_crc_temp_buf1[i]);
    }
	
	modbus1_send_byte(crc_hi);
	modbus1_send_byte(crc_lo);
}



//******************************************************************************************************
//*                                              MOBUS2                                                *
//*                                           ====��վ===                                              *
//******************************************************************************************************
//��վ������վ������1��ʾ�ɹ�������һ����Ч֡��0��ʾ���ݴ���û�и���,�ú��������Ѱ���OSDELAY
//slave_addr: ��վ��ַ
//func_code:  ���صĹ�����
//sub_addr:   ���صĴ�վ�ļĴ����ӵ�ַ
//rw_num:     ���صĶ�д�Ĵ����ĸ���
//buf:        �������վд�����ɹ����ر�ʾ����һ��д��������
//out_time:   ��Ҫ�ȴ����ʱ��
//mod_err:    modbus���޴������
uint16 subaddr = 0;
uint8 modbus2_listen(uint8 slave_addr, uint8 *func_code, uint16 *sub_addr, uint16 *rw_num, uint8 *buf, uint16 out_time, uint8 *modbus_err)
{
    uint8  err = 0;
    uint8  i = 0;
    
    OSSemPend(RecvUARTFlag2, out_time, &err);
	if (err == OS_NO_ERR)
	{
		if (8 == uart2_receive_num)			// 8�ֽ��ж�
		{
			if ((modbus2_temp[0] == slave_addr) && crc_detect(modbus2_temp, uart2_receive_num))
			{
	    		*func_code = modbus2_temp[1];	// ������
	                
			    switch (modbus2_temp[1])
		        {
		            // Request
		            // Function code		[1 byte]: 0x03
		            // Starting address		[2 byte]: 0x0000 to 0xFFFF
		            // Quantity of registers[2 byte]: 0x0001 to 0x007D(125)
		            case 0x03:  // read holding registers
		                *sub_addr = ((uint16)modbus2_temp[2] << 8) + modbus2_temp[3];	// Starting address
	                    *rw_num   = ((uint16)modbus2_temp[4] << 8) + modbus2_temp[5];	// Quantity of registers
	
	                    if (((*sub_addr + *rw_num) * 2) > MAX_BUFFER_SIZE2)
	                    {    
	                        *modbus_err = EXCEPTION_CODE_2;
	                        
	                        return 0;
	                    }
	                    
	                    if ((0 == (*rw_num)) || ((*rw_num) > 0x007D))
	                    {
	                    	*modbus_err = EXCEPTION_CODE_3;
	                    	
	                    	return 0;
	                    }
	                    
	                    *modbus_err = MODBUS_NO_ERR;
	                    
	                    return 1;
		            	
		            // Request
		            // Function code	[1 byte]: 0x05
		            // Output address	[2 byte]: 0x0000 to 0xFFFF
		            // Output value		[2 byte]: 0x0000 or 0xFF00
		            case 0x05:  // write single coil--OFF or ON
		                *sub_addr = modbus2_temp[3] + ((uint16)modbus2_temp[2] << 8);
		                
	                    if (*sub_addr * 2  > MAX_BUFFER_SIZE2) 
	                    {    
	                        *modbus_err = EXCEPTION_CODE_2;
	                        
	                        return 0;
	                    }
	                    
	                    if ((0xFF == modbus2_temp[4]) && (0x00 == modbus2_temp[5]))
	                    {
	                    	buf[0] = 0x01;
	                    }
	                    else if ((0x00 == modbus2_temp[4]) && (0x00 == modbus2_temp[5]))
	                    {
	                    	buf[0] = 0x00;
	                    }
	                    else
	                    {
	                    	*modbus_err = EXCEPTION_CODE_3;
	                    	
	                    	return 0;
	                    }
	                    
	                    *modbus_err = MODBUS_NO_ERR;
	                    
	                    return 1;
		            	
		            // Request
		            // Function code	[1 byte]: 0x06
		            // Output address	[2 byte]: 0x0000 to 0xFFFF
		            // Output value		[2 byte]: 0x0000 to 0xFFFF
		            case 0x06:  // write single register
		                *sub_addr = modbus2_temp[3] + ((uint16)modbus2_temp[2] << 8);
		                
	                    if ((*sub_addr + 1) * 2  > MAX_BUFFER_SIZE2)
	                    {    
	                        *modbus_err = EXCEPTION_CODE_2;
	                        
	                        return 0;
	                    }
	                    subaddr = *sub_addr;
	                    buf[*sub_addr * 2]     = modbus2_temp[4];
	                    buf[*sub_addr * 2 + 1] = modbus2_temp[5];
						
						*modbus_err = MODBUS_NO_ERR;
						
	                    return 1;
		            
		            default:
		            	*modbus_err = EXCEPTION_CODE_1;
		            	
		                return 0;
		        }
	        }
	        else
	        {
	            return 0;		// ��ַ��CRCУ�����
	        }
		}
		else if(uart2_receive_num > 8)
		{
			if ((modbus2_temp[0] == slave_addr) && crc_detect(modbus2_temp, uart2_receive_num))
			{
	    		*func_code = modbus2_temp[1];	// ������
	                
			    switch (modbus2_temp[1])
		        {
		            // Request
		            // Function code		[1 byte]: 0x10
		            // Starting address		[2 byte]: 0x0000 to 0xFFFF
		            // Quantity of registers[2 byte]: 0x0001 to 0x007B(123)
		            // Byte count			[1 byte]: 2 x N
		            // Registers value	[N x 2 byte]: value
		            case 0x10:  // write multiple registers
		                *sub_addr = ((uint16)modbus2_temp[2] << 8) + modbus2_temp[3];	// Starting address
	                    *rw_num   = ((uint16)modbus2_temp[4] << 8) + modbus2_temp[5];	// Quantity of registers
	                    
	                    if (((*sub_addr + *rw_num) * 2) > MAX_BUFFER_SIZE2)
	                    {
	                        *modbus_err = EXCEPTION_CODE_2;
	                        
	                        return 0;
	                    }
	                    
	                    if ((0 == (*rw_num)) || ((*rw_num) > 0x007B) || (((*rw_num) * 2) != modbus2_temp[6]))
	                    {
	                    	*modbus_err = EXCEPTION_CODE_3;
	                    	
	                    	return 0;
	                    }
	                    
	                    if ((uart2_receive_num - 9) != (modbus2_temp[6]))
	                    {
	                    	*modbus_err = EXCEPTION_CODE_4;
	                    	
	                    	return 0;
	                    }
	                    
	                    for (i = 0; i < *rw_num; i++)
	                    {
	                    	buf[(*sub_addr + i) * 2]     = modbus2_temp[7 + (i * 2)];
	                    	buf[(*sub_addr + i) * 2 + 1] = modbus2_temp[8 + (i * 2)];
	                    }
	                    
	                    *modbus_err = MODBUS_NO_ERR;
	                    
	                    return 1;
		            	
		            default: 
		                *modbus_err = EXCEPTION_CODE_1;
		                
		                return 0;
		        }
	        }
	        else
	        {
	            return 0;		// ��ַ��CRCУ�����
	        }
		}
		else
		{
			return 0;			// ���յ����� < 8���ֽ�
		}
    }
    else
    {
        return 0;				// UART���մ���
    }
}



//��վ�ظ���վ
//slave_addr: ��վ��ַ
//func_code:  ������
//sub_addr:   �ظ��ļĴ�����ַ
//rw_num:     �ظ��ļĴ����ĸ���
//buf:        ��Ҫ�ظ���վ�ļĴ���ֵ���ݿ��ַ
void modbus2_reply(uint8 slave_addr, uint8 func_code, uint16 sub_addr, uint16 rw_num, uint8 *buf)
{
    uint8  crc_temp_buf2[MAX_BUFFER_SIZE2 + 3] = {0};
    uint16 crcData = 0;
    uint8  crc_hi = 0, crc_lo = 0;
    uint16 i = 0;
    
    //slow_out(RS485DIR0, OUT);
    
    switch (func_code)
    {
        case 0x03:
            modbus2_send_byte(slave_addr);  	// �ӵ�ַ
            modbus2_send_byte(0x03);        	// ������
            modbus2_send_byte(rw_num * 2);  	// �ֽ���

            crc_temp_buf2[0] = slave_addr;
            crc_temp_buf2[1] = func_code;
            crc_temp_buf2[2] = rw_num * 2;
                    
            for (i = 0; i < rw_num * 2; i++)   // �Ĵ�������
            {
                if (i + sub_addr * 2 > MAX_BUFFER_SIZE2)
                {
                    break;
                }
                
                modbus2_send_byte(buf[i + sub_addr * 2]);
                crc_temp_buf2[3 + i] = buf[i + sub_addr * 2];
            }
			
            crcData = crc16(crc_temp_buf2, rw_num * 2 + 3);
        	crc_hi = crcData >> 8;
        	crc_lo = crcData & 0xff;
        	
        	modbus2_send_byte(crc_hi);
        	modbus2_send_byte(crc_lo);
        	
            break;
            
        case 0x05:
            crc_temp_buf2[0] = slave_addr;
            crc_temp_buf2[1] = 0x05;
            crc_temp_buf2[2] = (uint8)(sub_addr >> 8);
            crc_temp_buf2[3] = (uint8)sub_addr;
            
            if (1 == buf[0])
            {
            	crc_temp_buf2[4] = 0xFF;
            	crc_temp_buf2[5] = 0x00;
            }
            else
            {
            	crc_temp_buf2[4] = 0x00;
            	crc_temp_buf2[5] = 0x00;
            }
            crcData = crc16(crc_temp_buf2, 6);
        	crc_hi = crcData >> 8;
        	crc_lo = crcData & 0xff;

            for (i = 0; i < 6; i++)         //�Ĵ�������
            {
                modbus2_send_byte(crc_temp_buf2[i]);
            }

        	modbus2_send_byte(crc_hi);
        	modbus2_send_byte(crc_lo);
        	
            break;
        	
        case 0x06:
            crc_temp_buf2[0] = slave_addr;
            crc_temp_buf2[1] = 0x06;
            crc_temp_buf2[2] = (uint8)(sub_addr >> 8);
            crc_temp_buf2[3] = (uint8)sub_addr;
            crc_temp_buf2[4] = buf[sub_addr * 2];
            crc_temp_buf2[5] = buf[sub_addr * 2 + 1];

            crcData = crc16(crc_temp_buf2, 6);
        	crc_hi = crcData >> 8;
        	crc_lo = crcData & 0xff;

            for (i = 0; i < 6; i++)         //�Ĵ�������
            {
                modbus2_send_byte(crc_temp_buf2[i]);
            }
			
        	modbus2_send_byte(crc_hi);
        	modbus2_send_byte(crc_lo);
        	
            break;
            
        case 0x10:
            crc_temp_buf2[0] = slave_addr;
            crc_temp_buf2[1] = 0x10;
            crc_temp_buf2[2] = (uint8)(sub_addr >> 8);
            crc_temp_buf2[3] = (uint8)sub_addr;
            crc_temp_buf2[4] = (uint8)(rw_num >> 8);
            crc_temp_buf2[5] = (uint8)rw_num;

            crcData = crc16(crc_temp_buf2, 6);
        	crc_hi = crcData >> 8;
        	crc_lo = crcData & 0xff;

            for (i = 0; i < 6; i++)         //�Ĵ�������
            {
                modbus2_send_byte(crc_temp_buf2[i]);
            }
			
        	modbus2_send_byte(crc_hi);
        	modbus2_send_byte(crc_lo);
        	
            break;
        
        default:
            break;
    }
}


//��վ���޴������ظ�
//slave_addr: ��վ��ַ
//func_code:  ������
//modbus_err: �޴������
void modbus2_err_reply(uint8 slave_addr, uint8 func_code, uint8 modbus_err)
{
	uint8  err_crc_temp_buf2[3] = {0};
	uint16 crcData = 0;
	uint8  crc_hi = 0, crc_lo = 0;
    uint16 i = 0;
	
	err_crc_temp_buf2[0] = slave_addr;
	err_crc_temp_buf2[1] = func_code + 0x80;
	err_crc_temp_buf2[2] = modbus_err;
	
	crcData = crc16(err_crc_temp_buf2, 3);
    crc_hi = crcData >> 8;
    crc_lo = crcData & 0xff;
	
	for (i = 0; i < 3; i++)         //�Ĵ�������
    {
        modbus2_send_byte(err_crc_temp_buf2[i]);
    }
	
	modbus2_send_byte(crc_hi);
	modbus2_send_byte(crc_lo);
}


//******************************************************************************************************
//*                                              MOBUS3                                                *
//*                                           ====��վ===                                              *
//******************************************************************************************************
//��վ������վ������1��ʾ�ɹ�������һ����Ч֡��0��ʾ���ݴ���û�и���,�ú��������Ѱ���OSDELAY
//slave_addr: ��վ��ַ
//func_code:  ���صĹ�����
//sub_addr:   ���صĴ�վ�ļĴ����ӵ�ַ
//rw_num:     ���صĶ�д�Ĵ����ĸ���
//buf:        �������վд�����ɹ����ر�ʾ����һ��д��������
//out_time:   ��Ҫ�ȴ����ʱ��
//mod_err:    modbus���޴������
uint8 modbus3_listen(uint8 slave_addr, uint8 *func_code, uint16 *sub_addr, uint16 *rw_num, uint8 *buf, uint16 out_time, uint8 *modbus_err)
{
    uint8  err = 0;
    uint8  i = 0;
    
    //slow_out(RS485DIR0, IN);
    
    OSSemPend(RecvUARTFlag3, out_time, &err);
	if (err == OS_NO_ERR)
	{
		if (8 == uart3_receive_num)			// 8�ֽ��ж�
		{
			if ((modbus3_temp[0] == slave_addr) && crc_detect(modbus3_temp, uart3_receive_num))
			{
	    		*func_code = modbus3_temp[1];	// ������
	                
			    switch (modbus3_temp[1])
		        {
		            // Request
		            // Function code		[1 byte]: 0x03
		            // Starting address		[2 byte]: 0x0000 to 0xFFFF
		            // Quantity of registers[2 byte]: 0x0001 to 0x007D(125)
		            case 0x03:  // read holding registers
		                *sub_addr = ((uint16)modbus3_temp[2] << 8) + modbus3_temp[3];	// Starting address
	                    *rw_num   = ((uint16)modbus3_temp[4] << 8) + modbus3_temp[5];	// Quantity of registers
	
	                    if (((*sub_addr + *rw_num) * 2) > MAX_BUFFER_SIZE3)
	                    {
	                        *modbus_err = EXCEPTION_CODE_2;
	                        
	                        return 0;
	                    }
	                    
	                    if ((0 == (*rw_num)) || ((*rw_num) > 0x007D))
	                    {
	                    	*modbus_err = EXCEPTION_CODE_3;
	                    	
	                    	return 0;
	                    }
	                    
	                    *modbus_err = MODBUS_NO_ERR;
	                    
	                    return 1;
		            	
		            // Request
		            // Function code	[1 byte]: 0x05
		            // Output address	[2 byte]: 0x0000 to 0xFFFF
		            // Output value		[2 byte]: 0x0000 or 0xFF00
		            case 0x05:  // write single coil--OFF or ON
		                *sub_addr = modbus3_temp[3] + ((uint16)modbus3_temp[2] << 8);
		                
	                    if (*sub_addr * 2  > MAX_BUFFER_SIZE3) 
	                    {    
	                        *modbus_err = EXCEPTION_CODE_2;
	                        
	                        return 0;
	                    }
	                    
	                    if ((0xFF == modbus3_temp[4]) && (0x00 == modbus3_temp[5]))
	                    {
	                    	buf[0] = 0x01;
	                    }
	                    else if ((0x00 == modbus3_temp[4]) && (0x00 == modbus3_temp[5]))
	                    {
	                    	buf[0] = 0x00;
	                    }
	                    else
	                    {
	                    	*modbus_err = EXCEPTION_CODE_3;
	                    	
	                    	return 0;
	                    }
	                    
	                    *modbus_err = MODBUS_NO_ERR;
	                    
	                    return 1;
		            	
		            // Request
		            // Function code	[1 byte]: 0x06
		            // Output address	[2 byte]: 0x0000 to 0xFFFF
		            // Output value		[2 byte]: 0x0000 to 0xFFFF
		            case 0x06:  // write single register
		                *sub_addr = modbus3_temp[3] + ((uint16)modbus3_temp[2] << 8);
		                
	                    if ((*sub_addr + 1) * 2  > MAX_BUFFER_SIZE3)
	                    {    
	                        *modbus_err = EXCEPTION_CODE_2;
	                        
	                        return 0;
	                    }
	                    
	                    buf[*sub_addr * 2]     = modbus3_temp[4];
	                    buf[*sub_addr * 2 + 1] = modbus3_temp[5];
						
						*modbus_err = MODBUS_NO_ERR;
						
	                    return 1;
		            	
		            default:
		            	*modbus_err = EXCEPTION_CODE_1;
		            	
		                return 0;
		        }
	        }
	        else
	        {
	            return 0;		// ��ַ��CRCУ�����
	        }
		}
		else if(uart3_receive_num > 8)
		{
			if ((modbus3_temp[0] == slave_addr) && crc_detect(modbus3_temp, uart3_receive_num))
			{
	    		*func_code = modbus3_temp[1];	// ������
	                
			    switch (modbus3_temp[1])
		        {
		            // Request
		            // Function code		[1 byte]: 0x10
		            // Starting address		[2 byte]: 0x0000 to 0xFFFF
		            // Quantity of registers[2 byte]: 0x0001 to 0x007B(123)
		            // Byte count			[1 byte]: 2 x N
		            // Registers value	[N x 2 byte]: value
		            case 0x10:  // write multiple registers
		                *sub_addr = ((uint16)modbus3_temp[2] << 8) + modbus3_temp[3];	// Starting address
	                    *rw_num   = ((uint16)modbus3_temp[4] << 8) + modbus3_temp[5];	// Quantity of registers
	                    
	                    if (((*sub_addr + *rw_num) * 2) > MAX_BUFFER_SIZE3)
	                    {    
	                        *modbus_err = EXCEPTION_CODE_2;
	                        
	                        return 0;
	                    }
	                    
	                    if ((0 == (*rw_num)) || ((*rw_num) > 0x007B) || (((*rw_num) * 2) != modbus3_temp[6]))
	                    {
	                    	*modbus_err = EXCEPTION_CODE_3;
	                    	
	                    	return 0;
	                    }
	                    
	                    if ((uart3_receive_num - 9) != (modbus3_temp[6]))
	                    {
	                    	*modbus_err = EXCEPTION_CODE_4;
	                    	
	                    	return 0;
	                    }
	                    
	                    for (i = 0; i < *rw_num; i++)
	                    {
	                    	buf[(*sub_addr + i) * 2]     = modbus3_temp[7 + (i * 2)];
	                    	buf[(*sub_addr + i) * 2 + 1] = modbus3_temp[8 + (i * 2)];
	                    }
	                    
	                    *modbus_err = MODBUS_NO_ERR;
	                    
	                    return 1;
		            
		            default: 
		                *modbus_err = EXCEPTION_CODE_1;
		                
		                return 0;
		        }
	        }
	        else
	        {
	            return 0;		// ��ַ��CRCУ�����
	        }
		}
		else
		{
			return 0;			// ���յ����� < 8���ֽ�
		}
    }
    else
    {
        return 0;				// UART���մ���
    }
}


//��վ�ظ���վ
//slave_addr: ��վ��ַ
//func_code:  ������
//sub_addr:   �ظ��ļĴ�����ַ
//rw_num:     �ظ��ļĴ����ĸ���
//buf:        ��Ҫ�ظ���վ�ļĴ���ֵ���ݿ��ַ
void modbus3_reply(uint8 slave_addr, uint8 func_code, uint16 sub_addr, uint16 rw_num, uint8 *buf)
{
    uint8  crc_temp_buf3[MAX_BUFFER_SIZE3 + 3] = {0};
    uint16 crcData = 0;
    uint8  crc_hi = 0, crc_lo = 0;
    uint16 i = 0;
    
    //slow_out(RS485DIR0, OUT);
    
    switch (func_code)
    {
        case 0x03:
            modbus3_send_byte(slave_addr);  // �ӵ�ַ
            modbus3_send_byte(0x03);        // ������
            modbus3_send_byte(rw_num * 2);  // �ֽ���

            crc_temp_buf3[0] = slave_addr;
            crc_temp_buf3[1] = func_code;
            crc_temp_buf3[2] = rw_num * 2;
                    
            for (i = 0; i < rw_num * 2; i++)   //�Ĵ�������
            {
                if (i + sub_addr * 2 > MAX_BUFFER_SIZE3)
                {
                    break;
                }
                
                modbus3_send_byte(buf[i + sub_addr * 2]);
                crc_temp_buf3[3 + i] = buf[i + sub_addr * 2];
            }

            crcData = crc16(crc_temp_buf3, rw_num * 2 + 3);
        	crc_hi = crcData >> 8;
        	crc_lo = crcData & 0xff;
        	
        	modbus3_send_byte(crc_hi);
        	modbus3_send_byte(crc_lo);
        	
            break;
            
        case 0x05:
            crc_temp_buf3[0] = slave_addr;
            crc_temp_buf3[1] = 0x05;
            crc_temp_buf3[2] = (uint8)(sub_addr >> 8);
            crc_temp_buf3[3] = (uint8)sub_addr;
            
            if (1 == buf[0])
            {
            	crc_temp_buf3[4] = 0xFF;
            	crc_temp_buf3[5] = 0x00;
            }
            else
            {
            	crc_temp_buf3[4] = 0x00;
            	crc_temp_buf3[5] = 0x00;
            }
            crcData = crc16(crc_temp_buf3, 6);
        	crc_hi = crcData >> 8;
        	crc_lo = crcData & 0xff;

            for (i = 0; i < 6; i++)         //�Ĵ�������
            {
                modbus3_send_byte(crc_temp_buf3[i]);
            }

        	modbus3_send_byte(crc_hi);
        	modbus3_send_byte(crc_lo);
        	
            break;
        
        case 0x06:
            crc_temp_buf3[0] = slave_addr;
            crc_temp_buf3[1] = 0x06;
            crc_temp_buf3[2] = (uint8)(sub_addr >> 8);
            crc_temp_buf3[3] = (uint8)sub_addr;
            crc_temp_buf3[4] = buf[sub_addr * 2];
            crc_temp_buf3[5] = buf[sub_addr * 2 + 1];

            crcData = crc16(crc_temp_buf3, 6);
        	crc_hi = crcData >> 8;
        	crc_lo = crcData & 0xff;

            for (i = 0; i < 6; i++)         //�Ĵ�������
            {
                modbus3_send_byte(crc_temp_buf3[i]);
            }

        	modbus3_send_byte(crc_hi);
        	modbus3_send_byte(crc_lo);
        	
            break;
            
        case 0x10:
            crc_temp_buf3[0] = slave_addr;
            crc_temp_buf3[1] = 0x10;
            crc_temp_buf3[2] = (uint8)(sub_addr >> 8);
            crc_temp_buf3[3] = (uint8)sub_addr;
            crc_temp_buf3[4] = (uint8)(rw_num >> 8);
            crc_temp_buf3[5] = (uint8)rw_num;

            crcData = crc16(crc_temp_buf3, 6);	
        	crc_hi = crcData >> 8;
        	crc_lo = crcData & 0xff;

            for (i = 0; i < 6; i++)         //�Ĵ�������
            {
                modbus3_send_byte(crc_temp_buf3[i]);
            }

        	modbus3_send_byte(crc_hi);
        	modbus3_send_byte(crc_lo);
        	
            break;
        	
        default:
            break;
    }
}


//��վ���޴������ظ�
//slave_addr: ��վ��ַ
//func_code:  ������
//modbus_err: �޴������
void modbus3_err_reply(uint8 slave_addr, uint8 func_code, uint8 modbus_err)
{
	uint8  err_crc_temp_buf3[3] = {0};
	uint16 crcData = 0;
	uint8  crc_hi = 0, crc_lo = 0;
    uint16 i = 0;
	
	err_crc_temp_buf3[0] = slave_addr;
	err_crc_temp_buf3[1] = func_code + 0x80;
	err_crc_temp_buf3[2] = modbus_err;
	
	crcData = crc16(err_crc_temp_buf3, 3);
    crc_hi = crcData >> 8;
    crc_lo = crcData & 0xff;
	
	for (i = 0; i < 3; i++)         //�Ĵ�������
    {
        modbus3_send_byte(err_crc_temp_buf3[i]);
    }

	modbus3_send_byte(crc_hi);
	modbus3_send_byte(crc_lo);
}



//******************************************************************************************************
//*                                              MOBUS0                                                *
//*                                           ====��վ===                                              *
//******************************************************************************************************
//��վѯ�ʴ�վ[03������]
//slave_addr: ��վ��ַ
//sub_addr��  ��վ�ļĴ����ӵ�ַ(��ʼ��ַ)
//reg_num:    ѯ�ʼĴ����ĸ���
void modbus0_ask03(uint8 slave_addr, uint16 sub_addr, uint16 reg_num)
{
    uint8  crc_temp_buf0[6] = {0};
    uint16 crcData = 0;
    uint8  crc_hi = 0, crc_lo = 0;
    uint8  i = 0;
        
    //slow_out(RS485DIR0, OUT);
    
    crc_temp_buf0[0] = slave_addr;
	crc_temp_buf0[1] = 0x03;
	crc_temp_buf0[2] = (uint8)(sub_addr >> 8);
	crc_temp_buf0[3] = (uint8)sub_addr;
	crc_temp_buf0[4] = (uint8)(reg_num >> 8);
	crc_temp_buf0[5] = (uint8)reg_num;
	
	crcData = crc16(crc_temp_buf0, 6);
    crc_hi = crcData >> 8;
    crc_lo = crcData & 0xff;
    
    for (i = 0; i < 6; i++)
    {
    	modbus0_send_byte(crc_temp_buf0[i]);
    }

    modbus0_send_byte(crc_hi);
    modbus0_send_byte(crc_lo);
}


//��վ���մ�վ[03������]
//����MODBUS��������������1��ʾ���³ɹ���0��ʾ���ݴ���û�и���,�ú��������Ѱ���OSDELAY
//slave_addr: ѯ�ʵĴ�վ��ַ
//reg_num:    ѯ�ʼĴ����ĸ���
//buf:        ���³ɹ��󷵻ص�modbus������
//out_time��  ��Ҫ�ȴ����ʱ��
uint8 modbus0_read03(uint8 slave_addr, uint16 sub_addr, uint16 reg_num, uint8 *buf, uint16 out_time)
{
    uint8  err = 0;
    uint8  i = 0;
    
    if ((sub_addr + reg_num) * 2 > MAX_BUFFER_SIZE0)
    {
        return 0;
    }
    
    //slow_out(RS485DIR0, IN);
    uart0_timeout_8byte_stop();
    
    OSSemPend(RecvUARTFlag0, out_time, &err);
	if (err == OS_NO_ERR)
	{
		if ((modbus0_temp[0] == slave_addr) && (modbus0_temp[1] == 0x03) && (modbus0_temp[2] == reg_num * 2))
        {
        	if (1 == crc_detect(modbus0_temp, reg_num*2 + 5))
            {
                for (i = 0; i < reg_num * 2; i++)
        		{
        		    buf[sub_addr * 2 + i] = modbus0_temp[i + 3];      //��ȡ���������ݴ�����ݵ�modbus���ڴ���
        		}
        		
        		return 1;
            }
            else
            {
                return 0;
            }
        }  
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}


//��վ���ô�վ[05������]
//slave_addr: ��վ��ַ
//sub_addr��  ��վ�ļĴ����ӵ�ַ
//reg_value:  ���üĴ�����ֵ(0xFF00 or 0x0000)
void modbus0_set05(uint8 slave_addr, uint16 sub_addr, uint16 reg_value)
{
    uint8  crc_temp_buf0[6] = {0};
    uint16 crcData = 0;
    uint8  crc_hi = 0, crc_lo = 0;
    uint8  i = 0;
	
    //slow_out(RS485DIR0, OUT);
    
    crc_temp_buf0[0] = slave_addr;
	crc_temp_buf0[1] = 0x05;
	crc_temp_buf0[2] = (uint8)(sub_addr >> 8);
	crc_temp_buf0[3] = (uint8)sub_addr;
	crc_temp_buf0[4] = (uint8)(reg_value >> 8);
	crc_temp_buf0[5] = (uint8)reg_value;
	
	crcData = crc16(crc_temp_buf0, 6);
    crc_hi = crcData >> 8;
    crc_lo = crcData & 0xff;
    
    for (i = 0; i < 6; i++)
    {
    	modbus0_send_byte(crc_temp_buf0[i]);
    }

    modbus0_send_byte(crc_hi);
    modbus0_send_byte(crc_lo);
}


//��վ�ظ���վ[05������]
//����MODBUS��������������1��ʾ���³ɹ���0��ʾ���ݴ���û�и���,�ú��������Ѱ���OSDELAY
//slave_addr: ��Ҫ���صĴ�վ��ַ�����ش�ֵ������ȷ
//sub_addr:   ��Ҫ���صĴ�վ�Ĵ�����ַ�����ش�ֵ������ȷ
//reg_value:  ��Ҫ���صļĴ���ֵ�����ش�ֵ������ȷ
//out_time��  ��Ҫ�ȴ����ʱ��
uint8 modbus0_read05(uint8 slave_addr, uint16 sub_addr, uint16 reg_value, uint16 out_time)
{
    uint8 err = 0;
    
    if (sub_addr * 2 > MAX_BUFFER_SIZE0)
    {
        return 0;
    }
    
    //slow_out(RS485DIR0, IN);
    uart0_timeout_8byte_stop();
        
    OSSemPend(RecvUARTFlag0, out_time, &err);
	if (err == OS_NO_ERR)
	{
		if ((8 == uart0_receive_num) && (modbus0_temp[0] == slave_addr) && (modbus0_temp[1] == 0x05)
			&& (modbus0_temp[2] == (uint8)(sub_addr >> 8)) && (modbus0_temp[3] == (uint8)sub_addr))
        {
        	if (1 == crc_detect(modbus0_temp, 8))
            {
                if ((modbus0_temp[4] == (uint8)(reg_value >> 8)) && (modbus0_temp[5] == (uint8)reg_value))
        		{
        		    return 1;
                }
                else 
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }  
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}


//��վ���ô�վ[06������]
//slave_addr: ��վ��ַ
//sub_addr��  ��վ�ļĴ����ӵ�ַ
//reg_value:  ���üĴ�����ֵ
void modbus0_set06(uint8 slave_addr, uint16 sub_addr, uint16 reg_value)
{
    uint8  crc_temp_buf0[6] = {0};
    uint16 crcData = 0;
    uint8  crc_hi = 0, crc_lo = 0;
    uint8  i = 0;
    
    //slow_out(RS485DIR0, OUT);
    
    crc_temp_buf0[0] = slave_addr;
	crc_temp_buf0[1] = 0x06;
	crc_temp_buf0[2] = (uint8)(sub_addr >> 8);
	crc_temp_buf0[3] = (uint8)sub_addr;
	crc_temp_buf0[4] = (uint8)(reg_value >> 8);
	crc_temp_buf0[5] = (uint8)reg_value;
	
	crcData = crc16(crc_temp_buf0, 6);
    crc_hi = crcData >> 8;
    crc_lo = crcData & 0xff;
    
    for (i = 0; i < 6; i++)
    {
    	modbus0_send_byte(crc_temp_buf0[i]);
    }
	
    modbus0_send_byte(crc_hi);
    modbus0_send_byte(crc_lo);
}

//��վ�ظ���վ[06������]
//����MODBUS��������������1��ʾ���³ɹ���0��ʾ���ݴ���û�и���,�ú��������Ѱ���OSDELAY
//slave_addr: ��Ҫ���صĴ�վ��ַ�����ش�ֵ������ȷ
//sub_addr:   ��Ҫ���صĴ�վ�Ĵ�����ַ�����ش�ֵ������ȷ
//reg_value:  ��Ҫ���صļĴ���ֵ�����ش�ֵ������ȷ
//out_time��  ��Ҫ�ȴ����ʱ��
uint8 modbus0_read06(uint8 slave_addr, uint16 sub_addr, uint16 reg_value, uint16 out_time)
{
    uint8 err = 0;
    
    if (sub_addr * 2 > MAX_BUFFER_SIZE0)
    {
        return 0;
    }
    
    //slow_out(RS485DIR0, IN);
    uart0_timeout_8byte_stop();
        
    OSSemPend(RecvUARTFlag0, out_time, &err);
	if (err == OS_NO_ERR)
	{
		if ((8 == uart0_receive_num) && (modbus0_temp[0] == slave_addr) && (modbus0_temp[1] == 0x06)
			&& (modbus0_temp[2] == (uint8)(sub_addr >> 8)) && (modbus0_temp[3] == (uint8)sub_addr))
        {
        	if (1 == crc_detect(modbus0_temp, 8))
            {
                if ((modbus0_temp[4] == (uint8)(reg_value >> 8)) && (modbus0_temp[5] == (uint8)reg_value))
        		{
        		    return 1;
                }
                else 
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }  
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}



//��վ���ô�վ[16������]
//slave_addr: ��վ��ַ
//sub_addr��  ��վ�ļĴ����ӵ�ַ
//reg_value:  ���üĴ�����ֵ
//reg_num:    ѯ�ʼĴ����ĸ���
//reg_value_buf:�������õļĴ�������
void modbus0_set16(uint8 slave_addr, uint16 sub_addr,uint16 reg_num, uint8 *reg_value_buf)
{
    uint8  crc_temp_buf0[MAX_BUFFER_SIZE0 + 7] = {0};
    uint16 crcData = 0;
    uint8  crc_hi = 0, crc_lo = 0;
    uint8  i = 0;
    
    //slow_out(RS485DIR0, OUT);
    
    crc_temp_buf0[0] = slave_addr;
	crc_temp_buf0[1] = 0x10;
	crc_temp_buf0[2] = (uint8)(sub_addr >> 8);
	crc_temp_buf0[3] = (uint8)sub_addr;
	crc_temp_buf0[4] = (uint8)(reg_num >> 8);
	crc_temp_buf0[5] = (uint8)reg_num;
	crc_temp_buf0[6] = (uint8)(reg_num * 2);
	
	for (i = 0; i < (reg_num * 2); i++)
	{
		crc_temp_buf0[i + 7] = *reg_value_buf++;
	}
	
	crcData = crc16(crc_temp_buf0, (reg_num * 2 + 7));
    crc_hi = crcData >> 8;
    crc_lo = crcData & 0xff;
    
    for (i = 0; i < (reg_num * 2 + 7); i++)
    {
    	modbus0_send_byte(crc_temp_buf0[i]);
    }

    modbus0_send_byte(crc_hi);
    modbus0_send_byte(crc_lo);
}

//��վ�ظ���վ[16������]
//����MODBUS��������������1��ʾ���³ɹ���0��ʾ���ݴ���û�и���,�ú��������Ѱ���OSDELAY
//slave_addr: ��Ҫ���صĴ�վ��ַ�����ش�ֵ������ȷ
//sub_addr:   ��Ҫ���صĴ�վ�Ĵ�����ַ�����ش�ֵ������ȷ
//reg_num:    ��Ҫ���صļĴ����ĸ��������ش�ֵ������ȷ
//out_time��  ��Ҫ�ȴ����ʱ��
uint8 modbus0_read16(uint8 slave_addr, uint16 sub_addr, uint16 reg_num, uint16 out_time)
{
    uint8 err = 0;
    
    if (sub_addr * 2 > MAX_BUFFER_SIZE0)
    {
        return 0;
    }
    
    //slow_out(RS485DIR0, IN);
    uart0_timeout_8byte_stop();
        
    OSSemPend(RecvUARTFlag0, out_time, &err);
	if (err == OS_NO_ERR)
	{
		if ((8 == uart0_receive_num) && (modbus0_temp[0] == slave_addr) && (modbus0_temp[1] == 0x10)
			&& (modbus0_temp[2] == (uint8)(sub_addr >> 8)) && (modbus0_temp[3] == (uint8)sub_addr))
        {
        	if (1 == crc_detect(modbus0_temp, 8))
            {
                if ((modbus0_temp[4] == (uint8)(reg_num >> 8)) && (modbus0_temp[5] == (uint8)reg_num))
        		{
        		    return 1;
                }
                else 
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }  
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}



//******************************************************************************************************
//*                                              MOBUS1                                                *
//*                                           ====��վ===                                              *
//******************************************************************************************************
//��վѯ�ʴ�վ[03������]
//slave_addr: ��վ��ַ
//sub_addr��  ��վ�ļĴ����ӵ�ַ(��ʼ��ַ)
//reg_num:    ѯ�ʼĴ����ĸ���
void modbus1_ask03(uint8 slave_addr, uint16 sub_addr, uint16 reg_num)
{
    uint8  crc_temp_buf1[6] = {0};
    uint16 crcData = 0;
    uint8  crc_hi = 0, crc_lo = 0;
    uint8  i = 0;
        
    //slow_out(RS485DIR1, OUT);
    
    crc_temp_buf1[0] = slave_addr;
	crc_temp_buf1[1] = 0x03;
	crc_temp_buf1[2] = (uint8)(sub_addr >> 8);
	crc_temp_buf1[3] = (uint8)sub_addr;
	crc_temp_buf1[4] = (uint8)(reg_num >> 8);
	crc_temp_buf1[5] = (uint8)reg_num;
	
	crcData = crc16(crc_temp_buf1, 6);
    crc_hi = crcData >> 8;
    crc_lo = crcData & 0xff;
    
    for (i = 0; i < 6; i++)
    {
    	modbus1_send_byte(crc_temp_buf1[i]);
    }

    modbus1_send_byte(crc_hi);
    modbus1_send_byte(crc_lo);
}


//��վ���մ�վ[03������]
//����MODBUS��������������1��ʾ���³ɹ���0��ʾ���ݴ���û�и���,�ú��������Ѱ���OSDELAY
//slave_addr: ѯ�ʵĴ�վ��ַ
//reg_num:    ѯ�ʼĴ����ĸ���
//buf:        ���³ɹ��󷵻ص�modbus������
//out_time��  ��Ҫ�ȴ����ʱ��
uint8 modbus1_read03(uint8 slave_addr, uint16 sub_addr, uint16 reg_num, uint8 *buf, uint16 out_time)
{
    uint8  err = 0;
    uint8  i = 0;
    
    if ((sub_addr + reg_num) * 2 > MAX_BUFFER_SIZE1)
    {
        return 0;
    }
    
    //slow_out(RS485DIR1, IN);
    uart1_timeout_8byte_stop();
    
    OSSemPend(RecvUARTFlag1, out_time, &err);
	if (err == OS_NO_ERR)
	{
		if ((modbus1_temp[0] == slave_addr) && (modbus1_temp[1] == 0x03) && (modbus1_temp[2] == reg_num * 2))
        {
        	if (1 == crc_detect(modbus1_temp, reg_num*2 + 5))
            {
                for (i = 0; i < reg_num * 2; i++)
        		{
        		    buf[sub_addr * 2 + i] = modbus1_temp[i + 3];      //��ȡ���������ݴ�����ݵ�modbus���ڴ���
        		}
        		
        		return 1;
            }
            else
            {
                return 0;
            }
        }  
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}



//��վ���ô�վ[05������]
//slave_addr: ��վ��ַ
//sub_addr��  ��վ�ļĴ����ӵ�ַ
//reg_value:  ���üĴ�����ֵ(0xFF00 or 0x0000)
void modbus1_set05(uint8 slave_addr, uint16 sub_addr, uint16 reg_value)
{
    uint8  crc_temp_buf1[6] = {0};
    uint16 crcData = 0;
    uint8  crc_hi = 0, crc_lo = 0;
    uint8  i = 0;
	
    //slow_out(RS485DIR1, OUT);
    
    crc_temp_buf1[0] = slave_addr;
	crc_temp_buf1[1] = 0x05;
	crc_temp_buf1[2] = (uint8)(sub_addr >> 8);
	crc_temp_buf1[3] = (uint8)sub_addr;
	crc_temp_buf1[4] = (uint8)(reg_value >> 8);
	crc_temp_buf1[5] = (uint8)reg_value;
	
	crcData = crc16(crc_temp_buf1, 6);
    crc_hi = crcData >> 8;
    crc_lo = crcData & 0xff;
    
    for (i = 0; i < 6; i++)
    {
    	modbus1_send_byte(crc_temp_buf1[i]);
    }

    modbus1_send_byte(crc_hi);
    modbus1_send_byte(crc_lo);
}


//��վ�ظ���վ[05������]
//����MODBUS��������������1��ʾ���³ɹ���0��ʾ���ݴ���û�и���,�ú��������Ѱ���OSDELAY
//slave_addr: ��Ҫ���صĴ�վ��ַ�����ش�ֵ������ȷ
//sub_addr:   ��Ҫ���صĴ�վ�Ĵ�����ַ�����ش�ֵ������ȷ
//reg_value:  ��Ҫ���صļĴ���ֵ�����ش�ֵ������ȷ
//out_time��  ��Ҫ�ȴ����ʱ��
uint8 modbus1_read05(uint8 slave_addr, uint16 sub_addr, uint16 reg_value, uint16 out_time)
{
    uint8 err = 0;
    
    if (sub_addr * 2 > MAX_BUFFER_SIZE1)
    {
        return 0;
    }
    
    //slow_out(RS485DIR1, IN);
    uart1_timeout_8byte_stop();
        
    OSSemPend(RecvUARTFlag1, out_time, &err);
	if (err == OS_NO_ERR)
	{
		if ((8 == uart1_receive_num) && (modbus1_temp[0] == slave_addr) && (modbus1_temp[1] == 0x05)
			&& (modbus1_temp[2] == (uint8)(sub_addr >> 8)) && (modbus1_temp[3] == (uint8)sub_addr))
        {
        	if (1 == crc_detect(modbus1_temp, 8))
            {
                if ((modbus1_temp[4] == (uint8)(reg_value >> 8)) && (modbus1_temp[5] == (uint8)reg_value))
        		{
        		    return 1;
                }
                else 
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }  
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}


//��վ���ô�վ[06������]
//slave_addr: ��վ��ַ
//sub_addr��  ��վ�ļĴ����ӵ�ַ
//reg_value:  ���üĴ�����ֵ
void modbus1_set06(uint8 slave_addr, uint16 sub_addr, uint16 reg_value)
{
    uint8  crc_temp_buf1[6] = {0};
    uint16 crcData = 0;
    uint8  crc_hi = 0, crc_lo = 0;
    uint8  i = 0;
    
    //slow_out(RS485DIR1, OUT);
    
    crc_temp_buf1[0] = slave_addr;
	crc_temp_buf1[1] = 0x06;
	crc_temp_buf1[2] = (uint8)(sub_addr >> 8);
	crc_temp_buf1[3] = (uint8)sub_addr;
	crc_temp_buf1[4] = (uint8)(reg_value >> 8);
	crc_temp_buf1[5] = (uint8)reg_value;
	
	crcData = crc16(crc_temp_buf1, 6);
    crc_hi = crcData >> 8;
    crc_lo = crcData & 0xff;
    
    for (i = 0; i < 6; i++)
    {
    	modbus1_send_byte(crc_temp_buf1[i]);
    }
	
    modbus1_send_byte(crc_hi);
    modbus1_send_byte(crc_lo);
}

//��վ�ظ���վ[06������]
//����MODBUS��������������1��ʾ���³ɹ���0��ʾ���ݴ���û�и���,�ú��������Ѱ���OSDELAY
//slave_addr: ��Ҫ���صĴ�վ��ַ�����ش�ֵ������ȷ
//sub_addr:   ��Ҫ���صĴ�վ�Ĵ�����ַ�����ش�ֵ������ȷ
//reg_value:  ��Ҫ���صļĴ���ֵ�����ش�ֵ������ȷ
//out_time��  ��Ҫ�ȴ����ʱ��
uint8 modbus1_read06(uint8 slave_addr, uint16 sub_addr, uint16 reg_value, uint16 out_time)
{
    uint8 err = 0;
    
    if (sub_addr * 2 > MAX_BUFFER_SIZE1)
    {
        return 0;
    }
    
    //slow_out(RS485DIR1, IN);
    uart1_timeout_8byte_stop();
        
    OSSemPend(RecvUARTFlag1, out_time, &err);
	if (err == OS_NO_ERR)
	{
		if ((8 == uart1_receive_num) && (modbus1_temp[0] == slave_addr) && (modbus1_temp[1] == 0x06)
			&& (modbus1_temp[2] == (uint8)(sub_addr >> 8)) && (modbus1_temp[3] == (uint8)sub_addr))
        {
        	if (1 == crc_detect(modbus1_temp, 8))
            {
                if ((modbus1_temp[4] == (uint8)(reg_value >> 8)) && (modbus1_temp[5] == (uint8)reg_value))
        		{
        		    return 1;
                }
                else 
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }  
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}



//��վ���ô�վ[16������]
//slave_addr: ��վ��ַ
//sub_addr��  ��վ�ļĴ����ӵ�ַ
//reg_value:  ���üĴ�����ֵ
//reg_num:    ѯ�ʼĴ����ĸ���
//reg_value_buf:�������õļĴ�������
void modbus1_set16(uint8 slave_addr, uint16 sub_addr,uint16 reg_num, uint8 *reg_value_buf)
{
    uint8  crc_temp_buf1[MAX_BUFFER_SIZE1 + 7] = {0};
    uint16 crcData = 0;
    uint8  crc_hi = 0, crc_lo = 0;
    uint8  i = 0;
    
    //slow_out(RS485DIR1, OUT);
    
    crc_temp_buf1[0] = slave_addr;
	crc_temp_buf1[1] = 0x10;
	crc_temp_buf1[2] = (uint8)(sub_addr >> 8);
	crc_temp_buf1[3] = (uint8)sub_addr;
	crc_temp_buf1[4] = (uint8)(reg_num >> 8);
	crc_temp_buf1[5] = (uint8)reg_num;
	crc_temp_buf1[6] = (uint8)(reg_num * 2);
	
	for (i = 0; i < (reg_num * 2); i++)
	{
		crc_temp_buf1[i + 7] = *reg_value_buf++;
	}
	
	crcData = crc16(crc_temp_buf1, (reg_num * 2 + 7));
    crc_hi = crcData >> 8;
    crc_lo = crcData & 0xff;
    
    for (i = 0; i < (reg_num * 2 + 7); i++)
    {
    	modbus1_send_byte(crc_temp_buf1[i]);
    }

    modbus1_send_byte(crc_hi);
    modbus1_send_byte(crc_lo);
}

//��վ�ظ���վ[16������]
//����MODBUS��������������1��ʾ���³ɹ���0��ʾ���ݴ���û�и���,�ú��������Ѱ���OSDELAY
//slave_addr: ��Ҫ���صĴ�վ��ַ�����ش�ֵ������ȷ
//sub_addr:   ��Ҫ���صĴ�վ�Ĵ�����ַ�����ش�ֵ������ȷ
//reg_num:    ��Ҫ���صļĴ����ĸ��������ش�ֵ������ȷ
//out_time��  ��Ҫ�ȴ����ʱ��
uint8 modbus1_read16(uint8 slave_addr, uint16 sub_addr, uint16 reg_num, uint16 out_time)
{
    uint8 err = 0;
    
    if (sub_addr * 2 > MAX_BUFFER_SIZE1)
    {
        return 0;
    }
    
    //slow_out(RS485DIR1, IN);
    uart1_timeout_8byte_stop();
        
    OSSemPend(RecvUARTFlag1, out_time, &err);
	if (err == OS_NO_ERR)
	{
		if ((8 == uart1_receive_num) && (modbus1_temp[0] == slave_addr) && (modbus1_temp[1] == 0x10)
			&& (modbus1_temp[2] == (uint8)(sub_addr >> 8)) && (modbus1_temp[3] == (uint8)sub_addr))
        {
        	if (1 == crc_detect(modbus1_temp, 8))
            {
                if ((modbus1_temp[4] == (uint8)(reg_num >> 8)) && (modbus1_temp[5] == (uint8)reg_num))
        		{
        		    return 1;
                }
                else 
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }  
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}



//******************************************************************************************************
//*                                              MOBUS2                                                *
//*                                           ====��վ===                                              *
//******************************************************************************************************
//��վѯ�ʴ�վ[03������]
//slave_addr: ��վ��ַ
//sub_addr��  ��վ�ļĴ����ӵ�ַ(��ʼ��ַ)
//reg_num:    ѯ�ʼĴ����ĸ���
void modbus2_ask03(uint8 slave_addr, uint16 sub_addr, uint16 reg_num)
{
    uint8  crc_temp_buf2[6] = {0};
    uint16 crcData = 0;
    uint8  crc_hi = 0, crc_lo = 0;
    uint8  i = 0;
        
    //slow_out(RS485DIR1, OUT);
    
    crc_temp_buf2[0] = slave_addr;
	crc_temp_buf2[1] = 0x03;
	crc_temp_buf2[2] = (uint8)(sub_addr >> 8);
	crc_temp_buf2[3] = (uint8)sub_addr;
	crc_temp_buf2[4] = (uint8)(reg_num >> 8);
	crc_temp_buf2[5] = (uint8)reg_num;
	
	crcData = crc16(crc_temp_buf2, 6);
    crc_hi = crcData >> 8;
    crc_lo = crcData & 0xff;
    
    for (i = 0; i < 6; i++)
    {
    	modbus2_send_byte(crc_temp_buf2[i]);
    }

    modbus2_send_byte(crc_hi);
    modbus2_send_byte(crc_lo);
}


//��վ���մ�վ[03������]
//����MODBUS��������������1��ʾ���³ɹ���0��ʾ���ݴ���û�и���,�ú��������Ѱ���OSDELAY
//slave_addr: ѯ�ʵĴ�վ��ַ
//reg_num:    ѯ�ʼĴ����ĸ���
//buf:        ���³ɹ��󷵻ص�modbus������
//out_time��  ��Ҫ�ȴ����ʱ��
uint8 modbus2_read03(uint8 slave_addr, uint16 sub_addr, uint16 reg_num, uint8 *buf, uint16 out_time)
{
    uint8  err = 0;
    uint8  i = 0;
    
    if ((sub_addr + reg_num) * 2 > MAX_BUFFER_SIZE2)
    {
        return 0;
    }
    
    //slow_out(RS485DIR2, IN);
    uart2_timeout_8byte_stop();
    
    OSSemPend(RecvUARTFlag2, out_time, &err);
	if (err == OS_NO_ERR)
	{
		if ((modbus2_temp[0] == slave_addr) && (modbus2_temp[1] == 0x03) && (modbus2_temp[2] == reg_num * 2))
        {
        	if (1 == crc_detect(modbus2_temp, reg_num*2 + 5))
            {
                for (i = 0; i < reg_num * 2; i++)
        		{
        		    buf[sub_addr * 2 + i] = modbus2_temp[i + 3];      //��ȡ���������ݴ�����ݵ�modbus���ڴ���
        		}
        		
        		return 1;
            }
            else
            {
                return 0;
            }
        }  
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}



//��վ���ô�վ[05������]
//slave_addr: ��վ��ַ
//sub_addr��  ��վ�ļĴ����ӵ�ַ
//reg_value:  ���üĴ�����ֵ(0xFF00 or 0x0000)
void modbus2_set05(uint8 slave_addr, uint16 sub_addr, uint16 reg_value)
{
    uint8  crc_temp_buf2[6] = {0};
    uint16 crcData = 0;
    uint8  crc_hi = 0, crc_lo = 0;
    uint8  i = 0;
	
    //slow_out(RS485DIR1, OUT);
    
    crc_temp_buf2[0] = slave_addr;
	crc_temp_buf2[1] = 0x05;
	crc_temp_buf2[2] = (uint8)(sub_addr >> 8);
	crc_temp_buf2[3] = (uint8)sub_addr;
	crc_temp_buf2[4] = (uint8)(reg_value >> 8);
	crc_temp_buf2[5] = (uint8)reg_value;
	
	crcData = crc16(crc_temp_buf2, 6);
    crc_hi = crcData >> 8;
    crc_lo = crcData & 0xff;
    
    for (i = 0; i < 6; i++)
    {
    	modbus2_send_byte(crc_temp_buf2[i]);
    }

    modbus2_send_byte(crc_hi);
    modbus2_send_byte(crc_lo);
}


//��վ�ظ���վ[05������]
//����MODBUS��������������1��ʾ���³ɹ���0��ʾ���ݴ���û�и���,�ú��������Ѱ���OSDELAY
//slave_addr: ��Ҫ���صĴ�վ��ַ�����ش�ֵ������ȷ
//sub_addr:   ��Ҫ���صĴ�վ�Ĵ�����ַ�����ش�ֵ������ȷ
//reg_value:  ��Ҫ���صļĴ���ֵ�����ش�ֵ������ȷ
//out_time��  ��Ҫ�ȴ����ʱ��
uint8 modbus2_read05(uint8 slave_addr, uint16 sub_addr, uint16 reg_value, uint16 out_time)
{
    uint8 err = 0;
    
    if (sub_addr * 2 > MAX_BUFFER_SIZE2)
    {
        return 0;
    }
    
    //slow_out(RS485DIR2, IN);
    uart2_timeout_8byte_stop();
        
    OSSemPend(RecvUARTFlag2, out_time, &err);
	if (err == OS_NO_ERR)
	{
		if ((8 == uart2_receive_num) && (modbus2_temp[0] == slave_addr) && (modbus2_temp[1] == 0x05)
			&& (modbus2_temp[2] == (uint8)(sub_addr >> 8)) && (modbus2_temp[3] == (uint8)sub_addr))
        {
        	if (1 == crc_detect(modbus2_temp, 8))
            {
                if ((modbus2_temp[4] == (uint8)(reg_value >> 8)) && (modbus2_temp[5] == (uint8)reg_value))
        		{
        		    return 1;
                }
                else 
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }  
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}


//��վ���ô�վ[06������]
//slave_addr: ��վ��ַ
//sub_addr��  ��վ�ļĴ����ӵ�ַ
//reg_value:  ���üĴ�����ֵ
void modbus2_set06(uint8 slave_addr, uint16 sub_addr, uint16 reg_value)
{
    uint8  crc_temp_buf2[6] = {0};
    uint16 crcData = 0;
    uint8  crc_hi = 0, crc_lo = 0;
    uint8  i = 0;
    
    //slow_out(RS485DIR1, OUT);
    
    crc_temp_buf2[0] = slave_addr;
	crc_temp_buf2[1] = 0x06;
	crc_temp_buf2[2] = (uint8)(sub_addr >> 8);
	crc_temp_buf2[3] = (uint8)sub_addr;
	crc_temp_buf2[4] = (uint8)(reg_value >> 8);
	crc_temp_buf2[5] = (uint8)reg_value;
	
	crcData = crc16(crc_temp_buf2, 6);
    crc_hi = crcData >> 8;
    crc_lo = crcData & 0xff;
    
    for (i = 0; i < 6; i++)
    {
    	modbus2_send_byte(crc_temp_buf2[i]);
    }

    modbus2_send_byte(crc_hi);
    modbus2_send_byte(crc_lo);
}

//��վ�ظ���վ[06������]
//����MODBUS��������������1��ʾ���³ɹ���0��ʾ���ݴ���û�и���,�ú��������Ѱ���OSDELAY
//slave_addr: ��Ҫ���صĴ�վ��ַ�����ش�ֵ������ȷ
//sub_addr:   ��Ҫ���صĴ�վ�Ĵ�����ַ�����ش�ֵ������ȷ
//reg_value:  ��Ҫ���صļĴ���ֵ�����ش�ֵ������ȷ
//out_time��  ��Ҫ�ȴ����ʱ��
uint8 modbus2_read06(uint8 slave_addr, uint16 sub_addr, uint16 reg_value, uint16 out_time)
{
    uint8 err = 0;
    
    if (sub_addr * 2 > MAX_BUFFER_SIZE2)
    {
        return 0;
    }
    
    //slow_out(RS485DIR2, IN);
    uart2_timeout_8byte_stop();
        
    OSSemPend(RecvUARTFlag2, out_time, &err);
	if (err == OS_NO_ERR)
	{
		if ((8 == uart2_receive_num) && (modbus2_temp[0] == slave_addr) && (modbus2_temp[1] == 0x06)
			&& (modbus2_temp[2] == (uint8)(sub_addr >> 8)) && (modbus2_temp[3] == (uint8)sub_addr))
        {
        	if (1 == crc_detect(modbus2_temp, 8))
            {
                if ((modbus2_temp[4] == (uint8)(reg_value >> 8)) && (modbus2_temp[5] == (uint8)reg_value))
        		{
        		    return 1;
                }
                else 
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }  
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}



//��վ���ô�վ[16������]
//slave_addr: ��վ��ַ
//sub_addr��  ��վ�ļĴ����ӵ�ַ
//reg_value:  ���üĴ�����ֵ
//reg_num:    ѯ�ʼĴ����ĸ���
//reg_value_buf:�������õļĴ�������
void modbus2_set16(uint8 slave_addr, uint16 sub_addr,uint16 reg_num, uint8 *reg_value_buf)
{
    uint8  crc_temp_buf2[MAX_BUFFER_SIZE2 + 7] = {0};
    uint16 crcData = 0;
    uint8  crc_hi = 0, crc_lo = 0;
    uint8  i = 0;
    
    //slow_out(RS485DIR1, OUT);
    
    crc_temp_buf2[0] = slave_addr;
	crc_temp_buf2[1] = 0x10;
	crc_temp_buf2[2] = (uint8)(sub_addr >> 8);
	crc_temp_buf2[3] = (uint8)sub_addr;
	crc_temp_buf2[4] = (uint8)(reg_num >> 8);
	crc_temp_buf2[5] = (uint8)reg_num;
	crc_temp_buf2[6] = (uint8)(reg_num * 2);
	
	for (i = 0; i < (reg_num * 2); i++)
	{
		crc_temp_buf2[i + 7] = *reg_value_buf++;
	}
	
	crcData = crc16(crc_temp_buf2, (reg_num * 2 + 7));
    crc_hi = crcData >> 8;
    crc_lo = crcData & 0xff;
    
    for (i = 0; i < (reg_num * 2 + 7); i++)
    {
    	modbus2_send_byte(crc_temp_buf2[i]);
    }

    modbus2_send_byte(crc_hi);
    modbus2_send_byte(crc_lo);
}

//��վ�ظ���վ[16������]
//����MODBUS��������������1��ʾ���³ɹ���0��ʾ���ݴ���û�и���,�ú��������Ѱ���OSDELAY
//slave_addr: ��Ҫ���صĴ�վ��ַ�����ش�ֵ������ȷ
//sub_addr:   ��Ҫ���صĴ�վ�Ĵ�����ַ�����ش�ֵ������ȷ
//reg_num:    ��Ҫ���صļĴ����ĸ��������ش�ֵ������ȷ
//out_time��  ��Ҫ�ȴ����ʱ��
uint8 modbus2_read16(uint8 slave_addr, uint16 sub_addr, uint16 reg_num, uint16 out_time)
{
    uint8 err = 0;
    
    if (sub_addr * 2 > MAX_BUFFER_SIZE2)
    {
        return 0;
    }
    
    //slow_out(RS485DIR2, IN);
    uart2_timeout_8byte_stop();
        
    OSSemPend(RecvUARTFlag2, out_time, &err);
	if (err == OS_NO_ERR)
	{
		if ((8 == uart2_receive_num) && (modbus2_temp[0] == slave_addr) && (modbus2_temp[1] == 0x10)
			&& (modbus2_temp[2] == (uint8)(sub_addr >> 8)) && (modbus2_temp[3] == (uint8)sub_addr))
        {
        	if (1 == crc_detect(modbus2_temp, 8))
            {
                if ((modbus2_temp[4] == (uint8)(reg_num >> 8)) && (modbus2_temp[5] == (uint8)reg_num))
        		{
        		    return 1;
                }
                else 
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }  
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}



//******************************************************************************************************
//*                                              MOBUS3                                                *
//*                                           ====��վ===                                              *
//******************************************************************************************************
//��վѯ�ʴ�վ[03������]
//slave_addr: ��վ��ַ
//sub_addr��  ��վ�ļĴ����ӵ�ַ(��ʼ��ַ)
//reg_num:    ѯ�ʼĴ����ĸ���
void modbus3_ask03(uint8 slave_addr, uint16 sub_addr, uint16 reg_num)
{
    uint8  crc_temp_buf3[6] = {0};
    uint16 crcData = 0;
    uint8  crc_hi = 0, crc_lo = 0;
    uint8  i = 0;
        
    //slow_out(RS485DIR3, OUT);
    
    crc_temp_buf3[0] = slave_addr;
	crc_temp_buf3[1] = 0x03;
	crc_temp_buf3[2] = (uint8)(sub_addr >> 8);
	crc_temp_buf3[3] = (uint8)sub_addr;
	crc_temp_buf3[4] = (uint8)(reg_num >> 8);
	crc_temp_buf3[5] = (uint8)reg_num;
	
	crcData = crc16(crc_temp_buf3, 6);
    crc_hi = crcData >> 8;
    crc_lo = crcData & 0xff;
    
    for (i = 0; i < 6; i++)
    {
    	modbus3_send_byte(crc_temp_buf3[i]);
    }

    modbus3_send_byte(crc_hi);
    modbus3_send_byte(crc_lo);
}


//��վ���մ�վ[03������]
//����MODBUS��������������1��ʾ���³ɹ���0��ʾ���ݴ���û�и���,�ú��������Ѱ���OSDELAY
//slave_addr: ѯ�ʵĴ�վ��ַ
//reg_num:    ѯ�ʼĴ����ĸ���
//buf:        ���³ɹ��󷵻ص�modbus������
//out_time��  ��Ҫ�ȴ����ʱ��
uint8 modbus3_read03(uint8 slave_addr, uint16 sub_addr, uint16 reg_num, uint8 *buf, uint16 out_time)
{
    uint8  err = 0;
    uint8  i = 0;
    
    if ((sub_addr + reg_num) * 2 > MAX_BUFFER_SIZE3)
    {
        return 0;
    }
    
    //slow_out(RS485DIR3, IN);
    uart3_timeout_8byte_stop();
    
    OSSemPend(RecvUARTFlag3, out_time, &err);
	if (err == OS_NO_ERR)
	{
		if ((modbus3_temp[0] == slave_addr) && (modbus3_temp[1] == 0x03) && (modbus3_temp[2] == reg_num * 2))
        {
        	if (1 == crc_detect(modbus3_temp, reg_num*2 + 5))
            {
                for (i = 0; i < reg_num * 2; i++)
        		{
        		    buf[sub_addr * 2 + i] = modbus3_temp[i + 3];      //��ȡ���������ݴ�����ݵ�modbus���ڴ���
        		}
        		
        		return 1;
            }
            else
            {
                return 0;
            }
        }  
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}



//��վ���ô�վ[05������]
//slave_addr: ��վ��ַ
//sub_addr��  ��վ�ļĴ����ӵ�ַ
//reg_value:  ���üĴ�����ֵ(0xFF00 or 0x0000)
void modbus3_set05(uint8 slave_addr, uint16 sub_addr, uint16 reg_value)
{
    uint8  crc_temp_buf3[6] = {0};
    uint16 crcData = 0;
    uint8  crc_hi = 0, crc_lo = 0;
    uint8  i = 0;
	
    //slow_out(RS485DIR3, OUT);
    
    crc_temp_buf3[0] = slave_addr;
	crc_temp_buf3[1] = 0x05;
	crc_temp_buf3[2] = (uint8)(sub_addr >> 8);
	crc_temp_buf3[3] = (uint8)sub_addr;
	crc_temp_buf3[4] = (uint8)(reg_value >> 8);
	crc_temp_buf3[5] = (uint8)reg_value;
	
	crcData = crc16(crc_temp_buf3, 6);
    crc_hi = crcData >> 8;
    crc_lo = crcData & 0xff;
    
    for (i = 0; i < 6; i++)
    {
    	modbus3_send_byte(crc_temp_buf3[i]);
    }

    modbus3_send_byte(crc_hi);
    modbus3_send_byte(crc_lo);
}


//��վ�ظ���վ[05������]
//����MODBUS��������������1��ʾ���³ɹ���0��ʾ���ݴ���û�и���,�ú��������Ѱ���OSDELAY
//slave_addr: ��Ҫ���صĴ�վ��ַ�����ش�ֵ������ȷ
//sub_addr:   ��Ҫ���صĴ�վ�Ĵ�����ַ�����ش�ֵ������ȷ
//reg_value:  ��Ҫ���صļĴ���ֵ�����ش�ֵ������ȷ
//out_time��  ��Ҫ�ȴ����ʱ��
uint8 modbus3_read05(uint8 slave_addr, uint16 sub_addr, uint16 reg_value, uint16 out_time)
{
    uint8 err = 0;
    
    if (sub_addr * 2 > MAX_BUFFER_SIZE3)
    {
        return 0;
    }
    
    //slow_out(RS485DIR3, IN);
    uart3_timeout_8byte_stop();
        
    OSSemPend(RecvUARTFlag3, out_time, &err);
	if (err == OS_NO_ERR)
	{
		if ((8 == uart3_receive_num) && (modbus3_temp[0] == slave_addr) && (modbus3_temp[1] == 0x05)
			&& (modbus3_temp[2] == (uint8)(sub_addr >> 8)) && (modbus3_temp[3] == (uint8)sub_addr))
        {
        	if (1 == crc_detect(modbus3_temp, 8))
            {
                if ((modbus3_temp[4] == (uint8)(reg_value >> 8)) && (modbus3_temp[5] == (uint8)reg_value))
        		{
        		    return 1;
                }
                else 
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }  
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}


//��վ���ô�վ[06������]
//slave_addr: ��վ��ַ
//sub_addr��  ��վ�ļĴ����ӵ�ַ
//reg_value:  ���üĴ�����ֵ
void modbus3_set06(uint8 slave_addr, uint16 sub_addr, uint16 reg_value)
{
    uint8  crc_temp_buf3[6] = {0};
    uint16 crcData = 0;
    uint8  crc_hi = 0, crc_lo = 0;
    uint8  i = 0;
    
    //slow_out(RS485DIR3, OUT);
    
    crc_temp_buf3[0] = slave_addr;
	crc_temp_buf3[1] = 0x06;
	crc_temp_buf3[2] = (uint8)(sub_addr >> 8);
	crc_temp_buf3[3] = (uint8)sub_addr;
	crc_temp_buf3[4] = (uint8)(reg_value >> 8);
	crc_temp_buf3[5] = (uint8)reg_value;
	
	crcData = crc16(crc_temp_buf3, 6);
    crc_hi = crcData >> 8;
    crc_lo = crcData & 0xff;
    
    for (i = 0; i < 6; i++)
    {
    	modbus3_send_byte(crc_temp_buf3[i]);
    }
	
    modbus3_send_byte(crc_hi);
    modbus3_send_byte(crc_lo);
}

//��վ�ظ���վ[06������]
//����MODBUS��������������1��ʾ���³ɹ���0��ʾ���ݴ���û�и���,�ú��������Ѱ���OSDELAY
//slave_addr: ��Ҫ���صĴ�վ��ַ�����ش�ֵ������ȷ
//sub_addr:   ��Ҫ���صĴ�վ�Ĵ�����ַ�����ش�ֵ������ȷ
//reg_value:  ��Ҫ���صļĴ���ֵ�����ش�ֵ������ȷ
//out_time��  ��Ҫ�ȴ����ʱ��
uint8 modbus3_read06(uint8 slave_addr, uint16 sub_addr, uint16 reg_value, uint16 out_time)
{
    uint8 err = 0;
    
    if (sub_addr * 2 > MAX_BUFFER_SIZE3)
    {
        return 0;
    }
    
    //slow_out(RS485DIR3, IN);
    uart3_timeout_8byte_stop();
    
    OSSemPend(RecvUARTFlag3, out_time, &err);
	if (err == OS_NO_ERR)
	{
		if ((8 == uart3_receive_num) && (modbus3_temp[0] == slave_addr) && (modbus3_temp[1] == 0x06)
			&& (modbus3_temp[2] == (uint8)(sub_addr >> 8)) && (modbus3_temp[3] == (uint8)sub_addr))
        {
        	if (1 == crc_detect(modbus3_temp, 8))
            {
                if ((modbus3_temp[4] == (uint8)(reg_value >> 8)) && (modbus3_temp[5] == (uint8)reg_value))
        		{
        		    return 1;
                }
                else 
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }  
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}



//��վ���ô�վ[16������]
//slave_addr: ��վ��ַ
//sub_addr��  ��վ�ļĴ����ӵ�ַ
//reg_value:  ���üĴ�����ֵ
//reg_num:    ѯ�ʼĴ����ĸ���
//reg_value_buf:�������õļĴ�������
void modbus3_set16(uint8 slave_addr, uint16 sub_addr,uint16 reg_num, uint8 *reg_value_buf)
{
    uint8  crc_temp_buf3[MAX_BUFFER_SIZE3 + 7] = {0};
    uint16 crcData = 0;
    uint8  crc_hi = 0, crc_lo = 0;
    uint8  i = 0;
    
    //slow_out(RS485DIR3, OUT);
    
    crc_temp_buf3[0] = slave_addr;
	crc_temp_buf3[1] = 0x10;
	crc_temp_buf3[2] = (uint8)(sub_addr >> 8);
	crc_temp_buf3[3] = (uint8)sub_addr;
	crc_temp_buf3[4] = (uint8)(reg_num >> 8);
	crc_temp_buf3[5] = (uint8)reg_num;
	crc_temp_buf3[6] = (uint8)(reg_num * 2);
	
	for (i = 0; i < (reg_num * 2); i++)
	{
		crc_temp_buf3[i + 7] = *reg_value_buf++;
	}
	
	crcData = crc16(crc_temp_buf3, (reg_num * 2 + 7));
    crc_hi = crcData >> 8;
    crc_lo = crcData & 0xff;
    
    for (i = 0; i < (reg_num * 2 + 7); i++)
    {
    	modbus3_send_byte(crc_temp_buf3[i]);
    }

    modbus3_send_byte(crc_hi);
    modbus3_send_byte(crc_lo);
}

//��վ�ظ���վ[16������]
//����MODBUS��������������1��ʾ���³ɹ���0��ʾ���ݴ���û�и���,�ú��������Ѱ���OSDELAY
//slave_addr: ��Ҫ���صĴ�վ��ַ�����ش�ֵ������ȷ
//sub_addr:   ��Ҫ���صĴ�վ�Ĵ�����ַ�����ش�ֵ������ȷ
//reg_num:    ��Ҫ���صļĴ����ĸ��������ش�ֵ������ȷ
//out_time��  ��Ҫ�ȴ����ʱ��
uint8 modbus3_read16(uint8 slave_addr, uint16 sub_addr, uint16 reg_num, uint16 out_time)
{
    uint8 err = 0;
    
    if (sub_addr * 2 > MAX_BUFFER_SIZE3)
    {
        return 0;
    }
    
    //slow_out(RS485DIR3, IN);
    uart3_timeout_8byte_stop();
        
    OSSemPend(RecvUARTFlag3, out_time, &err);
	if (err == OS_NO_ERR)
	{
		if ((8 == uart3_receive_num) && (modbus3_temp[0] == slave_addr) && (modbus3_temp[1] == 0x10)
			&& (modbus3_temp[2] == (uint8)(sub_addr >> 8)) && (modbus3_temp[3] == (uint8)sub_addr))
        {
        	if (1 == crc_detect(modbus3_temp, 8))
            {
                if ((modbus3_temp[4] == (uint8)(reg_num >> 8)) && (modbus3_temp[5] == (uint8)reg_num))
        		{
        		    return 1;
                }
                else 
                {
                    return 0;
                }
            }
            else
            {
                return 0;
            }
        }  
        else
        {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
