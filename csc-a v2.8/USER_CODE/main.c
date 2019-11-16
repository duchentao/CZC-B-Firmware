/*
 * @Descripttion: 
 * @version: 
 * @Author: 
 * @Date: 2019-10-12 19:19:52
 * @LastEditors: 
 * @LastEditTime: 2019-11-11 14:22:57
 */
#include "main.h"
#include "watchdog.h"
#include "IO.h"
#include "uart.h"
#include "modbus.h"
#include "task_modbus.h"
#include "task_setup.h"
//#include "FNRAM.h"
/**********************************************************
**				Task ID, PRIO, STACK_SIZE
**
****************  uC/OS II 系统宏定义  ********************/
#define		TASK_IO_ID								1
#define		TASK_APP_ID								2
#define		TASK_SHOW_ID							3
#define		TASK_TIMER_ID							4


#define		TASK_IO_PRIO							10
#define		TASK_APP_PRIO				        	11
#define		TASK_SHOW_PRIO							12
#define		TASK_TIMER_PRIO					    	13


#define		TASK_IO_STACK_SIZE						400
#define		TASK_APP_STACK_SIZE			     		400
#define		TASK_SHOW_STACK_SIZE			    	400
#define		TASK_TIMER_STACK_SIZE					128


OS_STK		TASK_IO_STACK[TASK_IO_STACK_SIZE];
OS_STK		TASK_APP_STACK[TASK_APP_STACK_SIZE];
OS_STK		TASK_SHOW_STACK[TASK_SHOW_STACK_SIZE];
OS_STK		TASK_TIMER_STACK[TASK_TIMER_STACK_SIZE];
//************************************************************************************************
#define DEBUG_MODE		1
//*********************************************************************************************/
/************************END************************/
OS_EVENT  *RecvUARTFlag0;  
OS_EVENT  *RecvUARTFlag1;  
OS_EVENT  *RecvUARTFlag2;  
OS_EVENT  *RecvUARTFlag3; 
/************************global var************************/ 

/************************END************************/
uint8 flash_05s_flag=0;
uint8 flash_1s_flag = 0;
uint8 flash_2s_flag = 0;
uint8 flash_L3_flag = 0;
struct _parameter param;
uint32 upper_cmd = 0;
uint32 upper_flag = 0;
uint8 dis_flag = 0;
uint8 beep_cmd = 0;
uint32 input_source = 0;
uint32 input_flag = 0;
uint32 ctrl_flag = 0;
uint32 output_flag = 0;
	
/*************************延时子函数*************************/
void Delay1Us(void)
{
   uint8 i = 10;
   while(i--); 
}
void Delay10kUs(void)
{
   uint16 i = 10000;
   while(i--); 
}
/****************************************************************************
*****************************************************************************/
void Open_Delay(void)
{
	uint32 open_delay_count = 0;
	for (open_delay_count = 0; open_delay_count < 100000; open_delay_count++)
	{
		Delay1Us();
	}
}

//****************************************************************************
//**	任务名称:	TASK_TIMER
//**	任务功能：	时间计数器,基准 = 0.5s
//****************************************************************************
void TASK_TIMER(void *pvData)
{
	uint8 flash_05s_cnt = 0;		// 0.5s闪烁计数器
	uint8 flash_1s_cnt = 0;		    // 1s闪烁计数器	
	uint8 flash_2s_cnt = 0;			// 2s闪烁计数器
	flash_L3_flag = 0;

	while(1)
	{
		// 0.5s
		flash_05s_cnt++;
		if(flash_05s_cnt == FLASH_05S_DELAY_TIME)
		{
			flash_05s_flag = !flash_05s_flag;
			flash_05s_cnt = 0;
		}
		
		// 1s
		flash_1s_cnt++;
		if(flash_1s_cnt == FLASH_1S_DELAY_TIME)
		{
			flash_1s_flag = !flash_1s_flag;
			flash_1s_cnt = 0;
		}
		
		// 2s
		flash_2s_cnt++;
		if(flash_2s_cnt == FLASH_2S_DELAY_TIME)
		{
			flash_2s_flag = !flash_2s_flag;
			flash_2s_cnt = 0;
		}
		
		if(flash_L3_flag == 1)
		{
			IO_OutputCtrl(L3,ON);
			flash_L3_flag = 0;
		}
		else
		{
			IO_OutputCtrl(L3,OFF);
		}
		
		
		IO_OutputCtrl(L2,flash_1s_flag);
		OSTimeDly(OS_TICKS_PER_SEC / 4); 
	}
}
uint32 test_temp = 0;
uint8 tog_sw = 0;
uint8 SLAVE_ADDR1 = 0x01;
//****************************************************************************
//**	任务名称:	TASK_IO
//**	任务功能：	输入输出采样标志位等
//****************************************************************************
void TASK_IO(void *pvData)
{
  	uint8 j = 0;
	uint16 counter = 0;
	IO_init();
	while(1)
	{
		wdt_feed();  //10ms喂狗一次
		tog_sw = IO_ReadSwitch();			//读取拨码值
		
		if((tog_sw & 0x08) == 0x08)
		{
			SLAVE_ADDR1 = 0x02;
		}
		else
		{
			SLAVE_ADDR1 = 0x01;
		}
		//正常模式
		if(7 != (tog_sw & 0x07))
		{
			//读input，消抖
			input_source = IO_InputScan();
			//读取消音按钮
			IO_ReadMute();
			//常开常闭设置
			if(upper_flag != upper_cmd)
			{
				output_flag = upper_cmd;
				upper_flag = upper_cmd;
			}
			//输出output
			for(j=0;j<IO_INPUT_NUM;j++)
			{
				if (output_flag & (1ul<<j))
				{	
					IO_OutputCtrl(KA1+j,ON);
				}
				else
				{
					IO_OutputCtrl(KA1+j,OFF);
				}
			}
			
			if(beep_cmd == mute_flag)
			{
				IO_OutputCtrl(BEEP,OFF);
				beep_cmd = 0;
				dis_flag = 0;
				mute_flag = 0;
			}
			else
			{
				if(beep_cmd)
				{
					IO_OutputCtrl(BEEP, flash_1s_flag);
				}
				else
				{
					mute_flag = 0;
				}
				
			}
		}
        else if(7 == (tog_sw & 0x07))
        {
			//读input
			input_source = IO_InputScan();

			//常开常闭设置
			input_flag =  input_source;
			//输出outpu
			for(j=0;j<IO_INPUT_NUM;j++)
			{
				if(input_flag & (0x0001<<j))
				{
					IO_OutputCtrl(j+KA1,ON);
				}
				else
				{
					IO_OutputCtrl(j+KA1,OFF);
				}
			}
		}
		counter++;

		OSTimeDly(OS_TICKS_PER_SEC / 10);  //10ms
	}
}
//****************************************************************************
//**	任务名称:	TASK_SHOW
//**	任务功能：	74LS595指示灯显示
//****************************************************************************
void TASK_SHOW(void *pvData)
{
    while(1)
    {	
		
		IO_OutputCtrl(L3,ON);
		IO_OutputCtrl(L2,flash_1s_flag);
		
        OSTimeDly(OS_TICKS_PER_SEC / 4);
    }      
}



/**********************************************************************************************************

**********************************************************************************************************/
int main (void)
 {
	Delay10kUs();
	SystemInit();
	wdt_init(6000);				// 看门狗初始化
	
	//******************************  UART初始化   ******************************
	//UART0_Init(UART0_BAUD_RATE, 0);			//channel = 0(P0.2, P0.3)----调试用
	//UART1_Init(UART1_BAUD_RATE, 0);			//channel = 0(P0.15, P0.16), channel = 1(P2.0, P2.1)
	UART2_Init(UART2_BAUD_RATE, 1);			//channel = 0(P0.10, P0.11), channel = 1(P2.8, P2.9)
	//UART3_Init(UART3_BAUD_RATE, 0);			//channel = 0(P0.0, P0.1),   channel = 1(P0.25, P0.26), channel = 2(P4.28, P4.29)
   
   
	//*******************************  UC/OS初始化   ********************************
    OSInit(); 
	 
	OSTaskCreateExt(TASK_IO,
					(void *)0,
					&TASK_IO_STACK[TASK_IO_STACK_SIZE-1],
					TASK_IO_PRIO,
					TASK_IO_ID,
					&TASK_IO_STACK[0],
					TASK_IO_STACK_SIZE,
					(void *)0,
					0);
	 
	//  OSTaskCreateExt(TASK_SHOW,
	// 				(void *)0,
	// 				&TASK_SHOW_STACK[TASK_SHOW_STACK_SIZE-1],
	// 				TASK_SHOW_PRIO,
	// 				TASK_SHOW_ID,
	// 				&TASK_SHOW_STACK[0],
	// 				TASK_SHOW_STACK_SIZE,
	// 				(void *)0,
	// 				0);					
							
	OSTaskCreateExt(TASK_TIMER,
					(void *)0,
					&TASK_TIMER_STACK[TASK_TIMER_STACK_SIZE-1],
					TASK_TIMER_PRIO,
					TASK_TIMER_ID,
					&TASK_TIMER_STACK[0],
					TASK_TIMER_STACK_SIZE,
					(void *)0,
					0);		
	OSTaskCreateExt(TASK_TIME,
					(void *)0,
					&TASK_TIME_STACK[TASK_TIME_STACK_SIZE-1],
					TASK_TIME_PRIO,
					TASK_TIME_ID,
					&TASK_TIME_STACK[0],
					TASK_TIME_STACK_SIZE,
					(void *)0,
					0);						
	OSTaskCreateExt(TASK_MODBUS1_SLAVE,
					(void *)0,
					&TASK_MODBUS1_STACK[TASK_MODBUS1_STACK_SIZE-1],
					TASK_MODBUS1_PRIO,
					TASK_MODBUS1_ID,
					&TASK_MODBUS1_STACK[0],
					TASK_MODBUS1_STACK_SIZE,
					(void *)0,
					0);	
						
	RecvUARTFlag0  = OSSemCreate(0);
	RecvUARTFlag1  = OSSemCreate(0);
	RecvUARTFlag2  = OSSemCreate(0);
	RecvUARTFlag3  = OSSemCreate(0);
    	
    OSStart();    
    return 0;                                   
}
void modbus1_updata(uint8 func,uint8 *buf)
{
	flash_L3_flag = 1;
	switch(func)
	{
		case 0x03:
			buf[0] = (uint8)(input_source>>8);
			buf[1] = (uint8)(input_source>>0);
			//buf[2] = (uint8)(input_source>>24);
			//buf[3] = (uint8)(input_source>>16);
		break;
			
		case 0x06:
			upper_cmd = ( ((uint32)buf[2] << 8) | ((uint32)buf[3] << 0) |
						  ((uint32)buf[4] << 24) | ((uint32)buf[5] << 16) );
			//接收到消息后，置位		
			beep_cmd = !!(upper_cmd & 0x8000);
		break;
		default:
			
			
		break;
	}
}


