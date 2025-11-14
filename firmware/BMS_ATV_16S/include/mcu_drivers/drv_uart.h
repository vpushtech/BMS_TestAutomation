#ifndef DRIVERS_DRV_UART_H_
#define DRIVERS_DRV_UART_H_

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

/* ==================== INCLUDE ==================== */
#include <common.h>
#include "lpuart_driver.h"
#include "lpuart1.h"
#include "drv_gpio.h"

/* ==================== DEFINE ==================== */
#define DRV_UART_TIMEOUT_MS                (3000U)
#define DRV_UART_DEFAULT_IRQ_PRIORITY      (3U)

/* ==================== ENUMS ==================== */
typedef enum
{
    DRV_UART_INSTANCE_0,
    DRV_UART_INSTANCE_1,
    DRV_MAX_UART_INSTANCE
} DRV_uartInstance_ten;

typedef enum
{
    DRV_UART_SUCCESS,
    DRV_UART_FAILED,
    DRV_UART_BUSY,
    DRV_UART_TIMEOUT
} DRV_uartStatus_ten;

/* ==================== INITIALIZATION/CONFIGURATION ==================== */
DRV_uartStatus_ten DRV_UART_Init_gen(U8 uartPinIdx_argu8);
DRV_uartStatus_ten DRV_UART_DeInit_gen(U8 uartPinIdx_argu8);

/* ==================== DATA TRANSFER ==================== */
DRV_uartStatus_ten DRV_UART_WriteBlock_gen(U8 uartPinIdx_argu8, const U8 *data_argptru8, U32 size_argu32);
DRV_uartStatus_ten DRV_UART_ReadBlock_gen(U8 uartPinIdx_argu8, U8 *data_argptru8, U32 size_argu32);
/* ==================== INTERRUPT HANDLERS ==================== */
void LPUART1_RxTx_IRQHandler(void);

#endif /* DRIVERS_DRV_UART_H_ */
