/*
 * afe_data_proccessing.h
 *
 *  Created on: 22-Oct-2025
 *      Author: RushikeshNitinKamble
 */

#ifndef ECU_CONFIG_ECU_DRIVERS_AFE_DRIVERS_AFE_DATA_PROCCESSING_H_
#define ECU_CONFIG_ECU_DRIVERS_AFE_DRIVERS_AFE_DATA_PROCCESSING_H_
#include "afe_config.h"
/* ==================== TYPE DEFINITIONS ==================== */
typedef enum
{
    AFE_POWER_MODE_SHUTDOWN = 0,
    AFE_POWER_MODE_DEEP_SLEEP,
    AFE_POWER_MODE_EXIT_DEEP_SLEEP,
    AFE_POWER_MODE_SLEEP
} AFE_PowerMode_ten;
/* ==================== VOLTAGE MEASUREMENT STRUCTURES ==================== */
typedef struct {
    U16 AFE_MaxCellVoltage_u16;
    U16 AFE_MinCellVoltage_u16;
    U16 AFE_CellVoltageDifference_u16;
    U16 AFE_Pack_Voltage_u16;
    U16 AFE_LD_Voltage_u16;
    U16 AFE_PackPin_Voltage_u16;
    U16 AFE_Stack_Voltage_u16;
    U16 AFE_VREG18_u16;
    U16 AFE_VSS_u16;
    U16 AFE_CB_ActiveCells_u16;
    U16 AFE_CellVoltage_u16[16];
} AFE_Voltage_tst;

/* ==================== TEMPERATURE MEASUREMENT STRUCTURES ==================== */
typedef struct {
    F64 AFE_Internal_Temp_f64;
    F64 AFE_CFET_Temp_f64;
    F64 AFE_DFET_Temp_f64;
    F64 AFE_ALERT_Temp_f64;
    F64 AFE_TS1_Temp_f64;
    F64 AFE_TS2_Temp_f64;
    F64 AFE_TS3_Temp_f64;
    F64 AFE_HDQ_Temp_f64;
    F64 AFE_DCHG_Temp_f64;
    F64 AFE_DDSG_Temp_f64;
} AFE_Temp_tst;

typedef struct {
    I16 AFE_AvgCellTemperature_i16;
    I16 AFE_MaxCellTemperature_i16;
    I16 AFE_MinCellTemperature_i16;
    I16 AFE_FET_Temperature_i16;
    AFE_Temp_tst AFE_Temp_st;
    I16 AFE_CellTemperature_i16;
} AFE_Temperature_tst;

/* ==================== CURRENT MEASUREMENT STRUCTURES ==================== */
typedef struct {
    I16 AFE_CurrentmA_I16;
    I16 AFE_CC1_Current_i16;
    I16 AFE_CC2_Current_i16;
    I16 AFE_CC3_Current_i16;
    I16 AFE_CellCurrent_i16[16];
} AFE_Current_tst;

/* ==================== STATUS MONITORING STRUCTURES ==================== */
typedef struct {
    volatile U16 APP_AlarmStatusValue_vu16;
    volatile U16 AFE_AlarmRawStatus_u16;
    volatile U16 AFE_AlarmEnable_u16;
    U8 FET_Status_u8;
    U8 AFE_AlertA_u8;
    U8 AFE_SafetyStatusA_u8;
    U8 AFE_AlertB_u8;
    U8 AFE_SafetyStatusB_u8;
    U8 AFE_AlertC_u8;
    U8 AFE_SafetyStatusC_u8;
    U8 AFE_PF_AlertA_u8;
    U8 AFE_PF_StatusA_u8;
    U8 AFE_PF_AlertB_u8;
    U8 AFE_PF_StatusB_u8;
    U8 AFE_PF_AlertC_u8;
    U8 AFE_PF_StatusC_u8;
    U8 AFE_PF_AlertD_u8;
    U8 AFE_PF_StatusD_u8;
} AFE_Status_tst;

/* ==================== COMPOSITE MEASUREMENT STRUCTURES ==================== */
typedef struct {
    AFE_Voltage_tst AFE_Voltage_St;
    AFE_Temperature_tst AFE_Temperature_St;
    AFE_Current_tst AFE_Current_st;
} AFE_InputeMeasument_tst;

/* ==================== MEASUREMENT UNIT ENUMERATIONS ==================== */
typedef enum {
    AFE_MILLI_UNIT = 1,
    AFE_BASIC_UNIT = 10,
} AFE_MeasurementUnit_ten;
/* ==================== COMMAND ENUMERATIONS ==================== */
typedef enum
{
    AFE_FET_MODE_CONTROL,                 /* 0x0022 - Enable FET control */
    AFE_FET_PDSG_TEST_MODE_ON,              /* 0x001C - PDSG FET test */
    AFE_FET_PCHG_TEST_MODE_ON,              /* 0x001E - PCHG FET test */
    AFE_FET_CHG_TEST_MODE_ON,               /* 0x001F - CHG FET test */
    AFE_FET_DSG_TEST_MODE_ON,               /* 0x0020 - DSG FET test */
    AFE_FET_DSG_PDSG_OFF,               /* 0x0093 - Force PDSG FET off */
    AFE_FET_CHG_PCHG_OFF,               /* 0x0094 - Force PCHG FET off */
    AFE_FET_ALL_ON,                          /* 0x0096 - Turn all FETs on */
    AFE_FET_ALL_OFF,                        /* 0x0095 - Turn all FETs off */
    AFE_FET_ENABLE_CHARGING,             /* 0x0097 - Enable charging path */
    AFE_FET_ENABLE_DISCHARGING,           /* 0x0097 - Enable discharging path */
    AFE_FET_DISABLE_CHARGING,              /* 0x0097 - Disable CHG & PCHG (0x0C) */
    AFE_FET_DISABLE_DISCHARGING            /* 0x0097 - Disable DSG & PDSG (0x03) */
} AFE_FetControl_ten;
/* ==================== EXTERNAL VARIABLE DECLARATIONS ==================== */
extern AFE_InputeMeasument_tst AFE_InputeMeasument_St;
extern AFE_Status_tst AFE_Status_st;

/* ==================== FUNCTION DECLARATIONS ==================== */

/* ==================== MEASUREMENT FUNCTIONS ==================== */
void AFE_ReadAllSystemMeasurements_gv(void);
void AFE_ReadAllTemperatures_gv(void);
F32 AFE_ReadTemperature_gf32(U8 Regaddr_argu8);
F32 AFE_InternalTemp_f32(void);
AFE_Status_ten AFE_ReadAllDAStatus_gv(void);
I32 AFE_Read_Value_i32(U8 RegAddr_argu8, AFE_MeasurementUnit_ten Units_argen);
AFE_Status_ten AFE_DeviceID_mu16(void);
/* ==================== FET CONTROL FUNCTIONS ==================== */
AFE_Status_ten AFE_FetControl_gv(AFE_FetControl_ten FetControlMode_argen);
AFE_Status_ten AFE_FetControlWithRetry_gv(AFE_FetControl_ten FetControlMode_argen);
/* ==================== POWER MODE CONTROL FUNCTIONS ==================== */
AFE_Status_ten AFE_PowerModeControl_gv(AFE_PowerMode_ten PowerMode_argen);
/* ==================== STATUS MONITORING FUNCTIONS ==================== */
void AFE_ReadAllSafetyAlertStatus_gv(void);
AFE_Status_ten AFE_FETStatusRead_gv(void);
void AFE_ReadAllAlarmStatus_gv(void);
AFE_Status_ten AFE_AlarmStatusReset_gen(U16 value_argu16);
/* ==================== DIRECT AND SUBCOMMAND FUNCTIONS ==================== */
AFE_Status_ten AFE_SubCommand_ReadOperation_gv(U16 SubCommand_argu16, U8* Data_argu8, U8 DataSize_argu8);
AFE_Status_ten AFE_Direct_ReadOperation_gv(U8 RegAddr_argu8, U8* ReadData_argu8, U8 DataSize_argu8);
AFE_Status_ten AFE_ReadReg_gen(U8 regaddr_argu8, U8 *regdata_argptru8, U8 count_argu8);
AFE_Status_ten AFE_Direct_WriteOperation_gv(U8 DirectCommand_argu8, U8 DirectCommandData_argu8, U8 DirectCmd_len_argu8);
#endif /* ECU_CONFIG_ECU_DRIVERS_AFE_DRIVERS_AFE_DATA_PROCCESSING_H_ */
