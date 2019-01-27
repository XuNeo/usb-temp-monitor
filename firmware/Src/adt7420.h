#ifndef _ADT7420_H_
#define _ADT7420_H_
#include "stdint.h"

#define ADT7420_ADDR (0x48<<1)
void adt7420_init(void);
float adt7420_read_temp(void);

#endif
