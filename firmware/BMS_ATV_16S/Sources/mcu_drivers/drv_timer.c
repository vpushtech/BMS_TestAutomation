/*
 * drv_timer.c
 *
 *  Description     : Timer Driver
 *  Author          : Rushikesh
 *  Created On      : 09-Jul-2025
 *  Version         : 2.0
 *  Modification History:
 *  Date        Author      Description
 *  ----------------------------------------------------------------------------
 *  08-Jul-2025 RUSHIKESH   Timer Driver Architecture Implementation
 *  25-Jul-2025 RUSHIKESH   Timer APIs are tested with Different Delay in ms/micro Sec and Callback function Testing Done
 *  11-Aug-2025 RUSHIKESH   Guidelines Followed the naming Architecture Implementation
 ******************************************************************************/
/* ==================== INCLUDE FILES ==================== */
#include <app_timer.h>
#include <bsp_config.h>
#include "drv_timer.h"
/* ==================== STATIC VARIABLES ==================== */
static U16 DRV_dutyCycle = 0xFFFU;
static BIN DRV_increaseDutyCycle = false;

/* ==================== GLOBAL VARIABLES ==================== */
extern DRV_TimerConfig_tst DRV_TimerConfigTable_gst[BSP_MAX_TIMER_PIN];
extern BIN canTransmitEnable;
ftm_state_t DRV_ftmStateStruct[BSP_MAX_TIMER_PIN];

/* ==================== CONFIGURATION STRUCTURES ==================== */
const lpit_user_channel_config_t* DRV_timerChannelConfig[BSP_MAX_TIMER_PIN] = {
    &lpit1_ChnConfig0,
    &lpit1_ChnConfig1,
};

const lpit_user_config_t* DRV_timerConfig[BSP_MAX_TIMER_PIN] = {
    &lpit1_InitConfig,
    &lpit1_InitConfig,
};

const ftm_user_config_t *DRV_ftmConfig[BSP_MAX_TIMER_PIN] = {
    &flexTimer_pwm1_InitConfig
};

const U8 DRV_PWM_Instance[BSP_MAX_TIMER_PIN] = {INST_FLEXTIMER_PWM1};
ftm_pwm_param_t* DRV_ftmPWMConfig[BSP_MAX_TIMER_PIN] = {
    &flexTimer_pwm1_PwmConfig
};

/* ==================== INTERRUPT HANDLERS ==================== */
static void DRV_Timer_Ch0_IRQHandler(void);
static void DRV_Timer_Ch1_IRQHandler(void);
static void DRV_Timer_Ch2_IRQHandler(void);
static void DRV_Timer_Ch3_IRQHandler(void);

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_Timer_Init
*   Description   : Initializes timer module and channel
*   Parameters    : timerPinIdx - Timer pin index
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void DRV_Timer_Init_gv(U8 timerPinIdx)
{
    LPIT_DRV_Init(DRV_TimerConfigTable_gst[timerPinIdx].DRV_timerInstance_en,
                 DRV_timerConfig[DRV_TimerConfigTable_gst[timerPinIdx].DRV_timerInstance_en]);

    LPIT_DRV_InitChannel(DRV_TimerConfigTable_gst[timerPinIdx].DRV_timerInstance_en,
                        DRV_TimerConfigTable_gst[timerPinIdx].DRV_timerChannel_en,
                        DRV_timerChannelConfig[DRV_TimerConfigTable_gst[timerPinIdx].DRV_timerInstance_en]);

    /* IRQ Handling */
    INT_SYS_InstallHandler(LPIT0_Ch0_IRQn, DRV_Timer_Ch0_IRQHandler, (void *)NULL);
    INT_SYS_InstallHandler(LPIT0_Ch1_IRQn, DRV_Timer_Ch1_IRQHandler, (void *)NULL);
    INT_SYS_InstallHandler(LPIT0_Ch2_IRQn, DRV_Timer_Ch2_IRQHandler, (void *)NULL);
    INT_SYS_InstallHandler(LPIT0_Ch3_IRQn, DRV_Timer_Ch3_IRQHandler, (void *)NULL);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_Timer_PWM_Init
*   Description   : Initializes timer PWM functionality
*   Parameters    : timerPinIdx - Timer pin index
*   Return Value  : status_t - Operation status_en
*  --------------------------------------------------------------------------- */
DRV_TimerStatus_ten DRV_Timer_PWM_Init_gst(U8 timerPinIdx)
{
    status_t status_en = FTM_DRV_Init(DRV_PWM_Instance[timerPinIdx],
                                 DRV_ftmConfig[timerPinIdx],
                                 &DRV_ftmStateStruct[timerPinIdx]);
    if(status_en != STATUS_SUCCESS)
    {
        return DRV_TIMER_STATUS_ERR;
    }

    status_en = FTM_DRV_InitPwm(DRV_PWM_Instance[timerPinIdx],
                            DRV_ftmPWMConfig[timerPinIdx]);
    if(status_en != STATUS_SUCCESS)
    {
        return DRV_TIMER_STATUS_ERR;
    }

    return DRV_TIMER_STATUS_OK;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_Timer_UpdateDutyCycle
*   Description   : Updates PWM duty cycle
*   Parameters    : timerPinIdx - Timer pin index
*                   dutyCycle - New duty cycle value (0-100)
*   Return Value  : status_t - Operation status_en
*  --------------------------------------------------------------------------- */
DRV_TimerStatus_ten DRV_Timer_UpdateDutyCycle_gst(U8 timerPinIdx, U8 dutyCycle)
{
    U16 pwmValue = (dutyCycle * 0xFFFU) / 100U;

    status_t status_en = FTM_DRV_UpdatePwmChannel(DRV_PWM_Instance[timerPinIdx],
                                             0U,
                                             FTM_PWM_UPDATE_IN_DUTY_CYCLE,
                                             pwmValue,
                                             0U,
                                             true);
    if(status_en != STATUS_SUCCESS)
    {
        return DRV_TIMER_STATUS_ERR;
    }
    return DRV_TIMER_STATUS_OK;
}

/* ==================== UTILITY FUNCTIONS ==================== */
/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_Timer_DelayCycles_gst
*   Description   : Simple delay using NOP instructions
*   Parameters    : cycles - Number of cycles to delay
*   Return Value  : void
*  --------------------------------------------------------------------------- */
static void DRV_Timer_DelayCycles_gst(volatile U32 cycles)
{
    while (cycles--)
    {
        __asm("nop");
    }
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_Timer_UpdateLedStatus
*   Description   : Updates LED status_en with PWM breathing effect
*   Parameters    : timerPinIdx - Timer pin index
*   Return Value  : status_t - Operation status_en
*  --------------------------------------------------------------------------- */
DRV_TimerStatus_ten DRV_Timer_UpdateLedStatus_gst(U8 timerPinIdx)
{
    if (DRV_increaseDutyCycle == false)
    {
        DRV_dutyCycle--;
        if (DRV_dutyCycle < 1U)
            DRV_increaseDutyCycle = true;
    }
    else
    {
        DRV_dutyCycle++;
        if (DRV_dutyCycle > 0xFFFU)
            DRV_increaseDutyCycle = false;
    }

    status_t status_en = FTM_DRV_UpdatePwmChannel(DRV_PWM_Instance[timerPinIdx],
                                              0U,
                                              FTM_PWM_UPDATE_IN_DUTY_CYCLE,
                                              DRV_dutyCycle,
                                              0U,
                                              true);
    if(status_en != STATUS_SUCCESS)
    {
        return DRV_TIMER_STATUS_ERR;
    }

    DRV_Timer_DelayCycles_gst(0x5FFU);
    return DRV_TIMER_STATUS_OK;
}

/* ==================== TIMER CONTROL FUNCTIONS ==================== */
/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_Timer_Start_gv
*   Description   : Starts timer channel
*   Parameters    : timerPinIdx - Timer pin index
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void DRV_Timer_Start_gv(U8 timerPinIdx)
{
    LPIT_DRV_StartTimerChannels(DRV_TimerConfigTable_gst[timerPinIdx].DRV_timerInstance_en,
                               (uint32_t)DRV_TIMER_CHANNEL_MASK(DRV_TimerConfigTable_gst[timerPinIdx].DRV_timerChannel_en));
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_Timer_Stop_gv
*   Description   : Stops timer channel
*   Parameters    : timerPinIdx - Timer pin index
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void DRV_Timer_Stop_gv(U8 timerPinIdx)
{
    LPIT_DRV_StopTimerChannels(DRV_TimerConfigTable_gst[timerPinIdx].DRV_timerInstance_en,
                              (uint32_t)DRV_TIMER_CHANNEL_MASK(DRV_TimerConfigTable_gst[timerPinIdx].DRV_timerChannel_en));
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_Timer_Delay
*   Description   : Creates precise delay using timer
*   Parameters    : timerPinIdx - Timer pin index
*                   delayUnit - Delay units (ms or us)
*                   delayValue - Delay duration
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void DRV_Timer_Delay_gv(U8 timerPinIdx, DRV_TimerDelayUnit_ten delayUnit, U32 delayValue)
{
    lpit_user_channel_config_t channelConfig;
    LPIT_DRV_GetDefaultChanConfig(&channelConfig);

    channelConfig.isInterruptEnabled = false;
    channelConfig.timerMode = LPIT_PERIODIC_COUNTER;
    channelConfig.periodUnits = LPIT_PERIOD_UNITS_MICROSECONDS;

    if(DRV_DELAY_UNITS_MILLISECOND == delayUnit)
    {
        channelConfig.period = delayValue * 1000U;
    }
    else
    {
        channelConfig.period = delayValue;
    }

    LPIT_DRV_InitChannel(DRV_TimerConfigTable_gst[timerPinIdx].DRV_timerInstance_en,
                        DRV_TimerConfigTable_gst[timerPinIdx].DRV_timerChannel_en,
                        &channelConfig);

    DRV_Timer_Start_gv(timerPinIdx);

    while (!(LPIT_DRV_GetInterruptFlagTimerChannels(DRV_TimerConfigTable_gst[timerPinIdx].DRV_timerInstance_en,
                                                  DRV_TIMER_CHANNEL_MASK(DRV_TimerConfigTable_gst[timerPinIdx].DRV_timerChannel_en))))
    {
        /* Busy Wait */
    }

    LPIT_DRV_ClearInterruptFlagTimerChannels(DRV_TimerConfigTable_gst[timerPinIdx].DRV_timerInstance_en,
                                           DRV_TIMER_CHANNEL_MASK(DRV_TimerConfigTable_gst[timerPinIdx].DRV_timerChannel_en));
    DRV_Timer_Stop_gv(timerPinIdx);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_Timer_DeInit
*   Description   : Deinitializes timer module
*   Parameters    : timerPinIdx - Timer pin index
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void DRV_Timer_DeInit_gv(U8 timerPinIdx)
{
    LPIT_DRV_Deinit(DRV_TimerConfigTable_gst[timerPinIdx].DRV_timerInstance_en);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_Timer_InterruptConfig
*   Description   : Configures timer interrupt
*   Parameters    : timerPinIdx - Timer pin index
*                   irqIndex - Interrupt index
*                   priority - Interrupt priority
*                   interruptDelayMs - Interrupt delay in ms
*   Return Value  : status_t - Operation status_en
*  --------------------------------------------------------------------------- */
DRV_TimerStatus_ten DRV_Timer_InterruptConfig_gst(U8 timerPinIdx, U8 irqIndex, U8 priority, U32 interruptDelayMs, DRV_TimerDelayUnit_ten delayUnit_argen)
{
    lpit_user_channel_config_t channelConfig;
    LPIT_DRV_GetDefaultChanConfig(&channelConfig);

    channelConfig.isInterruptEnabled = true;
    channelConfig.timerMode = LPIT_PERIODIC_COUNTER;
    channelConfig.periodUnits = LPIT_PERIOD_UNITS_MICROSECONDS;
    if(DRV_DELAY_UNITS_MILLISECOND == delayUnit_argen)
    {
        channelConfig.period = interruptDelayMs * 1000U;
    }
    else
    {
        channelConfig.period = interruptDelayMs;
    }
    DRV_NVIC_Status_ten status_en =DRV_NVIC_IRQConfig_gen(irqIndex, priority);
    if (status_en != DRV_NVIC_STATUS_OK)
    {
        return DRV_TIMER_STATUS_ERR;
    }

    LPIT_DRV_InitChannel(DRV_TimerConfigTable_gst[timerPinIdx].DRV_timerInstance_en,
                        DRV_TimerConfigTable_gst[timerPinIdx].DRV_timerChannel_en,
                        &channelConfig);

    return DRV_TIMER_STATUS_OK;
}

/* ==================== INTERRUPT SERVICE ROUTINES ==================== */
/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_Timer_Ch0_IRQHandler
*   Description   : Timer channel 0 interrupt handler
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
static void DRV_Timer_Ch0_IRQHandler(void)
{
    if (LPIT_DRV_GetInterruptFlagTimerChannels(DRV_TIMER0, DRV_TIMER_CHANNEL_MASK(DRV_CHANNEL_0)))
    {
        LPIT_DRV_ClearInterruptFlagTimerChannels(DRV_TIMER0, DRV_TIMER_CHANNEL_MASK(DRV_CHANNEL_0));
    }
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_Timer_Ch1_IRQHandler
*   Description   : Timer channel 1 interrupt handler (handles CAN transmit and WDG refresh)
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
static void DRV_Timer_Ch1_IRQHandler(void)
{
    if(LPIT_DRV_GetInterruptFlagTimerChannels(DRV_TIMER0, DRV_TIMER_CHANNEL_MASK(DRV_CHANNEL_1)))
    {
        APP_Timer0Ch1_Callback();
        LPIT_DRV_ClearInterruptFlagTimerChannels(DRV_TIMER0, DRV_TIMER_CHANNEL_MASK(DRV_CHANNEL_1));
    }
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_Timer_Ch2_IRQHandler
*   Description   : Timer channel 2 interrupt handler
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
static void DRV_Timer_Ch2_IRQHandler(void)
{
    if(LPIT_DRV_GetInterruptFlagTimerChannels(DRV_TIMER0, DRV_TIMER_CHANNEL_MASK(DRV_CHANNEL_2)))
    {
        APP_Timer0Ch2_Callback();
        LPIT_DRV_ClearInterruptFlagTimerChannels(DRV_TIMER0, DRV_TIMER_CHANNEL_MASK(DRV_CHANNEL_2));
    }
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_Timer_Ch3_IRQHandler
*   Description   : Timer channel 3 interrupt handler
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
static void DRV_Timer_Ch3_IRQHandler(void)
{
    if(LPIT_DRV_GetInterruptFlagTimerChannels(DRV_TIMER0, DRV_TIMER_CHANNEL_MASK(DRV_CHANNEL_3)))
    {
        LPIT_DRV_ClearInterruptFlagTimerChannels(DRV_TIMER0, DRV_TIMER_CHANNEL_MASK(DRV_CHANNEL_3));
    }
}
