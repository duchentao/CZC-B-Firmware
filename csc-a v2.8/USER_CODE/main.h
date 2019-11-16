#ifndef __MAIN_H_ 
#define __MAIN_H_

#include "config.h"						
#include "IRQ.h"

#define PI  3.14159265358979

#define UART0_BAUD_RATE   38400    //UART0波特率
#define UART1_BAUD_RATE   9600      //UART1波特率   
#define UART2_BAUD_RATE   9600      //UART2波特率   此串口作为PARTOL_DM
#define UART3_BAUD_RATE   9600      //UART3波特率

//*************** UART0 ~ UART3 超时接收N字节接收长度设置  *******
// 若对应UARTn串口接收未用，请修改为一个较小的值，节省存储空间
#define MAX_UART0_BYTE_NUM		12				// UART接收字符串最大字节数N
#define MAX_UART1_BYTE_NUM		12				// UART接收字符串最大字节数N
#define MAX_UART2_BYTE_NUM		12				// UART接收字符串最大字节数N
#define MAX_UART3_BYTE_NUM		12				// UART接收字符串最大字节数N


// 站地址
#define SLAVE_ADDR0			0x01						// 从站地址
#define MAX_REG_SIZE0		10							// 最大寄存器数word
#define MAX_BUFFER_SIZE0	(MAX_REG_SIZE0 * 2)			// 最大数据字节数Byte = word * 2


#define MAX_REG_SIZE1		64							// 最大寄存器数word
#define MAX_BUFFER_SIZE1	(MAX_REG_SIZE1 * 2)			// 最大数据字节数Byte = word * 2

#define SLAVE_ADDR2			0x05						// 从站地址
#define MAX_REG_SIZE2		8							// 最大寄存器数word
#define MAX_BUFFER_SIZE2	(MAX_REG_SIZE2 * 2)			// 最大数据字节数Byte = word * 2

#define SLAVE_ADDR3			0x06						// 从站地址
#define MAX_REG_SIZE3		8							// 最大寄存器数word
#define MAX_BUFFER_SIZE3	(MAX_REG_SIZE3 * 2)			// 最大数据字节数Byte = word * 2



#define CAN0_SEND_OUTTIME  0         //当波特率为125K时，只有1只发送时，为6000，实测大概为3400次可以发完，这里取不到2倍的余量
#define CAN1_SEND_OUTTIME  0         //其他推算方式为：乘以只数，乘以(125K/波特率)的比例

//中断优先级,最多支持32个优先级
#define INT_CAN_P         8         //CAN
#define INT_UART0_P       7         //UART0
#define INT_UART1_P       12        //UART1
#define INT_UART2_P       6         //UART2
#define INT_UART3_P       24        //UART3


//*****************************************宏定义***********************************************
#define	FLASH_05S_DELAY_TIME	1			// 1*500=500ms,0.5s
#define	FLASH_1S_DELAY_TIME		2			// 2*500=1000ms,1s
#define	FLASH_2S_DELAY_TIME		4			// 4*500=2000ms,2s


#define	BTN_NOICE_TIME			5			// 5*10=50ms
#define	BTN_RST_TIME			5			// 5*10=50ms

#define	IO_IN_NUM			    30		    // IO输入数目
#define	NOICE_TIME			    5           // io_scan();函数中的滤波时间参数，5*10=50ms


#define IO_INPUT_NUM		13
#define IO_OUTPUT_NUM		13




typedef struct _parameter
{
	uint32 input_nc;	
	uint32 output_nc;
	
	
}parameter;
extern struct _parameter param;
#endif

