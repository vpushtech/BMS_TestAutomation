/* ==================== HEADER ==================== */
#include <bms_protection.h>

/* ==================== GLOBAL VARIABLES ==================== */
APP_GlobleProtectionLevelFaults_tst APP_GlobleProtectionLevelFaults_St = {0};
APP_FaultStatus_tst APP_FaultStatus_mst[APP_PROTECTION_LEVEL_COUNT][APP_Primary_isDefault] = {0};
extern AFE_Status_tst AFE_Status_st;
APP_criticalNonCriticalFault_tst APP_criticalNonCriticalFault_st={0};
/* ==================== PRIMARY ALARM MASKS CONFIGURATION ==================== */

const APP_PrimaryAlarmMasks_tst APP_PrimaryAlarmMasks_st[APP_Primary_isDefault] = {
    {.OVER_THRESHOLD_MASK = 0x40, .UNDER_THRESHOLD_MASK = 0x04},
    {.OVER_THRESHOLD_MASK = 0x01, .UNDER_THRESHOLD_MASK = 0x04},
    {.OVER_THRESHOLD_MASK = 0x40, .UNDER_THRESHOLD_MASK = 0x04},
    {.OVER_THRESHOLD_MASK = 0x08, .UNDER_THRESHOLD_MASK = 0x04},
	{.OVER_THRESHOLD_MASK = 0x20, .UNDER_THRESHOLD_MASK = 0x10},
	{.OVER_THRESHOLD_MASK = 0x02, .UNDER_THRESHOLD_MASK = 0x01},
	{.OVER_THRESHOLD_MASK = 0x08, .UNDER_THRESHOLD_MASK = 0x04},
};

/* ==================== STATIC VARIABLES ==================== */
static APP_TRStatus_ten APP_prev_state = APP_NORMAL;
static U32 APP_debounce_counter_mu32 = 0;
static F32 APP_prev_temp_mf32 = 0.0f;
static const U8 APP_TIME_STEP_S = 1.0f;
static APP_TRStatus_ten current_state_men;
static F32 dT_dt_mf32;

/* ==================== FAULT STATUS MANAGEMENT FUNCTIONS ==================== */

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : APP_GetFaultStatus_gst
*   Description   : Retrieves fault status for specified protection level and fault type index
*   Parameters    : protectionLevel_argen - Protection level (Primary/Secondary)
*                   faultTypeIndex - Fault type index
*   Return Value  : Pointer to fault status structure
*  --------------------------------------------------------------------------- */
APP_FaultStatus_tst* APP_GetFaultStatus_gst(APP_ProtectionLevel_ten protectionLevel_argen,
                                            U8 faultTypeIndex)
{
    static APP_FaultStatus_tst faultStatus_st = {0};

    if (protectionLevel_argen >= APP_PROTECTION_LEVEL_COUNT)
    {
        return &faultStatus_st;
    }
    if (protectionLevel_argen == APP_PRIMARY_PROTECTION)
    {
        if (faultTypeIndex >= APP_Primary_isDefault)
        {
            return &faultStatus_st;
        }
    }
    else if (protectionLevel_argen == APP_SECONDARY_PROTECTION)
    {
        if (faultTypeIndex >= APP_Secondary_isDefault)
        {
            return &faultStatus_st;
        }
    }

    return &APP_FaultStatus_mst[protectionLevel_argen][faultTypeIndex];
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : APP_SetFaultStatus_gv
*   Description   : Sets fault status for specified protection level and fault type
*   Parameters    : protectionLevel_argen - Protection level
*                   faultTypeIndex - Fault type index
*                   faultStatus_argst - Fault status to set
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void APP_SetFaultStatus_gv(APP_ProtectionLevel_ten protectionLevel_argen,
                          U8 faultTypeIndex,
                          APP_FaultStatus_tst faultStatus_argst)
{
    if (protectionLevel_argen >= APP_PROTECTION_LEVEL_COUNT)
    {
        return;
    }

    if (protectionLevel_argen == APP_PRIMARY_PROTECTION && faultTypeIndex < APP_Primary_isDefault)
    {
        APP_FaultStatus_mst[protectionLevel_argen][faultTypeIndex] = faultStatus_argst;
    }
    else if (protectionLevel_argen == APP_SECONDARY_PROTECTION && faultTypeIndex < APP_Secondary_isDefault)
    {
        APP_FaultStatus_mst[protectionLevel_argen][faultTypeIndex] = faultStatus_argst;
    }
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : APP_ResetFaultStatus_gv
*   Description   : Resets fault status for specified protection level and fault type
*   Parameters    : protectionLevel_argen - Protection level
*                   faultTypeIndex - Fault type index
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void APP_ResetFaultStatus_gv(APP_ProtectionLevel_ten protectionLevel_argen,
                            U8 faultTypeIndex)
{
    if (protectionLevel_argen >= APP_PROTECTION_LEVEL_COUNT)
    {
        return;
    }

    if (protectionLevel_argen == APP_PRIMARY_PROTECTION && faultTypeIndex < APP_Primary_isDefault)
    {
    	Common_Memset_gv(&APP_FaultStatus_mst[protectionLevel_argen][faultTypeIndex], 0, sizeof(APP_FaultStatus_tst));
    }
    else if (protectionLevel_argen == APP_SECONDARY_PROTECTION && faultTypeIndex < APP_Secondary_isDefault)
    {
    	Common_Memset_gv(&APP_FaultStatus_mst[protectionLevel_argen][faultTypeIndex], 0, sizeof(APP_FaultStatus_tst));
    }
}

/* ==================== THRESHOLD CHECK FUNCTIONS ==================== */

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : APP_SecondaryThresholdCheck_gv
*   Description   : Performs secondary protection threshold checking with debouncing
*   Parameters    : protectionLevel_argen - Protection level
*                   faultStatus_argen - Secondary fault type to check
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void APP_SecondaryThresholdCheck_gv(APP_ProtectionLevel_ten protectionLevel_argen,
                                   APP_SecondaryfaultStatus_ten faultStatus_argen)
{
    if (faultStatus_argen >= APP_Secondary_isDefault)
    {
        return;
    }

    APP_FaultStatus_tst *faultStatus_st = APP_GetFaultStatus_gst(protectionLevel_argen, faultStatus_argen);
    APP_FaultStatus_tst newFaultStatus = {0};

    I16 maxInput_i16 = APP_GlobleProtectionLevelFaults_St.APP_InputMeasument_St[faultStatus_argen].APP_MaxInputData_i16;
    I16 minInput_i16 = APP_GlobleProtectionLevelFaults_St.APP_InputMeasument_St[faultStatus_argen].APP_MinInputData_i16;
    I16 overThreshold = APP_GlobleProtectionLevelFaults_St.APP_ThresholdProt_St[faultStatus_argen].APP_Over_Threshold_i16;
    I16 underThreshold = APP_GlobleProtectionLevelFaults_St.APP_ThresholdProt_St[faultStatus_argen].APP_Under_Threshold_i16;
    I16 recoveryOver = APP_GlobleProtectionLevelFaults_St.APP_ThresholdProt_St[faultStatus_argen].APP_Recovery_Over_Threshold_i16;
    I16 recoveryUnder = APP_GlobleProtectionLevelFaults_St.APP_ThresholdProt_St[faultStatus_argen].APP_Recovery_Under_Threshold_i16;

    /* Over Threshold Check */
    if ((overThreshold >= 0 && maxInput_i16 > overThreshold) ||
        (overThreshold < 0 && maxInput_i16 < 0 && maxInput_i16 < overThreshold))
    {
        if (!APP_CHECK_FAULT_BIT(faultStatus_st, APP_BATTERY_OVER_THRESHOLD_BIT))
        {
            U8* debounceCounter = &APP_GlobleProtectionLevelFaults_St.APP_DebounceCounter_u8[protectionLevel_argen][APP_OverThreshold];
            (*debounceCounter)++;

            if (*debounceCounter >= APP_DEBOUNCE_COUNT)
            {
                newFaultStatus = *faultStatus_st;
                APP_SET_FAULT_BIT(&newFaultStatus, APP_BATTERY_OVER_THRESHOLD_BIT);
                APP_SetFaultStatus_gv(protectionLevel_argen, faultStatus_argen, newFaultStatus);
            }
        }
    }
    /* Over Threshold Recovery */
    else if ((recoveryOver >= 0 && maxInput_i16 < recoveryOver) ||
             (recoveryOver < 0 && (maxInput_i16 >= 0 || maxInput_i16 > recoveryOver)))
    {
        if (APP_CHECK_FAULT_BIT(faultStatus_st, APP_BATTERY_OVER_THRESHOLD_BIT))
        {
            APP_GlobleProtectionLevelFaults_St.APP_DebounceCounter_u8[protectionLevel_argen][APP_OverThreshold] = 0;
            APP_ResetFaultStatus_gv(protectionLevel_argen, faultStatus_argen);
        }
    }

    /* Under Threshold Check */
    if ((underThreshold >= 0 && minInput_i16 < underThreshold) ||
        (underThreshold < 0 && minInput_i16 < 0 && minInput_i16 > underThreshold))
    {
        if (!APP_CHECK_FAULT_BIT(faultStatus_st, APP_BATTERY_UNDER_THRESHOLD_BIT))
        {
            U8* debounceCounter = &APP_GlobleProtectionLevelFaults_St.APP_DebounceCounter_u8[protectionLevel_argen][APP_UnderThreshold];
            (*debounceCounter)++;

            if (*debounceCounter >= APP_DEBOUNCE_COUNT)
            {
                newFaultStatus = *faultStatus_st;
                APP_SET_FAULT_BIT(&newFaultStatus, APP_BATTERY_UNDER_THRESHOLD_BIT);
                APP_SetFaultStatus_gv(protectionLevel_argen, faultStatus_argen, newFaultStatus);
            }
        }
    }
    /* Under Threshold Recovery */
    else if ((recoveryUnder >= 0 && minInput_i16 > recoveryUnder) ||
             (recoveryUnder < 0 && (minInput_i16 >= 0 || minInput_i16 < recoveryUnder)))
    {
        if (APP_CHECK_FAULT_BIT(faultStatus_st, APP_BATTERY_UNDER_THRESHOLD_BIT))
        {
            APP_GlobleProtectionLevelFaults_St.APP_DebounceCounter_u8[protectionLevel_argen][APP_UnderThreshold] = 0;
            APP_ResetFaultStatus_gv(protectionLevel_argen, faultStatus_argen);
        }
    }
}
/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : APP_PrimaryThresholdCheck_gv
*   Description   : Performs primary protection threshold checking using AFE alarms
*   Parameters    : protectionLevel_argen - Protection level
*                   faultStatus_argen - Primary fault type to check
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void APP_PrimaryThresholdCheck_gv(APP_ProtectionLevel_ten protectionLevel_argen,
                                 APP_PrimaryfaultStatus_ten faultStatus_argen)
{
    if (faultStatus_argen >= APP_Primary_isDefault)
    {
        return;
    }

    APP_FaultStatus_tst *faultStatus_st = APP_GetFaultStatus_gst(protectionLevel_argen, faultStatus_argen);
    APP_FaultStatus_tst newFaultStatus = {0};
    const APP_PrimaryAlarmMasks_tst *alarmMasks = &APP_PrimaryAlarmMasks_st[faultStatus_argen];

    const APP_PrimaryAlarmMasks_tst Afe_Alarms_st[APP_Primary_isDefault] = {
         {AFE_Status_st.AFE_AlertB_u8, AFE_Status_st.AFE_AlertB_u8},
        {AFE_Status_st.AFE_AlertA_u8, AFE_Status_st.AFE_AlertA_u8},
        {AFE_Status_st.AFE_SafetyStatusB_u8, AFE_Status_st.AFE_SafetyStatusB_u8},
        {AFE_Status_st.AFE_SafetyStatusA_u8, AFE_Status_st.AFE_SafetyStatusA_u8},
		{AFE_Status_st.AFE_PF_StatusA_u8, AFE_Status_st.AFE_PF_StatusA_u8},
		{AFE_Status_st.AFE_PF_StatusA_u8, AFE_Status_st.AFE_PF_StatusA_u8},
		{AFE_Status_st.AFE_PF_StatusA_u8, AFE_Status_st.AFE_PF_StatusA_u8},
    };

    /* Over Threshold Check */
    if ((Afe_Alarms_st[faultStatus_argen].OVER_THRESHOLD_MASK & alarmMasks->OVER_THRESHOLD_MASK) == alarmMasks->OVER_THRESHOLD_MASK &&
        !APP_CHECK_FAULT_BIT(faultStatus_st, APP_BATTERY_OVER_THRESHOLD_BIT))
    {
        U8* debounceCounter = &APP_GlobleProtectionLevelFaults_St.APP_DebounceCounter_u8[protectionLevel_argen][APP_OverThreshold];
        (*debounceCounter)++;

        if (*debounceCounter >= APP_DEBOUNCE_COUNT)
        {
            newFaultStatus = *faultStatus_st;
            APP_SET_FAULT_BIT(&newFaultStatus, APP_BATTERY_OVER_THRESHOLD_BIT);
            APP_SetFaultStatus_gv(protectionLevel_argen, faultStatus_argen, newFaultStatus);
        }
    }
    /* Over Threshold Recovery */
    else if ((Afe_Alarms_st[faultStatus_argen].OVER_THRESHOLD_MASK & alarmMasks->OVER_THRESHOLD_MASK) != alarmMasks->OVER_THRESHOLD_MASK &&
             APP_CHECK_FAULT_BIT(faultStatus_st, APP_BATTERY_OVER_THRESHOLD_BIT))
    {
        APP_GlobleProtectionLevelFaults_St.APP_DebounceCounter_u8[protectionLevel_argen][APP_OverThreshold] = 0;
        APP_ResetFaultStatus_gv(protectionLevel_argen, faultStatus_argen);
    }

    /* Under Threshold Check - Only if UNDER_THRESHOLD_MASK is defined */
    if (alarmMasks->UNDER_THRESHOLD_MASK != 0 &&
        (Afe_Alarms_st[faultStatus_argen].UNDER_THRESHOLD_MASK & alarmMasks->UNDER_THRESHOLD_MASK) == alarmMasks->UNDER_THRESHOLD_MASK &&
        !APP_CHECK_FAULT_BIT(faultStatus_st, APP_BATTERY_UNDER_THRESHOLD_BIT))
    {
        U8* debounceCounter = &APP_GlobleProtectionLevelFaults_St.APP_DebounceCounter_u8[protectionLevel_argen][APP_UnderThreshold];
        (*debounceCounter)++;

        if (*debounceCounter >= APP_DEBOUNCE_COUNT)
        {
            newFaultStatus = *faultStatus_st;
            APP_SET_FAULT_BIT(&newFaultStatus, APP_BATTERY_UNDER_THRESHOLD_BIT);
            APP_SetFaultStatus_gv(protectionLevel_argen, faultStatus_argen, newFaultStatus);
        }
    }
    /* Under Threshold Recovery */
    else if (alarmMasks->UNDER_THRESHOLD_MASK != 0 &&
             (Afe_Alarms_st[faultStatus_argen].UNDER_THRESHOLD_MASK & alarmMasks->UNDER_THRESHOLD_MASK) != alarmMasks->UNDER_THRESHOLD_MASK &&
             APP_CHECK_FAULT_BIT(faultStatus_st, APP_BATTERY_UNDER_THRESHOLD_BIT))
    {
        APP_GlobleProtectionLevelFaults_St.APP_DebounceCounter_u8[protectionLevel_argen][APP_UnderThreshold] = 0;
        APP_ResetFaultStatus_gv(protectionLevel_argen, faultStatus_argen);
    }
}

/* ==================== SAFETY CHECK MAIN FUNCTIONS ==================== */

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : APP_SecondarySafetyCheck_Func_gv
*   Description   : Performs secondary safety check for all secondary fault types
*   Parameters    : void
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void APP_SecondarySafetyCheck_Func_gv(void)
{
    for (APP_SecondaryfaultStatus_ten faultType = APP_SECONDARY_TEMP_ALERT; faultType < APP_Secondary_isDefault; faultType++)
    {
        APP_SecondaryThresholdCheck_gv(APP_SECONDARY_PROTECTION, faultType);
    }
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : APP_PrimarySafetyCheck_Func_gv
*   Description   : Performs primary safety check for all primary fault types
*   Parameters    : void
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void APP_PrimarySafetyCheck_Func_gv(void)
{
    for (APP_PrimaryfaultStatus_ten faultType = APP_PRIMARY_TEMP_ALERT; faultType < APP_Primary_isDefault; faultType++)
    {
        APP_PrimaryThresholdCheck_gv(APP_PRIMARY_PROTECTION, faultType);
    }
}

/* ==================== FAULT CATEGORIZATION FUNCTIONS ====================

 -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : APP_GetRecoverableFaults_gen
*   Description   : Extracts recoverable faults directly from global fault status arrays
*                   Recoverable: All non-PF faults
*   Parameters    : void
*   Return Value  : U16 - Combined recoverable faults
*  ---------------------------------------------------------------------------*/
U16 APP_GetRecoverableFaults_gen(void)
{
    U16 recoverableFaults_u16 = 0x0000;

    const U8 primaryBitPositions_mu8[APP_Primary_isDefault] = {0, 2, 4, 6, 8, 10};

    for (APP_PrimaryfaultStatus_ten faultType = APP_PRIMARY_TEMP_ALERT;
         faultType < APP_PRIMARY_TEMP_PF; faultType++)
    {
        APP_FaultStatus_tst *faultStatus = APP_GetFaultStatus_gst(APP_PRIMARY_PROTECTION, faultType);
        U8 bitPosition = primaryBitPositions_mu8[faultType];

        recoverableFaults_u16 |= (APP_GET_FAULT_BIT(faultStatus, APP_BATTERY_OVER_THRESHOLD_BIT) << bitPosition);
        recoverableFaults_u16 |= (APP_GET_FAULT_BIT(faultStatus, APP_BATTERY_UNDER_THRESHOLD_BIT) << (bitPosition + 1));
    }

    const U8 secondaryBitPositions_mu8[APP_Secondary_isDefault] = {12, 14, 16,18,20};
    for (APP_SecondaryfaultStatus_ten faultType = APP_SECONDARY_TEMP_ALERT;
         faultType < APP_SECONDARY_TEMP_PF; faultType++)
    {
        APP_FaultStatus_tst *faultStatus = APP_GetFaultStatus_gst(APP_SECONDARY_PROTECTION, faultType);
        U8 bitPosition = secondaryBitPositions_mu8[faultType];
        recoverableFaults_u16 |= (APP_GET_FAULT_BIT(faultStatus, APP_BATTERY_OVER_THRESHOLD_BIT) << bitPosition);
        recoverableFaults_u16 |= (APP_GET_FAULT_BIT(faultStatus, APP_BATTERY_UNDER_THRESHOLD_BIT) << (bitPosition + 1));
    }

    return recoverableFaults_u16;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : APP_GetNonRecoverableFaults_gen
*   Description   : Extracts non-recoverable faults directly from global fault status arrays
*                   Non-Recoverable: All PF (Permanent Fault) types + additional system faults
*   Parameters    : void
*   Return Value  : U16 - Non-recoverable faults
*  ---------------------------------------------------------------------------*/
U16 APP_GetNonRecoverableFaults_gen(void)
{
    U16 nonRecoverableFaults_u16 = 0x0000;

    const U8 primaryBitPositions_mu8[APP_Primary_isDefault] = {0, 2, 4, 6, 8, 10};

    for (APP_PrimaryfaultStatus_ten faultType = APP_PRIMARY_TEMP_PF;
         faultType < APP_Primary_isDefault; faultType++)
    {
        APP_FaultStatus_tst *faultStatus = APP_GetFaultStatus_gst(APP_PRIMARY_PROTECTION, faultType);
        U8 bitPosition = primaryBitPositions_mu8[faultType - APP_PRIMARY_TEMP_PF];

        nonRecoverableFaults_u16 |= (APP_GET_FAULT_BIT(faultStatus, APP_BATTERY_OVER_THRESHOLD_BIT) << bitPosition);
        nonRecoverableFaults_u16 |= (APP_GET_FAULT_BIT(faultStatus, APP_BATTERY_UNDER_THRESHOLD_BIT) << (bitPosition + 1));
    }

    const U8 secondaryBitPositions_mu8[APP_Secondary_isDefault] = {12, 14, 16, 18,20};

    for (APP_SecondaryfaultStatus_ten faultType = APP_SECONDARY_TEMP_PF;
         faultType < APP_Secondary_isDefault; faultType++)
    {
        APP_FaultStatus_tst *faultStatus = APP_GetFaultStatus_gst(APP_SECONDARY_PROTECTION, faultType);
        U8 bitPosition = secondaryBitPositions_mu8[faultType - APP_SECONDARY_TEMP_PF];

        nonRecoverableFaults_u16 |= (APP_GET_FAULT_BIT(faultStatus, APP_BATTERY_OVER_THRESHOLD_BIT) << bitPosition);
        nonRecoverableFaults_u16 |= (APP_GET_FAULT_BIT(faultStatus, APP_BATTERY_UNDER_THRESHOLD_BIT) << (bitPosition + 1));
    }

    return nonRecoverableFaults_u16;
}
/* ==================== THERMAL RUNAWAY DETECTION FUNCTION ==================== */

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : APP_DetectThermalRunaway_gen
*   Description   : Detects thermal runaway states based on temperature and rate of change
*   Parameters    : temp_argf32 - Current temperature in °C
*   Return Value  : APP_TR_State_t - Detected thermal runaway state
*  --------------------------------------------------------------------------- */
APP_TRStatus_ten APP_DetectThermalRunaway_gen(F32 temp_argf32)
{
    current_state_men = APP_NORMAL;

    dT_dt_mf32 = ((temp_argf32 - APP_prev_temp_mf32) / APP_TIME_STEP_S);

    if (dT_dt_mf32 > APP_dT_SELF_HEATING && temp_argf32 >= APP_TSH_MIN && temp_argf32 <= APP_TSH_MAX)
    {
        current_state_men = APP_SELF_HEATING;
    }

    else if (dT_dt_mf32 > APP_dT_TR && temp_argf32 >= APP_TR_ONSET_MIN && temp_argf32 <= APP_TR_ONSET_MAX)
    {
        current_state_men = APP_TR_DETECTED;
    }
    else
    {
        current_state_men = APP_NORMAL;
    }

    if (current_state_men == APP_prev_state)
    {
        APP_debounce_counter_mu32 = 0;
    }
    else
    {
        APP_debounce_counter_mu32++;
        if (APP_debounce_counter_mu32 < APP_DEBOUNCE_COUNT)
        {
            return APP_prev_state;
        }
        APP_debounce_counter_mu32 = 0;
        APP_prev_state = current_state_men;
    }

    APP_prev_temp_mf32 = temp_argf32;
    return current_state_men;
}

