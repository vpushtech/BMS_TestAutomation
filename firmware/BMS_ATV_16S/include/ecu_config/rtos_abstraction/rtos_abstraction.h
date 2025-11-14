/*
 * RTOS_Abstraction.h
 *
 *  Created on: 14-Oct-2025
 *  Author: RushikeshNitinKamble
 *  Description: Core RTOS Abstraction APIs
 */

#ifndef RTOS_ABSTRACTION_H_
#define RTOS_ABSTRACTION_H_

/* ==================== INCLUDES ==================== */
#include <common.h>
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "semphr.h"
#include "timers.h"

/* ==================== TYPE DEFINITIONS ==================== */
typedef enum {
    RTOS_STATUS_SUCCESS,
    RTOS_STATUS_ERROR,
    RTOS_STATUS_INVALID_PARAM,
    RTOS_STATUS_TIMEOUT
} RTOS_Status_ten;
typedef struct {
    U32 taskExecutionTime;
    U32 taskPeriod;
    U32 taskCount;
} RTOS_TaskTiming_tst;

typedef struct {
    RTOS_TaskTiming_tst RTOS_DataProcessing;
    RTOS_TaskTiming_tst RTOS_BMS_StateProcessing;
    RTOS_TaskTiming_tst RTOS_SoCEstimation;
    RTOS_TaskTiming_tst RTOS_WDT;
} APP_TaskTiming_tst;
typedef struct
{
    U32 RTOS_BMS_StateProccesing_PeakStack;
    U32 RTOS_DataProccessing_PeakStack;
    U32 RTOS_SoCEstimation_PeakStack;
    U32 RTOS_WdG_PeakStack;
} RTOS_PeakStack_tst;
typedef void (*RTOS_TaskFunction_t)(void *pvParameters);
typedef void (*RTOS_TimerCallback_t)(TimerHandle_t xTimer);
/* ==================== TASK MANAGEMENT APIs ==================== */
RTOS_Status_ten RTOS_TaskCreateStatic_st(U8 task_Idx_argu8);
RTOS_Status_ten RTOS_TaskDelete_st(U8 task_argu8);

RTOS_Status_ten RTOS_TaskSuspend_st(U8 task_argu8);
RTOS_Status_ten RTOS_TaskResume_st(U8 task_argu8);
U32 RTOS_TaskGetStackHighWaterMark_st(U8 task_Idx_argu8);
/* ==================== QUEUE MANAGEMENT APIs ==================== */
RTOS_Status_ten RTOS_QueueCreateStatic_st(U8 QueueIdx_argu8);
RTOS_Status_ten RTOS_QueueSend_st(U8 QueueIdx_argu8, const void* item_argpv, U32 timeoutMs_argu32);
RTOS_Status_ten RTOS_QueueSendFromISR_st(U8 QueueIdx_argu8, const void* item_argpv);
RTOS_Status_ten RTOS_QueueReceive_st(U8 QueueIdx_argu8, void* buffer_argpv, U32 timeoutMs_argu32);
RTOS_Status_ten RTOS_QueueReceiveFromISR_st(U8 QueueIdx_argu8, void* buffer_argpv);
RTOS_Status_ten RTOS_QueueDelete_st(U8 QueueIdx_argu8);

/* ==================== SEMAPHORE MANAGEMENT APIs ==================== */
RTOS_Status_ten RTOS_BinarySemaphoreCreateStatic_st(U8 SemaphoreIdx_argu8);
RTOS_Status_ten RTOS_MutexCreateStatic_st(U8 SemaphoreIdx_argu8);
RTOS_Status_ten RTOS_SemaphoreTake_st(U8 SemaphoreIdx_argu8, U32 timeoutMs_argu32);
RTOS_Status_ten RTOS_SemaphoreGive_st(U8 SemaphoreIdx_argu8);
RTOS_Status_ten RTOS_SemaphoreGiveFromISR_st(U8 SemaphoreIdx_argu8);

/* ==================== TIMER MANAGEMENT APIs ==================== */
RTOS_Status_ten RTOS_TimerCreateStatic_st(U8 TimerIdx_argu8);
RTOS_Status_ten RTOS_TimerStart_st(U8 TimerIdx_argu8, U32 timeoutMs_argu32);
RTOS_Status_ten RTOS_TimerStartFromISR_st(U8 TimerIdx_argu8);
RTOS_Status_ten RTOS_TimerStop_st(U8 TimerIdx_argu8, U32 timeoutMs_argu32);
RTOS_Status_ten RTOS_TimerStopFromISR_st(U8 TimerIdx_argu8);
RTOS_Status_ten RTOS_TimerDelete_st(U8 TimerIdx_argu8);

/* ==================== SYSTEM MANAGEMENT APIs ==================== */
void RTOS_StartScheduler_gv(void);
/* ==================== TASK DELAY APIs ==================== */
RTOS_Status_ten RTOS_TaskDelayUntil_st(U32* lastWakeTime, U32 periodMs);
U32 RTOS_GetTickCount_u32(void);
RTOS_Status_ten RTOS_TaskDelay_st(U32 delayMs_argu32);


/* ==================== TASK TIMING APIs ==================== */
void RTOS_TaskTimingStart_st(U8 taskIdx_argu8);
void RTOS_TaskTimingEnd_st(U8 taskIdx_argu8);
void RTOS_GetTaskTimingStats_st(APP_TaskTiming_tst* timingStats);
/* ==================== STACK MONITORING APIs ==================== */
void RTOS_PeakStackUsed_st(U8 taskIdx_argu8);
#endif /* RTOS_ABSTRACTION_H_ */
