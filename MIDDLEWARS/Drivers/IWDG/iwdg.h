/* ../MIDDLEWARS/Drivers/IWDG/iwdg.h */
#ifndef __IWDG_H__
#define __IWDG_H__

#include "../SYSTEM/sys/sys.h"

void iwdg_init(uint8_t prer, uint16_t rlr);
void iwdg_feed(void);

#endif /* __IWDG_H__ */