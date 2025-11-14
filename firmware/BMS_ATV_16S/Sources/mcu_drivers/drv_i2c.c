/*******************************************************************************
 *  Description     : I2C Driver
 *  Author          : Rushikesh
 *  Created On      : 08-Jul-2025
 *  Version         : 2.0
 *  Modification History:
 *  Date        Author      Description
 *  ----------------------------------------------------------------------------
 *  18-Jul-2025 RUSHIKESH   I2C Driver Architecture Implementation
 *  31-Jul-2025 RUSHIKESH   I2C Driver Testing completed
 *  11-Aug-2025 RUSHIKESH   Guidelines Followed the naming Architecture Implementation
 ******************************************************************************/

/* ==================== INCLUDE FILES ==================== */
#include "drv_i2c.h"
#include <bsp_config.h>

/* ==================== CONFIGURATION DEFINITIONS ==================== */
/* Configuration for selecting I2C interface type */
#ifndef USE_LPI2C_INTERFACE
#define USE_LPI2C_INTERFACE 0  /* Set to 1 to use LPI2C, 0 for FlexIO */
#endif

/* ==================== STATE VARIABLES ==================== */
/* State variables for both interfaces */
#if (USE_LPI2C_INTERFACE == 1)
lpi2c_master_state_t lpi2cMasterState[BSP_MAX_I2C_PIN];
#else
flexio_i2c_master_state_t i2cMaster[BSP_MAX_I2C_PIN];
flexio_device_state_t flexIODeviceState[BSP_MAX_I2C_PIN];
#endif

/* ==================== STATIC VARIABLES ==================== */
static const U8 i2cInstances_arrst[BSP_MAX_I2C_PIN] = {
#if (USE_LPI2C_INTERFACE == 1)
    INST_LPI2C1  /* LPI2C instance mapping */
#else
    INST_FLEXIO_I2C  /* FlexIO instance mapping */
#endif
};

#if (USE_LPI2C_INTERFACE == 1)
static const lpi2c_master_user_config_t* i2cMasterConfig_arrst[BSP_MAX_I2C_PIN] = {
    &lpi2c1_MasterConfig0
};
#else
static const flexio_i2c_master_user_config_t* i2cMasterConfig_arrst[BSP_MAX_I2C_PIN] = {
    &flexio_i2c_MasterConfig0
};
#endif

/* ==================== GLOBAL VARIABLES ==================== */
extern DRV_I2CConfig_tst DRV_i2cConfigTable_gst[BSP_MAX_I2C_PIN];

/* ==================== PUBLIC FUNCTIONS ==================== */

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_I2C_Init_gen
*   Description   : Initializes I2C interface for specified pin
*   Parameters    : i2cPinIdx_argu8 - I2C pin index
*   Return Value  : DRV_i2cStatus_ten - Status of initialization
*  --------------------------------------------------------------------------- */
DRV_i2cStatus_ten DRV_I2C_Init_gen(U8 i2cPinIdx_argu8)
{
    status_t status;

#if (USE_LPI2C_INTERFACE == 1)

    /* Initialize LPI2C interface */
    status = LPI2C_DRV_MasterInit(i2cInstances_arrst[i2cPinIdx_argu8],
                                i2cMasterConfig_arrst[i2cPinIdx_argu8],
                                &lpi2cMasterState[i2cPinIdx_argu8]);
#else
    /* Initialize FlexIO interface */

    status = FLEXIO_DRV_InitDevice(i2cInstances_arrst[i2cPinIdx_argu8],
                                 &flexIODeviceState[i2cPinIdx_argu8]);
    if (status == STATUS_SUCCESS)
    {
        status = FLEXIO_I2C_DRV_MasterInit(i2cInstances_arrst[i2cPinIdx_argu8],
                                          i2cMasterConfig_arrst[i2cPinIdx_argu8],
                                          &i2cMaster[i2cPinIdx_argu8]);
    }
#endif

    return (status == STATUS_SUCCESS) ? DRV_I2C_SUCCESS : DRV_I2C_FAILED;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_I2C_DeInit_gen
*   Description   : Deinitializes I2C interface for specified pin
*   Parameters    : i2cPinIdx_argu8 - I2C pin index
*   Return Value  : DRV_i2cStatus_ten - Status of deinitialization
*  --------------------------------------------------------------------------- */
DRV_i2cStatus_ten DRV_I2C_DeInit_gen(U8 i2cPinIdx_argu8)
{
    status_t status;

#if (USE_LPI2C_INTERFACE == 1)
    status = LPI2C_DRV_MasterDeinit(i2cInstances_arrst[i2cPinIdx_argu8]);
#else
    status = FLEXIO_I2C_DRV_MasterDeinit(&i2cMaster[i2cPinIdx_argu8]);
    if (status == STATUS_SUCCESS)
    {
        status = FLEXIO_DRV_DeinitDevice(i2cInstances_arrst[i2cPinIdx_argu8]);
    }
#endif

    return (status == STATUS_SUCCESS) ? DRV_I2C_SUCCESS : DRV_I2C_FAILED;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_I2C_ReadBlock_gen
*   Description   : Reads data from I2C slave device in blocking mode
*   Parameters    : i2cPinIdx_argu8 - I2C pin index
*                   data_argptru8 - Pointer to store received data
*                   size_argu32 - Number of bytes to read
*                   stopBit_argb - Generate stop condition after transfer
*   Return Value  : DRV_i2cStatus_ten - Status of read operation
*  --------------------------------------------------------------------------- */
DRV_i2cStatus_ten DRV_I2C_ReadBlock_gen(U8 i2cPinIdx_argu8, U8 *data_argptru8, U32 size_argu32, BIN stopBit_argb)
{
    status_t status=DRV_I2C_FAILED;
    U8 slaveAddress_u8 = DRV_i2cConfigTable_gst[i2cPinIdx_argu8].DRV_SlaveAddr_u8;

#if (USE_LPI2C_INTERFACE == 1)
    BIN is10BitAddr_b=DRV_i2cConfigTable_gst[i2cPinIdx_argu8].DRV_is10BitAddr_b;
    LPI2C_DRV_MasterSetSlaveAddr(i2cInstances_arrst[i2cPinIdx_argu8],
                                slaveAddress_u8,
								is10BitAddr_b);

    status = LPI2C_DRV_MasterReceiveDataBlocking(i2cInstances_arrst[i2cPinIdx_argu8],
                                               data_argptru8,
                                               size_argu32,
                                               stopBit_argb,
                                               DRV_I2C_TIMEOUT_MS);
#else

    FLEXIO_I2C_DRV_MasterSetSlaveAddr(&i2cMaster[i2cPinIdx_argu8],
                                     slaveAddress_u8);


        status = FLEXIO_I2C_DRV_MasterReceiveDataBlocking(&i2cMaster[i2cPinIdx_argu8],
									                    data_argptru8,
														size_argu32,
														stopBit_argb,
                                                        DRV_I2C_TIMEOUT_MS);
#endif

    if (status == STATUS_I2C_RECEIVED_NACK)
    {
        return DRV_I2C_TRANSFER_ERROR;
    }
    else if (status == STATUS_TIMEOUT)
    {
        return DRV_I2C_TIMEOUT;
    }

    return (status == STATUS_SUCCESS) ? DRV_I2C_SUCCESS : DRV_I2C_FAILED;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_I2C_WriteBlock_gen
*   Description   : Writes data to I2C slave device in blocking mode
*   Parameters    : i2cPinIdx_argu8 - I2C pin index
*                   data_argptru8 - Pointer to data to be transmitted
*                   size_argu32 - Number of bytes to write
*                   stopBit_argb - Generate stop condition after transfer
*   Return Value  : DRV_i2cStatus_ten - Status of write operation
*  --------------------------------------------------------------------------- */
DRV_i2cStatus_ten DRV_I2C_WriteBlock_gen(U8 i2cPinIdx_argu8, const U8 *data_argptru8, U32 size_argu32, BIN stopBit_argb)
{
    status_t status;
    U8 slaveAddress_u8 = DRV_i2cConfigTable_gst[i2cPinIdx_argu8].DRV_SlaveAddr_u8;


#if (USE_LPI2C_INTERFACE == 1)
    BIN is10BitAddr_b=DRV_i2cConfigTable_gst[i2cPinIdx_argu8].DRV_is10BitAddr_b;
    LPI2C_DRV_MasterSetSlaveAddr(i2cInstances_arrst[i2cPinIdx_argu8],
                                slaveAddress_u8,
								is10BitAddr_b);

    status = LPI2C_DRV_MasterSendDataBlocking(i2cInstances_arrst[i2cPinIdx_argu8],
                                            data_argptru8,
                                            size_argu32,
                                            stopBit_argb,
                                            DRV_I2C_TIMEOUT_MS);
#else
    FLEXIO_I2C_DRV_MasterSetSlaveAddr(&i2cMaster[i2cPinIdx_argu8],
                                    slaveAddress_u8);
        status = FLEXIO_I2C_DRV_MasterSendDataBlocking(&i2cMaster[i2cPinIdx_argu8],
                                                     data_argptru8,
                                                     size_argu32,
                                                     stopBit_argb,
													 DRV_I2C_TIMEOUT_MS);

#endif

    if (status == STATUS_I2C_RECEIVED_NACK)
    {
        return DRV_I2C_TRANSFER_ERROR;
    }
    else if (status == STATUS_TIMEOUT)
    {
        return DRV_I2C_TIMEOUT;
    }

    return (status == STATUS_SUCCESS) ? DRV_I2C_SUCCESS : DRV_I2C_FAILED;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_I2C_WriteReadBlock_gen
*   Description   : Performs combined write followed by read operation
*   Parameters    : i2cPinIdx_argu8 - I2C pin index
*                   txdata_argptru8 - Pointer to transmit data
*                   txsize_argu32 - Number of bytes to write
*                   rxdata_argptru8 - Pointer to store received data
*                   rxsize_argu32 - Number of bytes to read
*   Return Value  : DRV_i2cStatus_ten - Status of combined operation
*  --------------------------------------------------------------------------- */
DRV_i2cStatus_ten DRV_I2C_WriteReadBlock_gen(U8 i2cPinIdx_argu8, const U8 *txdata_argptru8, U32 txsize_argu32,
                                          U8 *rxdata_argptru8, U32 rxsize_argu32)
{
    DRV_i2cStatus_ten status_en;

    status_en = DRV_I2C_WriteBlock_gen(i2cPinIdx_argu8, txdata_argptru8, txsize_argu32, true);

    if (status_en != DRV_I2C_SUCCESS)
    {
        return DRV_I2C_FAILED;
    }

    status_en = DRV_I2C_ReadBlock_gen(i2cPinIdx_argu8, rxdata_argptru8, rxsize_argu32, false);

    if (status_en != DRV_I2C_SUCCESS)
    {
        return DRV_I2C_FAILED;
    }

    return DRV_I2C_SUCCESS;
}
