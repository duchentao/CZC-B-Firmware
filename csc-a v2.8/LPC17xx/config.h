/****************************************Copyright (c)**************************************************
**                         Guangzhou ZHIYUAN electronics Co.,LTD.                               
**                                     
**                               http://www.embedtools.com
**
**--------------File Info-------------------------------------------------------------------------------
** File Name			: config.h
** Last modified Date	: 2004-09-17
** Last Version			: V1.00
** Descriptions			: User Configurable File
**
**------------------------------------------------------------------------------------------------------
** Created By			: Chenmingji
** Created date			: 2004-09-17
** Version				: V1.00
** Descriptions			: First version
**
**------------------------------------------------------------------------------------------------------
** Modified by			: LinEnqiang
** Modified date		: 2007-05-15
** Version				: V1.01
** Descriptions			: Modified for LPC17xx
**
********************************************************************************************************/
#ifndef __CONFIG_H
#define __CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

//这一段无需改动
#ifndef TRUE
#define TRUE  1
#endif

#ifndef FALSE
#define FALSE 0
#endif


typedef unsigned char  uint8;                   // 无符号8位整型变量  
typedef signed   char  int8;                    // 有符号8位整型变量  
typedef unsigned short uint16;                  // 无符号16位整型变量
typedef signed   short int16;                   // 有符号16位整型变量 
typedef unsigned int   uint32;                  // 无符号32位整型变量 
typedef signed   int   int32;                   // 有符号32位整型变量 
typedef float          fp32;                    // 单精度浮点数（32位长度） 
typedef double         fp64;                    // 双精度浮点数（64位长度） 
//#define GLOBAL	 extern	


/********************************
**    结构体定义      
********************************/
union UINT8_ONEBIT
{	
	struct
	{
		unsigned b0:1;
		unsigned b1:1;
		unsigned b2:1;
		unsigned b3:1;
		unsigned b4:1;
		unsigned b5:1;
		unsigned b6:1;
		unsigned b7:1;
	}OneBit;
unsigned char AllBit;
};


union UINT16_ONEBIT
{	
	struct
	{
		unsigned d0:1;			// 字节
		unsigned d1:1;
	}EightBit;
	
	struct
	{
		unsigned h0:1;			// 半字节
		unsigned h1:1;
		unsigned h2:1;
		unsigned h3:1;
	}FourBit;
	
	struct						// 位
	{
		unsigned b0:1;
		unsigned b1:1;
		unsigned b2:1;
		unsigned b3:1;
		unsigned b4:1;
		unsigned b5:1;
		unsigned b6:1;
		unsigned b7:1;
		unsigned b8:1;
		unsigned b9:1;
		unsigned b10:1;
		unsigned b11:1;
		unsigned b12:1;
		unsigned b13:1;
		unsigned b14:1;
		unsigned b15:1;
	}OneBit;
unsigned char AllBit;
};


union UINT32_ONEBIT 
{
	struct						// 双字节
	{
		unsigned int w0:16;
		unsigned int w1:16;
	}SixteenBit;
	
	struct						// 字节
	{
		unsigned int d0:8;
		unsigned int d1:8;
		unsigned int d2:8;
		unsigned int d3:8;
	}EightBit;

	struct						// 半字节
	{
		unsigned int h0:4;
		unsigned int h1:4;
		unsigned int h2:4;
		unsigned int h3:4;
		unsigned int h4:4;
		unsigned int h5:4;
		unsigned int h6:4;
		unsigned int h7:4;
	}FourBit;
	
	struct						// 位
	{
		unsigned int b0:1;
		unsigned int b1:1;
		unsigned int b2:1;
		unsigned int b3:1;
		unsigned int b4:1;
		unsigned int b5:1;
		unsigned int b6:1;
		unsigned int b7:1;
		unsigned int b8:1;
		unsigned int b9:1;
		unsigned int b10:1;
		unsigned int b11:1;
		unsigned int b12:1;
		unsigned int b13:1;
		unsigned int b14:1;
		unsigned int b15:1;
		unsigned int b16:1;
		unsigned int b17:1;
		unsigned int b18:1;
		unsigned int b19:1;
		unsigned int b20:1;
		unsigned int b21:1;
		unsigned int b22:1;
		unsigned int b23:1;
		unsigned int b24:1;
		unsigned int b25:1;
		unsigned int b26:1;
		unsigned int b27:1;
		unsigned int b28:1;
		unsigned int b29:1;
		unsigned int b30:1;
		unsigned int b31:1;
	}OneBit;	
	
	unsigned int AllBit;		// 4 byte
	
};



/********************************
**      uC/OS-II的特殊代码      
********************************/
//#define     USER_USING_MODE    0x10         	//  用户模式,ARM代码    for LPC23xx             
                                             	//  只能是0x10,0x30,0x1f,0x3f之一      
#include	"includes.h"						//  uCOS-II头文件

/********************************
**      ARM的特殊代码                    
*********************************/
//这一段无需改动

#include    "LPC17xx.h"							/* LPC17xx外设寄存器            */

/********************************
**     应用程序配置             
*********************************/
//以下根据需要改动
#include    <stdio.h>
#include    <string.h>
#include    <ctype.h>
#include    <stdlib.h>
#include    <setjmp.h>
#include    <rt_misc.h>
//#include    "target.h"              		 	//这一句不能删除 for LPC23xx
//#include    "VIC_Control.h"              		//这一句不能删除 for LPC23xx


/********************************
**     本例程的配置  
*********************************/
	//for LPC23xx
#define	USE_USB				1

#define Fosc                12000000			//	OSC = 12MHz

#if USE_USB	
	
	#define Fusbclk	48000000
	
	#define Fcclk	(Fosc * 4)					        // 主频 Fcclk = 48MHz
	#define Fcco	(Fusbclk * (USBCLKDivValue+1))		// 如果用了USB,则Fcco是Fcclk与Fusbclk在275~550M之间的最小公倍数
														// Fcco是Fcclk在275~550M之间的最小倍数
	#define Fpclk	(Fcclk / 2)
	
	#define PLL_NValue			1
	#define PLL_MValue			(((Fcco/Fosc)*(PLL_NValue+1)/2)-1)
	#define CCLKDivValue		(Fcco/Fcclk-1)
	#define USBCLKDivValue		5	
#else
	#define Fcclk	(Fosc * 4)							// 主频 Fcclk = 48MHz
	#define Fcco    (Fcclk* 6)
	
	#define Fpclk	(Fcclk / 4)
	
	#define PLL_NValue			1	
	#define PLL_MValue			(((Fcco/Fosc)*(PLL_NValue+1)/2)-1)
	#define CCLKDivValue		(Fcco/Fcclk-1)
	#define USBCLKDivValue		254		
#endif	




#ifdef __cplusplus
	}
#endif



#endif
/*********************************************************************************************************
**                            End Of File
********************************************************************************************************/
