#include "watchdog.h"

void wdt_init(uint32 dog_time)  //��λ��ms
{
	LPC_WDT->WDTC   = dog_time * 1000;			// ���ÿ��Ź���ʱ������
	LPC_WDT->WDMOD  = 0x03;						// ���ÿ��Ź�ģʽ���ж��Ҹ�λ
	LPC_WDT->WDFEED = 0xAA;						// ��һ��ι������WDT
	LPC_WDT->WDFEED = 0x55;
}

void wdt_feed(void)
{
	OS_ENTER_CRITICAL();
	LPC_WDT->WDFEED = 0xAA;						// ι��WDT
	LPC_WDT->WDFEED = 0x55;
	OS_EXIT_CRITICAL();
}

