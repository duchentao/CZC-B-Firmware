//******************************************************************************************************
//*                                UART驱动模块 for LPC1768											   *
//******************************************************************************************************
#ifndef _UART_H_
#define _UART_H_

#include "main.h"

//********************       编译宏定义         *****************
//			定义UART接收超时应用    8字节/N（N>8）字节
//**************************** UART0  ***************************
#define UART0_RCVBUF_N

//**************************** UART1  ***************************
#define UART1_RCVBUF_N

//**************************** UART2  ***************************
#define UART2_RCVBUF_N

//**************************** UART3  ***************************
#define UART3_RCVBUF_N

// UART0超时接收变量
extern uint8  uart0_timeout_start;			        // 8字节后的超时中断计数器开启标志
extern uint8  uart0_timeout_cnt;			        // 8字节超时计数器
extern uint8  uart0_temp[MAX_UART0_BYTE_NUM];    // UART0接收缓冲器
extern uint16 uart0_rcv_counter;			        // UART0收到数据个数计数值


// UART1超时接收变量
extern uint8  uart1_timeout_start;			        // 8字节后的超时中断计数器开启标志
extern uint8  uart1_timeout_cnt;			        // 8字节超时计数器
extern uint8  uart1_temp[MAX_UART1_BYTE_NUM];    // UART1接收缓冲器
extern uint16 uart1_rcv_counter ;			        // UART1收到数据个数计数值

// UART2超时接收变量
extern uint8  uart2_timeout_start;			        // 8字节后的超时中断计数器开启标志
extern uint8  uart2_timeout_cnt ;			        // 8字节超时计数器
extern uint8  uart2_temp[MAX_UART2_BYTE_NUM];    // UART2接收缓冲器
extern uint16 uart2_rcv_counter;			        // UART2收到数据个数计数值

// UART3超时接收变量
extern uint8  uart3_timeout_start;			        // 8字节后的超时中断计数器开启标志
extern uint8  uart3_timeout_cnt;			        // 8字节超时计数器
extern uint8  uart3_temp[MAX_UART3_BYTE_NUM];    // UART3接收缓冲器
extern uint16 uart3_rcv_counter;			        // UART3收到数据个数计数值



// 提供外部应用函数,main.c中建立一个Task_TIME任务。
void TIME_PROCESS(void);

void uart0_timeout_8byte_start(void);			// 启动uart0的8字节超时判断定时器
void uart0_timeout_8byte_stop(void);			// 停止uart0的8字节超时判断定时器
void uart1_timeout_8byte_start(void);			// 启动uart1的8字节超时判断定时器
void uart1_timeout_8byte_stop(void);			// 停止uart1的8字节超时判断定时器
void uart2_timeout_8byte_start(void);			// 启动uart2的8字节超时判断定时器
void uart2_timeout_8byte_stop(void);			// 停止uart2的8字节超时判断定时器
void uart3_timeout_8byte_start(void);			// 启动uart3的8字节超时判断定时器
void uart3_timeout_8byte_stop(void);			// 停止uart3的8字节超时判断定时器


void UART0_SendStr(unsigned char str[],unsigned int ulNum);
void UART0_SendByte(uint8 dat);
void UART0_Init(uint32 Baud_Rate, uint8 channel); //channel = 0(P0.2, P0.3)

void UART1_SendStr(unsigned char str[],unsigned int ulNum);
void UART1_SendByte(uint8 dat);
void UART1_Init(uint32 Baud_Rate, uint8 channel); //channel = 0(P0.15, P0.16), channel = 1(P2.0, P2.1)

void UART2_SendStr(unsigned char str[],unsigned int ulNum);
void UART2_SendByte(uint8 dat);
void UART2_Init(uint32 Baud_Rate, uint8 channel); //channel = 0(P0.10, P0.11), channel = 1(P2.8, P2.9)

void UART3_SendStr(unsigned char str[],unsigned int ulNum);
void UART3_SendByte(uint8 dat);
void UART3_Init(uint32 Baud_Rate, uint8 channel); //channel = 0(P0.0, P0.1),   channel = 1(P0.25, P0.26), channel = 2(P4.28, P4.29)

//****************************************************************


#endif




