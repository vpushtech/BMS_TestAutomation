/*
 * RTOS_100msThread_entry.c
 *
 *  Created on: 01-Sep-2025
 *      Author: RushikeshNitinKamble
 */

/* ==================== INCLUDES ==================== */
#include <can_comm.h>
#include <hsm.h>

#include <vpush_rtos.h>
/* ==================== EXTERN VARIABLES ==================== */
extern QueueHandle_t RTOS_CAN_Queue_st;
/* ==================== GLOBAL VARIABLES ==================== */
CAN_QueueDataFrame_tst receivedFrame_st;
/* ==================== THREAD ENTRY FUNCTION ==================== */

/* -----------------------------------------------------------------------------
 *  Function Name : RTOS_QueueReceiveThread_entry
 *  Description   : Main entry point for Queue Receive Thread
 *                  - Listens for incoming CAN messages from RTOS_CAN_Queue_st
 *                  - Processes different types of CAN messages (UDS, DEBUG, VEHICLE)
 *                  - Implements message routing based on message type
 *  Parameters    : pvParameters - RTOS task parameters (unused)
 *  Return Value  : None
 * ---------------------------------------------------------------------------*/
void RTOS_QueueReceiveThread_entry(void *pvParameters)
{
    (void) pvParameters;

	for (;;) {
		/* Wait for incoming CAN message from queue with infinite timeout */
		if (RTOS_QueueReceive_st(CAN_QUEUE, &receivedFrame_st, 0) == RTOS_STATUS_SUCCESS)
		{
			switch (receivedFrame_st.CAN_MassageType_en)
			{
			case CAN_MSG_UDS:
			{
				/* Process UDS diagnostic messages */
				CAN_Proccess_UDS_Messages(&receivedFrame_st);
				break;
			}

			case CAN_MSG_DEBUG:
			{
				/* Process debug and development messages */
				CAN_Proccess_Debug_Messages(&receivedFrame_st);
				break;
			}
			case CAN_MSG_VEHICLE:
			{
				/* Process vehicle operational messages */
				CAN_Proccess_Vehicle_Messages(&receivedFrame_st);
				break;
			}

			default:
			{
				break;
			}
			}
		}
	}
}
