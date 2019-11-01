
#include "time.h"

XTmrCtr TimerCounter;
u8 Counter;



int initTime(u16 DeviceId, u8 CounterNuber){

	int Status;
	XTmrCtr *TmrCtrInstancePtr = &TimerCounter;

	Counter = CounterNuber;

	/*
	* Initialize the TmrCtr driver so that it iss ready to use
	*/
	Status = XTmrCtr_Initialize(TmrCtrInstancePtr, DeviceId);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}
	XTmrCtr_SetResetValue(TmrCtrInstancePtr, Counter, 0x00);

	XTmrCtr_Start(TmrCtrInstancePtr, Counter);

	return XST_SUCCESS;
}

u32 getCountTime(){
	u32 clock_cnt;
	clock_cnt = XTmrCtr_GetValue(&TimerCounter, Counter);
	return clock_cnt;
}

int millis(){
	u32 clock_cnt;
	clock_cnt = XTmrCtr_GetValue(&TimerCounter, Counter);
	return (clock_cnt*((float)1000U/XPAR_AXI_TIMER_0_CLOCK_FREQ_HZ));

	//return (clock_cnt/100000U);
}

