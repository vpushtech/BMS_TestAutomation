/*
 * RTOS_SoCEstimationThread_entry.c
 *
 *  Created on: 07-Oct-2025
 *      Author: RushikeshNitinKamble
 */

#include <vpush_rtos.h>
void RTOS_SoCEstimationThread_entry(void *pvParameters)
{
    (void) pvParameters;
    TickType_t LastwaitTime = xTaskGetTickCount();
    for (;;)
    {

    	RTOS_TaskTimingStart_st(RTOS_SoCEstimation);
    	RTOS_TaskTimingEnd_st(RTOS_SoCEstimation);
    	RTOS_PeakStackUsed_st(RTOS_SoCEstimation);
    	RTOS_TaskDelayUntil_st(&LastwaitTime , 1000);
    }
 }
