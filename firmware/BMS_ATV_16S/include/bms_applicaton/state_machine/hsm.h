/******************************************************************************
 * Application_BMS_States.h
 *
 * Description      : BMS Hierarchical State Machine - Header File
 * Author           : Rushikesh
 * Created On       : 03-Nov-2025
 * Version          : 1.0
 * Modification History:
 * Date        Author      Description
 * ----------------------------------------------------------------------------
 * 03-Nov-2025 Rushikesh        Initial HSM Architecture Implementation
 * 03-Nov-2025 Rushikesh        State machine skeleton with pure state flow
 ******************************************************************************/

#ifndef APPLICATION_BMS_STATES_H
#define APPLICATION_BMS_STATES_H

/* ==================== INCLUDE FILES ==================== */
#include "afe_data_proccessing.h"
#include "app_timer.h"
#include "bms_protection.h"
/* ==================== ENUM DEFINITIONS ==================== */

/* Event Enumeration */
typedef enum {
    HSM_EVT_TICK,
    HSM_EVT_IGNITION_ON,
    HSM_EVT_CAN_ON,
    HSM_EVT_SLEEP_REQUEST,
    HSM_EVT_FAULT_RECOVERABLE,
    HSM_EVT_FAULT_NONRECOVERABLE,
    HSM_EVT_PRE_DISCHARGE_TIMEOUT,
    HSM_EVT_LIMP_DISCHARGE,
    HSM_EVT_NO_DISCHARGE,
    HSM_EVT_NORMAL_DISCHARGE,
    HSM_EVT_LIMP_CHARGE,
    HSM_EVT_NO_CHARGE,
    HSM_EVT_NORMAL_CHARGE,
    HSM_EVT_MAX
} HSM_Event_ten;

/* Status Enumeration */
typedef enum {
    HSM_STATUS_HANDLED = 0,
    HSM_STATUS_TRAN,
    HSM_STATUS_SUPER,
    HSM_STATUS_ERROR
} HSM_Status_ten;
/* ==================== ENUM DEFINITIONS ==================== */

/* State ID Enumeration */
typedef enum {
    HSM_STATE_TOP = 0,

    HSM_STATE_IDLE,

    HSM_STATE_DISCHARGING,
    HSM_STATE_DIS_PRE,
    HSM_STATE_DIS_LIMP,
    HSM_STATE_DIS_NO,
    HSM_STATE_DIS_NORMAL,

    HSM_STATE_CHARGING,
    HSM_STATE_CHG_PRE,
    HSM_STATE_CHG_LIMP,
    HSM_STATE_CHG_NO,
    HSM_STATE_CHG_NORMAL,

    HSM_STATE_SLEEP,
    HSM_STATE_SLEEP_NORMAL,
    HSM_STATE_SLEEP_DEEP,
    HSM_STATE_SLEEP_SHUTDOWN,

    HSM_STATE_ERROR,
    HSM_STATE_ERR_REC,
    HSM_STATE_ERR_NONREC,
    HSM_STATE_MAX
} HSM_StateId_ten;
/* ==================== FUNCTION PROTOTYPES ==================== */
/* HSM Initialization and Processing */
void HSM_Init_gen(void);
HSM_Status_ten HSM_Process_gen(void);
HSM_StateId_ten HSM_GetCurrentState_gen(void);
#endif /* APPLICATION_BMS_STATES_H */
