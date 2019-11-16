#ifndef _IO_H_
#define _IO_H_

#include "main.h"




#define     ON  	   0
#define     OFF 	   1

#define     OUT        0
#define     IN         1

//输出控制信号宏定义
#define    KA1				1      	//
#define    KA2				2      	//
#define    KA3				3      	//
#define    KA4				4      	//
#define    KA5				5      	//
#define    KA6				6      	//
#define    KA7				7      	//
#define    KA8				8      	//
#define    KA9				9      	//
#define    KA10				10      //
#define    KA11				11      //
#define    BEAK_UP_OUT		12      //
#define    BEEP				13      //

#define    L1				14      //
#define    L2				15      //
#define    L3				16      //
#define    CON_485_1		17      //RS485_1输入输出控制脚
#define    CON_485_2		18      //RS485_2输入输出控制脚
/*******************************************************/
//输出管脚定义
#define    L1_PIN			(1 << 18)	  	//P0.18
#define    L2_PIN			(1 << 2)	  	//P2.2
#define    L3_PIN			(1 << 1)	  	//P2.1

#define    L1_IO_CON		 LPC_GPIO0
#define    L2_IO_CON		 LPC_GPIO2	
#define    L3_IO_CON		 LPC_GPIO2	

#define    CON_485_1_PIN		(1 << 17)     	//P0.17
#define    CON_485_2_PIN		(1 << 4)     	//P2.4
                            
#define    CON_485_1_IO_CON		LPC_GPIO0
#define    CON_485_2_IO_CON		LPC_GPIO2
                            
                          
/*******************************************************/

/*******************************************************/
//输入管脚定义
#define    IN1_PIN   	    (1 << 29)	  //P1.29   
#define    IN2_PIN		    (1 << 0)	  //P0.0  
#define    IN3_PIN		    (1 << 1)	  //P0.1  
#define    IN4_PIN		    (1 << 10)	  //P0.10      
#define    IN5_PIN		    (1 << 11)	  //P0.11  
#define    IN6_PIN		    (1 << 13)	  //P2.13  
#define    IN7_PIN		    (1 << 12)	  //P2.12 
#define    IN8_PIN		    (1 << 11)	  //P2.11
#define    IN9_PIN		    (1 << 22)	  //P0.22
#define    IN10_PIN		    (1 << 21)	  //P0.21
#define    IN11_PIN		    (1 << 20)	  //P0.20
#define    IN12_PIN		    (1 << 19)	  //P0.19
#define    IN13_PIN		    (1 << 28)	  //P1.28 

#define    IN1_IO_CON		 LPC_GPIO1
#define    IN2_IO_CON		 LPC_GPIO0
#define    IN3_IO_CON		 LPC_GPIO0
#define    IN4_IO_CON		 LPC_GPIO0
#define    IN5_IO_CON		 LPC_GPIO0
#define    IN6_IO_CON		 LPC_GPIO2
#define    IN7_IO_CON		 LPC_GPIO2
#define    IN8_IO_CON		 LPC_GPIO2
#define    IN9_IO_CON		 LPC_GPIO0
#define    IN10_IO_CON		 LPC_GPIO0
#define    IN11_IO_CON		 LPC_GPIO0
#define    IN12_IO_CON		 LPC_GPIO0
#define    IN13_IO_CON		 LPC_GPIO1

                        
#define    KA1_PIN		    (1 << 26)	  //P0.26 
#define    KA2_PIN		    (1 << 25)	  //P0.25
#define    KA3_PIN		    (1 << 24)	  //P0.24 
#define    KA4_PIN		    (1 << 23)	  //P0.23 
#define    KA5_PIN		    (1 << 18)	  //P1.18 
#define    KA6_PIN		    (1 << 19)	  //P1.19 
#define    KA7_PIN		    (1 << 20)	  //P1.20 
#define    KA8_PIN		    (1 << 21)	  //P1.21 
#define    KA9_PIN		    (1 << 22)	  //P1.22 
#define    KA10_PIN		    (1 << 23)	  //P1.23 
#define    KA11_PIN		    (1 << 24)	  //P1.24 
#define    KA12_PIN		    (1 << 25)	  //P1.25 
#define    KA13_PIN		    (1 << 26)	  //P1.26 


#define    KA1_IO_CON	     LPC_GPIO0
#define    KA2_IO_CON		 LPC_GPIO0
#define    KA3_IO_CON		 LPC_GPIO0
#define    KA4_IO_CON		 LPC_GPIO0
#define    KA5_IO_CON		 LPC_GPIO1 
#define    KA6_IO_CON		 LPC_GPIO1
#define    KA7_IO_CON		 LPC_GPIO1
#define    KA8_IO_CON		 LPC_GPIO1
#define    KA9_IO_CON		 LPC_GPIO1
#define    KA10_IO_CON		 LPC_GPIO1
#define    KA11_IO_CON		 LPC_GPIO1
#define    KA12_IO_CON		 LPC_GPIO1
#define    KA13_IO_CON		 LPC_GPIO1
                          
/*******************************************************/
#define SET_UART1_OUT		IO_OutputCtrl(CON_485_1, ON)		// 用UART1
#define SET_UART1_IN		IO_OutputCtrl(CON_485_1, OFF)		

#define SET_UART2_OUT		IO_OutputCtrl(CON_485_2, ON)		// 用UART2
#define SET_UART2_IN		IO_OutputCtrl(CON_485_2, OFF)	
	
/*******************************************************/
#define READ_MUTE_PIN		(!(IN1_IO_CON->FIOPIN & IN1_PIN))



#define    D0_PIN		    (1 << 14)	  //P0.5 
#define    D0_IO_CON		 LPC_GPIO1
#define    D1_PIN		    (1 << 15)	  //P0.5 
#define    D1_IO_CON		 LPC_GPIO1
#define    D2_PIN		    (1 << 16)	  //P0.5 
#define    D2_IO_CON		 LPC_GPIO1
#define    D3_PIN		    (1 << 17)	  //P0.5 
#define    D3_IO_CON		 LPC_GPIO1


extern uint8 mute_flag;
void IO_init(void);

void IO_OutputCtrl(uint8 device_num, uint8 state);//输出控制，参数device_num表示设备名称，state表示设备的开或关
                                                //.eg --- Out_Control(LED1, ON) 表示把LED1点亮
										
uint32 IO_InputScan(void);


uint8 IO_ReadMute(void);

uint8 IO_ReadSwitch(void);
#endif


