/*
 * BMS_Application.h
 *
 *  Created on: 14-Oct-2025
 *  Author: RushikeshNitinKamble
 *  Description: BMS Application Header - Command Interface and Timer Callbacks
 *  Version: 1.0
 *  Modification History:
 *  Date            Author          Description
 *  ----------------------------------------------------------------------------
 *  14-Oct-2025     Rushikesh       Initial BMS Application Header
 *  16-Oct-2025     Rushikesh       Added Timer Callback Declarations
 *  18-Oct-2025     Rushikesh       Added Command Interface Functions
 ******************************************************************************/

#ifndef BMS_APPLICATION_BMS_APPLICATION_H_
#define BMS_APPLICATION_BMS_APPLICATION_H_
#include <bsp_config.h>
/* ==================== EXTERN VARIABLES ==================== */

/* ==================== APPLICATION STATUS VARIABLES ==================== */
 extern U32 CAN_Rx_Counter_u32;
/* ==================== STRUCTURES ==================== */

/* ==================== BMS COMMAND STRUCTURE ==================== */
typedef enum {
    APP_CAN_FLAG	=0,
	APP_IGNITION_FLAG,
	APP_CV_MODE_FLAG,
	APP_CHARGE_DONE_MODE_FLAG,
	APP_DISCHARGE_DONE_MODE_FLAG,
	APP_SLEEP_MODE_FLAG,
	APP_PRECHARGE_TIMEOUT_FLAG,
	APP_PREDISCHARGE_TIMEOUT_FLAG,
	APP_INIT_TIMEOUT_FLAG,
    APP_TOTAL_FLAGS
} APP_Flag_En;
/* ==================== FUNCTION DECLARATIONS ==================== */

/* ==================== TIMER CALLBACK FUNCTIONS ==================== */
extern void APP_Timer0Ch1_Callback(void);
extern void APP_Timer0Ch2_Callback(void);

/* ==================== COMMAND INTERFACE FUNCTIONS ==================== */
void APP_SetFlag_gv(U8 Set_Idx_argu8);
void APP_ResetFlag_gv(U8 Reset_Idx_argu8);
BIN APP_GetFlag_gv(U8 Get_Idx_argu8);
U32 APP_GetCurrentMS_Count_gu32();
#endif /* BMS_APPLICATION_BMS_APPLICATION_H_ */
