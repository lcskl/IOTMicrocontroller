#ifndef INCLUDE_TIME_H_
#define INCLUDE_TIME_H_

#include "xparameters.h"
#include "xtmrctr.h"


int initTime(u16 DeviceId, u8 CounterNuber);
u32 getCountTime();
int millis();


#endif /* INCLUDE_TIME_H_ */
