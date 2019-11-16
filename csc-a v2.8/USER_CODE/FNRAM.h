//**************************************************************************************
//							��ͷ�ļ�����FM24L256  for LPC1768
//									I2C����洢��
//**************************************************************************************
#ifndef _FNRAM_
#define _FNRAM_

#include "config.h"
/************************************************************************/
//���Ŷ���
//�洢оƬ FM24L256
//����ַ 0x7FFF ~ 0x0000
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

#define FM_ADDRESS      0xA0   //����洢����IIC����������ַ

#define WRITE_FM_ENABLE  CLR_WP
#define WRITE_FM_DISABLE SET_WP

void FNRAM_init(void);  //�ܽų�ʼ��

/*******************************************************************
                     ���ӵ�ַ�����ֽ����ݺ���                                    
����: ���������ߵ����͵�ַ��д���ݣ��������ߵ�ȫ����,��������ַsla.
      �������1��ʾ�����ɹ��������������
********************************************************************/
uint8 ISendByte(uint8 sla,uint8 c);

/*******************************************************************
                    ���ӵ�ַ���ֽ����ݺ���               
����: ���������ߵ����͵�ַ�������ݣ��������ߵ�ȫ����,��������
      ַsla������ֵ��c.
      �������1��ʾ�����ɹ��������������
********************************************************************/
uint8 IRcvByte(uint8 sla,uint8 *c);

//���ͣ�������ַ�� �洢�ߣ����ֽڵ�ַ��д���ݿ�ͷָ�룬����
uint8 ISendStr(uint8 sla, uint8 subah, uint8 subal, uint8 *s,uint8 no);

//���գ�������ַ�� �洢�ߣ����ֽڵ�ַ�������ݿ�ͷָ�룬����
uint8 IRcvStr(uint8 sla,uint8 subah, uint8 subal, uint8 *s,uint8 no);

#endif





