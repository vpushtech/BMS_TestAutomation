/*
 * RTOS_250msThread_entry.c
 *
 *  Created on: 01-Sep-2025
 *      Author: RushikeshNitinKamble
 */

/* ==================== INCLUDES ==================== */
#include <afe_data_proccessing.h>
#include <bms_protection.h>
#include <can_comm.h>
#include <vpush_rtos.h>
#include"automation.h"
/* ==================== EXTERN VARIABLES ==================== */
extern APP_GlobleProtectionLevelFaults_tst APP_GlobleProtectionLevelFaults_St;
extern AFE_Status_tst AFE_Status_st;
/* ==================== STATIC VARIABLES ==================== */

/* ==================== STATIC FUNCTION DECLARATIONS ==================== */
static void BMS_DataProccessing_mv(void);

static void APP_BMS_InpurDataUpdate_gv(void);
/* ==================== THREAD ENTRY FUNCTION ==================== */

/* -----------------------------------------------------------------------------
 *  Function Name : RTOS_DataProcessingThread_entry
 *  Description   : Main entry point for Data Processing Thread
 *                  - Initializes BMS drivers and configuration
 *                  - Performs periodic data processing and safety checks
 *                  - Handles CAN communication and AFE monitoring
 *  Parameters    : pvParameters - RTOS task parameters (unused)
 *  Return Value  : None
 * ---------------------------------------------------------------------------*/
void RTOS_DataProcessingThread_entry(void *pvParameters)
{
    (void) pvParameters;

    for (;;)
    {
    	 RTOS_TaskTimingStart_st(RTOS_DataProcessing);
        /* BMS Data processing  */
       // BMS_DataProccessing_mv();
       // APP_BMS_InpurDataUpdate_gv();

        /* Safety checks - primary and secondary levels */
        APP_PrimarySafetyCheck_Func_gv();
        APP_SecondarySafetyCheck_Func_gv();
        /* CAN communication handling */
        CAN_Massages_Tranmission();

        RTOS_TaskTimingEnd_st(RTOS_DataProcessing);
        RTOS_PeakStackUsed_st(RTOS_DataProcessing);
        RTOS_TaskDelay_st(100);
    }
}

/* ==================== STATIC FUNCTIONS ==================== */

/* -----------------------------------------------------------------------------
 *  Function Name : BMS_DataProccessing_mv
 *  Description   : Handles BMS data processing and AFE communication
 *                  - Reads AFE alarm and safety status with mutex protection
 *                  - Reads detailed measurements if alarms are present
 *                  - Handles AFE communication faults and recovery
 *  Parameters    : None
 *  Return Value  : None
 * ---------------------------------------------------------------------------*/
static void BMS_DataProccessing_mv(void)
{
	if (RTOS_SemaphoreTake_st(I2C_MUTEX, portMAX_DELAY) == RTOS_STATUS_SUCCESS)
    {
        /* Read AFE alarm and safety status */
        AFE_ReadAllAlarmStatus_gv();
        AFE_ReadAllSafetyAlertStatus_gv();
        if (APP_CHECK_BIT(AFE_Status_st.APP_AlarmStatusValue_vu16, 7))
        {
            AFE_ReadAllSystemMeasurements_gv();
            AFE_ReadAllTemperatures_gv();
            AFE_FETStatusRead_gv();
            AFE_ReadAllDAStatus_gv();
        }

        RTOS_SemaphoreGive_st(I2C_MUTEX);
    }
}


/* ==================== INPUT DATA UPDATE FUNCTIONS ==================== */

static void APP_BMS_InpurDataUpdate_gv(void)
{
    APP_GlobleProtectionLevelFaults_St.APP_InputMeasument_St[APP_SECONDARY_TEMP_ALERT].APP_MaxInputData_i16 =AFE_InputeMeasument_St.AFE_Temperature_St.AFE_Temp_st.AFE_Internal_Temp_f64 ;
    APP_GlobleProtectionLevelFaults_St.APP_InputMeasument_St[APP_SECONDARY_TEMP_ALERT].APP_MinInputData_i16 = AFE_InputeMeasument_St.AFE_Temperature_St.AFE_Temp_st.AFE_Internal_Temp_f64;
    APP_GlobleProtectionLevelFaults_St.APP_InputMeasument_St[APP_SECONDARY_VOLT_ALERT].APP_MaxInputData_i16 =AFE_InputeMeasument_St.AFE_Voltage_St.AFE_MaxCellVoltage_u16;
    APP_GlobleProtectionLevelFaults_St.APP_InputMeasument_St[APP_SECONDARY_VOLT_ALERT].APP_MinInputData_i16 = AFE_InputeMeasument_St.AFE_Voltage_St.AFE_MinCellVoltage_u16;
    APP_GlobleProtectionLevelFaults_St.APP_InputMeasument_St[APP_SECONDARY_CURRENT_ALERT].APP_MaxInputData_i16 = AFE_InputeMeasument_St.AFE_Current_st.AFE_CC2_Current_i16;
    APP_GlobleProtectionLevelFaults_St.APP_InputMeasument_St[APP_SECONDARY_CURRENT_ALERT].APP_MinInputData_i16 = 0;
}

