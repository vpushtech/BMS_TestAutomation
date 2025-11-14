/*
  * bq76952_afe.c
 *
 *  Description     : AFE Driver for bq76952 Battery Monitor
 *  Author          : Rushikesh
 *  Created On      : 08-Aug-2025
 *  Version         : 1.0
 *  Modification History:
 *  Date        Author      Description
 *  ----------------------------------------------------------------------------
 *  08-Aug-2025 RUSHIKESH   AFE Architecture Implementation
 *  09-Aug-2025 RUSHIKESH   AFE Reading Parameter Like Voltages,Current,Temperature
 *  11-Aug-2025 RUSHIKESH   AFE Power Mode,Alert,Status Implementation
 *  12-Aug-2025 RUSHIKESH   getFaulstatus,Some Architecture  specific coding standard followed correctness
 ******************************************************************************/
/* ==================== INCLUDE FILES ==================== */
#ifndef AFE_CONFIG_BQ76952_AFE_C_
#define AFE_CONFIG_BQ76952_AFE_C_
/* ==================== INCLUDE FILES ==================== */
#include <afe_config.h>
#include <afe_settings.h>
#include"afe_data_proccessing.h"
/* ==================== GLOBAL VARIABLES ==================== */
/* ==================== STATIC VARIABLES==================== */

/* ==================== EXTERN VARIABLES ==================== */

/* ==================== THRESHOLD CONFIGURATION DATA ==================== */
const AFE_Config_tst AFE_ThresholdConfigTable_mst[] = {
    /* Cell Undervoltage Protection */
    {AFE_CUV_THRESHOLD_ADDR, AFE_CUV_THR_REG_VALUE, AFE_ONEBYTE},
    {AFE_CUV_DELAY_ADDR, AFE_CUV_DELAY_REG_VALUE, AFE_TWOBYTE},
    {AFE_CUV_RECOVERY_HYSTERESIS_ADDR, AFE_CUV_HYS_REG_VALUE, AFE_ONEBYTE},

    /* Cell Overvoltage Protection */
    {AFE_COV_THRESHOLD_ADDR, AFE_COV_THR_REG_VALUE, AFE_ONEBYTE},
    {AFE_COV_DELAY_ADDR, AFE_COV_DELAY_REG_VALUE, AFE_TWOBYTE},
    {AFE_COV_RECOVERY_HYSTERESIS_ADDR, AFE_COV_HYS_REG_VALUE, AFE_ONEBYTE},

    /* Cell Overvoltage Latch */
    {AFE_COVL_LATCH_LIMIT_ADDR, AFE_COVL_LMTCNT_VALUE, AFE_ONEBYTE},
    {AFE_COVL_CNTR_DEC_DELAY_ADDR, AFE_COVL_LMTDECDEL_VALUE, AFE_ONEBYTE},
    {AFE_COVL_RECOVERY_TIME_ADDR, AFE_COVL_LMTRECOVERY_VALUE, AFE_ONEBYTE},

    /* Overcurrent Charge Protection */
    {AFE_OCC_THRESHOLD_ADDR, AFE_OCC_THR_REG_VALUE, AFE_ONEBYTE},
    {AFE_OCC_DELAY_ADDR, AFE_OCC_DELAY_REG_VALUE, AFE_ONEBYTE},
    {AFE_OCC_RECOVERY_THRESHOLD_ADDR, AFE_OCC_HYS_REG_VALUE, AFE_TWOBYTE},
    {AFE_OCC_PACK_TOS_DELTA_ADDR, AFE_OCC_PACKTOS_REG_VALUE, AFE_TWOBYTE},

    /* Overcurrent Discharge Protection - Tier 1 */
    {AFE_OCD1_THRESHOLD_ADDR, AFE_OCD1_THR_REG_VALUE, AFE_ONEBYTE},
    {AFE_OCD1_DELAY_ADDR, AFE_OCD1_DELAY_REG_VALUE, AFE_ONEBYTE},

    /* Overcurrent Discharge Protection - Tier 2 */
    {AFE_OCD2_THRESHOLD_ADDR, AFE_OCD2_THR_REG_VALUE, AFE_ONEBYTE},
    {AFE_OCD2_DELAY_ADDR, AFE_OCD2_DELAY_REG_VALUE, AFE_ONEBYTE},

    /* Short Circuit Protection */
    {AFE_SCD_THRESHOLD_ADDR, AFE_SCD_THR_REG_VALUE, AFE_ONEBYTE},
    {AFE_SCD_DELAY_ADDR, AFE_SCD_DELAY_REG_VALUE, AFE_ONEBYTE},
    {AFE_SCD_RECOVERY_TIME_ADDR, AFE_SCD_HYS_REG_VALUE, AFE_ONEBYTE},

    /* Overcurrent Discharge Protection - Tier 3 */
    {AFE_OCD3_THRESHOLD_ADDR, AFE_OCD3_THR_REG_VALUE, AFE_TWOBYTE},
    {AFE_OCD3_DELAY_ADDR, AFE_OCD3_DELAY_REG_VALUE, AFE_ONEBYTE},
    {AFE_OCD_RECOVERY_THRESHOLD_ADDR, AFE_OCD_HYS_REG_VALUE, AFE_TWOBYTE},

    /* Overcurrent Discharge Latch */
    {AFE_OCDL_LATCH_LIMIT_ADDR, AFE_OCDL_LMTCNT_VALUE, AFE_ONEBYTE},
    {AFE_OCDL_CNTR_DEC_DELAY_ADDR, AFE_OCDL_LMTDECDEL_VALUE, AFE_ONEBYTE},
    {AFE_OCDL_RECOVERY_TIME_ADDR, AFE_OCDL_LMTRECOVERY_VALUE, AFE_ONEBYTE},
    {AFE_OCDL_RECOVERY_THRESHOLD_ADDR, AFE_OCDL_HYS_REG_VALUE, AFE_TWOBYTE},

    /* Short Circuit Discharge Latch */
    {AFE_SCDL_LATCH_LIMIT_ADDR, AFE_SCDL_LMTCNT_VALUE, AFE_ONEBYTE},
    {AFE_SCDL_CNTR_DEC_DELAY_ADDR, AFE_SCDL_LMTDECDEL_VALUE, AFE_ONEBYTE},
    {AFE_SCDL_RECOVERY_TIME_ADDR, AFE_SCDL_LMTRECOVERY_VALUE, AFE_ONEBYTE},
    {AFE_SCDL_RECOVERY_THRESHOLD_ADDR, AFE_SCDL_HYS_REG_VALUE, AFE_TWOBYTE},

    /* Overtemperature Protection - Charge */
    {AFE_OTC_THRESHOLD_ADDR, AFE_OTC_THR_REG_VALUE, AFE_ONEBYTE},
    {AFE_OTC_DELAY_ADDR, AFE_OTC_DELAY_REG_VALUE, AFE_ONEBYTE},
    {AFE_OTC_RECOVERY_ADDR, AFE_OTC_HYS_REG_VALUE, AFE_ONEBYTE},

    /* Overtemperature Protection - Discharge */
    {AFE_OTD_THRESHOLD_ADDR, AFE_OTD_THR_REG_VALUE, AFE_ONEBYTE},
    {AFE_OTD_DELAY_ADDR, AFE_OTD_DELAY_REG_VALUE, AFE_ONEBYTE},
    {AFE_OTD_RECOVERY_ADDR, AFE_OTD_HYS_REG_VALUE, AFE_ONEBYTE},

    /* Overtemperature Protection - FET */
    {AFE_OTF_THRESHOLD_ADDR, AFE_OTF_THR_REG_VALUE, AFE_ONEBYTE},
    {AFE_OTF_DELAY_ADDR, AFE_OTF_DELAY_REG_VALUE, AFE_ONEBYTE},
    {AFE_OTF_RECOVERY_ADDR, AFE_OTF_HYS_REG_VALUE, AFE_ONEBYTE},

    /* Overtemperature Protection - Internal */
    {AFE_OTINT_THRESHOLD_ADDR, AFE_OTINT_THR_REG_VALUE, AFE_ONEBYTE},
    {AFE_OTINT_DELAY_ADDR, AFE_OTINT_DELAY_REG_VALUE, AFE_ONEBYTE},
    {AFE_OTINT_RECOVERY_ADDR, AFE_OTINT_HYS_REG_VALUE, AFE_ONEBYTE},

    /* Undertemperature Protection - Charge */
    {AFE_UTC_THRESHOLD_ADDR, AFE_UTC_THR_REG_VALUE, AFE_ONEBYTE},
    {AFE_UTC_DELAY_ADDR, AFE_UTC_DELAY_REG_VALUE, AFE_ONEBYTE},
    {AFE_UTC_RECOVERY_ADDR, AFE_UTC_HYS_REG_VALUE, AFE_ONEBYTE},

    /* Undertemperature Protection - Discharge */
    {AFE_UTD_THRESHOLD_ADDR, AFE_UTD_THR_REG_VALUE, AFE_ONEBYTE},
    {AFE_UTD_DELAY_ADDR, AFE_UTD_DELAY_REG_VALUE, AFE_ONEBYTE},
    {AFE_UTD_RECOVERY_ADDR, AFE_UTD_HYS_REG_VALUE, AFE_ONEBYTE},

    /* Undertemperature Protection - Internal */
    {AFE_UTINT_THRESHOLD_ADDR, AFE_UTINT_THR_REG_VALUE, AFE_ONEBYTE},
    {AFE_UTINT_DELAY_ADDR, AFE_UTINT_DELAY_REG_VALUE, AFE_ONEBYTE},
    {AFE_UTINT_RECOVERY_ADDR, AFE_UTINT_HYS_REG_VALUE, AFE_ONEBYTE},

    /* Protection Recovery and Watchdog */
    {AFE_RECOVERY_TIME_ADDR, AFE_PROT_RECOVERY_TIME_REG_VALUE, AFE_ONEBYTE},
    {AFE_HWD_DELAY_ADDR, AFE_PROT_WDG_TIME_REG_VALUE, AFE_TWOBYTE},
    {AFE_LD_ACTIVE_TIME_ADDR, AFE_PROT_LD_TIME_REG_VALUE, AFE_ONEBYTE},
    {AFE_LD_RETRY_DELAY_ADDR, AFE_PROT_LDRETRY_TIME_REG_VALUE, AFE_ONEBYTE},
    {AFE_LD_TIMEOUT_ADDR, AFE_PROT_LD_TIMEOUT_REG_VALUE, AFE_TWOBYTE},

    /* Precharge Timeout Protection */
    {AFE_PTO_CHARGE_THRESHOLD_ADDR, AFE_PTO_THR_REG_VALUE, AFE_TWOBYTE},
    {AFE_PTO_DELAY_ADDR, AFE_PTO_DELAY_REG_VALUE, AFE_TWOBYTE},
    {AFE_PTO_RESET_ADDR, AFE_PTO_RESET_REG_VALUE, AFE_TWOBYTE},


};

const size_t AFE_ThresholdConfigSize = sizeof(AFE_ThresholdConfigTable_mst) / sizeof(AFE_ThresholdConfigTable_mst[0]);
/* ==================== OTHER PROTECTION CONFIGURATION DATA ==================== */
const AFE_Config_tst AFE_OtherProtConfigTable_mst[] = {
    /* Alarm and Protection Masks */
    {AFE_ALARM_DEFAULT_MASK_ADDR, AFE_ALARM_DEFAULT_MASK_VALUE, AFE_TWOBYTE},
    {AFE_PF_ALERT_MASK_A_ADDR, AFE_PF_ALERT_MASK_VALUE, AFE_ONEBYTE},
    {AFE_PF_ALERT_MASK_B_ADDR, AFE_PF_ALERT_MASK_VALUE, AFE_ONEBYTE},
    {AFE_PF_ALERT_MASK_C_ADDR, AFE_PF_ALERT_MASK_VALUE, AFE_ONEBYTE},
    {AFE_PF_ALERT_MASK_D_ADDR, AFE_PF_ALERT_MASK_VALUE, AFE_ONEBYTE},

    /* Protection Enable Flags */
    {AFE_ENABLED_PF_A_ADDR, AFE_ENABLED_PF_VALUE, AFE_ONEBYTE},
    {AFE_ENABLED_PF_B_ADDR, AFE_ENABLED_PF_VALUE, AFE_ONEBYTE},
	{AFE_ENABLED_PF_D_ADDR, AFE_ENABLED_PF_VALUE, AFE_ONEBYTE},
    {AFE_ENABLED_PF_C_ADDR, AFE_ENABLED_PF_VALUE, AFE_ONEBYTE},
    /* FET Configuration */
    {AFE_FET_OPTIONS_ADDR, AFE_FET_OPTIONS_VALUE, AFE_ONEBYTE},

    /* Precharge Configuration */
    {AFE_PRECHARGE_START_VOLTAGE_ADDR, AFE_PRECHARGE_START_VOLTAGE, AFE_TWOBYTE},
    {AFE_PRECHARGE_STOP_VOLTAGE_ADDR, AFE_PRECHARGE_STOP_VOLTAGE, AFE_TWOBYTE},
    {AFE_PREDISCHARGE_TIMEOUT_ADDR, AFE_PRE_DHG_TIMEOUT, AFE_ONEBYTE},
    {AFE_PREDISCHARGE_STOP_DELTA_ADDR, AFE_PREDISCHARGE_STOP_DELTA_VALUE, AFE_ONEBYTE},

    /* Current Thresholds */
    {AFE_CHARGE_CURRENT_THRESHOLD_ADDR, AFE_CHG_CURRENT_THRESHOLD, AFE_TWOBYTE},
    {AFE_DISCHARGE_CURRENT_THRESHOLD_ADDR, AFE_DHG_CURRENT_THRESHOLD, AFE_TWOBYTE},
    /* Manufacturing Status */
    {AFE_MFG_STATUS_INIT_ADDR, AFE_MFG_STATUS_INIT_VALUE, AFE_TWOBYTE},

    /* Cell Balancing Configuration */
    {AFE_BALANCING_CONFIG_ADDR, AFE_BALANCING_CONFIG_VALUE, AFE_ONEBYTE},
    {AFE_CELL_BALANCING_LEVEL_ADDR, AFE_CB_MAX_CELL_VOLTAGE, AFE_TWOBYTE},
    {AFE_CELL_BALANCE_INTERVAL_ADDR, AFE_CB_TIME_INTERVAL, AFE_ONEBYTE},

    /* Temperature Limits */
    {AFE_MIN_CELL_TEMP_ADDR, AFE_MIN_CELL_TEMP, AFE_ONEBYTE},
    {AFE_MAX_CELL_TEMP_ADDR, AFE_MAX_CELL_TEMP, AFE_ONEBYTE},
    {AFE_MAX_INTERNAL_TEMP_ADDR, AFE_MAX_TEMPERATURE, AFE_ONEBYTE},

    /* Cell Balancing Voltage Thresholds */
    {AFE_MIN_CELL_BALANCE_CHARGE_ADDR, AFE_CB_MIN_CELL_VOLTAGE_CHG, AFE_TWOBYTE},
    {AFE_MIN_CELL_BALANCE_RELAX_ADDR, AFE_CB_MIN_CELL_VOLTAGE_RELAX, AFE_TWOBYTE},
    {AFE_MIN_CELL_BALANCE_DELTA_CHARGE_ADDR, AFE_CB_MIN_DELTA, AFE_ONEBYTE},
    {AFE_STOP_CELL_BALANCE_DELTA_CHARGE_ADDR, AFE_CB_STOP_DELTA, AFE_ONEBYTE},
    {AFE_MIN_CELL_BALANCE_DELTA_RELAX_ADDR, AFE_CB_MIN_DELTA, AFE_ONEBYTE},
    {AFE_STOP_CELL_BALANCE_DELTA_RELAX_ADDR, AFE_CB_STOP_DELTA, AFE_ONEBYTE},

    /* Shutdown and Sleep Voltages */
    {AFE_SHUTDOWN_CELL_VOLTAGE_ADDR, AFE_SHUTDOWN_STACK_CELL_VOLATGE, AFE_TWOBYTE},
    {AFE_SHUTDOWN_STACK_VOLTAGE_ADDR, AFE_SHUTDOWN_STACK_VOLATGE, AFE_TWOBYTE},
    {AFE_SLEEP_CHARGE_VOLTAGE_ADDR, AFE_SLEEP_CHG_VOLTAGE, AFE_TWOBYTE},

    /* Pin Configuration */
    {AFE_DDSG_PIN_CONFIG_ADDR, AFE_DDSG_PIN_CONFIG_VALUE, AFE_ONEBYTE},
    {AFE_DCHG_PIN_CONFIG_ADDR, AFE_DCHG_PIN_CONFIG_VALUE, AFE_ONEBYTE},
    {AFE_CFETOFF_PIN_CONFIG_ADDR, AFE_CFETOFF_PIN_CONFIG_VALUE, AFE_ONEBYTE},
    {AFE_DFETOFF_PIN_CONFIG_ADDR, AFE_DFETOFF_PIN_CONFIG_VALUE, AFE_ONEBYTE},
    {AFE_TS3_PIN_CONFIG_ADDR, AFE_TS3_PIN_CONFIG_VALUE, AFE_ONEBYTE},
    {AFE_TS1_PIN_CONFIG_ADDR, AFE_TS1_PIN_CONFIG_VALUE, AFE_ONEBYTE},
	{AFE_TS2_CONFIG_ADDR    , AFE_TS2_CONFIG_VALUE,     AFE_ONEBYTE},
    {AFE_ALERT_PIN_CONFIG_ADDR, AFE_ALERT_PIN_CONFIG_VALUE, AFE_ONEBYTE},

    /* System Configuration */
    {AFE_VCELL_MODE_CONFIG_ADDR, AFE_CELL_MODE_CONFIG, AFE_TWOBYTE},
    {AFE_PROTECTION_CONFIG_ADDR, AFE_FET_PROTECTION_VOLTAGE, AFE_TWOBYTE},



    /* Safety Fault Masks */
    {AFE_SF_ALERT_MASK_A_ADDR, AFE_SF_ALERT_MASK_A_VALUE, AFE_ONEBYTE},
    {AFE_SF_ALERT_MASK_B_ADDR, AFE_SF_ALERT_MASK_B_VALUE, AFE_ONEBYTE},
    {AFE_SF_ALERT_MASK_C_ADDR, AFE_SF_ALERT_MASK_C_VALUE, AFE_ONEBYTE},

    /* Open Wire Detection */
    {AFE_OPEN_WIRE_CHECK_TIME_ADDR, AFE_OPEN_WIRE_CHECK_TIME_VALUE, AFE_TWOBYTE},

    /* Cell Balancing Limits */
    {AFE_MAX_CELL_BALANCE_ADDR, AFE_MAX_CB_NUMBER, AFE_ONEBYTE}
};

const size_t AFE_OtherProtConfigSize = sizeof(AFE_OtherProtConfigTable_mst) / sizeof(AFE_OtherProtConfigTable_mst[0]);

/* ==================== PROTECTION COMMAND CONFIGURATION DATA ==================== */
const AFE_Config_tst AFE_ProtCommandConfigTable_mst[] = {
    /* Protection Group A */
    {AFE_PROTECTION_A_ADDR, AFE_PROTECTION_A_VALUE, AFE_ONEBYTE},
    {AFE_CHG_PROTECTION_A_ADDR, AFE_CHG_PROTECTION_A_VALUE, AFE_ONEBYTE},
    {AFE_DSG_PROTECTION_A_ADDR, AFE_DSG_PROTECTION_A_VALUE, AFE_ONEBYTE},

    /* Protection Group B */
    {AFE_PROTECTION_B_ADDR, AFE_PROTECTION_B_VALUE, AFE_ONEBYTE},
    {AFE_CHG_PROTECTION_B_ADDR, AFE_CHG_PROTECTION_B_VALUE, AFE_ONEBYTE},
    {AFE_DSG_PROTECTION_B_ADDR, AFE_DSG_PROTECTION_B_VALUE, AFE_ONEBYTE},

    /* Protection Group C */
    {AFE_PROTECTION_C_ADDR, AFE_PROTECTION_C_VALUE, AFE_ONEBYTE},
    {AFE_CHG_PROTECTION_C_ADDR, AFE_CHG_PROTECTION_C_VALUE, AFE_ONEBYTE},
    {AFE_DSG_PROTECTION_C_ADDR, AFE_DSG_PROTECTION_C_VALUE, AFE_ONEBYTE}
};

const size_t AFE_ProtCommandConfigSize = sizeof(AFE_ProtCommandConfigTable_mst) / sizeof(AFE_ProtCommandConfigTable_mst[0]);

/* ==================== STATIC  Function ==================== */
static AFE_Status_ten AFE_Configure_men(const AFE_Config_tst* configs_argst, size_t count_argsize);
static AFE_Status_ten AFE_Threshold_Config_men(void);
static AFE_Status_ten AFE_Protection_Setting_men(void);
static AFE_Status_ten AFE_Other_PROT_Setting_men(void);
/* ==================== FUNCTIONS  ==================== */
AFE_Status_ten AFE_init_mv()
{
    AFE_Status_ten status_en = AFE_STATUS_SUCCESS;
        /* Initialize I2C */
        status_en += DRV_I2C_Init_gen(BSP_BQ76952Afe);
        /* Reset AFE - Direct subcommand calls */
        status_en += AFE_SubCommand_WriteOperation_gv(AFE_RESET_ADDR, AFE_RESET_VALUE, 0x03);
        status_en += AFE_SubCommand_WriteOperation_gv(AFE_I2C_SWAP_ADDR, AFE_I2C_SWAP_VALUE, 0x03);
        status_en += AFE_SubCommand_WriteOperation_gv(AFE_CRC_CONFIG_ADDR, AFE_CRC_CONFIG_VALUE, 0x03);
        BSP_TimerDelay(1000, DRV_DELAY_UNITS_MILLISECOND);

        /* Enter Config Update Mode - Direct subcommand call */
        status_en += AFE_SubCommand_WriteOperation_gv(AFE_CONFIG_UPDATE_MODE_ADDR, AFE_CONFIG_UPDATE_MODE_VALUE, 0x03);
        BSP_TimerDelay(10, DRV_DELAY_UNITS_MILLISECOND);

        /* Power Configuration - Direct subcommand call */
        status_en += AFE_SubCommand_WriteOperation_gv(AFE_POWER_CONFIG_ADDR, AFE_POWER_CONFIG_VALUE, 0x06);
        BSP_TimerDelay(10, DRV_DELAY_UNITS_MILLISECOND);

        /* REG0 Configuration - Direct subcommand call */
        status_en += AFE_SubCommand_WriteOperation_gv(AFE_REG0_CONFIG_ADDR, AFE_REG0_CONFIG_VALUE, 0x05);
        BSP_TimerDelay(10, DRV_DELAY_UNITS_MILLISECOND);

        /* REG12 Configuration - Direct subcommand call */
        status_en += AFE_SubCommand_WriteOperation_gv(AFE_REG12_CONFIG_ADDR, AFE_REG12_CONFIG_VALUE, 0x05);
        BSP_TimerDelay(10, DRV_DELAY_UNITS_MILLISECOND);

        /* Other Protection Setting */
        status_en += AFE_Other_PROT_Setting_men();
        /* Protection Setting */
        status_en += AFE_Protection_Setting_men();
        /* Threshold Configuration */
        status_en += AFE_Threshold_Config_men();
        /* Exit Config Update Mode - Direct sub command call */
        status_en += AFE_SubCommand_WriteOperation_gv(AFE_CONFIG_UPDATE_EXIT_ADDR, AFE_CONFIG_UPDATE_EXIT_VALUE, 0x05);

        BSP_TimerDelay(1000, DRV_DELAY_UNITS_MICROSECOND);
        return status_en;

}
/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : AFE_ConfigureBatch_men
*   Description   : Unified function to configure AFE settings from any configuration array
*   Parameters    : configs_argst - Pointer to configuration array
*                   count_argsize - Number of configurations in the array
*   Return Value  : AFE_Status_ten - Success if all configurations applied, error on first failure
*  ---------------------------------------------------------------------------*/
static AFE_Status_ten AFE_Configure_men(const AFE_Config_tst* configs_argst, size_t count_argsize)
{
	 AFE_Status_ten status_en = AFE_STATUS_SUCCESS;
    for (size_t i = 0; i < count_argsize; i++)
    {
        const AFE_Config_tst* config_st = &configs_argst[i];
        status_en += AFE_SubCommand_WriteOperation_gv(
            config_st->AFE_Address_u16,
            config_st->AFE_Value_u32,
            (config_st->AFE_Data_size_u8 == AFE_ONEBYTE) ? 0x05 : 0x06
        );

        BSP_TimerDelay(10, DRV_DELAY_UNITS_MILLISECOND);
    }
    return status_en;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : AFE_Other_PROT_Setting_men
*   Description   : Configures other protection settings using unified batch function
*   Parameters    : None
*   Return Value  : AFE_Status_ten - Configuration status
*  ---------------------------------------------------------------------------*/
static AFE_Status_ten AFE_Other_PROT_Setting_men(void)
{
    return AFE_Configure_men(AFE_OtherProtConfigTable_mst, AFE_OtherProtConfigSize);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : AFE_Protection_Setting_men
*   Description   : Configures protection settings using unified batch function
*   Parameters    : None
*   Return Value  : AFE_Status_ten - Configuration status
*  ---------------------------------------------------------------------------*/
static AFE_Status_ten AFE_Protection_Setting_men(void)
{
    return AFE_Configure_men(AFE_ProtCommandConfigTable_mst, AFE_ProtCommandConfigSize);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : AFE_Threshold_Config_men
*   Description   : Configures threshold settings using unified batch function
*   Parameters    : None
*   Return Value  : AFE_Status_ten - Configuration status
*  ---------------------------------------------------------------------------*/
static AFE_Status_ten AFE_Threshold_Config_men(void)
{
    return AFE_Configure_men(AFE_ThresholdConfigTable_mst, AFE_ThresholdConfigSize);
}



 /* -----------------------------------------------------------------------------
  *  FUNCTION DESCRIPTION
  *  -----------------------------------------------------------------------------
  *   Function Name : AFE_SubCommand_WriteOperation_gv
  *   Description   : Executes AFE subcommand write operation with retry mechanism
  *                   and fault handling, including checksum validation
  *   Parameters    : SubCommand_argu16 - 16-bit subcommand to execute
  *                   Subcommand_Data_u32 - 32-bit data for subcommand
  *                   SubcommandSize_u8 - Length of command data
  *   Return Value  : AFE_Status_ten - Communication status (success/error)
  *   Note          : Implements retry mechanism (AFE_MAXCMDRETIES attempts)
  *                   Sets AFE_COMM_ERROR fault if all retries fail
  *  ---------------------------------------------------------------------------*/
 AFE_Status_ten AFE_SubCommand_WriteOperation_gv(U16 SubCommand_argu16, U32 Subcommand_Data_u32, U8 SubcommandSize_u8) {
     AFE_Status_ten status_en;
     U8 messagePayLoad_arru8[7];
     U8 checksumDataByte_arru8[3];
     BIN writeSucces_b = false;
     U8 retryCounts_u8 = AFE_MAXCMDRETIES;

     messagePayLoad_arru8[0] = CMD_DIR_SUBCMD_LOW;
     messagePayLoad_arru8[1] = LOW_BYTE(SubCommand_argu16);
     messagePayLoad_arru8[2] = HIGH_BYTE(SubCommand_argu16);
     messagePayLoad_arru8[3] = LOW_BYTE(Subcommand_Data_u32);
     messagePayLoad_arru8[4] = HIGH_BYTE(Subcommand_Data_u32);
     messagePayLoad_arru8[5] = SECOND_LOW_BYTE(Subcommand_Data_u32);
     messagePayLoad_arru8[6] = SECOND_HIGH_BYTE(Subcommand_Data_u32);

     checksumDataByte_arru8[0] = CMD_DIR_RESP_CHKSUM;
     checksumDataByte_arru8[1] = AFE_Checksum_gu8(messagePayLoad_arru8, sizeof(messagePayLoad_arru8));
     checksumDataByte_arru8[2] = SubcommandSize_u8;

     do {
         status_en = AFE_WriteReg_gen(messagePayLoad_arru8, sizeof(messagePayLoad_arru8));
         RTOS_TaskDelay_st(10);
         if (status_en == AFE_STATUS_SUCCESS) {
             writeSucces_b = true;
         }
     } while (retryCounts_u8-- && !writeSucces_b);

     if (!writeSucces_b) {

         return AFE_STATUS_ERROR;
     }

     writeSucces_b = false;
     retryCounts_u8 = AFE_MAXCMDRETIES;
     do {
         status_en = AFE_WriteReg_gen(checksumDataByte_arru8, 3);
         RTOS_TaskDelay_st(10);
         if (status_en == AFE_STATUS_SUCCESS) {
             writeSucces_b = true;

         }
     } while (retryCounts_u8-- && !writeSucces_b);

     if (!writeSucces_b) {

     }
     return status_en;
 }



 /* -----------------------------------------------------------------------------
  *  FUNCTION DESCRIPTION
  *  -----------------------------------------------------------------------------
  *   Function Name : AFE_WriteReg_gen
  *   Description   : Low-level I2C register write operation wrapper
  *   Parameters    : regdata_argptru8 - Pointer to data buffer to write
  *                   count_argsize - Number of bytes to write
  *   Return Value  : AFE_Status_ten - Communication status (success/error)
  *   Note          : Direct wrapper around I2C_WriteBlock_gen function
  *                   Converts I2C status to AFE status codes
  *  ---------------------------------------------------------------------------*/
 AFE_Status_ten AFE_WriteReg_gen(U8 *regdata_argptru8, U8 count_argsize)
 {
     AFE_Status_ten status_en = DRV_I2C_WriteBlock_gen(BSP_BQ76952Afe, &regdata_argptru8[0], count_argsize, true);

     if (status_en != AFE_STATUS_SUCCESS)
     {
         return AFE_STATUS_ERROR;
     }
     return AFE_STATUS_SUCCESS;
 }


 /* ==================== UTILITY FUNCTIONS ==================== */
 /* -----------------------------------------------------------------------------
 *  FUNCTION DESCRIPTION
 *  -----------------------------------------------------------------------------
 *   Function Name : AFE_Checksum_gu8
 *   Description   : Calculates 8-bit checksum for data integrity verification
 *                   - Computes ones' complement of the sum of all bytes
 *                   - Skips first byte in the array (typically command byte)
 *                   - Formula: AFE_Checksum_gu8 = 0xFF & ~(sum of bytes[1..len_argu8-1])
 *   Parameters    : data_argptru8 - Pointer to the data buffer
 *                   len_argu8 - Length of the data buffer (in bytes)
 *   Return Value  : uint8_t - Calculated checksum value
 *  ---------------------------------------------------------------------------*/
 U8 AFE_Checksum_gu8(U8 *data_argptru8, U8 len_argu8)
 {
     U8 checksum_u8 = 0;
     for (int i = 1; i < len_argu8; i++)
    	 checksum_u8 = (U8) (checksum_u8 + data_argptru8[i]);
     checksum_u8 = (U8) (0xff & ~checksum_u8);

     return (checksum_u8);
 }
#endif
