//******************************************************************************************************
//*                                UART����ģ�� for LPC1768											   *
//******************************************************************************************************
#ifndef _UART_H_
#define _UART_H_

#include "main.h"

//********************       ����궨��         *****************
//			����UART���ճ�ʱӦ��    8�ֽ�/N��N>8���ֽ�
//**************************** UART0  ***************************
#define UART0_RCVBUF_N

//**************************** UART1  ***************************
#define UART1_RCVBUF_N

//**************************** UART2  ***************************
#define UART2_RCVBUF_N

//**************************** UART3  ***************************
#define UART3_RCVBUF_N

// UART0��ʱ���ձ���
extern uint8  uart0_timeout_start;			        // 8�ֽں�ĳ�ʱ�жϼ�����������־
extern uint8  uart0_timeout_cnt;			        // 8�ֽڳ�ʱ������
extern uint8  uart0_temp[MAX_UART0_BYTE_NUM];    // UART0���ջ�����
extern uint16 uart0_rcv_counter;			        // UART0�յ����ݸ�������ֵ


// UART1��ʱ���ձ���
extern uint8  uart1_timeout_start;			        // 8�ֽں�ĳ�ʱ�жϼ�����������־
extern uint8  uart1_timeout_cnt;			        // 8�ֽڳ�ʱ������
extern uint8  uart1_temp[MAX_UART1_BYTE_NUM];    // UART1���ջ�����
extern uint16 uart1_rcv_counter ;			        // UART1�յ����ݸ�������ֵ

// UART2��ʱ���ձ���
extern uint8  uart2_timeout_start;			        // 8�ֽں�ĳ�ʱ�жϼ�����������־
extern uint8  uart2_timeout_cnt ;			        // 8�ֽڳ�ʱ������
extern uint8  uart2_temp[MAX_UART2_BYTE_NUM];    // UART2���ջ�����
extern uint16 uart2_rcv_counter;			        // UART2�յ����ݸ�������ֵ

// UART3��ʱ���ձ���
extern uint8  uart3_timeout_start;			        // 8�ֽں�ĳ�ʱ�жϼ�����������־
extern uint8  uart3_timeout_cnt;			        // 8�ֽڳ�ʱ������
extern uint8  uart3_temp[MAX_UART3_BYTE_NUM];    // UART3���ջ�����
extern uint16 uart3_rcv_counter;			        // UART3�յ����ݸ�������ֵ



// �ṩ�ⲿӦ�ú���,main.c�н���һ��Task_TIME����
void TIME_PROCESS(void);

void uart0_timeout_8byte_start(void);			// ����uart0��8�ֽڳ�ʱ�ж϶�ʱ��
void uart0_timeout_8byte_stop(void);			// ֹͣuart0��8�ֽڳ�ʱ�ж϶�ʱ��
void uart1_timeout_8byte_start(void);			// ����uart1��8�ֽڳ�ʱ�ж϶�ʱ��
void uart1_timeout_8byte_stop(void);			// ֹͣuart1��8�ֽڳ�ʱ�ж϶�ʱ��
void uart2_timeout_8byte_start(void);			// ����uart2��8�ֽڳ�ʱ�ж϶�ʱ��
void uart2_timeout_8byte_stop(void);			// ֹͣuart2��8�ֽڳ�ʱ�ж϶�ʱ��
void uart3_timeout_8byte_start(void);			// ����uart3��8�ֽڳ�ʱ�ж϶�ʱ��
void uart3_timeout_8byte_stop(void);			// ֹͣuart3��8�ֽڳ�ʱ�ж϶�ʱ��


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




