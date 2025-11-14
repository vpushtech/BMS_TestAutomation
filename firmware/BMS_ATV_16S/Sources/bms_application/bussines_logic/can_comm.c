
/*
 * CAN_Communication.c
 *
 *  Created on: [Date]
 *  Author: RushikeshNitinKamble
 *  Description: CAN Communication Module - Message Processing and Management
 *  Version: 1.0
 *  Modification History:
 *  Date            Author          Description
 *  ----------------------------------------------------------------------------
 *  [Date]          Rushikesh       Initial CAN Communication Implementation
 *  [Date]          Rushikesh       Added CAN Message Processing Functions
 *  [Date]          Rushikesh       Integrated RTOS Queue Support
 ******************************************************************************/

#include <app_timer.h>
#include <can_comm.h>
#include <vpush_rtos.h>
#include "drv_can.h"
#include"hsm.h"
#include"automation.h"
/* ==================== GLOBAL VARIABLE DEFINITIONS ==================== */
CAN_Status_tst CAN_Status_St = {0};
extern U32 CAN_Rx_Counter_u32;
BIN CAN_Tx_Timer_b = false;
/* ==================== CAN QUEUE DATA FRAME CONFIGURATION ==================== */
CAN_QueueDataFrame_tst CAN_QueueDataFrame_St[CAN_TOTAL_ID] = {
    [CAN_ID_0x01] = {
        CAN_MSG_DEBUG,CAN_BUFFER_IDX_0,
        {0x01, DRV_CAN_ID_MODE_STANDARD, DRV_CAN_FRAME_TYPE_DATA, 8, {0}}
    },
    [CAN_ID_0x02] = {
        CAN_MSG_DEBUG,CAN_BUFFER_IDX_1,
        {0x02, DRV_CAN_ID_MODE_STANDARD, DRV_CAN_FRAME_TYPE_DATA, 8, {0}}
    },
    [CAN_ID_0x03] = {
        CAN_MSG_DEBUG,CAN_BUFFER_IDX_2,
        {0x03, DRV_CAN_ID_MODE_STANDARD, DRV_CAN_FRAME_TYPE_DATA, 8, {0}}
    },
    [CAN_ID_0x04] = {
        CAN_MSG_DEBUG,CAN_BUFFER_IDX_3,
        {0x04, DRV_CAN_ID_MODE_STANDARD, DRV_CAN_FRAME_TYPE_DATA, 8, {0}}
    },
    [CAN_ID_0x05] = {
        CAN_MSG_DEBUG,CAN_BUFFER_IDX_4,
        {0x05, DRV_CAN_ID_MODE_STANDARD, DRV_CAN_FRAME_TYPE_DATA, 8, {0}}
    },
    [CAN_ID_0x06] = {
        CAN_MSG_DEBUG,CAN_BUFFER_IDX_5,
        {0x06, DRV_CAN_ID_MODE_STANDARD, DRV_CAN_FRAME_TYPE_DATA, 8, {0}}
    },
    [CAN_ID_0x07] = {
        CAN_MSG_DEBUG,CAN_BUFFER_IDX_6,
        {0x07, DRV_CAN_ID_MODE_STANDARD, DRV_CAN_FRAME_TYPE_DATA, 8, {0}}
    },
    [CAN_ID_0x08] = {
        CAN_MSG_DEBUG,CAN_BUFFER_IDX_7,
        {0x08, DRV_CAN_ID_MODE_STANDARD, DRV_CAN_FRAME_TYPE_DATA, 8, {0}}
    },
    [CAN_ID_0x1B0] = {
        CAN_MSG_DEBUG,CAN_BUFFER_IDX_8,
        {0x1B0, DRV_CAN_ID_MODE_STANDARD, DRV_CAN_FRAME_TYPE_DATA, 8, {0}}
    },
    [CAN_ID_0x1C0] = {
        CAN_MSG_DEBUG,CAN_BUFFER_IDX_9,
        {0x1C0, DRV_CAN_ID_MODE_STANDARD, DRV_CAN_FRAME_TYPE_DATA, 8, {0}}
    },
    [CAN_ID_0x1D0] = {
        CAN_MSG_DEBUG,CAN_BUFFER_IDX_10,
        {0x1D0, DRV_CAN_ID_MODE_STANDARD, DRV_CAN_FRAME_TYPE_DATA, 8, {0}}
    },
    [CAN_ID_0x1E0] = {
        CAN_MSG_DEBUG,CAN_BUFFER_IDX_11,
        {0x1E0, DRV_CAN_ID_MODE_STANDARD, DRV_CAN_FRAME_TYPE_DATA, 8, {0}}
    },
};

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : CAN_Callback
*   Description   : CAN interrupt callback function for RX/TX events
*                   Handles received messages and sends to RTOS queue
*   Parameters    : instance - CAN instance identifier
*                   eventType - Type of CAN event (RX/TX/Error)
*                   buffIdx - Buffer index for the event
*                   flexcanState - CAN controller state information
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void CAN_Callback(U8 instance, flexcan_event_type_t eventType, U32 buffIdx, flexcan_state_t *flexcanState)
{
    (void)flexcanState;
    (void)instance;
    CAN_QueueDataFrame_tst CAN_Recieve_Frame;
    switch (eventType)
    {
        case FLEXCAN_EVENT_RX_COMPLETE:
        {
            CAN_SetMode(CAN_RX_MODE);

            switch (buffIdx)
            {
                case CAN_BUFFER_IDX_8:
                {
                    if(DRV_CAN_ReceiveNonBlock_gen(DRV_CAN_INSTANCE_1, CAN_BUFFER_IDX_8, &CAN_Recieve_Frame.CAN_DataFrame_st) == DRV_CAN_STATUS_SUCCESS)
                    {
                    	CAN_Rx_Counter_u32=0;
/*                       APP_SetFlag_gv(APP_CAN_FLAG);*/
                        CAN_Recieve_Frame.CAN_MassageType_en=CAN_MSG_DEBUG;
                        RTOS_QueueSendFromISR_st(CAN_QUEUE, &CAN_Recieve_Frame);
                    }
                    break;
                }
                case CAN_BUFFER_IDX_9: {
                if (DRV_CAN_ReceiveNonBlock_gen(DRV_CAN_INSTANCE_1, CAN_BUFFER_IDX_9,&CAN_Recieve_Frame.CAN_DataFrame_st)== DRV_CAN_STATUS_SUCCESS) {
				CAN_Recieve_Frame.CAN_MassageType_en = CAN_MSG_DEBUG;
				Common_Memcpy_gv(&CAN_QueueDataFrame_St[CAN_ID_0x1C0],& CAN_Recieve_Frame, sizeof(CAN_QueueDataFrame_tst));
				TEST_ReadTestCases_gv();
				RTOS_QueueSendFromISR_st(CAN_QUEUE, &CAN_Recieve_Frame);
			}
			break;
		}
                default:
                    break;
            }
            break;
        }
        case FLEXCAN_EVENT_TX_COMPLETE:

            break;

        case FLEXCAN_EVENT_ERROR:

            break;

        default:
            break;
    }
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : CAN_Proccess_Vehicle_Messages
*   Description   : Processes vehicle-specific CAN messages
*                   Handles both TX and RX modes for vehicle communication
*   Parameters    : message_argst - Pointer to CAN message_argst structure
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void CAN_Proccess_Vehicle_Messages(CAN_QueueDataFrame_tst *message_argst)
{
    CAN_Mode_ten CAN_Mode_u8 = CAN_GetMode();

    switch(CAN_Mode_u8)
    {
        case CAN_TX_MODE:
        {
            switch(message_argst->CAN_DataFrame_st.DRV_CanId_u32)
            {

                default:
                    break;
            }
            break;
        }
        case CAN_RX_MODE:
        {
            switch(message_argst->CAN_DataFrame_st.DRV_CanId_u32)
            {

                default:
                    break;
            }
            break;
        }
    }
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : CAN_Proccess_Debug_Messages
*   Description   : Processes debug CAN messages for system monitoring
*                   Configures and transmits debug data via CAN
*   Parameters    : message_argst - Pointer to CAN message_argst structure
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void CAN_Proccess_Debug_Messages(CAN_QueueDataFrame_tst *message_argst)
{
    CAN_Mode_ten CAN_Mode_u8 = CAN_GetMode();

    switch(CAN_Mode_u8)
    {
        case CAN_TX_MODE:
        {
        	  DRV_CAN_ConfigTxBuffer_gen(DRV_CAN_INSTANCE_1, message_argst->CAN_BufferIdx_en,&message_argst->CAN_DataFrame_st,message_argst->CAN_DataFrame_st.DRV_CanId_u32);
        	  DRV_CAN_TransmitBlock_gen(DRV_CAN_INSTANCE_1, message_argst->CAN_BufferIdx_en, &message_argst->CAN_DataFrame_st);
            break;
        }
        case CAN_RX_MODE:
        {
            switch(message_argst->CAN_DataFrame_st.DRV_CanId_u32)
            {
                case 0x1B0:
                {
                    Common_Memcpy_gv(&CAN_QueueDataFrame_St[CAN_ID_0x1B0], message_argst, sizeof(CAN_QueueDataFrame_tst));
                    break;
                }
                case 0x1C0:
                {
                	// Common_Memcpy_gv(&CAN_QueueDataFrame_St[CAN_ID_0x1C0], message_argst, sizeof(CAN_QueueDataFrame_tst));
                }

                default:
                    break;
            }
            break;
        }
    }
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : CAN_Proccess_UDS_Messages
*   Description   : Processes UDS (Unified Diagnostic Services) CAN messages
*                   Handles diagnostic communication protocols
*   Parameters    : message_argst - Pointer to CAN message_argst structure
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void CAN_Proccess_UDS_Messages(CAN_QueueDataFrame_tst *message_argst)
{
    CAN_Mode_ten CAN_Mode_u8 = CAN_GetMode();

    switch(CAN_Mode_u8)
    {
        case CAN_TX_MODE:
        {
            switch(message_argst->CAN_DataFrame_st.DRV_CanId_u32)
            {

                default:
                    break;
            }
            break;
        }
        case CAN_RX_MODE:
        {
            switch(message_argst->CAN_DataFrame_st.DRV_CanId_u32)
            {

                default:
                    break;
            }
            break;
        }
    }
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : CAN_SetMode
*   Description   : Sets the current CAN communication mode
*   Parameters    : CAN_Mode_ten - CAN mode to set (TX/RX)
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void CAN_SetMode(CAN_Mode_ten CAN_Mode_argen)
{
    CAN_Status_St.CAN_Status_u8 = CAN_Mode_argen;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : CAN_GetMode
*   Description   : Gets the current CAN communication mode
*   Parameters    : None
*   Return Value  : CAN_Mode_ten - Current CAN mode
*  --------------------------------------------------------------------------- */
CAN_Mode_ten CAN_GetMode(void)
{
    return CAN_Status_St.CAN_Status_u8;
}
/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : CAN_MessageSetBit_gu8
*   Description   : Sets a specific bit in a byte
*   Parameters    : num_argu8 - Input byte value
*                   bitPosition_argu8 - Bit position to set (0-7)
*   Return Value  : U8 - Modified byte with bit set
*  --------------------------------------------------------------------------- */
U8 CAN_MessageSetBit_gu8(U8 num_argu8, U8 bitPosition_argu8)
{
    U8 bitmask = 1 << bitPosition_argu8;
    return num_argu8 | bitmask;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : CAN_MessageClearBit_gu8
*   Description   : Clears a specific bit in a byte
*   Parameters    : num_argu8 - Input byte value
*                   bitPosition_argu8 - Bit position to clear (0-7)
*   Return Value  : U8 - Modified byte with bit cleared
*  --------------------------------------------------------------------------- */
U8 CAN_MessageClearBit_gu8(U8 num_argu8, U8 bitPosition_argu8)
{
    U8 bitmask = (U8) ~(1 << bitPosition_argu8);
    return num_argu8 & bitmask;
}
