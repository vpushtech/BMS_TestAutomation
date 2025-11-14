#ifndef BOARD_CONFIG_BSP_H_
#define BOARD_CONFIG_BSP_H_

/* ==================== INCLUDES ==================== */
#include "drv_gpio.h"
#include "clock.h"
#include "clockMan1.h"
#include "drv_timer.h"
#include "drv_i2c.h"
#include "drv_adc.h"
#include "drv_can.h"
#include "drv_wdg.h"

/* ==================== DEFINES & MACROS ==================== */

/* ==================== TYPEDEFS & ENUMS ==================== */
typedef enum {
    /* JTAG Debug Pins */
    BSP_JTAG_TMS,
    BSP_JTAG_RESET,
    BSP_JTAG_TCK,
    BSP_JTAG_TDI,
    BSP_JTAG_TDO,

    /* Control Pins */
    BSP_SCD_RESET_MCU,
    BSP_MCU_SHT_DWN,
    BSP_LED_MCU,
    BSP_SW_OPT_MCU,
    BSP_CAN_LOUT_MCU,
    BSP_DCDC_LOW_PWR,

    /* ADC Pins */
    BSP_CURR_ADC_MCU,
    BSP_NTC5,
    BSP_NTC6,
    BSP_LOAD_VLTG_ADC,
    BSP_NTC8,
    BSP_NTC7,

    /* UART Pins */
    BSP_UART1_RX,
	BSP_UART1_TX,

    /* FET Control Pins */
    BSP_MCU_DFET_CTRL,
    BSP_MCU_CFET_CTRL,
    BSP_FET_DSG_CTL_MCU,
    BSP_FET_CHG_CTL_MCU,
    BSP_PCHG_MCU,

    /* I2C Pins */
    BSP_AFE_SDA,
    BSP_AFE_SCL,

    /* AFE Communication Pins */
    BSP_AFE_WAKE_MCU,
    BSP_ALERT_MCU,

    /* Power Control */
    BSP_MCU_PWR_LATCH,

    /* SPI/SD Card Pins */
    BSP_SPI_CS,
    BSP_SPI_MOSI,
    BSP_SPI_CLK,
    BSP_SPI_MISO,

    /* CAN Communication Pins */
    BSP_CAN_RX_MCU,
    BSP_CAN_TX_MCU,

    /* CAN Wake/Control Pins */
   // BSP_CAN_PWAKE_MCU,
    BSP_CAN_WLATCH_CLR_MCU,
    BSP_ALERT_MCU_COM,

    /* Crystal Pins */
    BSP_XTAL_MINUS,
    BSP_XTAL_PLUS,

    BSP_MAX_PIN
} BSP_GpioPin_en;

typedef enum {
    BSP_LPTI_TIMER0_CH0,
    BSP_LPTI_TIMER0_CH1,
	BSP_LPTI_TIMER0_CH2,
    BSP_MAX_TIMER_PIN,
} BSP_TimerglobleTable_en;


typedef enum {
    BSP_BoardTemp,
	BSP_ADC_MAX_PIN
} BSP_adcChgroup_en;


typedef enum {
    BSP_BQ76952Afe,
    BSP_MAX_I2C_PIN
} BSP_i2c_PinConfig_en;
typedef enum {
    BSP_UART_BLE,
	BSP_MAX_UART_PIN
} BSP_uart_PinConfig_en;

/* ==================== EXTERN VARIABLES ==================== */
/* Configuration Tables */
extern DRV_GPIOConfig_tst DRV_GpioConfigTable_gst[BSP_MAX_PIN];
extern DRV_TimerConfig_tst DRV_TimerConfigTable_gst[BSP_MAX_TIMER_PIN];
extern DRV_I2CConfig_tst DRV_i2cConfigTable_gst[BSP_MAX_I2C_PIN];


/* CAN Frame Buffers */
/* ==================== FUNCTION DECLARATIONS ==================== */
/* Board Support Package Initialization */
extern void BSP_Init(void);
void BSP_DRV_Config_gv(void);
/* I2C Communication Functions */
/* Timer Utility Functions */
void BSP_TimerDelay(U32 delay_argu32, DRV_TimerDelayUnit_ten unit_argen);
/* ==================== I2C BUS RECOVERY FUNCTIONS ==================== */
DRV_i2cStatus_ten BSP_I2C_BusFaultRecover_gv(void);
#endif /* BOARD_CONFIG_BSP_H_ */
