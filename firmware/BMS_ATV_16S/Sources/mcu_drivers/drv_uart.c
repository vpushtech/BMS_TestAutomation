/*******************************************************************************
 *  Description     : UART Driver
 *  Author          : Rushikesh
 *  Created On      : 08-Jul-2025
 *  Version         : 2.0
 *  Modification History:
 *  Date        Author      Description
 *  ----------------------------------------------------------------------------
 *  18-Jul-2025 RUSHIKESH   UART Driver Architecture Implementation
 *  31-Jul-2025 RUSHIKESH   UART Driver Testing completed
 *  11-Aug-2025 RUSHIKESH   Guidelines Followed the naming Architecture Implementation
 ******************************************************************************/

/* ==================== INCLUDE FILES ==================== */
#include "drv_uart.h"
#include <bsp_config.h>

/* ==================== STATIC VARIABLES ==================== */
static const U8 uartInstances_arrst[BSP_MAX_UART_PIN] = {
    INST_LPUART1  /* UART instance mapping */
};

static const lpuart_user_config_t* uartConfig_arrst[BSP_MAX_UART_PIN] = {
    &lpuart1_InitConfig0
};

/* ==================== STATE VARIABLES ==================== */
static lpuart_state_t uartState[BSP_MAX_UART_PIN];

/* ==================== STATIC FUNCTIONS ==================== */
/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : uart_callback_mv
*   Description   : Callback function for UART read operations
*   Parameters    : driverState_argp - Pointer to driver state
*                   event_arg - UART event type
*                   userData_argp - Pointer to user data
*   Return Value  : None
*  --------------------------------------------------------------------------- */
static void uart_callback_mv(void *driverState_argp, uart_event_t event_arg, void *userData_argp)
{
    /* Unused parameters */
    (void)driverState_argp;
    (void)userData_argp;

    /* Check the event type */
    if (event_arg == UART_EVENT_RX_FULL)
    {
        /* Handle receive complete event */
        /* Add application-specific logic here */
    }
}

/* ==================== PUBLIC FUNCTIONS ==================== */

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_UART_Init_gen
*   Description   : Initializes UART interface for specified pin
*   Parameters    : uartPinIdx_argu8 - UART pin index
*   Return Value  : DRV_uartStatus_ten - Status of initialization
*  --------------------------------------------------------------------------- */
DRV_uartStatus_ten DRV_UART_Init_gen(U8 uartPinIdx_argu8)
{
    status_t status;

    if (uartPinIdx_argu8 >= BSP_MAX_UART_PIN)
    {
        return DRV_UART_FAILED;
    }

    status = LPUART_DRV_Init(uartInstances_arrst[uartPinIdx_argu8],
                           &uartState[uartPinIdx_argu8],
                           uartConfig_arrst[uartPinIdx_argu8]);

    if (status == STATUS_SUCCESS)
    {
        LPUART_DRV_InstallRxCallback(uartInstances_arrst[uartPinIdx_argu8],
                                   uart_callback_mv,
                                   NULL);
        return DRV_UART_SUCCESS;
    }

    return DRV_UART_FAILED;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_UART_DeInit_gen
*   Description   : Deinitializes UART interface for specified pin
*   Parameters    : uartPinIdx_argu8 - UART pin index
*   Return Value  : DRV_uartStatus_ten - Status of deinitialization
*  --------------------------------------------------------------------------- */
DRV_uartStatus_ten DRV_UART_DeInit_gen(U8 uartPinIdx_argu8)
{
    status_t status;

    if (uartPinIdx_argu8 >= BSP_MAX_UART_PIN)
    {
        return DRV_UART_FAILED;
    }

    status = LPUART_DRV_Deinit(uartInstances_arrst[uartPinIdx_argu8]);

    return (status == STATUS_SUCCESS) ? DRV_UART_SUCCESS : DRV_UART_FAILED;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_UART_WriteBlock_gen
*   Description   : Writes data to UART in blocking mode
*   Parameters    : uartPinIdx_argu8 - UART pin index
*                   data_argptru8 - Pointer to data to be transmitted
*                   size_argu32 - Number of bytes to write
*   Return Value  : DRV_uartStatus_ten - Status of write operation
*  --------------------------------------------------------------------------- */
DRV_uartStatus_ten DRV_UART_WriteBlock_gen(U8 uartPinIdx_argu8, const U8 *data_argptru8, U32 size_argu32)
{
    status_t status;

    if ((uartPinIdx_argu8 >= BSP_MAX_UART_PIN) || (data_argptru8 == NULL) || (size_argu32 == 0U))
    {
        return DRV_UART_FAILED;
    }

    status = LPUART_DRV_SendDataBlocking(uartInstances_arrst[uartPinIdx_argu8],
                                       data_argptru8,
                                       size_argu32,
                                       DRV_UART_TIMEOUT_MS);

    if (status == STATUS_SUCCESS)
    {
        return DRV_UART_SUCCESS;
    }
    else if (status == STATUS_BUSY)
    {
        return DRV_UART_BUSY;
    }
    else if (status == STATUS_TIMEOUT)
    {
        return DRV_UART_TIMEOUT;
    }

    return DRV_UART_FAILED;
}
/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_UART_ReadBlock_gen
*   Description   : Reads data from UART in blocking mode
*   Parameters    : uartPinIdx_argu8 - UART pin index
*                   data_argptru8 - Pointer to store received data
*                   size_argu32 - Number of bytes to read
*   Return Value  : DRV_uartStatus_en - Status of read operation
*  --------------------------------------------------------------------------- */
DRV_uartStatus_ten DRV_UART_ReadBlock_gen(U8 uartPinIdx_argu8, U8 *data_argptru8, U32 size_argu32)
{
    status_t status;

    if ((uartPinIdx_argu8 >= BSP_MAX_UART_PIN) || (data_argptru8 == NULL) || (size_argu32 == 0U))
    {
        return DRV_UART_FAILED;
    }

    status = LPUART_DRV_ReceiveDataBlocking(uartInstances_arrst[uartPinIdx_argu8],
                                          data_argptru8,
                                          size_argu32,
                                          DRV_UART_TIMEOUT_MS);

    if (status == STATUS_SUCCESS)
    {
        return DRV_UART_SUCCESS;
    }
    else if (status == STATUS_BUSY)
    {
        return DRV_UART_BUSY;
    }
    else if (status == STATUS_TIMEOUT)
    {
        return DRV_UART_TIMEOUT;
    }

    return DRV_UART_FAILED;
}


/* ======================================== END ======================================== */
