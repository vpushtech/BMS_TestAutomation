#ifndef APPLICATION_SAFETYCHECK_H
#define APPLICATION_SAFETYCHECK_H

/* ==================== INCLUDES ==================== */
#include <afe_data_proccessing.h>
#include <bsp_config.h>

/* ==================== SAFETY CHECK CONFIGURATION ==================== */
#define APP_DEBOUNCE_COUNT                0

/* ==================== THERMAL RUNAWAY DETECTION CONSTANTS ==================== */
#define APP_TSH_MIN             0.0f       /* °C (self-heating onset) */
#define APP_TSH_MAX             60.0f      /* °C */
#define APP_TR_ONSET_MIN        60.0f      /* °C (TR onset) */
#define APP_TR_ONSET_MAX        150.0f     /* °C */
#define APP_dT_SELF_HEATING     0.02f      /* °C/sec */
#define APP_dT_TR               1.0f       /* °C/sec */

/* ==================== TYPE DEFINITIONS ==================== */

/* ---------- Thermal Runaway State Enumeration ---------- */
typedef enum {
    APP_NORMAL = 0,
    APP_SELF_HEATING,
    APP_TR_DETECTED,
} APP_TRStatus_ten;

/* ---------- Protection Level Enumeration ---------- */
typedef enum {
    APP_PRIMARY_PROTECTION = 0,
    APP_SECONDARY_PROTECTION,
    APP_PROTECTION_LEVEL_COUNT
} APP_ProtectionLevel_ten;

/* ---------- Fault Bit Positions ---------- */
typedef enum {
    APP_BATTERY_OVER_THRESHOLD_BIT  = 1,
    APP_BATTERY_UNDER_THRESHOLD_BIT = 2,
} APP_FaultBitPosition_ten;

/* ---------- Primary Fault Type Enumerations ---------- */
typedef enum {
    APP_PRIMARY_TEMP_ALERT = 0,
    APP_PRIMARY_VOLT_ALERT,
	APP_PRIMARY_TEMP_SAFETY,
	APP_PRIMARY_VOLT_SAFETY,
	APP_PRIMARY_TEMP_PF,
	APP_PRIMARY_VOLT_PF,
	APP_PRIMARY_CURRENT_PF,
    APP_Primary_isDefault
} APP_PrimaryfaultStatus_ten;
/* ---------- Secondary Fault Type Enumerations ---------- */
typedef enum {
    APP_SECONDARY_TEMP_ALERT = 0,
	APP_SECONDARY_VOLT_ALERT,
	APP_SECONDARY_CURRENT_ALERT,
	APP_SECONDARY_TEMP_PF,
	APP_SECONDARY_VOLT_PF,
	APP_SECONDARY_CURRENT_PF,
    APP_Secondary_isDefault
} APP_SecondaryfaultStatus_ten;
/* ---------- Combined Fault Status Structure ---------- */
typedef struct {
    APP_SecondaryfaultStatus_ten SecondaryFaultStatus_En;
    APP_PrimaryfaultStatus_ten PrimaryFaultStatus_En;
} APP_faultStatus_tst;

/* ---------- Threshold Status Enumeration ---------- */
typedef enum {
    APP_OverThreshold    = 1,
    APP_UnderThreshold   = 2,
    APP_ThresholdStatus_COUNT
} APP_ThresholdfaultStatus_ten;

/* ==================== STRUCTURE DEFINITIONS ==================== */

/* ---------- Secondary Threshold Protection Structure ---------- */
typedef struct {
    I16 APP_Over_Threshold_i16;
    I16 APP_Recovery_Over_Threshold_i16;
    I16 APP_Under_Threshold_i16;
    I16 APP_Recovery_Under_Threshold_i16;
} APP_ThresholdProt_tst;

/* ---------- Input Measurement Structure ---------- */
typedef struct {
    I16 APP_MaxInputData_i16;
    I16 APP_MinInputData_i16;
} APP_InputMeasument_tst;

/* ---------- Primary Alarm Bit Masks Structure ---------- */
typedef struct {
    U8 OVER_THRESHOLD_MASK;
    U8 UNDER_THRESHOLD_MASK;
} APP_PrimaryAlarmMasks_tst;

/* ---------- Global Protection Level Faults Structure ---------- */
typedef struct {
    APP_ThresholdProt_tst APP_ThresholdProt_St[APP_Secondary_isDefault];
    U8 APP_DebounceCounter_u8[APP_PROTECTION_LEVEL_COUNT][APP_ThresholdStatus_COUNT];
    APP_InputMeasument_tst APP_InputMeasument_St[APP_Secondary_isDefault];
} APP_GlobleProtectionLevelFaults_tst;

/* ---------- Fault Status Structure ---------- */
typedef struct {
    U8 faultsThreshold_Bits;
} APP_FaultStatus_tst;
typedef struct{
	U16 APP_RecoverableFault_u16;
	U16 APP_NotRecoverableFault_u16;
}APP_criticalNonCriticalFault_tst;

/* ==================== GLOBAL VARIABLES ==================== */
extern APP_GlobleProtectionLevelFaults_tst APP_GlobleProtectionLevelFaults_St;
extern APP_FaultStatus_tst APP_FaultStatus_mst[APP_PROTECTION_LEVEL_COUNT][APP_Primary_isDefault];
extern const APP_PrimaryAlarmMasks_tst APP_PrimaryAlarmMasks_st[APP_Primary_isDefault];
extern APP_criticalNonCriticalFault_tst APP_criticalNonCriticalFault_st;
/* ==================== BIT MANIPULATION MACROS ==================== */
#define APP_SET_FAULT_BIT(faultStatus, bitPosition) \
    ((faultStatus)->faultsThreshold_Bits |= (1U << (bitPosition)))

#define APP_CLEAR_FAULT_BIT(faultStatus, bitPosition) \
    ((faultStatus)->faultsThreshold_Bits &= ~(1U << (bitPosition)))

#define APP_GET_FAULT_BIT(faultStatus, bitPosition) \
    (((faultStatus)->faultsThreshold_Bits >> (bitPosition)) & 1U)

#define APP_CHECK_FAULT_BIT(faultStatus, bitPosition) \
    (APP_GET_FAULT_BIT(faultStatus, bitPosition) == 1U)

#define APP_CHECK_BIT(value, bitPosition) \
    ((((value) >> (bitPosition)) & 1U) == 1U)

/* ==================== FUNCTION CATEGORIZATION ==================== */

/* ---------- Fault Status Management Functions ---------- */
APP_FaultStatus_tst* APP_GetFaultStatus_gst(APP_ProtectionLevel_ten protectionLevel_argen,
                                            U8 faultTypeIndex);
void APP_SetFaultStatus_gv(APP_ProtectionLevel_ten protectionLevel_argen,
                          U8 faultTypeIndex,
						  APP_FaultStatus_tst faultStatus_argst);
void APP_ResetFaultStatus_gv(APP_ProtectionLevel_ten protectionLevel_argen,
                            U8 faultTypeIndex);

/* ---------- Threshold Check Functions ---------- */
void APP_SecondaryThresholdCheck_gv(APP_ProtectionLevel_ten protectionLevel_argen,
                                   APP_SecondaryfaultStatus_ten faultStatus_argen);
void APP_PrimaryThresholdCheck_gv(APP_ProtectionLevel_ten protectionLevel_argen,
                                 APP_PrimaryfaultStatus_ten faultStatus_argen);

/* ---------- Safety Check Main Functions ---------- */
void APP_SecondarySafetyCheck_Func_gv(void);
void APP_PrimarySafetyCheck_Func_gv(void);
/* ---------- Recoverable and Non-Recoverable Fault Status Functions ---------- */
U16 APP_GetRecoverableFaults_gen(void);
U16 APP_GetNonRecoverableFaults_gen(void);
/* ---------- Thermal Runaway Detection Function ---------- */
APP_TRStatus_ten APP_DetectThermalRunaway_gen(F32 temp_argf32);




#endif /* APPLICATION_SAFETYCHECK_H */
