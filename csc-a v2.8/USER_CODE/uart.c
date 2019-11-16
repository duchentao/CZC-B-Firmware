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


// UART0超时接收变量
uint8  uart0_timeout_start = 0;			        // 8字节后的超时中断计数器开启标志
uint8  uart0_timeout_cnt = 0;			        // 8字节超时计数器
uint8  uart0_temp[MAX_UART0_BYTE_NUM] = {0};    // UART0接收缓冲器
uint16 uart0_rcv_counter = 0;			        // UART0收到数据个数计数值


// UART1超时接收变量
uint8  uart1_timeout_start = 0;			        // 8字节后的超时中断计数器开启标志
uint8  uart1_timeout_cnt = 0;			        // 8字节超时计数器
uint8  uart1_temp[MAX_UART1_BYTE_NUM] = {0};    // UART1接收缓冲器
uint16 uart1_rcv_counter = 0;			        // UART1收到数据个数计数值

// UART2超时接收变量
uint8  uart2_timeout_start = 0;			        // 8字节后的超时中断计数器开启标志
uint8  uart2_timeout_cnt = 0;			        // 8字节超时计数器
uint8  uart2_temp[MAX_UART2_BYTE_NUM] = {0};    // UART2接收缓冲器
uint16 uart2_rcv_counter = 0;			        // UART2收到数据个数计数值

// UART3超时接收变量
uint8  uart3_timeout_start = 0;			        // 8字节后的超时中断计数器开启标志
uint8  uart3_timeout_cnt = 0;			        // 8字节超时计数器
uint8  uart3_temp[MAX_UART3_BYTE_NUM] = {0};    // UART3接收缓冲器
uint16 uart3_rcv_counter = 0;			        // UART3收到数据个数计数值



// 启动uart0的8字节超时判断定时器
void uart0_timeout_8byte_start(void)
{
	uart0_timeout_start = 1;
	uart0_timeout_cnt = 0;  
}

// 停止uart0的8字节超时判断定时器
void uart0_timeout_8byte_stop(void)
{
    uart0_timeout_start = 0;
	uart0_timeout_cnt = 0; 
	uart0_rcv_counter = 0;   
}


// 启动uart1的8字节超时判断定时器
void uart1_timeout_8byte_start(void)
{
	uart1_timeout_start = 1;
	uart1_timeout_cnt = 0;  
}

// 停止uart1的8字节超时判断定时器
void uart1_timeout_8byte_stop(void)
{
    uart1_timeout_start = 0;
	uart1_timeout_cnt = 0; 
	uart1_rcv_counter = 0;   
}


// 启动uart2的8字节超时判断定时器
void uart2_timeout_8byte_start(void)
{
	uart2_timeout_start = 1;
	uart2_timeout_cnt = 0;  
}

// 停止uart2的8字节超时判断定时器
void uart2_timeout_8byte_stop(void)
{
    uart2_timeout_start = 0;
	uart2_timeout_cnt = 0; 
	uart2_rcv_counter = 0;   
}


// 启动uart3的8字节超时判断定时器
void uart3_timeout_8byte_start(void)
{
	uart3_timeout_start = 1;
	uart3_timeout_cnt = 0;  
}

// 停止uart3的8字节超时判断定时器
void uart3_timeout_8byte_stop(void)
{
    uart3_timeout_start = 0;
	uart3_timeout_cnt = 0; 
	uart3_rcv_counter = 0;   
}


// 超时计数
void TIME_PROCESS(void)
{
// 12字节(8字节为正常接收所要时间，外加4个超时时间)
// 此处好像要加大到160，在140的时候7个字节中断可能会误以为此处超时，设大后，这里进来的概率减小了
// 8字节深度触发
	uint16 uart0_timeout_dly = OS_TICKS_PER_SEC * 120 / UART0_BAUD_RATE + 1;	//12字节(8字节为正常接收所要时间，外加4个超时时间)
	uint16 uart1_timeout_dly = OS_TICKS_PER_SEC * 120 / UART1_BAUD_RATE + 1;	//12字节(8字节为正常接收所要时间，外加4个超时时间)
	uint16 uart2_timeout_dly = OS_TICKS_PER_SEC * 120 / UART2_BAUD_RATE + 1;	//12字节(8字节为正常接收所要时间，外加4个超时时间)
	uint16 uart3_timeout_dly = OS_TICKS_PER_SEC * 120 / UART3_BAUD_RATE + 1;	//12字节(8字节为正常接收所要时间，外加4个超时时间)

	while (1)
	{
#ifdef UART0_RCVBUF_N
		if(uart0_timeout_start)
        {
            uart0_timeout_cnt++;
            
            if(uart0_timeout_cnt > uart0_timeout_dly)				// 判断超时
            {
                uart0_receive_num = uart0_rcv_counter;
                
                uart0_timeout_8byte_stop();
                
                OSSemPost(RecvUARTFlag0);							// 8字节超时中断信号量发送
            }
        }
#endif
		
#ifdef UART1_RCVBUF_N
		if(uart1_timeout_start)
        {
            uart1_timeout_cnt++;
            
            if(uart1_timeout_cnt > uart1_timeout_dly)				// 判断超时
            {
                uart1_receive_num = uart1_rcv_counter;
                
                uart1_timeout_8byte_stop();
                
                OSSemPost(RecvUARTFlag1);							// 8字节超时中断信号量发送
            }
        }
#endif

#ifdef UART2_RCVBUF_N
        if(uart2_timeout_start)
        {
            uart2_timeout_cnt++;
            
            if(uart2_timeout_cnt > uart2_timeout_dly)				// 判断超时
            {
                uart2_receive_num = uart2_rcv_counter;
                
                uart2_timeout_8byte_stop();
                
                OSSemPost(RecvUARTFlag2);							// 8字节超时中断信号量发送
            }
        }
#endif

#ifdef UART3_RCVBUF_N
		if(uart3_timeout_start)
        {
            uart3_timeout_cnt++;
            
            if(uart3_timeout_cnt > uart3_timeout_dly)				// 判断超时
            {
                uart3_receive_num = uart3_rcv_counter;
                
                uart3_timeout_8byte_stop();
                
                OSSemPost(RecvUARTFlag3);							// 8字节超时中断信号量发送
            }
        }
#endif

		OSTimeDly(1);
	} 
}



//******************************************************************************************************
//*                                              UART0                                                 *
//******************************************************************************************************
// UART0初始化
void UART0_Init(uint32 Baud_Rate, uint8 channel)		//channel = 0(P0.2, P0.3)
{
	uint16 Fdiv;
	LPC_SC->PCONP |= (1 << 3);							//开启UART0外设(功率控制PCONP)

	// 初始化相关的IO
	if(channel == 0)
	{
	    LPC_PINCON->PINSEL0 &= 0xffffff0f;
	    LPC_PINCON->PINSEL0 |= (0x01 << 4) | (0x01 << 6);// 设置P0.2和P0.3连接到UART0
	}
	else
	{
	    return;
	}
	
	send_time_out0 = (uint32)(100000000.0 / Baud_Rate);
	
	LPC_UART0->LCR = 0x83;								// 字长为8，1位停止，无奇偶校验，禁止间隔发送，允许设置波特率
	Fdiv  = (SystemFrequency / 4 / 16) / Baud_Rate;		// 设置波特率
	
	LPC_UART0->DLM  = Fdiv / 256;
	LPC_UART0->DLL  = Fdiv % 256;
	LPC_UART0->LCR  = 0x03;								// 设置完成，令DLAB位为0，锁定波特率
	LPC_UART0->FCR  = 0x87;								// 使能FIFO，设置8个字节触发深度,Rx FIFO复位,Tx FIFO复位-----0x81 for 2368

	NVIC_EnableIRQ(UART0_IRQn);							// NVIC设置
	NVIC_SetPriority(UART0_IRQn, INT_UART0_P);

	LPC_UART0->IER  = 0x01;								// 允许接收数据可用中断(RDA中断)和字符超时中断(CTI中断),即允许接收中断
}


// 字节发送
void UART0_SendByte(uint8 dat)
{
	uint32 time_out_counter = send_time_out0;
	
	LPC_UART0->THR = dat;
	while (((LPC_UART0->LSR & 0x40) == 0) && (time_out_counter))// 等待发送FIFO和UnTSR均为空，数据发送完毕
	{
	    time_out_counter--;
	}
}


// 字符串发送
//void UART0_SendStr(unsigned char *pucStr,unsigned int ulNum)	//发送函数需要传递数组的第一个元素地址和发送的字节数
void UART0_SendStr(unsigned char str[],unsigned int ulNum)		//发送函数需要传递数组的第一个元素地址和发送的字节数
{
	unsigned int i;

	for(i =0; i < ulNum; i++)
	{ 
		//UART0_SendByte(*pucStr++);
		UART0_SendByte(str[i]);		//字节发送
	}
}


// UART0中断服务程序
void  UART0_IRQHandler(void)				// UART0中断函数
{
	uint8 i;
	
	OSIntEnter();

	if ((LPC_UART0->IIR & 0x0F) == 0x04)		// 是否接收中断(RDA中断)
	{
		for (i = 0; i < 8; i++)										// 连续接收8个字节
		{
			if (i + uart0_rcv_counter >= MAX_UART0_BYTE_NUM)		// 防止数组溢出
		    {
		        uart0_timeout_8byte_stop();
		        
		        break;  
		    }
			else
			{
			    uart0_temp[i + uart0_rcv_counter] = LPC_UART0->RBR;	// 读取FIFO的数据，并清除中断
		    }
		}
    	
    	uart0_rcv_counter += 8;
		
		uart0_timeout_8byte_start();
	}
	
	if ((LPC_UART0->IIR & 0x0F) == 0x0c)		// 是否超时中断(CTI中断)
	{
		while ((LPC_UART0->LSR & 0x01) == 1)						// 是否有未处理的数据
		{
			if (uart0_rcv_counter >= MAX_UART0_BYTE_NUM)			// 防止数组溢出
		    {
		        uart0_timeout_8byte_stop();	  
		        
		        break;  
		    }
			else
			{
    			uart0_temp[uart0_rcv_counter] = LPC_UART0->RBR;		// 读取FIFO的数据，并清除中断
    			uart0_rcv_counter++;
    	    }  
		}	
		
		uart0_receive_num = uart0_rcv_counter;
		
		uart0_timeout_8byte_stop();	
		
		OSSemPost(RecvUARTFlag0);									// 非8字节超时中断信号量发送
    }
	
	OSIntExit();
}



//******************************************************************************************************
//*                                              UART1                                                 *
//******************************************************************************************************
// UART1初始化
void UART1_Init(uint32 Baud_Rate, uint8 channel)		//channel = 0(P0.15, P0.16), channel = 1(P2.0, P2.1)
{
	uint16 Fdiv;
	LPC_SC->PCONP |= (1 << 4);							//开启UART1外设(功率控制PCONP)
	
	// 初始化相关的IO
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
	    return; //失败    
	}

	send_time_out1 = (uint32)(100000000.0 / Baud_Rate);
	
	LPC_UART1->LCR = 0x83;								// 字长为8，1位停止，无奇偶校验，禁止间隔发送，允许设置波特率
	Fdiv  = (SystemFrequency / 4 / 16) / Baud_Rate;		// 设置波特率
	
	LPC_UART1->DLM  = Fdiv / 256;
	LPC_UART1->DLL  = Fdiv % 256;
	LPC_UART1->LCR  = 0x03;								// 设置完成，锁定波特率
	LPC_UART1->FCR  = 0x87;								// 使能FIFO，设置8个字节触发深度,Rx FIFO复位,Tx FIFO复位

	NVIC_EnableIRQ(UART1_IRQn);							// NVIC设置
	NVIC_SetPriority(UART1_IRQn, INT_UART1_P);

	LPC_UART1->IER  = 0x01;								// 允许接收数据可用中断(RDA中断)和字符超时中断(CTI中断),即允许接收中断
}


// 字节发送
void UART1_SendByte(uint8 dat)
{
	uint32 time_out_counter = send_time_out1;
	
	LPC_UART1->THR = dat;
	while (((LPC_UART1->LSR & 0x40) == 0) && (time_out_counter))// 等待发送FIFO和UnTSR均为空，数据发送完毕	
	{
	    time_out_counter--;
	}
}


// 字符串发送
//void UART1_SendStr(unsigned char *pucStr,unsigned int ulNum)	//发送函数需要传递数组的第一个元素地址和发送的字节数
void UART1_SendStr(unsigned char str[],unsigned int ulNum)		//发送函数需要传递数组的第一个元素地址和发送的字节数
{
	unsigned int i;

	for(i =0; i < ulNum; i++)
	{ 
		//UART1_SendByte(*pucStr++);
		UART1_SendByte(str[i]);		//字节发送
	}
}


// UART1中断服务程序
void  UART1_IRQHandler(void)				// UART0中断函数
{
	uint8 i;
	
	OSIntEnter();
	
	if ((LPC_UART1->IIR & 0x0F) == 0x04)		// 是否接收中断(RDA中断)
	{
		for (i = 0; i < 8; i++)										// 连续接收8个字节
		{
			if (i + uart1_rcv_counter >= MAX_UART1_BYTE_NUM)		// 防止数组溢出
		    {
		        uart1_timeout_8byte_stop();
		        
		        break;  
		    }
			else
			{
			    uart1_temp[i + uart1_rcv_counter] = LPC_UART1->RBR;	// 读取FIFO的数据，并清除中断
		    }
		}
    	
    	uart1_rcv_counter += 8;
		
		uart1_timeout_8byte_start();
	}
	
	if ((LPC_UART1->IIR & 0x0F) == 0x0c)		// 是否超时中断(CTI中断)
	{
		while ((LPC_UART1->LSR & 0x01) == 1)						// 是否有未处理的数据
		{
			if (uart1_rcv_counter >= MAX_UART1_BYTE_NUM)			// 防止数组溢出
		    {
		        uart1_timeout_8byte_stop();	  
		        
		        break;  
		    }
			else
			{
    			uart1_temp[uart1_rcv_counter] = LPC_UART1->RBR;		// 读取FIFO的数据，并清除中断
    			uart1_rcv_counter++;
    	    }  
		}	
		
		uart1_receive_num = uart1_rcv_counter;
		
		uart1_timeout_8byte_stop();	
		
		OSSemPost(RecvUARTFlag1);									// 非8字节超时中断信号量发送
    }
	
	OSIntExit();
}



//******************************************************************************************************
//*                                              UART2                                                 *
//******************************************************************************************************
// UART2初始化
void UART2_Init(uint32 Baud_Rate, uint8 channel)//channel = 0(P0.10, P0.11), channel = 1(P2.8, P2.9)
{
	uint16 Fdiv;
	LPC_SC->PCONP |= (1 << 24);							//开启UART2外设(功率控制PCONP)
	
	// 初始化相关的IO
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
	
	LPC_UART2->LCR = 0x83;								// 字长为8，1位停止，无奇偶校验，禁止间隔发送，允许设置波特率
	Fdiv  = (SystemFrequency / 4 / 16) / Baud_Rate;		// 设置波特率
	
	LPC_UART2->DLM  = Fdiv / 256;
	LPC_UART2->DLL  = Fdiv % 256;
	LPC_UART2->LCR  = 0x03;								// 设置完成，锁定波特率
	LPC_UART2->FCR  = 0x87;								// 使能FIFO，设置8个字节触发深度,Rx FIFO复位,Tx FIFO复位

	NVIC_EnableIRQ(UART2_IRQn);							// NVIC设置
	NVIC_SetPriority(UART2_IRQn, INT_UART2_P);

	LPC_UART2->IER  = 0x01;								// 允许接收数据可用中断(RDA中断)和字符超时中断(CTI中断),即允许接收中断
}


// 字节发送
void UART2_SendByte(uint8 dat)
{
	uint32 time_out_counter = send_time_out2;
	
	LPC_UART2->THR = dat;
	while (((LPC_UART2->LSR & 0x40) == 0) && (time_out_counter))// 等待发送FIFO和UnTSR均为空，数据发送完毕	
	{
	    time_out_counter--;
	}
}


// 字符串发送
//void UART2_SendStr(unsigned char *pucStr,unsigned int ulNum)	//发送函数需要传递数组的第一个元素地址和发送的字节数
void UART2_SendStr(unsigned char str[],unsigned int ulNum)		//发送函数需要传递数组的第一个元素地址和发送的字节数
{
	unsigned int i;

	for(i =0; i < ulNum; i++)
	{ 
		//UART2_SendByte(*pucStr++);
		UART2_SendByte(str[i]);				//字节发送
	}
}


// UART2中断服务程序
void  UART2_IRQHandler(void)				// UART0中断函数
{
	uint8 i;
	
	OSIntEnter();

	if ((LPC_UART2->IIR & 0x0F) == 0x04)		// 是否接收中断(RDA中断)
	{
		for (i = 0; i < 8; i++)										// 连续接收8个字节
		{
			if (i + uart2_rcv_counter >= MAX_UART2_BYTE_NUM)		// 防止数组溢出
		    {
		        uart2_timeout_8byte_stop();
		        
		        break;  
		    }
			else
			{
			    uart2_temp[i + uart2_rcv_counter] = LPC_UART2->RBR;	// 读取FIFO的数据，并清除中断
		    }
		}
    	
    	uart2_rcv_counter += 8;
		
		uart2_timeout_8byte_start();
	}
	
	if ((LPC_UART2->IIR & 0x0F) == 0x0c)		// 是否超时中断(CTI中断)
	{
		while ((LPC_UART2->LSR & 0x01) == 1)						// 是否有未处理的数据
		{
			if (uart2_rcv_counter >= MAX_UART2_BYTE_NUM)			// 防止数组溢出
		    {
		        uart2_timeout_8byte_stop();	  
		        
		        break;  
		    }
			else
			{
    			uart2_temp[uart2_rcv_counter] = LPC_UART2->RBR;		// 读取FIFO的数据，并清除中断
    			uart2_rcv_counter++;
    	    }  
		}	
		
		uart2_receive_num = uart2_rcv_counter;
		
		uart2_timeout_8byte_stop();	
		
		OSSemPost(RecvUARTFlag2);									// 非8字节超时中断信号量发送
    }
	
	OSIntExit();
}



//******************************************************************************************************
//*                                              UART3                                                 *
//******************************************************************************************************
// UART3初始化
void UART3_Init(uint32 Baud_Rate, uint8 channel)		//channel = 0(P0.0, P0.1),   channel = 1(P0.25, P0.26), channel = 2(P4.28, P4.29)
{
	uint16 Fdiv;
	LPC_SC->PCONP |= (1 << 25);							//开启UART3外设(功率控制PCONP)
	
	// 初始化相关的IO
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
	
	LPC_UART3->LCR = 0x83;								// 字长为8，1位停止，无奇偶校验，禁止间隔发送，允许设置波特率
	Fdiv  = (SystemFrequency / 4 / 16) / Baud_Rate;		// 设置波特率
	
	LPC_UART3->DLM  = Fdiv / 256;
	LPC_UART3->DLL  = Fdiv % 256;
	LPC_UART3->LCR  = 0x03;								// 设置完成，锁定波特率
	LPC_UART3->FCR  = 0x87;								// 使能FIFO，设置8个字节触发深度,Rx FIFO复位,Tx FIFO复位

	NVIC_EnableIRQ(UART3_IRQn);							// NVIC设置
	NVIC_SetPriority(UART3_IRQn, INT_UART3_P);

	LPC_UART3->IER  = 0x01;								// 允许接收数据可用中断(RDA中断)和字符超时中断(CTI中断),即允许接收中断
}


// 字节发送
void UART3_SendByte(uint8 dat)
{
	uint32 time_out_counter = send_time_out3;
	
	LPC_UART3->THR = dat;
	while (((LPC_UART3->LSR & 0x40) == 0) && (time_out_counter))// 等待发送FIFO和UnTSR均为空，数据发送完毕	
	{
	    time_out_counter--;
	}
}


// 字符串发送
//void UART3_SendStr(unsigned char *pucStr,unsigned int ulNum)	//发送函数需要传递数组的第一个元素地址和发送的字节数
void UART3_SendStr(unsigned char str[],unsigned int ulNum)		//发送函数需要传递数组的第一个元素地址和发送的字节数
{
	unsigned int i;

	for(i =0; i < ulNum; i++)
	{ 
		//UART3_SendByte(*pucStr++);
		UART3_SendByte(str[i]);				//字节发送
	}
}


// UART3中断服务程序
void  UART3_IRQHandler(void)				// UART0中断函数
{
	uint8 i;
	
	OSIntEnter();

	if ((LPC_UART3->IIR & 0x0F) == 0x04)		// 是否接收中断(RDA中断)
	{
		for (i = 0; i < 8; i++)											// 连续接收8个字节
		{
			if (i + uart3_rcv_counter >= MAX_UART3_BYTE_NUM)			// 防止数组溢出
		    {
		        uart3_timeout_8byte_stop();
		        
		        break;  
		    }
			else
			{
			    uart3_temp[i + uart3_rcv_counter] = LPC_UART3->RBR;		// 读取FIFO的数据，并清除中断
		    }
		}
    	
    	uart3_rcv_counter += 8;
		
		uart3_timeout_8byte_start();
	}
	
	if ((LPC_UART3->IIR & 0x0F) == 0x0c)		// 是否超时中断(CTI中断)
	{
		while ((LPC_UART3->LSR & 0x01) == 1)							// 是否有未处理的数据
		{
			if (uart3_rcv_counter >= MAX_UART3_BYTE_NUM)				// 防止数组溢出
		    {
		        uart3_timeout_8byte_stop();	  
		        
		        break;  
		    }
			else
			{
    			uart3_temp[uart3_rcv_counter] = LPC_UART3->RBR;			// 读取FIFO的数据，并清除中断
    			uart3_rcv_counter++;
    	    }  
		}	
		
		uart3_receive_num = uart3_rcv_counter;
		
		uart3_timeout_8byte_stop();	
		
		OSSemPost(RecvUARTFlag3);										// 非8字节超时中断信号量发送
    }
	
	OSIntExit();
}


