#include "watchdog.h"

void wdt_init(uint32 dog_time)  //单位：ms
{
	LPC_WDT->WDTC   = dog_time * 1000;			// 设置看门狗定时器参数
	LPC_WDT->WDMOD  = 0x03;						// 设置看门狗模式：中断且复位
	LPC_WDT->WDFEED = 0xAA;						// 第一次喂狗启动WDT
	LPC_WDT->WDFEED = 0x55;
}

void wdt_feed(void)
{
	OS_ENTER_CRITICAL();
	LPC_WDT->WDFEED = 0xAA;						// 喂狗WDT
	LPC_WDT->WDFEED = 0x55;
	OS_EXIT_CRITICAL();
}

