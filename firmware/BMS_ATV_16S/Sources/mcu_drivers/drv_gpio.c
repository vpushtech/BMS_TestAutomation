/*
 * drv_gpio.c
 *
 *  Description     : GPIO Driver
 *  Author          : Rushikesh
 *  Created On      : 08-Jul-2025
 *  Version         : 2.0
 *  Modification History:
 *  Date        Author      Description
 *  ----------------------------------------------------------------------------
 *  08-Jul-2025 RUSHIKESH   GPIO Driver Architecture Implementation
 *  14-Jul-2025 RUSHIKESH   GPIO Driver Tested Done with LEDs and Button taken the Trace over Logic Analyzer
 *  11-Aug-2025 RUSHIKESH   Guidelines Followed the naming Architecture Implementation
 ******************************************************************************/
/* ==================== INCLUDE FILES ==================== */
#include <bsp_config.h>
#include "drv_gpio.h"

/* ==================== GLOBAL VARIABLES ==================== */
extern DRV_GPIOConfig_tst DRV_GpioConfigTable_gst[BSP_MAX_PIN];

/* ==================== STATIC FUNCTION DECLARATIONS ==================== */
static void DRV_GPIOA_ISRHandler_v(void);
static void DRV_GPIOB_ISRHandler_v(void);
static void DRV_GPIOC_ISRHandler_v(void);
static void DRV_GPIOD_ISRHandler_v(void);
static void DRV_GPIOE_ISRHandler_v(void);

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_GPIO_Init_gen
*   Description   : Initializes all GPIO pins and installs interrupt handlers
*   Parameters    : None
*   Return Value  : DRV_GpioStatus_ten - Initialization status
*  --------------------------------------------------------------------------- */
DRV_GpioStatus_ten DRV_GPIO_Init_gen(void)
{
    status_t status = PINS_DRV_Init(NUM_OF_CONFIGURED_PINS, g_pin_mux_InitConfigArr);
    if (status != STATUS_SUCCESS)
    {
        return DRV_GPIO_STATUS_ERR;
    }
    for(U8 pinIdx_argu8=0;pinIdx_argu8<BSP_MAX_PIN;pinIdx_argu8++)
    {
        if(DRV_GpioConfigTable_gst[pinIdx_argu8].DRV_Port_en >= DRV_GPIO_PORT_MAX)
        {
            return DRV_GPIO_STATUS_ERR;
        }
    	 U32 port_base_u32 = PTA_BASE + (0x40 * DRV_GpioConfigTable_gst[pinIdx_argu8].DRV_Port_en);
         U32 portbase=PORTA_BASE+(0x1000*DRV_GpioConfigTable_gst[pinIdx_argu8].DRV_Port_en);
         PINS_DRV_SetMuxModeSel((PORT_Type*)portbase,DRV_GpioConfigTable_gst[pinIdx_argu8].DRV_Pin_en,DRV_GpioConfigTable_gst[pinIdx_argu8].DRV_PinMode_en);
    	 PINS_DRV_SetPinDirection((GPIO_Type *)port_base_u32,(U32)DRV_GpioConfigTable_gst[pinIdx_argu8].DRV_Pin_en,DRV_GpioConfigTable_gst[pinIdx_argu8].DRV_GpioDirection_en);
    	 if(DRV_GpioConfigTable_gst[pinIdx_argu8].DRV_IntruptEnabled_b)
    	 {
    		 PINS_DRV_SetPinIntSel((PORT_Type *)portbase,(U32)DRV_GpioConfigTable_gst[pinIdx_argu8].DRV_Pin_en,DRV_GpioConfigTable_gst[pinIdx_argu8].DRV_IntOptions_en);
    	 }

    	 PINS_DRV_SetPullSel((PORT_Type *)portbase,(U32)DRV_GpioConfigTable_gst[pinIdx_argu8].DRV_Pin_en,DRV_GpioConfigTable_gst[pinIdx_argu8].DRV_GpioPuPdConfig_en);

        }
    INT_SYS_InstallHandler(PORTA_IRQn, DRV_GPIOA_ISRHandler_v, (void *)NULL);
    INT_SYS_InstallHandler(PORTB_IRQn, DRV_GPIOB_ISRHandler_v, (void *)NULL);
    INT_SYS_InstallHandler(PORTC_IRQn, DRV_GPIOC_ISRHandler_v, (void *)NULL);
    INT_SYS_InstallHandler(PORTD_IRQn, DRV_GPIOD_ISRHandler_v, (void *)NULL);
    INT_SYS_InstallHandler(PORTE_IRQn, DRV_GPIOE_ISRHandler_v, (void *)NULL);
    return DRV_GPIO_STATUS_OK;
}

/* ==================== GPIO OPERATION FUNCTIONS ==================== */
/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_GPIO_ClearPin_gen
*   Description   : Clears (sets to low) specified GPIO pin
*   Parameters    : pinIdx_argu8 - GPIO pin index
*   Return Value  : DRV_GpioStatus_ten - Operation status
*  --------------------------------------------------------------------------- */
DRV_GpioStatus_ten DRV_GPIO_ClearPin_gen(U8 pinIdx_argu8)
{
    if(pinIdx_argu8<BSP_MAX_PIN)
    {
         U32 port_base_u32 = PTA_BASE + (0x40 * DRV_GpioConfigTable_gst[pinIdx_argu8].DRV_Port_en);
         PINS_DRV_ClearPins((GPIO_Type *)port_base_u32,(U32)DRV_GpioConfigTable_gst[pinIdx_argu8].DRV_Pin_en);
         return DRV_GPIO_STATUS_OK;
    }
    return DRV_GPIO_STATUS_ERR;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_GPIO_SetOutputPin_gen
*   Description   : Sets GPIO pin to specified state
*   Parameters    : pinIdx_argu8 - GPIO pin index
*                   state_argen - Desired pin state
*   Return Value  : DRV_GpioStatus_ten - Operation status
*  --------------------------------------------------------------------------- */
DRV_GpioStatus_ten DRV_GPIO_SetOutputPin_gen(U8 pinIdx_argu8, DRV_GpioState_ten state_argen)
{
    if(pinIdx_argu8<BSP_MAX_PIN)
    {
        U32 port_base_u32 = PTA_BASE + (0x40 * DRV_GpioConfigTable_gst[pinIdx_argu8].DRV_Port_en);
        PINS_DRV_WritePin((GPIO_Type *)port_base_u32,
                         (U32)DRV_GpioConfigTable_gst[pinIdx_argu8].DRV_Pin_en,
                         (U8)state_argen);
        return DRV_GPIO_STATUS_OK;
    }
    return DRV_GPIO_STATUS_ERR;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_GPIO_TogglePin_gen
*   Description   : Toggles specified GPIO pin state
*   Parameters    : pinIdx_argu8 - GPIO pin index
*   Return Value  : DRV_GpioStatus_ten - Operation status
*  --------------------------------------------------------------------------- */
DRV_GpioStatus_ten DRV_GPIO_TogglePin_gen(U8 pinIdx_argu8)
{
    if(pinIdx_argu8<BSP_MAX_PIN)
    {
        U32 port_base_u32 = PTA_BASE + (0x40 * DRV_GpioConfigTable_gst[pinIdx_argu8].DRV_Port_en);
        PINS_DRV_TogglePins((GPIO_Type *)port_base_u32,
                           DRV_GPIO_PIN_MASK(DRV_GpioConfigTable_gst[pinIdx_argu8].DRV_Pin_en));
        return DRV_GPIO_STATUS_OK;
    }
    return DRV_GPIO_STATUS_ERR;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_GPIO_ReadPin_gen
*   Description   : Reads current state of specified GPIO pin
*   Parameters    : pinIdx_argu8 - GPIO pin index
*                   value_argu8 - Pointer to store pin state
*   Return Value  : DRV_GpioStatus_ten - Operation status
*  --------------------------------------------------------------------------- */
DRV_GpioStatus_ten DRV_GPIO_ReadPin_gen(U8 pinIdx_argu8, U8* value_argu8)
{
    if(pinIdx_argu8<BSP_MAX_PIN)
    {
        U32 port_base_u32 = PTA_BASE + (0x40 * DRV_GpioConfigTable_gst[pinIdx_argu8].DRV_Port_en);
        *value_argu8 = (PINS_DRV_ReadPins((GPIO_Type *)port_base_u32) &
                       DRV_GPIO_PIN_MASK(DRV_GpioConfigTable_gst[pinIdx_argu8].DRV_Pin_en)) ? 1 : 0;
        return DRV_GPIO_STATUS_OK;
    }
    return DRV_GPIO_STATUS_ERR;
}

DRV_GpioStatus_ten DRV_GPIO_PinConfig_gen(U8 pinIdx_argu8,GPIO_PinMode_ten GPIO_PinMode_argen)
{
	 U32 portbase=PORTA_BASE+(0x1000*DRV_GpioConfigTable_gst[pinIdx_argu8].DRV_Port_en);
	 if(pinIdx_argu8<BSP_MAX_PIN)
	 {
		 PINS_DRV_SetMuxModeSel((PORT_Type*)portbase,DRV_GpioConfigTable_gst[pinIdx_argu8].DRV_Pin_en,GPIO_PinMode_argen);
		  return DRV_GPIO_STATUS_OK;
	 }


    return DRV_GPIO_STATUS_ERR;
}
/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_GPIO_ClearIntrupt_gen
*   Description   : Clears GPIO interrupt flag for specified pin
*   Parameters    : pinIdx_argu8 - GPIO pin index
*   Return Value  : DRV_GpioStatus_ten - Operation status
*  --------------------------------------------------------------------------- */
DRV_GpioStatus_ten DRV_GPIO_ClearIntrupt_gen(U8 pinIdx_argu8)
{
    if(pinIdx_argu8<BSP_MAX_PIN)
    {
        U32 portbase=PORTA_BASE+(0x1000*DRV_GpioConfigTable_gst[pinIdx_argu8].DRV_Port_en);
        PINS_DRV_ClearPinIntFlagCmd((PORT_Type*)portbase,DRV_GpioConfigTable_gst[pinIdx_argu8].DRV_Pin_en);
        return DRV_GPIO_STATUS_OK;
    }
    return DRV_GPIO_STATUS_ERR;
}



/* ==================== INTERRUPT HANDLERS ==================== */
/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_GPIOA_ISRHandler_v
*   Description   : GPIO Port A interrupt service routine
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
static void DRV_GPIOA_ISRHandler_v(void)
{
	DRV_GPIO_ClearIntrupt_gen(BSP_MCU_CFET_CTRL);
	DRV_GPIO_ClearIntrupt_gen(BSP_MCU_DFET_CTRL);
	DRV_GPIO_ClearIntrupt_gen(BSP_SW_OPT_MCU);
	DRV_GPIO_ClearIntrupt_gen(BSP_AFE_WAKE_MCU);
    /* Implementation to be added */
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_GPIOB_ISRHandler_v
*   Description   : GPIO Port B interrupt service routine
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
static void DRV_GPIOB_ISRHandler_v(void)
{
	DRV_GPIO_ClearIntrupt_gen(BSP_AFE_WAKE_MCU);
    /* Implementation to be added */
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_GPIOC_ISRHandler_v
*   Description   : GPIO Port C interrupt service routine
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
static void DRV_GPIOC_ISRHandler_v(void)
{
	DRV_GPIO_ClearIntrupt_gen(BSP_FET_DSG_CTL_MCU);
	DRV_GPIO_ClearIntrupt_gen(BSP_FET_CHG_CTL_MCU);
    /* Implementation to be added */
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_GPIOD_ISRHandler_v
*   Description   : GPIO Port D interrupt service routine
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
static void DRV_GPIOD_ISRHandler_v(void)
{
	DRV_GPIO_ClearIntrupt_gen(BSP_FET_DSG_CTL_MCU);
	DRV_GPIO_ClearIntrupt_gen(BSP_FET_CHG_CTL_MCU);
	DRV_GPIO_ClearIntrupt_gen(BSP_CAN_WLATCH_CLR_MCU);
	DRV_GPIO_ClearIntrupt_gen(BSP_ALERT_MCU_COM);
	DRV_GPIO_ClearIntrupt_gen(BSP_PCHG_MCU);
    /* Implementation to be added */
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_GPIOE_ISRHandler_v
*   Description   : GPIO Port E interrupt service routine
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
static void DRV_GPIOE_ISRHandler_v(void)
{
	DRV_GPIO_ClearIntrupt_gen(BSP_DCDC_LOW_PWR);
    /* Implementation to be added */
}
