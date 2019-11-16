/****************************************Copyright (c)**************************************************
**                               Guangzhou ZHIYUAN electronics Co.,LTD.
**                                     
**                                 http://www.zyinside.com
**
**--------------File Info-------------------------------------------------------------------------------
** File Name:          main.c
** Last modified Date: 2006-01-11 
** Last Version:       v1.0
** Descriptions:       主函数
**
**------------------------------------------------------------------------------------------------------
** Modified by:        
** Modified date:      2006-01-06
** Version:            v1.0
** Descriptions:       创建
**
**------------------------------------------------------------------------------------------------------
** Modified by:      
** Modified date:     
** Version:           
** Descriptions:      
**
********************************************************************************************************/
#include "config.h"


#define	Task0StkLengh	64          // Define the Task0 stack length 定义用户任务0的堆栈长度

OS_STK	Task0Stk [Task0StkLengh];   // Define the Task0 stack 定义用户任务0的堆栈


void 	Task0(void *pdata);			// Task0 任务0

#define	TaskModBUSStkLengh	64
OS_STK	TaskModBUSStk [TaskModBUSStkLengh];	

/*********************************************************************************************************
** Function name: main
** Descriptions:  主函数
** Input:         无 
** Output:        系统返回值0
** Created by:    
** Created Date:  2006-01-12
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified Date: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
int main (void)
{
	OSInit ();																										
	OSTaskCreate (Task0,(void *)0, &Task0Stk[Task0StkLengh - 1], 2);		
	OSTaskCreate (OSModbusServe,(void *)0, &TaskModBUSStk[TaskModBUSStkLengh - 1], 4);	
	OSStart ();
	return 0;															
}

/****************************************************************************************************/
/*	设定MODBUS从机信息																				*/
/****************************************************************************************************/
uint8	ADUBuff[MAX_ADU_LENGTH]; 	// ADU数据缓冲区
SLAVE_INFORMATION 	 SlaveDevice = {
	0x01,					// 该Modbus从机地址
	0x00,					// 链路层协议
	1152,					// 波特率 = BaudRate * 100	
	0,						// 奇偶校验
	2,						// 停止位
	ADUBuff					// 主机请求从帧指针
}; 


// 定义蜂鸣器控制口
#define   	BEEP		   	(1<<10)     	/* GPH10口 */	
#define   	BEEP_MASK	  	(~BEEP)
// 蜂鸣器B1 GPIO初始化 
#define IniBUZZ()			 rGPHCON = (rGPHCON & (~(0x03<<20))) | (0x01<<20);   // rGPHCON[21:20] = 01b，设置GPH10为GPIO输出模式 												

#define SetBUZZ()			rGPHDAT = rGPHDAT | BEEP        // GPF4为高电平，则设置GPH10=1								// 蜂鸣器蜂鸣
#define ClrBUZZ()			rGPHDAT = rGPHDAT & BEEP_MASK  // GPF4为低电平，则设置GPH10=0								// 蜂鸣器关闭

//**************************************************
// 函数名称：MB_SetCoil
// 输入参数：Address，线圈地址
//			 CoilValue,线圈值（0\1）
// 输出参数：返回寄存器值
// 功能描述：设置线圈值函数，访函数由用户编写
//**************************************************
uint8 MB_SetCoil(uint16 Address,uint8 CoilValue)
{	

	if(Address==0)				// 地址为1
	{	
		if( CoilValue == 1 )
			ClrBUZZ();			// 蜂鸣器蜂鸣
		else
			SetBUZZ();			// 蜂鸣器关闭
	}
	return TRUE;
}

/*********************************************************************************************************
** Function name: Task0
** Descriptions:  任务0
** Input:         
** Output:        无
** Created by:    
** Created Date:  2006-01-12
**-------------------------------------------------------------------------------------------------------
** Modified by:
** Modified Date: 
**------------------------------------------------------------------------------------------------------
********************************************************************************************************/
void Task0	(void *pdata)
{
	pdata = pdata;
	TargetInit ();
	IniBUZZ();
	while (1)
	{
		OSTimeDly(OS_TICKS_PER_SEC);
	}
}




/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
