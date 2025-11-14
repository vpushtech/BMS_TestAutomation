/*
 * RTOS_WDGThread_entry.c
 *
 *  Created on: 07-Oct-2025
 *      Author: RushikeshNitinKamble
 */

#include <vpush_rtos.h>
extern APP_TaskTiming_tst taskTiming_St ;
U32 taskCount=0;
void RTOS_WDGThread_entry(void *pvParameters) {
	(void) pvParameters;
	TickType_t LastwaitTime = xTaskGetTickCount();

	for (;;)
	{
		 RTOS_TaskTimingStart_st(RTOS_WDT);

		 if(taskTiming_St.RTOS_BMS_StateProcessing.taskCount>=5&&
			taskTiming_St.RTOS_DataProcessing.taskCount>=2 &&
			taskTiming_St.RTOS_SoCEstimation.taskCount>=20)
		 {
			 taskTiming_St.RTOS_BMS_StateProcessing.taskCount=0;
			 taskTiming_St.RTOS_DataProcessing.taskCount=0;
			 taskTiming_St.RTOS_SoCEstimation.taskCount=0;
			 taskCount++;
		 }
		 RTOS_TaskTimingEnd_st(RTOS_WDT);
		 RTOS_PeakStackUsed_st(RTOS_WDT);
		RTOS_TaskDelayUntil_st(&LastwaitTime, 50);
	}
}



