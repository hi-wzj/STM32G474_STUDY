/* ../MIDDLEWARS/Drivers/EXTI/exti.h */
#ifndef __EXTI_H__
#define __EXTI_H__

#include "../SYSTEM/sys/sys.h"

/* GPIO10~GPIO15共用一个中断线以及中断服务函数，所以只定义一个 */
#define KEY_INT_IRQn        EXTI15_10_IRQn
#define KEY_INT_IRQHandler  EXTI15_10_IRQHandler

void extix_init(void);

#endif /* __EXTI_H__ */
