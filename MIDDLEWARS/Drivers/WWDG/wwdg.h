/* ../MIDDLEWARS/Drivers/WWDG/wwdg.h */
#ifndef __WWDG_H__
#define __WWDG_H__

#include "../SYSTEM/sys/sys.h"

#define WWDG_CLK_ENABLE()   do { __HAL_RCC_WWDG_CLK_ENABLE(); } while(0)

void wwdg_init(uint32_t fprer, uint8_t wr, uint8_t tr);

#endif /* __WWDG_H__ */
