/******************************************************************************
 * Application_BMS_States.c
 *
 * Description      : BMS Hierarchical State Machine - Implementation
 * Author           : Rushikesh
 * Created On       : 03-Nov-2025
 * Version          : 1.0
 * Modification History:
 * Date        Author      Description
 * ----------------------------------------------------------------------------
 * 03-Nov-2025 Rushikesh        QP-style HSM with pure state flow skeleton
 * 03-Nov-2025 Rushikesh        Entry/Exit as comments, no actions/loops/conditions
 ******************************************************************************/

/* ==================== INCLUDE FILES ==================== */
#include "hsm.h"
/* ==================== STATIC VARIABLES ==================== */
static HSM_StateId_ten HSM_CurrentState_men = HSM_STATE_TOP;
static HSM_Event_ten HSM_Event_men = HSM_EVT_TICK;
/* ==================== TYPE DEFINITIONS ==================== */
typedef HSM_Status_ten (*HSM_StateHandler_fptr)(void);
/* ==================== EXTERN VARIABLE  ==================== */
extern AFE_InputeMeasument_tst AFE_InputeMeasument_St;
extern APP_criticalNonCriticalFault_tst APP_criticalNonCriticalFault_st;
/* ==================== FORWARD DECLARATIONS ==================== */

/* Top Level States */
static HSM_Status_ten HSM_TopInitial_men(void);

/* Main States */
static HSM_Status_ten HSM_Idle_men(void);
static HSM_Status_ten HSM_Discharging_men(void);
static HSM_Status_ten HSM_Charging_men(void);
static HSM_Status_ten HSM_Sleep_men(void);
static HSM_Status_ten HSM_Error_men(void);

/* Discharging Sub-States */
static HSM_Status_ten HSM_DisPre_men(void);
static HSM_Status_ten HSM_DisLimp_men(void);
static HSM_Status_ten HSM_DisNo_men(void);
static HSM_Status_ten HSM_DisNormal_men(void);

/* Charging Sub-States */
static HSM_Status_ten HSM_ChgPre_men(void);
static HSM_Status_ten HSM_ChgLimp_men(void);
static HSM_Status_ten HSM_ChgNo_men(void);
static HSM_Status_ten HSM_ChgNormal_men(void);

/* Sleep Sub-States */
static HSM_Status_ten HSM_SleepNormal_men(void);
static HSM_Status_ten HSM_SleepDeep_men(void);
static HSM_Status_ten HSM_SleepShutdown_men(void);

/* Error Sub-States */
static HSM_Status_ten HSM_ErrRec_men(void);
static HSM_Status_ten HSM_ErrNonRec_men(void);

/* ==================== STATE HANDLER TABLE ==================== */
static const HSM_StateHandler_fptr HSM_StateTable_fptr[HSM_STATE_MAX] = {
    [HSM_STATE_TOP]            = HSM_TopInitial_men,
    [HSM_STATE_IDLE]           = HSM_Idle_men,

    [HSM_STATE_DISCHARGING]    = HSM_Discharging_men,
    [HSM_STATE_DIS_PRE]        = HSM_DisPre_men,
    [HSM_STATE_DIS_LIMP]       = HSM_DisLimp_men,
    [HSM_STATE_DIS_NO]         = HSM_DisNo_men,
    [HSM_STATE_DIS_NORMAL]     = HSM_DisNormal_men,

    [HSM_STATE_CHARGING]       = HSM_Charging_men,
    [HSM_STATE_CHG_PRE]        = HSM_ChgPre_men,
    [HSM_STATE_CHG_LIMP]       = HSM_ChgLimp_men,
    [HSM_STATE_CHG_NO]         = HSM_ChgNo_men,
    [HSM_STATE_CHG_NORMAL]     = HSM_ChgNormal_men,

    [HSM_STATE_SLEEP]          = HSM_Sleep_men,
    [HSM_STATE_SLEEP_NORMAL]   = HSM_SleepNormal_men,
    [HSM_STATE_SLEEP_DEEP]     = HSM_SleepDeep_men,
    [HSM_STATE_SLEEP_SHUTDOWN] = HSM_SleepShutdown_men,

    [HSM_STATE_ERROR]          = HSM_Error_men,
    [HSM_STATE_ERR_REC]        = HSM_ErrRec_men,
    [HSM_STATE_ERR_NONREC]     = HSM_ErrNonRec_men
};

/* ==================== QP-STYLE CONTROL MACROS ==================== */
#define Q_TRAN(next_state)   do { HSM_CurrentState_men = (next_state); return HSM_STATUS_TRAN; } while(0)
#define Q_SUPER(super)       return (super)()
#define Q_HANDLED()          return HSM_STATUS_HANDLED

/* ==================== STATE HANDLER IMPLEMENTATIONS ==================== */

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : HSM_TopInitial_men
*   Description   : Top level initial state handler
*   Parameters    : None
*   Return Value  : HSM_Status_ten - State machine status
*  --------------------------------------------------------------------------- */
static HSM_Status_ten HSM_TopInitial_men(void)
{
    /* ENTRY: Idle state entry actions */
    /* Configure secondary protection thresholds for temperature */
    APP_GlobleProtectionLevelFaults_St.APP_ThresholdProt_St[APP_SECONDARY_TEMP_ALERT] =
        (APP_ThresholdProt_tst){60, 50, -10.0f, 0.0f};

    /* Configure secondary protection thresholds for voltage */
    APP_GlobleProtectionLevelFaults_St.APP_ThresholdProt_St[APP_SECONDARY_VOLT_ALERT] =
        (APP_ThresholdProt_tst){3650, 3550, 2500, 2550};

    /* Configure secondary protection thresholds for current */
    APP_GlobleProtectionLevelFaults_St.APP_ThresholdProt_St[APP_SECONDARY_CURRENT_ALERT] =
        (APP_ThresholdProt_tst){20, 5, 0, 0};

    /* Reset all debounce counters */
    Common_Memset_gv(APP_GlobleProtectionLevelFaults_St.APP_DebounceCounter_u8, 0,
           sizeof(APP_GlobleProtectionLevelFaults_St.APP_DebounceCounter_u8));
    Q_TRAN(HSM_STATE_IDLE);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : HSM_Idle_men
*   Description   : Main idle state handler
*   Parameters    : None
*   Return Value  : HSM_Status_ten - State machine status
*  --------------------------------------------------------------------------- */
static HSM_Status_ten HSM_Idle_men(void)
{


    /* Evaluate system conditions */
    if(APP_GetFlag_gv(APP_INIT_TIMEOUT_FLAG))
    {
        if (APP_criticalNonCriticalFault_st.APP_RecoverableFault_u16) {
            //HSM_Event_men = HSM_EVT_FAULT_RECOVERABLE;
        } else if (APP_criticalNonCriticalFault_st.APP_NotRecoverableFault_u16) {
           // HSM_Event_men = HSM_EVT_FAULT_NONRECOVERABLE;
        } else if (APP_GetFlag_gv(APP_SLEEP_MODE_FLAG)) {
            HSM_Event_men = HSM_EVT_SLEEP_REQUEST;
        } else if (APP_GetFlag_gv(APP_CAN_FLAG)) {
            HSM_Event_men = HSM_EVT_CAN_ON;
        } else if (APP_GetFlag_gv(APP_IGNITION_FLAG)) {
            HSM_Event_men = HSM_EVT_IGNITION_ON;
        }
        APP_ResetFlag_gv(APP_INIT_TIMEOUT_FLAG);
    }

    /* Process current event */
    switch (HSM_Event_men) {
        case HSM_EVT_IGNITION_ON:
        {
            /* ENTRY: Normal discharge state entry actions */
            APP_GlobleProtectionLevelFaults_St.APP_ThresholdProt_St[APP_SECONDARY_CURRENT_ALERT] =
                (APP_ThresholdProt_tst){-90, -20, 0, 0}; /* Negative current for discharge */
            /* Reset all debounce counters */
            Common_Memset_gv(APP_GlobleProtectionLevelFaults_St.APP_DebounceCounter_u8, 0,
                   sizeof(APP_GlobleProtectionLevelFaults_St.APP_DebounceCounter_u8));
            Q_TRAN(HSM_STATE_DIS_PRE);
            break;
        }

        case HSM_EVT_CAN_ON:
        {
            APP_GlobleProtectionLevelFaults_St.APP_ThresholdProt_St[APP_SECONDARY_CURRENT_ALERT] =
                (APP_ThresholdProt_tst){20, 5, 0, 0}; /* Negative current for discharge */
            /* Reset all debounce counters */
            Common_Memset_gv(APP_GlobleProtectionLevelFaults_St.APP_DebounceCounter_u8, 0,
                   sizeof(APP_GlobleProtectionLevelFaults_St.APP_DebounceCounter_u8));
            Q_TRAN(HSM_STATE_CHG_PRE);
            break;
        }

        case HSM_EVT_SLEEP_REQUEST:
        {
            APP_ResetFlag_gv(APP_SLEEP_MODE_FLAG);
            Q_TRAN(HSM_STATE_SLEEP_NORMAL);
            break;
        }

        case HSM_EVT_FAULT_NONRECOVERABLE:
        {
            Q_TRAN(HSM_STATE_ERR_NONREC);
            break;
        }

        case HSM_EVT_FAULT_RECOVERABLE:
        {
            Q_TRAN(HSM_STATE_ERR_REC);
            break;
        }

        default:
        {
            break;
        }
    }

    /* EXIT: Idle state exit actions */
    Q_HANDLED();
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : HSM_Discharging_men
*   Description   : Main discharging state handler
*   Parameters    : None
*   Return Value  : HSM_Status_ten - State machine status
*  --------------------------------------------------------------------------- */
static HSM_Status_ten HSM_Discharging_men(void)
{
    /* ENTRY: Discharging state entry actions */

    /* Evaluate system conditions */
    if (APP_criticalNonCriticalFault_st.APP_RecoverableFault_u16) {
        HSM_Event_men = HSM_EVT_FAULT_RECOVERABLE;
    } else if (APP_criticalNonCriticalFault_st.APP_NotRecoverableFault_u16) {
        HSM_Event_men = HSM_EVT_FAULT_NONRECOVERABLE;
    } else if (APP_GetFlag_gv(APP_SLEEP_MODE_FLAG)) {
        HSM_Event_men = HSM_EVT_SLEEP_REQUEST;
    } else if (APP_GetFlag_gv(APP_CAN_FLAG)) {
        HSM_Event_men = HSM_EVT_CAN_ON;
    }

    /* Process current event */
    switch (HSM_Event_men) {
        case HSM_EVT_CAN_ON:
        {
            APP_GlobleProtectionLevelFaults_St.APP_ThresholdProt_St[APP_SECONDARY_CURRENT_ALERT] =
                (APP_ThresholdProt_tst){20, 5, 0, 0}; /* Negative current for discharge */
            /* Reset all debounce counters */
            Common_Memset_gv(APP_GlobleProtectionLevelFaults_St.APP_DebounceCounter_u8, 0,
                   sizeof(APP_GlobleProtectionLevelFaults_St.APP_DebounceCounter_u8));
            Q_TRAN(HSM_STATE_CHG_PRE);
            break;
        }

        case HSM_EVT_SLEEP_REQUEST:
        {
            APP_ResetFlag_gv(APP_SLEEP_MODE_FLAG);
            Q_TRAN(HSM_STATE_SLEEP_NORMAL);
            break;
        }

        case HSM_EVT_FAULT_NONRECOVERABLE:
        {
            Q_TRAN(HSM_STATE_ERR_NONREC);
            break;
        }

        case HSM_EVT_FAULT_RECOVERABLE:
        {
            Q_TRAN(HSM_STATE_ERR_REC);
            break;
        }

        default:
        {
            break;
        }
    }

    /* EXIT: Discharging state exit actions */
    Q_HANDLED();
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : HSM_DisPre_men
*   Description   : Pre-discharge state handler
*   Parameters    : None
*   Return Value  : HSM_Status_ten - State machine status
*  --------------------------------------------------------------------------- */
static HSM_Status_ten HSM_DisPre_men(void)
{
    /* ENTRY: Pre-discharge state entry actions */
    if ((AFE_InputeMeasument_St.AFE_Temperature_St.AFE_MaxCellTemperature_i16 * 0.01) <= 40 && APP_GetFlag_gv(APP_PREDISCHARGE_TIMEOUT_FLAG)) {
        HSM_Event_men = HSM_EVT_NORMAL_DISCHARGE;
    }else if ((AFE_InputeMeasument_St.AFE_Temperature_St.AFE_MaxCellTemperature_i16 * 0.01) >= 40 && APP_GetFlag_gv(APP_PREDISCHARGE_TIMEOUT_FLAG)){
         HSM_Event_men = HSM_EVT_LIMP_DISCHARGE;
    }
    /* Process current event */
    switch (HSM_Event_men) {
        case HSM_EVT_NORMAL_DISCHARGE:
        {
            APP_ResetFlag_gv(APP_PREDISCHARGE_TIMEOUT_FLAG);
            Q_TRAN(HSM_STATE_DIS_NORMAL);
            break;
        }

        case HSM_EVT_LIMP_DISCHARGE:
        {
            APP_ResetFlag_gv(APP_PREDISCHARGE_TIMEOUT_FLAG);
            Q_TRAN(HSM_STATE_DIS_LIMP);
            break;
        }

        default:
        {
            break;
        }
    }

    Q_SUPER(HSM_Discharging_men);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : HSM_DisLimp_men
*   Description   : Limp discharge state handler
*   Parameters    : None
*   Return Value  : HSM_Status_ten - State machine status
*  --------------------------------------------------------------------------- */
static HSM_Status_ten HSM_DisLimp_men(void)
{
    /* ENTRY: Limp discharge state entry actions */
    if ((AFE_InputeMeasument_St.AFE_Temperature_St.AFE_MaxCellTemperature_i16 * 0.01) <= 40) {
        HSM_Event_men = HSM_EVT_NORMAL_DISCHARGE;
    }else  if ((AFE_InputeMeasument_St.AFE_Temperature_St.AFE_MaxCellTemperature_i16 * 0.01)>=55) {
        HSM_Event_men = HSM_EVT_NO_DISCHARGE;
    }else{
        ;
    }
    /* Process current event */
    switch (HSM_Event_men) {
        case HSM_EVT_NO_DISCHARGE:
        {
            Q_TRAN(HSM_STATE_DIS_NO);
            break;
        }

        case HSM_EVT_NORMAL_DISCHARGE:
        {
            Q_TRAN(HSM_STATE_DIS_NORMAL);
            break;
        }

        default:
        {
            break;
        }
    }

    Q_SUPER(HSM_Discharging_men);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : HSM_DisNo_men
*   Description   : No discharge state handler
*   Parameters    : None
*   Return Value  : HSM_Status_ten - State machine status
*  --------------------------------------------------------------------------- */
static HSM_Status_ten HSM_DisNo_men(void)
{
    /* ENTRY: No discharge state entry actions */
    if ((AFE_InputeMeasument_St.AFE_Temperature_St.AFE_MaxCellTemperature_i16 * 0.01) <= 40 && APP_GetFlag_gv(APP_PREDISCHARGE_TIMEOUT_FLAG)) {
        HSM_Event_men = HSM_EVT_NORMAL_DISCHARGE;
    }else if ((AFE_InputeMeasument_St.AFE_Temperature_St.AFE_MaxCellTemperature_i16 * 0.01) >= 40 && APP_GetFlag_gv(APP_PREDISCHARGE_TIMEOUT_FLAG)){
         HSM_Event_men = HSM_EVT_LIMP_DISCHARGE;
    }else{
        ;
    }
    /* Process current event */
    switch (HSM_Event_men) {
        case HSM_EVT_NORMAL_DISCHARGE:
        {
            Q_TRAN(HSM_STATE_DIS_NORMAL);
            break;
        }

        case HSM_EVT_LIMP_DISCHARGE:
        {
            Q_TRAN(HSM_STATE_DIS_LIMP);
            break;
        }

        default:
        {
            break;
        }
    }

    Q_SUPER(HSM_Discharging_men);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : HSM_DisNormal_men
*   Description   : Normal discharge state handler
*   Parameters    : None
*   Return Value  : HSM_Status_ten - State machine status
*  --------------------------------------------------------------------------- */
static HSM_Status_ten HSM_DisNormal_men(void)
{

    /* Evaluate discharge-specific conditions */
    if ((AFE_InputeMeasument_St.AFE_Temperature_St.AFE_MaxCellTemperature_i16 * 0.01) >= 40) {
        HSM_Event_men = HSM_EVT_LIMP_DISCHARGE;
    }

    if ((APP_GetFlag_gv(APP_DISCHARGE_DONE_MODE_FLAG) == false) &&
        (AFE_InputeMeasument_St.AFE_Voltage_St.AFE_MinCellVoltage_u16 <= 2800)) {
        APP_SetFlag_gv(APP_DISCHARGE_DONE_MODE_FLAG);
        HSM_Event_men = HSM_EVT_NO_DISCHARGE;
    }

    /* Process current event */
    switch (HSM_Event_men) {
        case HSM_EVT_LIMP_DISCHARGE:
        {
            Q_TRAN(HSM_STATE_DIS_LIMP);
            break;
        }

        case HSM_EVT_NO_DISCHARGE:
        {
            APP_ResetFlag_gv(APP_DISCHARGE_DONE_MODE_FLAG);
            Q_TRAN(HSM_STATE_DIS_NO);
            break;
        }

        default:
        {
            break;
        }
    }

    Q_SUPER(HSM_Discharging_men);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : HSM_Charging_men
*   Description   : Main charging state handler
*   Parameters    : None
*   Return Value  : HSM_Status_ten - State machine status
*  --------------------------------------------------------------------------- */
static HSM_Status_ten HSM_Charging_men(void)
{
    /* ENTRY: Charging state entry actions */
    /* Evaluate system conditions */
    if (APP_criticalNonCriticalFault_st.APP_RecoverableFault_u16) {
        HSM_Event_men = HSM_EVT_FAULT_RECOVERABLE;
    } else if (APP_criticalNonCriticalFault_st.APP_NotRecoverableFault_u16) {
        HSM_Event_men = HSM_EVT_FAULT_NONRECOVERABLE;
    } else if (APP_GetFlag_gv(APP_SLEEP_MODE_FLAG)) {
        HSM_Event_men = HSM_EVT_SLEEP_REQUEST;
    } else if (APP_GetFlag_gv(APP_IGNITION_FLAG)) {
        HSM_Event_men = HSM_EVT_IGNITION_ON;
    }
    /* Process current event */
    switch (HSM_Event_men) {
        case HSM_EVT_IGNITION_ON:
        {
            APP_GlobleProtectionLevelFaults_St.APP_ThresholdProt_St[APP_SECONDARY_CURRENT_ALERT] =
                (APP_ThresholdProt_tst){-90, -20, 0, 0}; /* Negative current for discharge */
            /* Reset all debounce counters */
            Common_Memset_gv(APP_GlobleProtectionLevelFaults_St.APP_DebounceCounter_u8, 0,
                   sizeof(APP_GlobleProtectionLevelFaults_St.APP_DebounceCounter_u8));
            Q_TRAN(HSM_STATE_DIS_PRE);
            break;
        }

        case HSM_EVT_SLEEP_REQUEST:
        {
            APP_ResetFlag_gv(APP_SLEEP_MODE_FLAG);
            Q_TRAN(HSM_STATE_SLEEP_NORMAL);
            break;
        }

        case HSM_EVT_FAULT_NONRECOVERABLE:
        {
            Q_TRAN(HSM_STATE_ERR_NONREC);
            break;
        }

        case HSM_EVT_FAULT_RECOVERABLE:
        {
            Q_TRAN(HSM_STATE_ERR_REC);
            break;
        }

        default:
        {
            break;
        }
    }

    /* EXIT: Charging state exit actions */
    Q_HANDLED();
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : HSM_ChgPre_men
*   Description   : Pre-charge state handler
*   Parameters    : None
*   Return Value  : HSM_Status_ten - State machine status
*  --------------------------------------------------------------------------- */
static HSM_Status_ten HSM_ChgPre_men(void)
{
    /* ENTRY: Pre-charge state entry actions */
    if ((AFE_InputeMeasument_St.AFE_Temperature_St.AFE_MaxCellTemperature_i16 * 0.01) <= 40 && APP_GetFlag_gv(APP_PRECHARGE_TIMEOUT_FLAG)) {
        HSM_Event_men = HSM_EVT_NORMAL_CHARGE;
    }else if ((AFE_InputeMeasument_St.AFE_Temperature_St.AFE_MaxCellTemperature_i16 * 0.01) >= 40 && APP_GetFlag_gv(APP_PRECHARGE_TIMEOUT_FLAG)){
         HSM_Event_men = HSM_EVT_NORMAL_CHARGE;
    }
    /* Process current event */
    switch (HSM_Event_men) {
        case HSM_EVT_LIMP_CHARGE:
        {
            APP_ResetFlag_gv(APP_PRECHARGE_TIMEOUT_FLAG);
            Q_TRAN(HSM_STATE_CHG_LIMP);
            break;
        }

        case HSM_EVT_NORMAL_CHARGE:
        {
            APP_ResetFlag_gv(APP_PRECHARGE_TIMEOUT_FLAG);
            Q_TRAN(HSM_STATE_CHG_NORMAL);
            break;
        }

        default:
        {
            break;
        }
    }

    Q_SUPER(HSM_Charging_men);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : HSM_ChgLimp_men
*   Description   : Limp charge state handler
*   Parameters    : None
*   Return Value  : HSM_Status_ten - State machine status
*  --------------------------------------------------------------------------- */
static HSM_Status_ten HSM_ChgLimp_men(void)
{
    /* ENTRY: Limp charge state entry actions */
    if ((AFE_InputeMeasument_St.AFE_Temperature_St.AFE_MaxCellTemperature_i16 * 0.01) <= 40) {
        HSM_Event_men = HSM_EVT_NORMAL_CHARGE;
    }else if ((AFE_InputeMeasument_St.AFE_Temperature_St.AFE_MaxCellTemperature_i16 * 0.01)>=55)
    {
        HSM_Event_men = HSM_EVT_NO_CHARGE;
    }else{
        ;
    }
    /* Process current event */
    switch (HSM_Event_men) {
        case HSM_EVT_NORMAL_CHARGE:
        {
            Q_TRAN(HSM_STATE_CHG_NORMAL);
            break;
        }

        case HSM_EVT_NO_CHARGE:
        {
            Q_TRAN(HSM_STATE_CHG_NO);
            break;
        }

        default:
        {
            break;
        }
    }

    Q_SUPER(HSM_Charging_men);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : HSM_ChgNo_men
*   Description   : No charge state handler
*   Parameters    : None
*   Return Value  : HSM_Status_ten - State machine status
*  --------------------------------------------------------------------------- */
static HSM_Status_ten HSM_ChgNo_men(void)
{
    /* ENTRY: No charge state entry actions */
    if ((AFE_InputeMeasument_St.AFE_Temperature_St.AFE_MaxCellTemperature_i16 * 0.01) <= 40 && APP_GetFlag_gv(APP_PRECHARGE_TIMEOUT_FLAG)) {
        HSM_Event_men = HSM_EVT_NORMAL_CHARGE;
    }else if ((AFE_InputeMeasument_St.AFE_Temperature_St.AFE_MaxCellTemperature_i16 * 0.01) >= 40 && APP_GetFlag_gv(APP_PRECHARGE_TIMEOUT_FLAG)){
         HSM_Event_men = HSM_EVT_NORMAL_CHARGE;
    }else{
        ;
    }
    /* Process current event */
    switch (HSM_Event_men) {
        case HSM_EVT_NORMAL_CHARGE:
        {
            Q_TRAN(HSM_STATE_CHG_NORMAL);
            break;
        }

        case HSM_EVT_LIMP_CHARGE:
        {
            Q_TRAN(HSM_STATE_CHG_LIMP);
            break;
        }

        default:
        {
            break;
        }
    }

    Q_SUPER(HSM_Charging_men);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : HSM_ChgNormal_men
*   Description   : Normal charge state handler
*   Parameters    : None
*   Return Value  : HSM_Status_ten - State machine status
*  --------------------------------------------------------------------------- */
static HSM_Status_ten HSM_ChgNormal_men(void)
{
    /* ENTRY: Normal charge state entry actions */
    if ((AFE_InputeMeasument_St.AFE_Temperature_St.AFE_MaxCellTemperature_i16 * 0.01) >= 40) {
        HSM_Event_men = HSM_EVT_LIMP_CHARGE;
    }

    if ((AFE_InputeMeasument_St.AFE_Voltage_St.AFE_MaxCellVoltage_u16 >= 3470) &&
        (APP_GetFlag_gv(APP_CV_MODE_FLAG) == false) &&
        (APP_GetFlag_gv(APP_CHARGE_DONE_MODE_FLAG) == false)) {

        APP_SetFlag_gv(APP_CV_MODE_FLAG);

        if (AFE_InputeMeasument_St.AFE_Voltage_St.AFE_MaxCellVoltage_u16 >= 3575) {
            APP_SetFlag_gv(APP_CHARGE_DONE_MODE_FLAG);
            HSM_Event_men = HSM_EVT_NO_CHARGE;
        }
    } else if ((APP_GetFlag_gv(APP_CV_MODE_FLAG) == true) &&
               (AFE_InputeMeasument_St.AFE_Current_st.AFE_CurrentmA_I16 <= 250) &&
               (APP_GetFlag_gv(APP_CHARGE_DONE_MODE_FLAG) == false)) {

        APP_SetFlag_gv(APP_CHARGE_DONE_MODE_FLAG);
        HSM_Event_men = HSM_EVT_NO_CHARGE;
    }

    /* Process current event */
    switch (HSM_Event_men) {
        case HSM_EVT_NO_CHARGE:
        {
            APP_ResetFlag_gv(APP_CV_MODE_FLAG);
            APP_ResetFlag_gv(APP_CHARGE_DONE_MODE_FLAG);
            Q_TRAN(HSM_STATE_CHG_NO);
            break;
        }

        case HSM_EVT_LIMP_CHARGE:
        {
            Q_TRAN(HSM_STATE_CHG_LIMP);
            break;
        }

        default:
        {
            break;
        }
    }

    Q_SUPER(HSM_Charging_men);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : HSM_Sleep_men
*   Description   : Main sleep state handler
*   Parameters    : None
*   Return Value  : HSM_Status_ten - State machine status
*  --------------------------------------------------------------------------- */
static HSM_Status_ten HSM_Sleep_men(void)
{
    if (HSM_Event_men == HSM_EVT_TICK) {
        /* EXIT: Sleep state exit actions */
        Q_TRAN(HSM_STATE_IDLE);
    }

    Q_HANDLED();
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : HSM_SleepNormal_men
*   Description   : Normal sleep state handler
*   Parameters    : None
*   Return Value  : HSM_Status_ten - State machine status
*  --------------------------------------------------------------------------- */
static HSM_Status_ten HSM_SleepNormal_men(void)
{
    /* ENTRY: Normal sleep state entry actions */

    if (HSM_Event_men == HSM_EVT_TICK) {
        /* EXIT: Normal sleep state exit actions */
        Q_TRAN(HSM_STATE_IDLE);
    }

    Q_SUPER(HSM_Sleep_men);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : HSM_SleepDeep_men
*   Description   : Deep sleep state handler
*   Parameters    : None
*   Return Value  : HSM_Status_ten - State machine status
*  --------------------------------------------------------------------------- */
static HSM_Status_ten HSM_SleepDeep_men(void)
{
    /* ENTRY: Deep sleep state entry actions */
    Q_SUPER(HSM_Sleep_men);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : HSM_SleepShutdown_men
*   Description   : Shutdown sleep state handler
*   Parameters    : None
*   Return Value  : HSM_Status_ten - State machine status
*  --------------------------------------------------------------------------- */
static HSM_Status_ten HSM_SleepShutdown_men(void)
{
    /* ENTRY: Shutdown sleep state entry actions */
    Q_SUPER(HSM_Sleep_men);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : HSM_Error_men
*   Description   : Main error state handler
*   Parameters    : None
*   Return Value  : HSM_Status_ten - State machine status
*  --------------------------------------------------------------------------- */
static HSM_Status_ten HSM_Error_men(void)
{
    /* ENTRY: Error state entry actions */

    /* Evaluate error exit conditions */
    if ((APP_criticalNonCriticalFault_st.APP_RecoverableFault_u16 == 0) &&
        (APP_criticalNonCriticalFault_st.APP_NotRecoverableFault_u16 == 0)) {

        if (APP_GetFlag_gv(APP_CAN_FLAG)) {
            HSM_Event_men = HSM_EVT_CAN_ON;
        } else if (APP_GetFlag_gv(APP_IGNITION_FLAG)) {
            HSM_Event_men = HSM_EVT_IGNITION_ON;
        } else if (APP_GetFlag_gv(APP_SLEEP_MODE_FLAG)) {
            HSM_Event_men = HSM_EVT_SLEEP_REQUEST;
        }
    }

    /* Process current event */
    switch (HSM_Event_men) {
        case HSM_EVT_IGNITION_ON:
        {
            APP_GlobleProtectionLevelFaults_St.APP_ThresholdProt_St[APP_SECONDARY_CURRENT_ALERT] =
                (APP_ThresholdProt_tst){-90, -20, 0, 0}; /* Negative current for discharge */
            /* Reset all debounce counters */
            Common_Memset_gv(APP_GlobleProtectionLevelFaults_St.APP_DebounceCounter_u8, 0,
                   sizeof(APP_GlobleProtectionLevelFaults_St.APP_DebounceCounter_u8));
            Q_TRAN(HSM_STATE_DIS_PRE);
            break;
        }

        case HSM_EVT_CAN_ON:
        {
            APP_GlobleProtectionLevelFaults_St.APP_ThresholdProt_St[APP_SECONDARY_CURRENT_ALERT] =
                (APP_ThresholdProt_tst){20, 5, 0, 0}; /* Negative current for discharge */
            /* Reset all debounce counters */
            Common_Memset_gv(APP_GlobleProtectionLevelFaults_St.APP_DebounceCounter_u8, 0,
                   sizeof(APP_GlobleProtectionLevelFaults_St.APP_DebounceCounter_u8));
            Q_TRAN(HSM_STATE_CHG_PRE);
            break;
        }

        case HSM_EVT_SLEEP_REQUEST:
        {
            APP_ResetFlag_gv(APP_SLEEP_MODE_FLAG);
            Q_TRAN(HSM_STATE_SLEEP_NORMAL);
            break;
        }

        default:
        {
            break;
        }
    }

    Q_HANDLED();
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : HSM_ErrRec_men
*   Description   : Recoverable error state handler
*   Parameters    : None
*   Return Value  : HSM_Status_ten - State machine status
*  --------------------------------------------------------------------------- */
static HSM_Status_ten HSM_ErrRec_men(void)
{
    /* ENTRY: Recoverable error state entry actions */
    if (APP_criticalNonCriticalFault_st.APP_NotRecoverableFault_u16) {
            HSM_Event_men = HSM_EVT_FAULT_NONRECOVERABLE;
        }
    if (HSM_Event_men == HSM_EVT_FAULT_NONRECOVERABLE) {
        /* EXIT: Normal sleep state exit actions */
        Q_TRAN(HSM_STATE_ERR_NONREC);
    }
    Q_SUPER(HSM_Error_men);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : HSM_ErrNonRec_men
*   Description   : Non-recoverable error state handler
*   Parameters    : None
*   Return Value  : HSM_Status_ten - State machine status
*  --------------------------------------------------------------------------- */
static HSM_Status_ten HSM_ErrNonRec_men(void)
{
    /* ENTRY: Non-recoverable error state entry actions */
    Q_SUPER(HSM_Error_men);
}

/* ==================== PUBLIC API IMPLEMENTATION ==================== */

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : HSM_Init_gen
*   Description   : Initializes BMS Hierarchical State Machine
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void HSM_Init_gen(void)
{
    HSM_CurrentState_men = HSM_STATE_TOP;
    HSM_StateTable_fptr[HSM_STATE_TOP]();
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : HSM_Process_gen
*   Description   : Processes events in BMS Hierarchical State Machine
*   Parameters    : None
*   Return Value  : HSM_Status_ten - State machine processing status
*  --------------------------------------------------------------------------- */
HSM_Status_ten HSM_Process_gen(void)
{
    if (HSM_CurrentState_men < HSM_STATE_MAX) {
        return HSM_StateTable_fptr[HSM_CurrentState_men]();
    }

    return HSM_STATUS_ERROR;
}
/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : HSM_GetCurrentSuperState_gen
*   Description   : Returns the current super state of BMS Hierarchical State Machine
*   Parameters    : None
*   Return Value  : HSM_StateId_ten - Current super state identifier
*  --------------------------------------------------------------------------- */
HSM_StateId_ten HSM_GetCurrentState_gen(void) {
    return HSM_CurrentState_men;
}
