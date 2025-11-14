/*
 * CAN_Communication.h
 *
 *  Created on: [Date]
 *  Author: RushikeshNitinKamble
 *  Description: CAN Communication Header - Message Types and Function Declarations
 *  Version: 1.0
 *  Modification History:
 *  Date            Author          Description
 *  ----------------------------------------------------------------------------
 *  [Date]          Rushikesh       Initial CAN Communication Header
 *  [Date]          Rushikesh       Added Message Type Enumerations
 *  [Date]          Rushikesh       Added CAN Buffer Index Definitions
 ******************************************************************************/

#ifndef CAN_COMMUNICATION_CAN_COMMUNICATION_H_
#define CAN_COMMUNICATION_CAN_COMMUNICATION_H_

/* ==================== INCLUDES ==================== */
#include "flexcan_driver.h"
#include "canCom1.h"
#include <bsp_config.h>

/* ==================== ENUMERATIONS ==================== */
/* ==================== CAN MESSAGE ID ENUMERATION ==================== */
typedef enum {
    CAN_ID_0x01,
    CAN_ID_0x02,
    CAN_ID_0x03,
    CAN_ID_0x04,
    CAN_ID_0x05,
    CAN_ID_0x06,
    CAN_ID_0x07,
    CAN_ID_0x08,
	CAN_ID_0x1B0,
	CAN_ID_0x1C0,
	CAN_ID_0x1D0,
	CAN_ID_0x1E0,
    CAN_TOTAL_ID
} CAN_MsgID_ten;

/* ==================== CAN MESSAGE TYPE ENUMERATION ==================== */
typedef enum {
    CAN_MSG_UDS,
    CAN_MSG_DEBUG,
    CAN_MSG_VEHICLE,
} CAN_MassageType_ten;

/* ==================== CAN MODE ENUMERATION ==================== */
typedef enum {
    CAN_TX_MODE = 0,
    CAN_RX_MODE
} CAN_Mode_ten;

/* ==================== CAN BUFFER INDEX ENUMERATION ==================== */
typedef enum
{
    CAN_BUFFER_IDX_0=0,
	CAN_BUFFER_IDX_1,
    CAN_BUFFER_IDX_2,
	CAN_BUFFER_IDX_3,
	CAN_BUFFER_IDX_4,
	CAN_BUFFER_IDX_5,
	CAN_BUFFER_IDX_6,
	CAN_BUFFER_IDX_7,
	CAN_BUFFER_IDX_8,
	CAN_BUFFER_IDX_9,
	CAN_BUFFER_IDX_10,
	CAN_BUFFER_IDX_11
} CAN_Buffer_IDX_ten;

/* ==================== STRUCTURES ==================== */

/* ==================== CAN STATUS STRUCTURE ==================== */
typedef struct {
    U8 CAN_Status_u8;
} CAN_Status_tst;

/* ==================== CAN QUEUE DATA FRAME STRUCTURE ==================== */
typedef struct {
    CAN_MassageType_ten CAN_MassageType_en;
    CAN_Buffer_IDX_ten CAN_BufferIdx_en;
    DRV_CanFrame_tst CAN_DataFrame_st;
} __attribute__((packed)) CAN_QueueDataFrame_tst;

/* ==================== EXTERN VARIABLES ==================== */
extern BIN CAN_Tx_Timer_b;
extern CAN_QueueDataFrame_tst CAN_QueueDataFrame_St[CAN_TOTAL_ID];

/* ==================== FUNCTION DECLARATIONS ==================== */

/* ==================== CAN CALLBACK FUNCTIONS ==================== */
extern void CAN_Callback(U8 instance, flexcan_event_type_t event,
                         U32 buffIdx, flexcan_state_t *state);

/* ==================== MESSAGE PROCESSING FUNCTIONS ==================== */
void CAN_Massages_Tranmission(void);
void CAN_Proccess_Vehicle_Messages(CAN_QueueDataFrame_tst *message_argst);
void CAN_Proccess_Debug_Messages(CAN_QueueDataFrame_tst *message_argst);
void CAN_Proccess_UDS_Messages(CAN_QueueDataFrame_tst *message_argst);

/* ==================== CAN MODE MANAGEMENT FUNCTIONS ==================== */
void CAN_SetMode(CAN_Mode_ten CAN_Mode_argen);
CAN_Mode_ten CAN_GetMode(void);
/* ==================== BIT MANIPULATION FUNCTIONS ==================== */
U8 CAN_MessageClearBit_gu8(U8 num_argu8, U8 bitPosition_argu8);
U8 CAN_MessageSetBit_gu8(U8 num_argu8, U8 bitPosition_argu8);

#endif /* CAN_COMMUNICATION_CAN_COMMUNICATION_H_ */
