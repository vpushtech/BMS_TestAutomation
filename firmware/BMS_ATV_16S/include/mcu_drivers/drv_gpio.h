#ifndef DRIVERS_DRV_GPIO_H_
#define DRIVERS_DRV_GPIO_H_
/* ==================== HEADERS FILES ==================== */
#include <common.h>
#include "pins_driver.h"
#include "interrupt_manager.h"
#include "pin_mux.h"
#include "device_registers.h"
#include "drv_nvic.h"

/* ==================== MACROS ==================== */
#define DRV_GPIO_PIN_MASK(pin)            (1U << (pin))      /* Creates bitmask for specific pin */
#define DRV_GPIO_CONFIG_TABLE_SIZE        (sizeof(DRV_GpioConfigTable_gst)/sizeof(DRV_GPIOConfig_tst)) /* GPIO config table size */

/* ==================== TYPE DEFINITIONS ==================== */
typedef enum {
    DRV_GPIO_PIN_0,
    DRV_GPIO_PIN_1,
    DRV_GPIO_PIN_2,
    DRV_GPIO_PIN_3,
    DRV_GPIO_PIN_4,
    DRV_GPIO_PIN_5,
    DRV_GPIO_PIN_6,
    DRV_GPIO_PIN_7,
    DRV_GPIO_PIN_8,
    DRV_GPIO_PIN_9,
    DRV_GPIO_PIN_10,
    DRV_GPIO_PIN_11,
    DRV_GPIO_PIN_12,
    DRV_GPIO_PIN_13,
    DRV_GPIO_PIN_14,
    DRV_GPIO_PIN_15,
    DRV_GPIO_PIN_16,
    DRV_GPIO_PIN_17,
    DRV_GPIO_MAX_PIN,
} DRV_GpioPin_ten;

typedef enum {
    DRV_GPIO_PORTA,
    DRV_GPIO_PORTB,
    DRV_GPIO_PORTC,
    DRV_GPIO_PORTD,
    DRV_GPIO_PORTE,
    DRV_GPIO_PORT_MAX
} DRV_GpioPort_ten;

typedef enum
{
	DRV_GPIO_PIN_DISABLE_MODE,
	DRV_GPIO_PIN__MUX_GPIO_MODE,
	DRV_GPIO_PIN__MUX_ALT_02,
	DRV_GPIO_PIN__MUX_ALT_03,
	DRV_GPIO_PIN__MUX_ALT_04,
	DRV_GPIO_PIN__MUX_ALT_05,
	DRV_GPIO_PIN__MUX_ALT_06,
	DRV_GPIO_PIN__MUX_ALT_07,
	DRV_GPIO_PIN__MUX_ALT_08
}GPIO_PinMode_ten;
typedef enum {
    DRV_GPIO_LOW = 0,
    DRV_GPIO_HIGH = 1
} DRV_GpioState_ten;

typedef enum
{
    DRV_GPIO_STATUS_OK,
    DRV_GPIO_STATUS_ERR,
} DRV_GpioStatus_ten;


typedef enum
{
    DRV_GPIO_DIRECTION_IN,
    DRV_GPIO_DIRECTION_OUT,
} DRV_GpioDirection_ten;

typedef enum
{
	DRV_GPIO_INTRUPT_LOGIC_0=0x8U,
    DRV_GPIO_INTRUPT_RISSING_EDGE,
    DRV_GPIO_INTRUPT_FALLING_EDGE,
    DRV_GPIO_INTRUPT_BOTH_EDGE,
    DRV_GPIO_INTRUPT_LOGIC_1,
    DRV_GPIO_INTRUPT_LOGIC_MAX
} DRV_GpioIntOptions_ten;

typedef enum
{
    DRV_GPIO_INTERNAL_NO_PUPD,
    DRV_GPIO_INTERNAL_PULLDOWN,
    DRV_GPIO_INTERNAL_PULLUP,
} DRV_GpioPuPdConfig_ten;

typedef struct
{
    DRV_GpioPort_ten DRV_Port_en;
    DRV_GpioPin_ten DRV_Pin_en;
    DRV_GpioDirection_ten DRV_GpioDirection_en;
    DRV_GpioState_ten DRV_Defaultstate_en;
    DRV_GpioPuPdConfig_ten DRV_GpioPuPdConfig_en;
    BIN DRV_IntruptEnabled_b;
    DRV_GpioIntOptions_ten DRV_IntOptions_en;
    GPIO_PinMode_ten DRV_PinMode_en;
} DRV_GPIOConfig_tst;

/* ==================== GLOBAL VARIABLES ==================== */

/* ==================== INITIALIZATION ==================== */
DRV_GpioStatus_ten DRV_GPIO_Init_gen(void);

/* ==================== PIN OPERATIONS ==================== */
DRV_GpioStatus_ten DRV_GPIO_SetOutputPin_gen(U8 pinIdx_argu8, DRV_GpioState_ten state_argen);
DRV_GpioStatus_ten DRV_GPIO_ClearPin_gen(U8 pinIdx_argu8);
DRV_GpioStatus_ten DRV_GPIO_TogglePin_gen(U8 pinIdx_argu8);
DRV_GpioStatus_ten DRV_GPIO_ReadPin_gen(U8 pinIdx_argu8, U8* value_argu8);
DRV_GpioStatus_ten DRV_GPIO_PinConfig_gen(U8 pinIdx_argu8,GPIO_PinMode_ten GPIO_PinMode_argen);
/* ==================== INTERRUPT CONFIGURATION ==================== */
DRV_GpioStatus_ten DRV_GPIO_ClearIntrupt_gen(U8 pinIdx_argu8);

#endif /* DRIVERS_DRV_GPIO_H_ */
