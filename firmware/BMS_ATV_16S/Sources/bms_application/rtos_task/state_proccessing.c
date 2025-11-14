/*
 * RTOS_BMS_StateProccessingThread_entry.c
 *
 *  Created on: 08-Oct-2025
 *  Author: RushikeshNitinKamble
 *  Description: BMS State Processing Thread Implementation
 *  Version: 1.0
 *  Modification History:
 *  Date            Author          Description
 *  ----------------------------------------------------------------------------
 *  08-Oct-2025     Rushikesh       Initial BMS State Machine Implementation
 *  10-Oct-2025     Rushikesh       Added Fault Handling and State Transitions
 *  12-Oct-2025     Rushikesh       Integrated CAN Communication and AFE Faults
 ******************************************************************************/

#include <app_timer.h>
#include <bms_protection.h>
#include <can_comm.h>
#include <hsm.h>
#include <vpush_rtos.h>
#include"automation.h"
/* ==================== STATIC FUNCTION DECLARATIONS ==================== */
/* ==================== EXTERNAL VARIABLE DECLARATIONS ==================== */

/* ==================== STATIC VARIABLE DEFINITIONS ==================== */

AFE_Status_ten status_en=AFE_STATUS_ERROR;
extern APP_criticalNonCriticalFault_tst APP_criticalNonCriticalFault_st;
extern Response_automationFramework_tst Responsedata_gst;
/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : RTOS_BMS_StateProccessingThread_entry
*   Description   : Main entry function for BMS State Processing Thread
*                   Handles BMS state machine transitions and fault monitoring
*   Parameters    : pvParameters - RTOS task parameters (unused)
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void RTOS_BMS_StateProccessingThread_entry(void *pvParameters)
{
    (void) pvParameters;
    TickType_t LastwaitTime= xTaskGetTickCount();
	U8 retryCounts_u8 = AFE_MAXCMDRETIES;

	U8 isConfigSuccFlag_u8 = 0;
    /* Suspend all other tasks during initialization */
    RTOS_TaskSuspend_st(RTOS_DataProcessing);
    RTOS_TaskSuspend_st(RTOS_SoCEstimation);
    RTOS_TaskSuspend_st(RTOS_QueueReceive);
    RTOS_TaskSuspend_st(RTOS_WDT);
	do {
		status_en = AFE_STATUS_SUCCESS;//AFE_init_mv();
		if (AFE_STATUS_SUCCESS == status_en) {
			isConfigSuccFlag_u8 = 0x01;
		} else {
			BSP_I2C_BusFaultRecover_gv();
			retryCounts_u8--;
		}
	} while (retryCounts_u8 && !isConfigSuccFlag_u8);
	BSP_DRV_Config_gv();
	    /* Set initial battery state and resume all tasks */
		AFE_FetControl_gv(AFE_FET_ALL_OFF);
		RTOS_TaskResume_st(RTOS_DataProcessing);
		RTOS_TaskResume_st(RTOS_SoCEstimation);
		RTOS_TaskResume_st(RTOS_QueueReceive);
		RTOS_TaskResume_st(RTOS_WDT);
	/* Initialize Idle State */
		HSM_Init_gen();
    /* Main task loop */
    for (;;)
    {

    	 RTOS_TaskTimingStart_st(RTOS_BMS_StateProcessing);
        /* Read current fault status from BMS and AFE */
    	 APP_criticalNonCriticalFault_st.APP_RecoverableFault_u16= APP_GetRecoverableFaults_gen();
    	 APP_criticalNonCriticalFault_st.APP_NotRecoverableFault_u16= APP_GetNonRecoverableFaults_gen();
        /* Process all state transitions */
    	 //TEST_ReadTestCases_gv();
    	 HSM_Process_gen();
    	 Responsedata_gst.TEST_datatype_u16=APP_criticalNonCriticalFault_st.APP_RecoverableFault_u16;
    	 Responsedata_gst.TEST_datatype_i16=APP_criticalNonCriticalFault_st.APP_RecoverableFault_u16;
    	 Responsedata_gst.TEST_datatype_f32=APP_criticalNonCriticalFault_st.APP_RecoverableFault_u16;
    	 if(input_data1 == 0x01)
    	 {
    		 Test_ResponseFrame_gv(&Responsedata_gst);
    		 input_data1 = 0x00;
    	 }
        RTOS_TaskTimingEnd_st(RTOS_BMS_StateProcessing);
        RTOS_PeakStackUsed_st(RTOS_BMS_StateProcessing);
        /* Wait for next cycle with precise timing */
    	RTOS_TaskDelayUntil_st(&LastwaitTime, 250);
    }
}
