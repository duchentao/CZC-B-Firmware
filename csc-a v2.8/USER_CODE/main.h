#ifndef __MAIN_H_ 
#define __MAIN_H_

#include "config.h"						
#include "IRQ.h"

#define PI  3.14159265358979

#define UART0_BAUD_RATE   38400    //UART0������
#define UART1_BAUD_RATE   9600      //UART1������   
#define UART2_BAUD_RATE   9600      //UART2������   �˴�����ΪPARTOL_DM
#define UART3_BAUD_RATE   9600      //UART3������

//*************** UART0 ~ UART3 ��ʱ����N�ֽڽ��ճ�������  *******
// ����ӦUARTn���ڽ���δ�ã����޸�Ϊһ����С��ֵ����ʡ�洢�ռ�
#define MAX_UART0_BYTE_NUM		12				// UART�����ַ�������ֽ���N
#define MAX_UART1_BYTE_NUM		12				// UART�����ַ�������ֽ���N
#define MAX_UART2_BYTE_NUM		12				// UART�����ַ�������ֽ���N
#define MAX_UART3_BYTE_NUM		12				// UART�����ַ�������ֽ���N


// վ��ַ
#define SLAVE_ADDR0			0x01						// ��վ��ַ
#define MAX_REG_SIZE0		10							// ���Ĵ�����word
#define MAX_BUFFER_SIZE0	(MAX_REG_SIZE0 * 2)			// ��������ֽ���Byte = word * 2


#define MAX_REG_SIZE1		64							// ���Ĵ�����word
#define MAX_BUFFER_SIZE1	(MAX_REG_SIZE1 * 2)			// ��������ֽ���Byte = word * 2

#define SLAVE_ADDR2			0x05						// ��վ��ַ
#define MAX_REG_SIZE2		8							// ���Ĵ�����word
#define MAX_BUFFER_SIZE2	(MAX_REG_SIZE2 * 2)			// ��������ֽ���Byte = word * 2

#define SLAVE_ADDR3			0x06						// ��վ��ַ
#define MAX_REG_SIZE3		8							// ���Ĵ�����word
#define MAX_BUFFER_SIZE3	(MAX_REG_SIZE3 * 2)			// ��������ֽ���Byte = word * 2



#define CAN0_SEND_OUTTIME  0         //��������Ϊ125Kʱ��ֻ��1ֻ����ʱ��Ϊ6000��ʵ����Ϊ3400�ο��Է��꣬����ȡ����2��������
#define CAN1_SEND_OUTTIME  0         //�������㷽ʽΪ������ֻ��������(125K/������)�ı���

//�ж����ȼ�,���֧��32�����ȼ�
#define INT_CAN_P         8         //CAN
#define INT_UART0_P       7         //UART0
#define INT_UART1_P       12        //UART1
#define INT_UART2_P       6         //UART2
#define INT_UART3_P       24        //UART3


//*****************************************�궨��***********************************************
#define	FLASH_05S_DELAY_TIME	1			// 1*500=500ms,0.5s
#define	FLASH_1S_DELAY_TIME		2			// 2*500=1000ms,1s
#define	FLASH_2S_DELAY_TIME		4			// 4*500=2000ms,2s


#define	BTN_NOICE_TIME			5			// 5*10=50ms
#define	BTN_RST_TIME			5			// 5*10=50ms

#define	IO_IN_NUM			    30		    // IO������Ŀ
#define	NOICE_TIME			    5           // io_scan();�����е��˲�ʱ�������5*10=50ms


#define IO_INPUT_NUM		13
#define IO_OUTPUT_NUM		13




typedef struct _parameter
{
	uint32 input_nc;	
	uint32 output_nc;
	
	
}parameter;
extern struct _parameter param;
#endif

