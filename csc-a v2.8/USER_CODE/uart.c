#include "uart.h"
#include "main.h"


extern OS_EVENT  *RecvUARTFlag0;  
extern OS_EVENT  *RecvUARTFlag1;  
extern OS_EVENT  *RecvUARTFlag2;  
extern OS_EVENT  *RecvUARTFlag3;


uint16 uart0_receive_num;				// 
uint16 uart1_receive_num;				// 
uint16 uart2_receive_num;				// 
uint16 uart3_receive_num;				// 




uint32 send_time_out0 = 6000;
uint32 send_time_out1 = 6000;
uint32 send_time_out2 = 6000;
uint32 send_time_out3 = 6000;


// UART0��ʱ���ձ���
uint8  uart0_timeout_start = 0;			        // 8�ֽں�ĳ�ʱ�жϼ�����������־
uint8  uart0_timeout_cnt = 0;			        // 8�ֽڳ�ʱ������
uint8  uart0_temp[MAX_UART0_BYTE_NUM] = {0};    // UART0���ջ�����
uint16 uart0_rcv_counter = 0;			        // UART0�յ����ݸ�������ֵ


// UART1��ʱ���ձ���
uint8  uart1_timeout_start = 0;			        // 8�ֽں�ĳ�ʱ�жϼ�����������־
uint8  uart1_timeout_cnt = 0;			        // 8�ֽڳ�ʱ������
uint8  uart1_temp[MAX_UART1_BYTE_NUM] = {0};    // UART1���ջ�����
uint16 uart1_rcv_counter = 0;			        // UART1�յ����ݸ�������ֵ

// UART2��ʱ���ձ���
uint8  uart2_timeout_start = 0;			        // 8�ֽں�ĳ�ʱ�жϼ�����������־
uint8  uart2_timeout_cnt = 0;			        // 8�ֽڳ�ʱ������
uint8  uart2_temp[MAX_UART2_BYTE_NUM] = {0};    // UART2���ջ�����
uint16 uart2_rcv_counter = 0;			        // UART2�յ����ݸ�������ֵ

// UART3��ʱ���ձ���
uint8  uart3_timeout_start = 0;			        // 8�ֽں�ĳ�ʱ�жϼ�����������־
uint8  uart3_timeout_cnt = 0;			        // 8�ֽڳ�ʱ������
uint8  uart3_temp[MAX_UART3_BYTE_NUM] = {0};    // UART3���ջ�����
uint16 uart3_rcv_counter = 0;			        // UART3�յ����ݸ�������ֵ



// ����uart0��8�ֽڳ�ʱ�ж϶�ʱ��
void uart0_timeout_8byte_start(void)
{
	uart0_timeout_start = 1;
	uart0_timeout_cnt = 0;  
}

// ֹͣuart0��8�ֽڳ�ʱ�ж϶�ʱ��
void uart0_timeout_8byte_stop(void)
{
    uart0_timeout_start = 0;
	uart0_timeout_cnt = 0; 
	uart0_rcv_counter = 0;   
}


// ����uart1��8�ֽڳ�ʱ�ж϶�ʱ��
void uart1_timeout_8byte_start(void)
{
	uart1_timeout_start = 1;
	uart1_timeout_cnt = 0;  
}

// ֹͣuart1��8�ֽڳ�ʱ�ж϶�ʱ��
void uart1_timeout_8byte_stop(void)
{
    uart1_timeout_start = 0;
	uart1_timeout_cnt = 0; 
	uart1_rcv_counter = 0;   
}


// ����uart2��8�ֽڳ�ʱ�ж϶�ʱ��
void uart2_timeout_8byte_start(void)
{
	uart2_timeout_start = 1;
	uart2_timeout_cnt = 0;  
}

// ֹͣuart2��8�ֽڳ�ʱ�ж϶�ʱ��
void uart2_timeout_8byte_stop(void)
{
    uart2_timeout_start = 0;
	uart2_timeout_cnt = 0; 
	uart2_rcv_counter = 0;   
}


// ����uart3��8�ֽڳ�ʱ�ж϶�ʱ��
void uart3_timeout_8byte_start(void)
{
	uart3_timeout_start = 1;
	uart3_timeout_cnt = 0;  
}

// ֹͣuart3��8�ֽڳ�ʱ�ж϶�ʱ��
void uart3_timeout_8byte_stop(void)
{
    uart3_timeout_start = 0;
	uart3_timeout_cnt = 0; 
	uart3_rcv_counter = 0;   
}


// ��ʱ����
void TIME_PROCESS(void)
{
// 12�ֽ�(8�ֽ�Ϊ����������Ҫʱ�䣬���4����ʱʱ��)
// �˴�����Ҫ�Ӵ�160����140��ʱ��7���ֽ��жϿ��ܻ�����Ϊ�˴���ʱ��������������ĸ��ʼ�С��
// 8�ֽ���ȴ���
	uint16 uart0_timeout_dly = OS_TICKS_PER_SEC * 120 / UART0_BAUD_RATE + 1;	//12�ֽ�(8�ֽ�Ϊ����������Ҫʱ�䣬���4����ʱʱ��)
	uint16 uart1_timeout_dly = OS_TICKS_PER_SEC * 120 / UART1_BAUD_RATE + 1;	//12�ֽ�(8�ֽ�Ϊ����������Ҫʱ�䣬���4����ʱʱ��)
	uint16 uart2_timeout_dly = OS_TICKS_PER_SEC * 120 / UART2_BAUD_RATE + 1;	//12�ֽ�(8�ֽ�Ϊ����������Ҫʱ�䣬���4����ʱʱ��)
	uint16 uart3_timeout_dly = OS_TICKS_PER_SEC * 120 / UART3_BAUD_RATE + 1;	//12�ֽ�(8�ֽ�Ϊ����������Ҫʱ�䣬���4����ʱʱ��)

	while (1)
	{
#ifdef UART0_RCVBUF_N
		if(uart0_timeout_start)
        {
            uart0_timeout_cnt++;
            
            if(uart0_timeout_cnt > uart0_timeout_dly)				// �жϳ�ʱ
            {
                uart0_receive_num = uart0_rcv_counter;
                
                uart0_timeout_8byte_stop();
                
                OSSemPost(RecvUARTFlag0);							// 8�ֽڳ�ʱ�ж��ź�������
            }
        }
#endif
		
#ifdef UART1_RCVBUF_N
		if(uart1_timeout_start)
        {
            uart1_timeout_cnt++;
            
            if(uart1_timeout_cnt > uart1_timeout_dly)				// �жϳ�ʱ
            {
                uart1_receive_num = uart1_rcv_counter;
                
                uart1_timeout_8byte_stop();
                
                OSSemPost(RecvUARTFlag1);							// 8�ֽڳ�ʱ�ж��ź�������
            }
        }
#endif

#ifdef UART2_RCVBUF_N
        if(uart2_timeout_start)
        {
            uart2_timeout_cnt++;
            
            if(uart2_timeout_cnt > uart2_timeout_dly)				// �жϳ�ʱ
            {
                uart2_receive_num = uart2_rcv_counter;
                
                uart2_timeout_8byte_stop();
                
                OSSemPost(RecvUARTFlag2);							// 8�ֽڳ�ʱ�ж��ź�������
            }
        }
#endif

#ifdef UART3_RCVBUF_N
		if(uart3_timeout_start)
        {
            uart3_timeout_cnt++;
            
            if(uart3_timeout_cnt > uart3_timeout_dly)				// �жϳ�ʱ
            {
                uart3_receive_num = uart3_rcv_counter;
                
                uart3_timeout_8byte_stop();
                
                OSSemPost(RecvUARTFlag3);							// 8�ֽڳ�ʱ�ж��ź�������
            }
        }
#endif

		OSTimeDly(1);
	} 
}



//******************************************************************************************************
//*                                              UART0                                                 *
//******************************************************************************************************
// UART0��ʼ��
void UART0_Init(uint32 Baud_Rate, uint8 channel)		//channel = 0(P0.2, P0.3)
{
	uint16 Fdiv;
	LPC_SC->PCONP |= (1 << 3);							//����UART0����(���ʿ���PCONP)

	// ��ʼ����ص�IO
	if(channel == 0)
	{
	    LPC_PINCON->PINSEL0 &= 0xffffff0f;
	    LPC_PINCON->PINSEL0 |= (0x01 << 4) | (0x01 << 6);// ����P0.2��P0.3���ӵ�UART0
	}
	else
	{
	    return;
	}
	
	send_time_out0 = (uint32)(100000000.0 / Baud_Rate);
	
	LPC_UART0->LCR = 0x83;								// �ֳ�Ϊ8��1λֹͣ������żУ�飬��ֹ������ͣ��������ò�����
	Fdiv  = (SystemFrequency / 4 / 16) / Baud_Rate;		// ���ò�����
	
	LPC_UART0->DLM  = Fdiv / 256;
	LPC_UART0->DLL  = Fdiv % 256;
	LPC_UART0->LCR  = 0x03;								// ������ɣ���DLABλΪ0������������
	LPC_UART0->FCR  = 0x87;								// ʹ��FIFO������8���ֽڴ������,Rx FIFO��λ,Tx FIFO��λ-----0x81 for 2368

	NVIC_EnableIRQ(UART0_IRQn);							// NVIC����
	NVIC_SetPriority(UART0_IRQn, INT_UART0_P);

	LPC_UART0->IER  = 0x01;								// ����������ݿ����ж�(RDA�ж�)���ַ���ʱ�ж�(CTI�ж�),����������ж�
}


// �ֽڷ���
void UART0_SendByte(uint8 dat)
{
	uint32 time_out_counter = send_time_out0;
	
	LPC_UART0->THR = dat;
	while (((LPC_UART0->LSR & 0x40) == 0) && (time_out_counter))// �ȴ�����FIFO��UnTSR��Ϊ�գ����ݷ������
	{
	    time_out_counter--;
	}
}


// �ַ�������
//void UART0_SendStr(unsigned char *pucStr,unsigned int ulNum)	//���ͺ�����Ҫ��������ĵ�һ��Ԫ�ص�ַ�ͷ��͵��ֽ���
void UART0_SendStr(unsigned char str[],unsigned int ulNum)		//���ͺ�����Ҫ��������ĵ�һ��Ԫ�ص�ַ�ͷ��͵��ֽ���
{
	unsigned int i;

	for(i =0; i < ulNum; i++)
	{ 
		//UART0_SendByte(*pucStr++);
		UART0_SendByte(str[i]);		//�ֽڷ���
	}
}


// UART0�жϷ������
void  UART0_IRQHandler(void)				// UART0�жϺ���
{
	uint8 i;
	
	OSIntEnter();

	if ((LPC_UART0->IIR & 0x0F) == 0x04)		// �Ƿ�����ж�(RDA�ж�)
	{
		for (i = 0; i < 8; i++)										// ��������8���ֽ�
		{
			if (i + uart0_rcv_counter >= MAX_UART0_BYTE_NUM)		// ��ֹ�������
		    {
		        uart0_timeout_8byte_stop();
		        
		        break;  
		    }
			else
			{
			    uart0_temp[i + uart0_rcv_counter] = LPC_UART0->RBR;	// ��ȡFIFO�����ݣ�������ж�
		    }
		}
    	
    	uart0_rcv_counter += 8;
		
		uart0_timeout_8byte_start();
	}
	
	if ((LPC_UART0->IIR & 0x0F) == 0x0c)		// �Ƿ�ʱ�ж�(CTI�ж�)
	{
		while ((LPC_UART0->LSR & 0x01) == 1)						// �Ƿ���δ���������
		{
			if (uart0_rcv_counter >= MAX_UART0_BYTE_NUM)			// ��ֹ�������
		    {
		        uart0_timeout_8byte_stop();	  
		        
		        break;  
		    }
			else
			{
    			uart0_temp[uart0_rcv_counter] = LPC_UART0->RBR;		// ��ȡFIFO�����ݣ�������ж�
    			uart0_rcv_counter++;
    	    }  
		}	
		
		uart0_receive_num = uart0_rcv_counter;
		
		uart0_timeout_8byte_stop();	
		
		OSSemPost(RecvUARTFlag0);									// ��8�ֽڳ�ʱ�ж��ź�������
    }
	
	OSIntExit();
}



//******************************************************************************************************
//*                                              UART1                                                 *
//******************************************************************************************************
// UART1��ʼ��
void UART1_Init(uint32 Baud_Rate, uint8 channel)		//channel = 0(P0.15, P0.16), channel = 1(P2.0, P2.1)
{
	uint16 Fdiv;
	LPC_SC->PCONP |= (1 << 4);							//����UART1����(���ʿ���PCONP)
	
	// ��ʼ����ص�IO
	if (channel == 0)
	{
    	LPC_PINCON->PINSEL0 &= 0x3fffffff;				//P0.15, P0.16
	    LPC_PINCON->PINSEL0 |= 0x40000000;
	    
	    LPC_PINCON->PINSEL1 &= 0xfffffffc;
	    LPC_PINCON->PINSEL1 |= 0x00000001;
	}
	else if (channel == 1)
	{
	    LPC_PINCON->PINSEL4 &= 0xfffffff0;				//P2.0, P2.1
    	LPC_PINCON->PINSEL4 |= (0x02 << 0) | (0x02 << 2);	    
	}
	else
	{
	    return; //ʧ��    
	}

	send_time_out1 = (uint32)(100000000.0 / Baud_Rate);
	
	LPC_UART1->LCR = 0x83;								// �ֳ�Ϊ8��1λֹͣ������żУ�飬��ֹ������ͣ��������ò�����
	Fdiv  = (SystemFrequency / 4 / 16) / Baud_Rate;		// ���ò�����
	
	LPC_UART1->DLM  = Fdiv / 256;
	LPC_UART1->DLL  = Fdiv % 256;
	LPC_UART1->LCR  = 0x03;								// ������ɣ�����������
	LPC_UART1->FCR  = 0x87;								// ʹ��FIFO������8���ֽڴ������,Rx FIFO��λ,Tx FIFO��λ

	NVIC_EnableIRQ(UART1_IRQn);							// NVIC����
	NVIC_SetPriority(UART1_IRQn, INT_UART1_P);

	LPC_UART1->IER  = 0x01;								// ����������ݿ����ж�(RDA�ж�)���ַ���ʱ�ж�(CTI�ж�),����������ж�
}


// �ֽڷ���
void UART1_SendByte(uint8 dat)
{
	uint32 time_out_counter = send_time_out1;
	
	LPC_UART1->THR = dat;
	while (((LPC_UART1->LSR & 0x40) == 0) && (time_out_counter))// �ȴ�����FIFO��UnTSR��Ϊ�գ����ݷ������	
	{
	    time_out_counter--;
	}
}


// �ַ�������
//void UART1_SendStr(unsigned char *pucStr,unsigned int ulNum)	//���ͺ�����Ҫ��������ĵ�һ��Ԫ�ص�ַ�ͷ��͵��ֽ���
void UART1_SendStr(unsigned char str[],unsigned int ulNum)		//���ͺ�����Ҫ��������ĵ�һ��Ԫ�ص�ַ�ͷ��͵��ֽ���
{
	unsigned int i;

	for(i =0; i < ulNum; i++)
	{ 
		//UART1_SendByte(*pucStr++);
		UART1_SendByte(str[i]);		//�ֽڷ���
	}
}


// UART1�жϷ������
void  UART1_IRQHandler(void)				// UART0�жϺ���
{
	uint8 i;
	
	OSIntEnter();
	
	if ((LPC_UART1->IIR & 0x0F) == 0x04)		// �Ƿ�����ж�(RDA�ж�)
	{
		for (i = 0; i < 8; i++)										// ��������8���ֽ�
		{
			if (i + uart1_rcv_counter >= MAX_UART1_BYTE_NUM)		// ��ֹ�������
		    {
		        uart1_timeout_8byte_stop();
		        
		        break;  
		    }
			else
			{
			    uart1_temp[i + uart1_rcv_counter] = LPC_UART1->RBR;	// ��ȡFIFO�����ݣ�������ж�
		    }
		}
    	
    	uart1_rcv_counter += 8;
		
		uart1_timeout_8byte_start();
	}
	
	if ((LPC_UART1->IIR & 0x0F) == 0x0c)		// �Ƿ�ʱ�ж�(CTI�ж�)
	{
		while ((LPC_UART1->LSR & 0x01) == 1)						// �Ƿ���δ���������
		{
			if (uart1_rcv_counter >= MAX_UART1_BYTE_NUM)			// ��ֹ�������
		    {
		        uart1_timeout_8byte_stop();	  
		        
		        break;  
		    }
			else
			{
    			uart1_temp[uart1_rcv_counter] = LPC_UART1->RBR;		// ��ȡFIFO�����ݣ�������ж�
    			uart1_rcv_counter++;
    	    }  
		}	
		
		uart1_receive_num = uart1_rcv_counter;
		
		uart1_timeout_8byte_stop();	
		
		OSSemPost(RecvUARTFlag1);									// ��8�ֽڳ�ʱ�ж��ź�������
    }
	
	OSIntExit();
}



//******************************************************************************************************
//*                                              UART2                                                 *
//******************************************************************************************************
// UART2��ʼ��
void UART2_Init(uint32 Baud_Rate, uint8 channel)//channel = 0(P0.10, P0.11), channel = 1(P2.8, P2.9)
{
	uint16 Fdiv;
	LPC_SC->PCONP |= (1 << 24);							//����UART2����(���ʿ���PCONP)
	
	// ��ʼ����ص�IO
	if(channel == 0)
	{
	    LPC_PINCON->PINSEL0 &= 0xff0fffff;				//P0.10, P0.11
	    LPC_PINCON->PINSEL0 |= 0x00500000;	                
	}
	else if(channel == 1)
	{
	    LPC_PINCON->PINSEL4 &= 0xfff0ffff;				//P2.8, P2.9
	    LPC_PINCON->PINSEL4 |= (0x02 << 16) | (0x02 << 18);	
	}
	else
	{
	    return;
	}
	
	send_time_out2 = (uint32)(100000000.0 / Baud_Rate);
	
	LPC_UART2->LCR = 0x83;								// �ֳ�Ϊ8��1λֹͣ������żУ�飬��ֹ������ͣ��������ò�����
	Fdiv  = (SystemFrequency / 4 / 16) / Baud_Rate;		// ���ò�����
	
	LPC_UART2->DLM  = Fdiv / 256;
	LPC_UART2->DLL  = Fdiv % 256;
	LPC_UART2->LCR  = 0x03;								// ������ɣ�����������
	LPC_UART2->FCR  = 0x87;								// ʹ��FIFO������8���ֽڴ������,Rx FIFO��λ,Tx FIFO��λ

	NVIC_EnableIRQ(UART2_IRQn);							// NVIC����
	NVIC_SetPriority(UART2_IRQn, INT_UART2_P);

	LPC_UART2->IER  = 0x01;								// ����������ݿ����ж�(RDA�ж�)���ַ���ʱ�ж�(CTI�ж�),����������ж�
}


// �ֽڷ���
void UART2_SendByte(uint8 dat)
{
	uint32 time_out_counter = send_time_out2;
	
	LPC_UART2->THR = dat;
	while (((LPC_UART2->LSR & 0x40) == 0) && (time_out_counter))// �ȴ�����FIFO��UnTSR��Ϊ�գ����ݷ������	
	{
	    time_out_counter--;
	}
}


// �ַ�������
//void UART2_SendStr(unsigned char *pucStr,unsigned int ulNum)	//���ͺ�����Ҫ��������ĵ�һ��Ԫ�ص�ַ�ͷ��͵��ֽ���
void UART2_SendStr(unsigned char str[],unsigned int ulNum)		//���ͺ�����Ҫ��������ĵ�һ��Ԫ�ص�ַ�ͷ��͵��ֽ���
{
	unsigned int i;

	for(i =0; i < ulNum; i++)
	{ 
		//UART2_SendByte(*pucStr++);
		UART2_SendByte(str[i]);				//�ֽڷ���
	}
}


// UART2�жϷ������
void  UART2_IRQHandler(void)				// UART0�жϺ���
{
	uint8 i;
	
	OSIntEnter();

	if ((LPC_UART2->IIR & 0x0F) == 0x04)		// �Ƿ�����ж�(RDA�ж�)
	{
		for (i = 0; i < 8; i++)										// ��������8���ֽ�
		{
			if (i + uart2_rcv_counter >= MAX_UART2_BYTE_NUM)		// ��ֹ�������
		    {
		        uart2_timeout_8byte_stop();
		        
		        break;  
		    }
			else
			{
			    uart2_temp[i + uart2_rcv_counter] = LPC_UART2->RBR;	// ��ȡFIFO�����ݣ�������ж�
		    }
		}
    	
    	uart2_rcv_counter += 8;
		
		uart2_timeout_8byte_start();
	}
	
	if ((LPC_UART2->IIR & 0x0F) == 0x0c)		// �Ƿ�ʱ�ж�(CTI�ж�)
	{
		while ((LPC_UART2->LSR & 0x01) == 1)						// �Ƿ���δ���������
		{
			if (uart2_rcv_counter >= MAX_UART2_BYTE_NUM)			// ��ֹ�������
		    {
		        uart2_timeout_8byte_stop();	  
		        
		        break;  
		    }
			else
			{
    			uart2_temp[uart2_rcv_counter] = LPC_UART2->RBR;		// ��ȡFIFO�����ݣ�������ж�
    			uart2_rcv_counter++;
    	    }  
		}	
		
		uart2_receive_num = uart2_rcv_counter;
		
		uart2_timeout_8byte_stop();	
		
		OSSemPost(RecvUARTFlag2);									// ��8�ֽڳ�ʱ�ж��ź�������
    }
	
	OSIntExit();
}



//******************************************************************************************************
//*                                              UART3                                                 *
//******************************************************************************************************
// UART3��ʼ��
void UART3_Init(uint32 Baud_Rate, uint8 channel)		//channel = 0(P0.0, P0.1),   channel = 1(P0.25, P0.26), channel = 2(P4.28, P4.29)
{
	uint16 Fdiv;
	LPC_SC->PCONP |= (1 << 25);							//����UART3����(���ʿ���PCONP)
	
	// ��ʼ����ص�IO
	if(channel == 0)
	{
	    LPC_PINCON->PINSEL0 &= 0xfffffff0;				//P0.0, P0.1 
	    LPC_PINCON->PINSEL0 |= (0x02 << 0) | (0x02 << 2);	
	}
	else if(channel == 1)
	{
	    LPC_PINCON->PINSEL1 |= 0x003c0000;				//P0.25, P0.26    
	}
	else if(channel == 2)
	{
	   LPC_PINCON->PINSEL9 |= 0x0f000000;				//P4.28, P4.29
	}
	else
	{
	    return;    
	}

	send_time_out3 = (uint32)(100000000.0 / Baud_Rate);
	
	LPC_UART3->LCR = 0x83;								// �ֳ�Ϊ8��1λֹͣ������żУ�飬��ֹ������ͣ��������ò�����
	Fdiv  = (SystemFrequency / 4 / 16) / Baud_Rate;		// ���ò�����
	
	LPC_UART3->DLM  = Fdiv / 256;
	LPC_UART3->DLL  = Fdiv % 256;
	LPC_UART3->LCR  = 0x03;								// ������ɣ�����������
	LPC_UART3->FCR  = 0x87;								// ʹ��FIFO������8���ֽڴ������,Rx FIFO��λ,Tx FIFO��λ

	NVIC_EnableIRQ(UART3_IRQn);							// NVIC����
	NVIC_SetPriority(UART3_IRQn, INT_UART3_P);

	LPC_UART3->IER  = 0x01;								// ����������ݿ����ж�(RDA�ж�)���ַ���ʱ�ж�(CTI�ж�),����������ж�
}


// �ֽڷ���
void UART3_SendByte(uint8 dat)
{
	uint32 time_out_counter = send_time_out3;
	
	LPC_UART3->THR = dat;
	while (((LPC_UART3->LSR & 0x40) == 0) && (time_out_counter))// �ȴ�����FIFO��UnTSR��Ϊ�գ����ݷ������	
	{
	    time_out_counter--;
	}
}


// �ַ�������
//void UART3_SendStr(unsigned char *pucStr,unsigned int ulNum)	//���ͺ�����Ҫ��������ĵ�һ��Ԫ�ص�ַ�ͷ��͵��ֽ���
void UART3_SendStr(unsigned char str[],unsigned int ulNum)		//���ͺ�����Ҫ��������ĵ�һ��Ԫ�ص�ַ�ͷ��͵��ֽ���
{
	unsigned int i;

	for(i =0; i < ulNum; i++)
	{ 
		//UART3_SendByte(*pucStr++);
		UART3_SendByte(str[i]);				//�ֽڷ���
	}
}


// UART3�жϷ������
void  UART3_IRQHandler(void)				// UART0�жϺ���
{
	uint8 i;
	
	OSIntEnter();

	if ((LPC_UART3->IIR & 0x0F) == 0x04)		// �Ƿ�����ж�(RDA�ж�)
	{
		for (i = 0; i < 8; i++)											// ��������8���ֽ�
		{
			if (i + uart3_rcv_counter >= MAX_UART3_BYTE_NUM)			// ��ֹ�������
		    {
		        uart3_timeout_8byte_stop();
		        
		        break;  
		    }
			else
			{
			    uart3_temp[i + uart3_rcv_counter] = LPC_UART3->RBR;		// ��ȡFIFO�����ݣ�������ж�
		    }
		}
    	
    	uart3_rcv_counter += 8;
		
		uart3_timeout_8byte_start();
	}
	
	if ((LPC_UART3->IIR & 0x0F) == 0x0c)		// �Ƿ�ʱ�ж�(CTI�ж�)
	{
		while ((LPC_UART3->LSR & 0x01) == 1)							// �Ƿ���δ���������
		{
			if (uart3_rcv_counter >= MAX_UART3_BYTE_NUM)				// ��ֹ�������
		    {
		        uart3_timeout_8byte_stop();	  
		        
		        break;  
		    }
			else
			{
    			uart3_temp[uart3_rcv_counter] = LPC_UART3->RBR;			// ��ȡFIFO�����ݣ�������ж�
    			uart3_rcv_counter++;
    	    }  
		}	
		
		uart3_receive_num = uart3_rcv_counter;
		
		uart3_timeout_8byte_stop();	
		
		OSSemPost(RecvUARTFlag3);										// ��8�ֽڳ�ʱ�ж��ź�������
    }
	
	OSIntExit();
}


