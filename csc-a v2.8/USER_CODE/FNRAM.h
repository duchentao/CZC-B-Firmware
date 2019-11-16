//**************************************************************************************
//							此头文件用于FM24L256  for LPC1768
//									I2C铁电存储器
//**************************************************************************************
#ifndef _FNRAM_
#define _FNRAM_

#include "config.h"
/************************************************************************/
//引脚定义
//存储芯片 FM24L256
//最大地址 0x7FFF ~ 0x0000
#define FM_WP_PIN		    (1 << 21)  //P1.21		
#define FM_SCL_PIN   	    (1 << 22)  //P1.22			
#define FM_SDA_PIN			(1 << 23)  //P1.23			

#define SET_SCL  LPC_GPIO1->FIOSET = FM_SCL_PIN			
#define CLR_SCL	 LPC_GPIO1->FIOCLR = FM_SCL_PIN			
#define SET_SDA  LPC_GPIO1->FIOSET = FM_SDA_PIN			
#define CLR_SDA  LPC_GPIO1->FIOCLR = FM_SDA_PIN			
#define SET_WP   LPC_GPIO1->FIOSET = FM_WP_PIN			
#define CLR_WP   LPC_GPIO1->FIOCLR = FM_WP_PIN			

#define DATA_OUT LPC_GPIO1->FIODIR |= FM_SDA_PIN		
#define DATA_IN  LPC_GPIO1->FIODIR &= ~ FM_SDA_PIN		

#define READ_IO  (LPC_GPIO1->FIOPIN & FM_SDA_PIN)		// due to hardware

#define FM_ADDRESS      0xA0   //铁电存储器的IIC总线器件地址

#define WRITE_FM_ENABLE  CLR_WP
#define WRITE_FM_DISABLE SET_WP

void FNRAM_init(void);  //管脚初始化

/*******************************************************************
                     无子地址发送字节数据函数                                    
功能: 从启动总线到发送地址，写数据，结束总线的全过程,从器件地址sla.
      如果返回1表示操作成功，否则操作有误。
********************************************************************/
uint8 ISendByte(uint8 sla,uint8 c);

/*******************************************************************
                    无子地址读字节数据函数               
功能: 从启动总线到发送地址，读数据，结束总线的全过程,从器件地
      址sla，返回值在c.
      如果返回1表示操作成功，否则操作有误。
********************************************************************/
uint8 IRcvByte(uint8 sla,uint8 *c);

//发送，器件地址， 存储高，低字节地址，写数据块头指针，数量
uint8 ISendStr(uint8 sla, uint8 subah, uint8 subal, uint8 *s,uint8 no);

//接收，器件地址， 存储高，低字节地址，读数据块头指针，数量
uint8 IRcvStr(uint8 sla,uint8 subah, uint8 subal, uint8 *s,uint8 no);

#endif





