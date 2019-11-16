/****************************************Copyright (c)****************************************************
**                            Guangzhou ZHIYUAN electronics Co.,LTD.
**                                      
**                                 http://www.embedtools.com
**
**--------------File Info---------------------------------------------------------------------------------
** File name:               os_cpu.h
** Latest modified Date:    2009-07-24
** Latest Version:          1.0
** Descriptions:            ��COS-II��Cortex-M3�ϵ���ֲ����CPU���ò��֣���realview����
**
**--------------------------------------------------------------------------------------------------------
** Created by:              Chenmingji
** Created date:            2009-07-24
** Version:                 1.0
** Descriptions:            The original version
**
**--------------------------------------------------------------------------------------------------------
** Modified by:             
** Modified date:           
** Version:                 
** Descriptions:            
**
*********************************************************************************************************/
#ifndef __OS_CPU_H
#define __OS_CPU_H

/*********************************************************************************************************
  ������������޹ص���������
*********************************************************************************************************/
typedef unsigned char  BOOLEAN;                                         /*  ��������                    */
typedef unsigned char  INT8U;                                           /*  �޷���8λ���ͱ���           */
typedef signed   char  INT8S;                                           /*  �з���8λ���ͱ���           */
typedef unsigned short INT16U;                                          /*  �޷���16λ���ͱ���          */
typedef signed   short INT16S;                                          /*  �з���16λ���ͱ���          */
typedef unsigned long  INT32U;                                          /*  �޷���32λ���ͱ���          */
typedef signed   long  INT32S;                                          /*  �з���32λ���ͱ���          */
typedef float          FP32;                                            /*  �����ȸ�������32λ���ȣ�    */
typedef double         FP64;                                            /*  ˫���ȸ�������64λ���ȣ�    */

typedef INT32U         OS_STK;                                          /*  ��ջ��32λ���              */


/************************************   modified by Tangyejun   *****************************************/
typedef unsigned char  uint8;                   // �޷���8λ���ͱ���  
typedef signed   char  int8;                    // �з���8λ���ͱ���  
typedef unsigned short uint16;                  // �޷���16λ���ͱ���
typedef signed   short int16;                   // �з���16λ���ͱ��� 
typedef unsigned int   uint32;                  // �޷���32λ���ͱ��� 
typedef signed   int   int32;                   // �з���32λ���ͱ��� 
typedef float          fp32;                    // �����ȸ�������32λ���ȣ� 
typedef double         fp64;                    // ˫���ȸ�������64λ���ȣ� 
//#define GLOBAL	 extern	



/*********************************************************************************************************
  ��ϵ�ṹ��ص�һЩ����
*********************************************************************************************************/
__swi(0x00) void OsSwiHandle1(int Handle);

#define OS_CRITICAL_METHOD      2                                       /*  ѡ�񿪡����жϵķ�ʽ        */
#define OS_STK_GROWTH           1                                       /*  ��ջ�Ǵ������³���          */

#define OS_TASK_SW()            OsSwiHandle1(0)                         /*  ���������л�����          */
#define OS_ENTER_CRITICAL()     OsSwiHandle1(2)                         /*  ���ж�                      */
#define OS_EXIT_CRITICAL()      OsSwiHandle1(3)                         /*  ���ж�                      */

#define NVIC_SYS_PRI2           (*((volatile INT32U *)0xE000ED1C))
#define NVIC_SYS_PRI3           (*((volatile INT32U *)0xE000ED20))
#define SysNVICInit()            NVIC_SYS_PRI2 |=  0x00000000;\
                                 NVIC_SYS_PRI3 |=  0x80FF0000           //11 scvcall prio 0x00
                                                                        //15 systick prio 0x80 14 pendsv prio 0xFF

/*********************************************************************************************************
** Function name:           __s_changeToSYSMode
** Descriptions:            �����л���ϵͳģʽ
** input parameters:        none
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
#define changeToSYSMode()       OsSwiHandle1(4)

/*********************************************************************************************************
** Function name:           changeToUSRMode
** Descriptions:            �����л����û�ģʽ
** input parameters:        none
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
extern __asm void changeToUSRMode(void);

/*********************************************************************************************************
** Function name:           __s_OSCtxSw
** Descriptions:            uC/OS-II������Ⱥ���
** input parameters:        none
** output parameters:       none
** Returned value:          none
** Created by:              chenmingji
** Created Date:            2009-07-24
**--------------------------------------------------------------------------------------------------------
** Modified by:
** Modified date:
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
extern __asm void __s_OSCtxSw(void);

/*********************************************************************************************************
** Function name:       __sysTickIsr
** Descriptions:        ϵͳ�����ж�
** input parameters:    ��
** output parameters:   ��
** Returned value:      ��
*********************************************************************************************************/
extern void __sysTickIsr(void);

/*********************************************************************************************************
** Function name:           svcallHandle
** Descriptions:            svcall�������
** input parameters:        uiHandle:ѡ���ӹ���
** output parameters:       none
** Returned value:          none
*********************************************************************************************************/
extern __asm void svcallHandle(unsigned int uiHandle);

#endif                                                                  /*  __OS_CPU_H                  */

/*********************************************************************************************************
  END FILE
*********************************************************************************************************/
