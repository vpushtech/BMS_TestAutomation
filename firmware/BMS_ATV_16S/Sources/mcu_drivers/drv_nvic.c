/*
 * drv_nvic.c
 *
 *  Description     : NVIC Driver
 *  Author          : Rushikesh
 *  Created On      : 15-Jul-2025
 *  Version         : 2.0
 *  Modification History:
 *  Date        Author      Description
 *  ----------------------------------------------------------------------------
 *  08-Jul-2025 RUSHIKESH   NVIC Driver Architecture Implementation
 *  31-Jul-2025 RUSHIKESH   NVIC Testing Done with wdg and GPIOs and Timers
 *  11-Aug-2025 RUSHIKESH   Guidelines Followed the naming Architecture Implementation
 ******************************************************************************/

/* ==================== INCLUDE FILES ==================== */
#include "drv_nvic.h"
#include "interrupt_manager.h"

/* ==================== GLOBAL VARIABLES ==================== */
static const DRV_NVIC_IrqConfig_tst DRV_NVIC_IRQ_GlobleTable_arrst[MAX_NVIC_IRQ] = {
    [NVIC_GPIOA_IRQ] = {PORTA_IRQn},
    [NVIC_GPIOB_IRQ] = {PORTB_IRQn},
    [NVIC_GPIOC_IRQ] = {PORTC_IRQn},
    [NVIC_GPIOD_IRQ] = {PORTD_IRQn},
    [NVIC_GPIOE_IRQ] = {PORTE_IRQn},

    [NVIC_CAN0_0_15_IRQ] = {CAN0_ORed_0_15_MB_IRQn},
    [NVIC_CAN0_16_31_IRQ] = {CAN0_ORed_16_31_MB_IRQn},
    [NVIC_CAN1_0_15_IRQ] = {CAN1_ORed_0_15_MB_IRQn},
    [NVIC_CAN2_0_15_IRQ] = {CAN2_ORed_0_15_MB_IRQn},

    [NVIC_LPIT0_CH0_IRQ] = {LPIT0_Ch0_IRQn},
    [NVIC_LPIT0_CH1_IRQ] = {LPIT0_Ch1_IRQn},
    [NVIC_LPIT0_CH2_IRQ] = {LPIT0_Ch2_IRQn},
    [NVIC_LPIT0_CH3_IRQ] = {LPIT0_Ch3_IRQn},

    [NVIC_ADC0_IRQ] = {ADC0_IRQn},
    [NVIC_ADC1_IRQ] = {ADC1_IRQn},

    [NVIC_FLASH_COMPLETE_IRQ] = {FTFC_IRQn},
    [NVIC_FLASH_FAULT_IRQ] = {FTFC_Fault_IRQn},

    [NVIC_LPI2C0_MASTER_IRQ] = {LPI2C0_Master_IRQn},
    [NVIC_LPI2C0_SLAVE_IRQ] = {LPI2C0_Slave_IRQn},
	[NVIC_FLEXIO_IRQ] ={FLEXIO_IRQn},
    [NVIC_WDG_IRQ] = {WDOG_EWM_IRQn}
};

/* ==================== PRIVATE FUNCTION DECLARATIONS ==================== */
static DRV_NVIC_Status_ten DRV_NVIC_SetPriority_men(DRV_NVIC_Irq_ten IRQIdx_argen, U8 priority_argu8);

/* ==================== FUNCTION DEFINITIONS ==================== */

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_NVIC_EnableIRQ_gen
*   Description   : Enables the specified interrupt request
*   Parameters    : IRQIdx_argen - Interrupt index to enable
*   Return Value  : DRV_NVIC_Status_ten - DRV_NVIC_STATUS_OK if successful, DRV_NVIC_STATUS_ERR if invalid index
*  --------------------------------------------------------------------------- */
DRV_NVIC_Status_ten DRV_NVIC_EnableIRQ_gen(DRV_NVIC_Irq_ten IRQIdx_argen)
{
    if (IRQIdx_argen < MAX_NVIC_IRQ) {
        INT_SYS_EnableIRQ(DRV_NVIC_IRQ_GlobleTable_arrst[IRQIdx_argen].DRV_irqNumber_en);
        return DRV_NVIC_STATUS_OK;
    }
    return DRV_NVIC_STATUS_ERR;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_NVIC_DisableIRQ_gen
*   Description   : Disables the specified interrupt request
*   Parameters    : IRQIdx_argen - Interrupt index to disable
*   Return Value  : DRV_NVIC_Status_ten - DRV_NVIC_STATUS_OK if successful, DRV_NVIC_STATUS_ERR if invalid index
*  --------------------------------------------------------------------------- */
DRV_NVIC_Status_ten DRV_NVIC_DisableIRQ_gen(DRV_NVIC_Irq_ten IRQIdx_argen)
{
    if (IRQIdx_argen < MAX_NVIC_IRQ) {
        INT_SYS_DisableIRQ(DRV_NVIC_IRQ_GlobleTable_arrst[IRQIdx_argen].DRV_irqNumber_en);
        return DRV_NVIC_STATUS_OK;
    }
    return DRV_NVIC_STATUS_ERR;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_NVIC_SetPriority_men
*   Description   : Sets priority level for specified interrupt
*   Parameters    : IRQIdx_argen - Interrupt index to configure
*                   priority_argu8 - Priority level (0-255)
*   Return Value  : DRV_NVIC_Status_ten - DRV_NVIC_STATUS_OK if successful, DRV_NVIC_STATUS_ERR if invalid index
*  --------------------------------------------------------------------------- */
static DRV_NVIC_Status_ten DRV_NVIC_SetPriority_men(DRV_NVIC_Irq_ten IRQIdx_argen, U8 priority_argu8)
{
    if (IRQIdx_argen < MAX_NVIC_IRQ) {
        INT_SYS_SetPriority(DRV_NVIC_IRQ_GlobleTable_arrst[IRQIdx_argen].DRV_irqNumber_en, priority_argu8);
        return DRV_NVIC_STATUS_OK;
    }
    return DRV_NVIC_STATUS_ERR;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_NVIC_EnableAll_gen
*   Description   : Enables all available interrupts
*   Parameters    : None
*   Return Value  : DRV_NVIC_Status_ten - Always returns DRV_NVIC_STATUS_OK
*  --------------------------------------------------------------------------- */
DRV_NVIC_Status_ten DRV_NVIC_EnableAll_gen(void)
{
    for(DRV_NVIC_Irq_ten IRQ_Index = NVIC_GPIOA_IRQ; IRQ_Index < MAX_NVIC_IRQ; IRQ_Index++)
    {
        INT_SYS_EnableIRQ(DRV_NVIC_IRQ_GlobleTable_arrst[IRQ_Index].DRV_irqNumber_en);
    }
    return DRV_NVIC_STATUS_OK;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_NVIC_DisableAll_gen
*   Description   : Disables all available interrupts
*   Parameters    : None
*   Return Value  : DRV_NVIC_Status_ten - Always returns DRV_NVIC_STATUS_OK
*  --------------------------------------------------------------------------- */
DRV_NVIC_Status_ten DRV_NVIC_DisableAll_gen(void)
{
    for(DRV_NVIC_Irq_ten IRQ_Index = NVIC_GPIOA_IRQ; IRQ_Index < MAX_NVIC_IRQ; IRQ_Index++)
    {
        INT_SYS_DisableIRQ(DRV_NVIC_IRQ_GlobleTable_arrst[IRQ_Index].DRV_irqNumber_en);
    }
    return DRV_NVIC_STATUS_OK;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_NVIC_IRQConfig_gen
*   Description   : Configures interrupt with specified priority level
*   Parameters    : IRQIdx_argen - Interrupt index to configure
*                   priority_argu8 - Priority level (0-255)
*   Return Value  : DRV_NVIC_Status_ten - DRV_NVIC_STATUS_OK if successful, error status otherwise
*  --------------------------------------------------------------------------- */
DRV_NVIC_Status_ten DRV_NVIC_IRQConfig_gen(DRV_NVIC_Irq_ten IRQIdx_argen, U8 priority_argu8)
{
	DRV_NVIC_Status_ten Status_En;

    Status_En = DRV_NVIC_DisableIRQ_gen(IRQIdx_argen);
    if (Status_En != DRV_NVIC_STATUS_OK) {
        return Status_En;
    }

    Status_En = DRV_NVIC_SetPriority_men(IRQIdx_argen, priority_argu8);
    if (Status_En != DRV_NVIC_STATUS_OK) {
        return Status_En;
    }

    Status_En = DRV_NVIC_EnableIRQ_gen(IRQIdx_argen);
    return Status_En;
}
