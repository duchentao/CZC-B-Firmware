#ifndef __WDT_H 
#define __WDT_H

#include "config.h"

//����Ϊ���Ź���λ��ʱ��, ��λΪ����(ms)
void wdt_init(uint32 dog_time);
void wdt_feed(void);

#endif
