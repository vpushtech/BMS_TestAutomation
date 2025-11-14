/* ==================== HEADER ==================== */
/* -----------------------------------------------------------------------------
*  File Name     : TimerCallback.c
*  Created on    : 14-Aug-2025
*  Author        : Rushikesh
*  Description   : Timer callback functions implementation for RTOS application
*  Version       : 1.0
*  Modification History:
*  Date          Author          Description
*  ----------------------------------------------------------------------------
*  14-Aug-2025   Rushikesh       Initial Timer Callback Implementation
*  16-Aug-2025   Rushikesh       Added CAN Communication Timer Support
*  18-Aug-2025   Rushikesh       Added Sleep Mode and Shutdown Command Functions
* ---------------------------------------------------------------------------*/

/* ==================== INCLUDES ==================== */
#include <app_timer.h>
#include <can_comm.h>
#include"hsm.h"
/* ==================== GLOBAL VARIABLES ==================== */
 BIN APP_Set_Flag[APP_TOTAL_FLAGS] = {false};
 U32 CAN_Rx_Counter_u32=0;
 /* ==================== STATIC VARIABLES ==================== */
 static U32 APP_MS_Counter_u32=0;
 //static U32 APP_SleepCounter_m32=0;

 /* ==================== EXTERN VARIABLES ==================== */
 extern BIN CAN_Tx_Timer_b;
/* ==================== FUNCTION DEFINITIONS ==================== */

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : APP_Timer0Ch1_Callback
*   Description   : Timer Channel 1 interrupt service routine
*                   Manages sleep mode counter and triggers sleep mode
*   Parameters    : None
*   Return Value  : None
*  ---------------------------------------------------------------------------*/
void APP_Timer0Ch1_Callback(void)
{
	APP_MS_Counter_u32++;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : APP_Timer0Ch2_Callback
*   Description   : Timer Channel 2 interrupt service routine
*                   Manages CAN transmission timing and communication timeout
*   Parameters    : None
*   Return Value  : None
*  ---------------------------------------------------------------------------*/
void APP_Timer0Ch2_Callback(void)
{


	static U32 APP_InitTimeout_mu32=0;
	if(APP_GetFlag_gv(APP_INIT_TIMEOUT_FLAG)==false && HSM_STATE_IDLE==HSM_GetCurrentState_gen())
	{
		APP_InitTimeout_mu32++;
		if(0==APP_InitTimeout_mu32%500)
		{
			APP_InitTimeout_mu32=0;
			APP_SetFlag_gv(APP_INIT_TIMEOUT_FLAG);
		}
	}
	static U32 CAN_tx_Counter_mu32=0;
	 CAN_tx_Counter_mu32++;
    if(0==CAN_tx_Counter_mu32 % 500)
    {
    	CAN_tx_Counter_mu32=0;
    	CAN_Tx_Timer_b=true;
    }

   if(true==APP_GetFlag_gv(APP_CAN_FLAG))
   {
	   CAN_Rx_Counter_u32++;
	   if(0==CAN_Rx_Counter_u32%500)
	   {
		   CAN_Rx_Counter_u32=0;
		   APP_ResetFlag_gv(APP_CAN_FLAG);
	   }
   }


}
/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : APP_SetFlag_gv
*   Description   : Sets a specific application flag to true
*   Parameters    : Set_Idx_argu8 - Flag index from APP_FlagIndex_En
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void APP_SetFlag_gv(U8 Set_Idx_argu8) {

	if (Set_Idx_argu8 < APP_TOTAL_FLAGS) {
		APP_Set_Flag[Set_Idx_argu8] = true;
	}

}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : APP_Get_Flag
*   Description   : Gets the current state of a specific application flag
*   Parameters    : Get_Idx_argu8 - Flag index from APP_FlagIndex_En
*   Return Value  : BIN - Flag state (true/false)
*  --------------------------------------------------------------------------- */
BIN APP_GetFlag_gv(U8 Get_Idx_argu8) {
	if (Get_Idx_argu8 < APP_TOTAL_FLAGS) {
		return APP_Set_Flag[Get_Idx_argu8];
	}
	return false;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : APP_Reset_Flag
*   Description   : Resets a specific application flag to false
*   Parameters    : Reset_Idx_argu8 - Flag index from APP_FlagIndex_En
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void APP_ResetFlag_gv(U8 Reset_Idx_argu8) {
	if (Reset_Idx_argu8 < APP_TOTAL_FLAGS) {
		APP_Set_Flag[Reset_Idx_argu8] = false;
	}
}
U32 APP_GetCurrentMS_Count_gu32()
{
	return APP_MS_Counter_u32;
}
