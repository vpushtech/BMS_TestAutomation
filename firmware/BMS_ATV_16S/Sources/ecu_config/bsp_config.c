/*
 * [File Name].c
 *
 *  Created on: [Date]
 *  Author: RushikeshNitinKamble
 *  Description: Board Support Package (BSP) and Hardware Abstraction Layer (HAL) Implementation
 *  Version: 1.0
 *  Modification History:
 *  Date            Author          Description
 *  ----------------------------------------------------------------------------
 *  [Date]          Rushikesh       Initial BSP and HAL Implementation
 *  [Date]          Rushikesh       Added Peripheral Configuration Tables
 *  [Date]          Rushikesh       Integrated RTOS and CAN Communication
 ******************************************************************************/

#include <afe_config.h>
#include <bms_protection.h>
#include <bsp_config.h>
#include <can_comm.h>
#include <vpush_rtos.h>
#include"drv_flash.h"
/* ==================== GLOBAL VARIABLES ==================== */

/* =================== HARDWARE CONFIGURATION TABLES ==================== */

/* ==================== GPIO CONFIGURATION TABLE ==================== */
DRV_GPIOConfig_tst DRV_GpioConfigTable_gst[BSP_MAX_PIN] = {

    /* JTAG Debug Pins */
    {DRV_GPIO_PORTA, DRV_GPIO_PIN_4, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_ALT_07},      /* BSP_JTAG_TMS */
    {DRV_GPIO_PORTA, DRV_GPIO_PIN_5, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_ALT_07},      /* BSP_JTAG_RESET */
    {DRV_GPIO_PORTC, DRV_GPIO_PIN_4, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_ALT_07},      /* BSP_JTAG_TCK */
    {DRV_GPIO_PORTC, DRV_GPIO_PIN_5, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_ALT_07},      /* BSP_JTAG_TDI */
    {DRV_GPIO_PORTA, DRV_GPIO_PIN_10, DRV_GPIO_DIRECTION_OUT, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_ALT_07},    /* BSP_JTAG_TDO */

    /* Control Pins */
    {DRV_GPIO_PORTE, DRV_GPIO_PIN_0, DRV_GPIO_DIRECTION_OUT, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_NO_PUPD, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_GPIO_MODE},    /* BSP_SCD_RESET_MCU */
    {DRV_GPIO_PORTE, DRV_GPIO_PIN_1, DRV_GPIO_DIRECTION_OUT, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_NO_PUPD, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_GPIO_MODE},    /* BSP_MCU_SHT_DWN */
    {DRV_GPIO_PORTA, DRV_GPIO_PIN_11, DRV_GPIO_DIRECTION_OUT, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_NO_PUPD, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_GPIO_MODE},   /* BSP_LED_MCU */
    {DRV_GPIO_PORTA, DRV_GPIO_PIN_12, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, true, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_GPIO_MODE},      /* BSP_SW_OPT_MCU */
    {DRV_GPIO_PORTA, DRV_GPIO_PIN_13, DRV_GPIO_DIRECTION_OUT, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_NO_PUPD, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_GPIO_MODE},   /* BSP_CAN_LOUT_MCU */
    {DRV_GPIO_PORTE, DRV_GPIO_PIN_2, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, true, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_GPIO_MODE},       /* BSP_DCDC_LOW_PWR */

    /* ADC Pins - Using DISABLE MODE for analog pins */
    {DRV_GPIO_PORTE, DRV_GPIO_PIN_6, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_NO_PUPD, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN_DISABLE_MODE},    /* BSP_CURR_ADC_MCU */
    {DRV_GPIO_PORTD, DRV_GPIO_PIN_2, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_NO_PUPD, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN_DISABLE_MODE},    /* BSP_NTC5 */
    {DRV_GPIO_PORTD, DRV_GPIO_PIN_3, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_NO_PUPD, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN_DISABLE_MODE},    /* BSP_NTC6 */
    {DRV_GPIO_PORTD, DRV_GPIO_PIN_4, DRV_GPIO_DIRECTION_OUT, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_NO_PUPD, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_GPIO_MODE},   /* BSP_LOAD_VLTG_ADC */
    {DRV_GPIO_PORTA, DRV_GPIO_PIN_6, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_NO_PUPD, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN_DISABLE_MODE},    /* BSP_NTC8 */
    {DRV_GPIO_PORTA, DRV_GPIO_PIN_7, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_NO_PUPD, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN_DISABLE_MODE},    /* BSP_NTC7 */

    /* UART Pins  */
    {DRV_GPIO_PORTC, DRV_GPIO_PIN_6, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_ALT_03},      /* BSP_UART1_RX */
    {DRV_GPIO_PORTC, DRV_GPIO_PIN_7, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_ALT_03},      /* BSP_UART1_TX */

    /* FET Control Pins */
    {DRV_GPIO_PORTA, DRV_GPIO_PIN_0, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, true, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_GPIO_MODE},       /* BSP_MCU_DFET_CTRL */
    {DRV_GPIO_PORTA, DRV_GPIO_PIN_1, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, true, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_GPIO_MODE},       /* BSP_MCU_CFET_CTRL */
    {DRV_GPIO_PORTC, DRV_GPIO_PIN_17, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, true, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_GPIO_MODE},      /* BSP_FET_DSG_CTL_MCU */
    {DRV_GPIO_PORTC, DRV_GPIO_PIN_0, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, true, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_GPIO_MODE},       /* BSP_FET_CHG_CTL_MCU */
    {DRV_GPIO_PORTD, DRV_GPIO_PIN_7, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, true, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_GPIO_MODE},       /* BSP_PCHG_MCU */

    /* I2C Pins - Corrected to ALT_07 */
   /* {DRV_GPIO_PORTA, DRV_GPIO_PIN_2, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_ALT_07},       BSP_AFE_SDA
    {DRV_GPIO_PORTA, DRV_GPIO_PIN_3, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_ALT_07},       BSP_AFE_SCL */
	  //{DRV_GPIO_PORTE, DRV_GPIO_PIN_10, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_ALT_06},    /* BSP_AFE_SDA */
	 // {DRV_GPIO_PORTE, DRV_GPIO_PIN_11, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_ALT_06},    /* BSP_AFE_SCL */
    /* AFE Communication Pins */
    {DRV_GPIO_PORTB, DRV_GPIO_PIN_12, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, true, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_GPIO_MODE},      /* BSP_AFE_WAKE_MCU */
    {DRV_GPIO_PORTB, DRV_GPIO_PIN_13, DRV_GPIO_DIRECTION_OUT, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_NO_PUPD, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_GPIO_MODE},   /* BSP_ALERT_MCU */

    /* Power Control */
    {DRV_GPIO_PORTC, DRV_GPIO_PIN_9, DRV_GPIO_DIRECTION_OUT, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_NO_PUPD, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_GPIO_MODE},    /* BSP_MCU_PWR_LATCH */

    /* SPI/SD Card Pins - Corrected to ALT_03 */
    {DRV_GPIO_PORTB, DRV_GPIO_PIN_0, DRV_GPIO_DIRECTION_OUT, DRV_GPIO_HIGH, DRV_GPIO_INTERNAL_PULLUP, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_ALT_03},    /* BSP_SPI_CS */
    {DRV_GPIO_PORTB, DRV_GPIO_PIN_1, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_ALT_03},      /* BSP_SPI_MOSI */
    {DRV_GPIO_PORTB, DRV_GPIO_PIN_2, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_ALT_03},      /* BSP_SPI_CLK */
    {DRV_GPIO_PORTB, DRV_GPIO_PIN_3, DRV_GPIO_DIRECTION_OUT, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_ALT_03},     /* BSP_SPI_MISO */

    /* CAN Communication Pins - Corrected to ALT_03 */
    {DRV_GPIO_PORTE, DRV_GPIO_PIN_4, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_ALT_05},      /* BSP_CAN_RX_MCU */
    {DRV_GPIO_PORTE, DRV_GPIO_PIN_5, DRV_GPIO_DIRECTION_OUT, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_ALT_05},     /* BSP_CAN_TX_MCU */

    /* CAN Wake/Control Pins */
  //  {DRV_GPIO_PORTE, DRV_GPIO_PIN_11, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, true, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_GPIO_MODE},      /* BSP_CAN_PWAKE_MCU */
    {DRV_GPIO_PORTD, DRV_GPIO_PIN_0, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, true, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_GPIO_MODE},       /* BSP_CAN_WLATCH_CLR_MCU */
    {DRV_GPIO_PORTD, DRV_GPIO_PIN_1, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_PULLUP, true, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_GPIO_MODE},       /* BSP_ALERT_MCU_COM */

    /* Crystal Pins - Using ALT_01 as specified */
    {DRV_GPIO_PORTB, DRV_GPIO_PIN_6, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_NO_PUPD, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_GPIO_MODE},     /* BSP_XTAL_MINUS */
    {DRV_GPIO_PORTB, DRV_GPIO_PIN_7, DRV_GPIO_DIRECTION_IN, DRV_GPIO_LOW, DRV_GPIO_INTERNAL_NO_PUPD, false, DRV_GPIO_INTRUPT_BOTH_EDGE, DRV_GPIO_PIN__MUX_GPIO_MODE},     /* BSP_XTAL_PLUS */
};
/* ==================== TIMER CONFIGURATION TABLE ==================== */
DRV_TimerConfig_tst DRV_TimerConfigTable_gst[BSP_MAX_TIMER_PIN] = {
    {DRV_TIMER0, DRV_CHANNEL_0},
    {DRV_TIMER0, DRV_CHANNEL_1},
	{DRV_TIMER0, DRV_CHANNEL_2}
};

/* ==================== I2C CONFIGURATION TABLE ==================== */
DRV_I2CConfig_tst DRV_i2cConfigTable_gst[BSP_MAX_I2C_PIN]={
		{DRV_I2C_INSTANCE_0,0x08 ,false}
};

/* ==================== ADC CONFIGURATION TABLE ==================== */
DRV_AdcConfig_tst DRV_AdcConfigTable_gst[BSP_ADC_MAX_PIN]= {
    {DRV_ADC_INSTANCE_0, DRV_ADC_GROUP_0, 4}
};

/* ==================== PRIVATE FUNCTION PROTOTYPES ==================== */
static void BSP_HardwareInit(void);

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : HardwareInit
*   Description   : Initializes all hardware peripherals and drivers
*                   Configures GPIO, NVIC, Timers, ADC, and CAN
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
static void BSP_HardwareInit(void)
{
    /* Clock Initialization */
    CLOCK_DRV_Init(&clockMan1_InitConfig0);

    /* GPIO Initialization */
    DRV_GPIO_Init_gen();

    /* TIMER Initialization */
    DRV_Timer_Init_gv(BSP_LPTI_TIMER0_CH0);
    DRV_Timer_Init_gv(BSP_LPTI_TIMER0_CH1);
    DRV_Timer_Init_gv(BSP_LPTI_TIMER0_CH2);

    /* CAN Initialization */
    DRV_CAN_Init_gen(DRV_CAN_INSTANCE_1);
    /* NVIC I2C Configuration */
    DRV_NVIC_IRQConfig_gen(NVIC_FLEXIO_IRQ, 2);
    DRV_FLASH_Init_gen();
    /* WDT Initialization */
  // DRV_WDG_Init_gen(DRV_WDG_INSTANCE_1);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : bsp_init
*   Description   : Main board support package initialization function
*                   Initializes system clock, hardware, and RTOS
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void BSP_Init(void)
{
    BSP_HardwareInit();
    VPUSH_RTOS_Init_gv();
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : BPS_TimerDelay
*   Description   : Provides precise timer-based delay functionality
*   Parameters    : delay_argu32 - Delay value
*                   unit_argen - Time unit for delay
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void BSP_TimerDelay(U32 delay_argu32,  DRV_TimerDelayUnit_ten unit_argen)
{
	DRV_Timer_Delay_gv(BSP_LPTI_TIMER0_CH0,delay_argu32,unit_argen);
}
/* ==================== I2C BUS RECOVER ==================== */
/* ==================== FUNCTION DESCRIPTION ==================== */
/* -----------------------------------------------------------------------------
*  Function Name : BSP_I2C_BusFaultRecover_gv
*  Description   : Recovers I2C bus from fault condition by toggling SCL and generating STOP condition
*  Parameters    : None
*  Return Value  : DRV_GpioStatus_En - Operation status
*  --------------------------------------------------------------------------- */
DRV_i2cStatus_ten BSP_I2C_BusFaultRecover_gv(void)
{
	DRV_i2cStatus_ten status = DRV_I2C_SUCCESS;

    DRV_I2C_DeInit_gen(BSP_BQ76952Afe);

    // Switch SCL to GPIO mode
    status = DRV_GPIO_PinConfig_gen(BSP_AFE_SCL,DRV_GPIO_PIN__MUX_GPIO_MODE);
    if (status != DRV_I2C_SUCCESS)
        return status;

    // Set SCL to HIGH
    status = DRV_GPIO_SetOutputPin_gen( BSP_AFE_SCL, DRV_GPIO_HIGH);
    if (status != DRV_I2C_SUCCESS)
        return status;

    // Switch SDA to GPIO mode
    status = DRV_GPIO_PinConfig_gen(BSP_AFE_SDA,DRV_GPIO_PIN__MUX_GPIO_MODE);
    if (status != DRV_I2C_SUCCESS)
        return status;

    // Set SDA to HIGH
    status = DRV_GPIO_SetOutputPin_gen( BSP_AFE_SDA, DRV_GPIO_HIGH);
    if (status != DRV_I2C_SUCCESS)
        return status;

    // Toggle SCL 15 times
    for (uint8_t i = 0; i < 15; i++)
    {
   	 DRV_GPIO_TogglePin_gen(BSP_AFE_SCL);
        BSP_TimerDelay(10, DRV_DELAY_UNITS_MILLISECOND);
    }

    // Generate STOP condition (SDA going from LOW to HIGH when SCL is HIGH)
    status = DRV_GPIO_SetOutputPin_gen( BSP_AFE_SCL, DRV_GPIO_LOW);
    if (status != DRV_I2C_SUCCESS)
        return status;

    BSP_TimerDelay(10, DRV_DELAY_UNITS_MILLISECOND);

    status = DRV_GPIO_SetOutputPin_gen(BSP_AFE_SDA, DRV_GPIO_LOW);
    if (status != DRV_I2C_SUCCESS)
        return status;

    BSP_TimerDelay(10, DRV_DELAY_UNITS_MILLISECOND);

    status = DRV_GPIO_SetOutputPin_gen( BSP_AFE_SCL, DRV_GPIO_HIGH);
    if (status != DRV_I2C_SUCCESS)
        return status;

    BSP_TimerDelay(10, DRV_DELAY_UNITS_MILLISECOND);

    status = DRV_GPIO_SetOutputPin_gen(BSP_AFE_SDA, DRV_GPIO_HIGH);
    if (status != DRV_I2C_SUCCESS)
        return status;

    BSP_TimerDelay(10, DRV_DELAY_UNITS_MILLISECOND);

    // Switch back to I2C peripheral mode
    status = DRV_GPIO_PinConfig_gen(BSP_AFE_SCL,DRV_GPIO_PIN__MUX_ALT_06);
    if (status != DRV_I2C_SUCCESS)
        return status;

    status = DRV_GPIO_PinConfig_gen(BSP_AFE_SDA,DRV_GPIO_PIN__MUX_ALT_06);
    if (status != DRV_I2C_SUCCESS)
        return status;

    DRV_I2C_Init_gen(BSP_BQ76952Afe);

    return DRV_I2C_SUCCESS;
}
/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : BSP_DRV_Config_gv
*   Description   : Configures BMS hardware drivers and peripherals
*                   Sets up timer interrupts, CAN communication, and NVIC
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
 void BSP_DRV_Config_gv(void)
{
	/* NVIC Timer Configuration */
    DRV_Timer_InterruptConfig_gst(BSP_LPTI_TIMER0_CH1, NVIC_LPIT0_CH1_IRQ, 2, 1,DRV_DELAY_UNITS_MILLISECOND);
    DRV_Timer_InterruptConfig_gst(BSP_LPTI_TIMER0_CH2, NVIC_LPIT0_CH2_IRQ, 2,1, DRV_DELAY_UNITS_MILLISECOND);


    /* NVIC CAN Configuration */
    DRV_NVIC_IRQConfig_gen(NVIC_CAN0_0_15_IRQ, 4);



    /* NVIC GPIO Configuration */
    DRV_NVIC_IRQConfig_gen(NVIC_GPIOC_IRQ, 2);

    /* NVIC WDT Configuration */
//   DRV_NVIC_IRQConfig_gen(NVIC_WDG_IRQ, 3);

    /* Timer Start */
    DRV_Timer_Start_gv(BSP_LPTI_TIMER0_CH1);
    DRV_Timer_Start_gv(BSP_LPTI_TIMER0_CH2);
    /* CAN Buffer Configuration */
    DRV_NVIC_IRQConfig_gen(NVIC_FLEXIO_IRQ, 1);
    DRV_CAN_ConfigRxBuffer_gen(DRV_CAN_INSTANCE_1, CAN_BUFFER_IDX_8,
                              &CAN_QueueDataFrame_St[CAN_ID_0x1B0].CAN_DataFrame_st, 0x1B0);
    DRV_CAN_ReceiveNonBlock_gen(DRV_CAN_INSTANCE_1, CAN_BUFFER_IDX_8,&CAN_QueueDataFrame_St[CAN_ID_0x1B0].CAN_DataFrame_st);
    DRV_CAN_ConfigRxBuffer_gen(DRV_CAN_INSTANCE_1, CAN_BUFFER_IDX_9,
                                &CAN_QueueDataFrame_St[CAN_ID_0x1C0].CAN_DataFrame_st, 0x1C0);
      DRV_CAN_ReceiveNonBlock_gen(DRV_CAN_INSTANCE_1, CAN_BUFFER_IDX_9,&CAN_QueueDataFrame_St[CAN_ID_0x1C0].CAN_DataFrame_st);
}
