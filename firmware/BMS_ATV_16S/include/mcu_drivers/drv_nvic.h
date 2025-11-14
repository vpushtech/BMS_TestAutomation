#ifndef DRIVERS_DRV_NVIC_H_
#define DRIVERS_DRV_NVIC_H_

/*******************************************************************************
 *  HEADER FILES
 *******************************************************************************/
#include <common.h>
#include "interrupt_manager.h"

/*******************************************************************************
 *  MACRO DEFINITIONS
 *******************************************************************************/

/*******************************************************************************
 *  TYPE DEFINITIONS
 *******************************************************************************/

/* ==================== NVIC PERIPHERAL INTERRUPT ENUMERATION ==================== */
typedef enum {
    /* GPIO Interrupts */
    NVIC_GPIOA_IRQ,
    NVIC_GPIOB_IRQ,
    NVIC_GPIOC_IRQ,
    NVIC_GPIOD_IRQ,
    NVIC_GPIOE_IRQ,

    /* CAN Interrupts */
    NVIC_CAN0_0_15_IRQ,
    NVIC_CAN0_16_31_IRQ,
    NVIC_CAN1_0_15_IRQ,
    NVIC_CAN2_0_15_IRQ,

    /* Timer Interrupts */
    NVIC_LPIT0_CH0_IRQ,
    NVIC_LPIT0_CH1_IRQ,
    NVIC_LPIT0_CH2_IRQ,
    NVIC_LPIT0_CH3_IRQ,

    /* ADC Interrupts */
    NVIC_ADC0_IRQ,
    NVIC_ADC1_IRQ,

    /* Flash Memory Interrupts */
    NVIC_FLASH_COMPLETE_IRQ,
    NVIC_FLASH_FAULT_IRQ,

    /* I2C Interrupts */
    NVIC_LPI2C0_MASTER_IRQ,
    NVIC_LPI2C0_SLAVE_IRQ,
	NVIC_FLEXIO_IRQ,
    /* WDG Interrupt */
    NVIC_WDG_IRQ,
	MAX_NVIC_IRQ
} DRV_NVIC_Irq_ten;

/* ==================== NVIC STATUS ENUMERATION ==================== */
typedef enum {
    DRV_NVIC_STATUS_OK,
    DRV_NVIC_STATUS_ERR
} DRV_NVIC_Status_ten;

/* ==================== NVIC INTERRUPT CONFIGURATION STRUCTURE ==================== */
typedef struct {
    IRQn_Type DRV_irqNumber_en;
} DRV_NVIC_IrqConfig_tst;

/*******************************************************************************
 *  FUNCTION DECLARATIONS
 *******************************************************************************/

/* ==================== INTERRUPT CONTROL FUNCTIONS ==================== */
DRV_NVIC_Status_ten DRV_NVIC_EnableIRQ_gen(DRV_NVIC_Irq_ten IRQIdx_argen);
DRV_NVIC_Status_ten DRV_NVIC_DisableIRQ_gen(DRV_NVIC_Irq_ten IRQIdx_argen);
DRV_NVIC_Status_ten DRV_NVIC_EnableAll_gen(void);
DRV_NVIC_Status_ten DRV_NVIC_DisableAll_gen(void);

/* ==================== INTERRUPT CONFIGURATION FUNCTION ==================== */
DRV_NVIC_Status_ten DRV_NVIC_IRQConfig_gen(DRV_NVIC_Irq_ten IRQIdx_argen, U8 priority_argu8);

#endif /* DRIVERS_DRV_NVIC_H_ */
