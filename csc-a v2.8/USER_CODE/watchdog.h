#ifndef __WDT_H 
#define __WDT_H

#include "config.h"

//参数为看门狗复位的时间, 单位为毫秒(ms)
void wdt_init(uint32 dog_time);
void wdt_feed(void);

#endif
