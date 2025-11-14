#ifndef DRIVERS_DRV_CAN_H_
#define DRIVERS_DRV_CAN_H_

/*******************************************************************************
 *  HEADER FILE INCLUDES
 ******************************************************************************/
#include <common.h>
#include "device_registers.h"
/*#include <string.h>*/
#include "drv_nvic.h"

#define DRV_CAN_TIMEOUT  500
/* ==================== TYPE DEFINITIONS ==================== */
typedef enum {
    DRV_CAN_STATUS_SUCCESS,
    DRV_CAN_STATUS_ERROR,
} DRV_CanStatus_ten;

typedef enum {
    DRV_CAN_ID_MODE_STANDARD,
    DRV_CAN_ID_MODE_EXTENDED
} DRV_CanIdMode_ten;

typedef enum {
    DRV_CAN_FRAME_TYPE_DATA,
    DRV_CAN_FRAME_TYPE_REMOTE
} DRV_CanFrameType_ten;

typedef enum {
    DRV_CAN_FD_DISABLED = 0,
    DRV_CAN_FD_ENABLED
} DRV_CanFdMode_ten;

typedef enum {
    DRV_CAN_BRS_DISABLED = 0,
    DRV_CAN_BRS_ENABLED
} DRV_CanBitRateSwitch_ten;

typedef enum {
    DRV_CAN_INSTANCE_1 = 0,
    DRV_CAN_INSTANCE_2,
    DRV_CAN_INSTANCE_3,
    DRV_CAN_MAX_INSTANCE
} DRV_CanInstance_ten;

typedef struct {
    U32 DRV_CanId_u32;
    DRV_CanIdMode_ten DRV_IdMode_en;
    DRV_CanFrameType_ten DRV_FrameType_en;
    U8 DRV_DataLength_u8;
    U8 DRV_Data_arru8[8];
} DRV_CanFrame_tst;

/* ==================== INITIALIZATION/CONFIGURATION ==================== */
DRV_CanStatus_ten DRV_CAN_Init_gen(DRV_CanInstance_ten instance_arg);
DRV_CanStatus_ten DRV_CAN_Deinit_gen(DRV_CanInstance_ten instance_enarg);

/* ==================== TRANSMIT CONFIGURATION & OPERATIONS ==================== */
DRV_CanStatus_ten DRV_CAN_ConfigTxBuffer_gen(DRV_CanInstance_ten instance_arg,
                                  U8 bufferIdx_argu8,
                                  const DRV_CanFrame_tst* frameConfig_argst,
                                  U32 canId_u32);
DRV_CanStatus_ten DRV_CAN_TransmitBlock_gen(DRV_CanInstance_ten instance_enarg,
                            U8 bufferIdx_argu8,
                            const DRV_CanFrame_tst* frame_argst);
DRV_CanStatus_ten DRV_CAN_TransmitNonBlock_gen(DRV_CanInstance_ten instance_enarg,
                            U8 bufferIdx_argu8,
                            const DRV_CanFrame_tst* frame_argst);
/* ==================== RECEIVE CONFIGURATION & OPERATIONS ==================== */
DRV_CanStatus_ten DRV_CAN_ConfigRxBuffer_gen(DRV_CanInstance_ten instance_arg,
                                  U8 bufferIdx_argu8,
                                  const DRV_CanFrame_tst* frameConfig_argst,
                                  U32 RxmsgId_argu32);
DRV_CanStatus_ten DRV_CAN_ReceiveBlock_gen(DRV_CanInstance_ten instance_enarg,
                           U8 bufferIdx_argu8,
                           DRV_CanFrame_tst* frame_argst);
DRV_CanStatus_ten DRV_CAN_ReceiveNonBlock_gen(DRV_CanInstance_ten instance_enarg,
                           U8 bufferIdx_argu8,
                           DRV_CanFrame_tst* frame_argst);
DRV_CanStatus_ten DRV_CAN_SetRxFilter_gen(DRV_CanInstance_ten instance_arg,
                               U8 bufferIdx_argu8,
                               U32 mask_u32,
                               DRV_CanIdMode_ten idMode_enarg);
/* ==================== CAN Callback ==================== */

#endif /* DRIVERS_DRV_CAN_H_ */
