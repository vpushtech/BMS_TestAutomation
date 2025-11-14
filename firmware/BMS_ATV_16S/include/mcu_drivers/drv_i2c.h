#ifndef DRIVERS_DRV_I2C_H_
#define DRIVERS_DRV_I2C_H_
/* ==================== INCLUDE ==================== */
#include <common.h>
#include "flexio_i2c.h"
#include "lpi2c1.h"
#include "drv_gpio.h"

/* ==================== DEFINE ==================== */
#define DRV_I2C_TIMEOUT_MS                (1000)

/* ==================== ENUMS ==================== */
typedef enum
{
    DRV_I2C_INSTANCE_0,
    DRV_I2C_INSTANCE_1,
    DRV_MAX_I2C_INSTANCE
} DRV_i2cInstance_ten;

typedef enum
{
    DRV_I2C_SUCCESS,
    DRV_I2C_FAILED,
    DRV_I2C_TRANSFER_ERROR,
    DRV_I2C_TIMEOUT
} DRV_i2cStatus_ten;

typedef struct
{
    DRV_i2cInstance_ten DRV_i2CInstance;
    U8 DRV_SlaveAddr_u8;
    BIN DRV_is10BitAddr_b;
} DRV_I2CConfig_tst;

/* ==================== INITIALIZATION/CONFIGURATION ==================== */
DRV_i2cStatus_ten DRV_I2C_Init_gen(U8 i2cPinIdx_argu8);
DRV_i2cStatus_ten DRV_I2C_DeInit_gen(U8 i2cPinIdx_argu8);

/* ==================== DATA TRANSFER ==================== */
DRV_i2cStatus_ten DRV_I2C_ReadBlock_gen(U8 i2cPinIdx_argu8, U8 *data_argptru8, U32 size_argu32, BIN stopBit_argb);
DRV_i2cStatus_ten DRV_I2C_WriteBlock_gen(U8 i2cPinIdx_argu8, const U8 *data_argptru8, U32 size_argu32, BIN stopBit_argb);
DRV_i2cStatus_ten DRV_I2C_WriteReadBlock_gen(U8 i2cPinIdx_argu8, const U8 *txdata_argptru8, U32 txsize_argu32,
                                           U8 *rxdata_argptru8, U32 rxsize_argu32);

#endif /* DRIVERS_DRV_I2C_H_ */
