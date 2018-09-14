#ifndef UBW_INCLUDE_DEBUG_H_
#define UBW_INCLUDE_DEBUG_H_

#ifndef DISABLE_DEBUG
#include "soc.h"
#define DEBUG(v)  GPIO_NUM = (v)
#else
#define DEBUG(v)  ((void)0)
#endif

#endif // UBW_INCLUDE_DEBUG_H_
