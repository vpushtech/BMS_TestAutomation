/*
 * VPUSH_RTOS.c
 *
 *  Created on: 14-Oct-2025
 *  Author: RushikeshNitinKamble
 *  Description: BMS Application RTOS Configuration Implementation
 *  Version: 1.0
 *  Modification History:
 *  Date            Author          Description
 *  ----------------------------------------------------------------------------
 *  14-Oct-2025     Rushikesh       Initial RTOS Configuration Implementation
 *  16-Oct-2025     Rushikesh       Added Static Allocation for RTOS Objects
 *  18-Oct-2025     Rushikesh       Task and Queue Configuration Implementation
 *  20-Oct-2025     Rushikesh       Complete RTOS API Implementation
 ******************************************************************************/

#include <vpush_rtos.h>
/* ==================== STATIC TIMING VARIABLES ==================== */
APP_TaskTiming_tst taskTiming_St = {0};
static U32 RTOS_TaskstartTime_u32_arr[MAX_TASK] = {0};
static U32 RTOS_PreviousstartTime_u32_arr[MAX_TASK] = {0};
static U32 RTOS_PeriodSampleCount_arr[MAX_TASK] = {0};
static U32 RTOS_AccumulatedPeriod_arr[MAX_TASK] = {0};
static U32 RTOS_WatchdogTaskPeriod_u32 = 0;
RTOS_PeakStack_tst RTOS_PeakStack_St={0};
/* ==================== PRIVATE MACROS ==================== */
#define RTOS_MS_TO_TICKS(ms) ((ms) / portTICK_PERIOD_MS)
/* ==================== TASK MANAGEMENT APIs ==================== */

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : RTOS_TaskCreateStatic_st
*   Description   : Creates a static RTOS task using pre-allocated memory
*   Parameters    : task_Idx_argu8 - Task index in configuration table
*   Return Value  : RTOS_Status_ten - Status of task creation
*  --------------------------------------------------------------------------- */
RTOS_Status_ten RTOS_TaskCreateStatic_st(U8 task_Idx_argu8)
{
    if (task_Idx_argu8 >= MAX_TASK)
    {
        return RTOS_STATUS_INVALID_PARAM;
    }

    const RTOS_TaskConfig_tst* config = &RTOS_TaskConfig_st[task_Idx_argu8];

    if ((config->taskFunction_arg == NULL) || (config->stackBuffer == NULL))
    {
        return RTOS_STATUS_INVALID_PARAM;
    }

    taskHandle[task_Idx_argu8] = xTaskCreateStatic(
        config->taskFunction_arg,
        config->taskName,
        config->Stack_Size_u32,
        NULL,
        config->TaskPriority_u8,
        (StackType_t*)config->stackBuffer,
        &StaticTask[task_Idx_argu8]
    );

    return (taskHandle[task_Idx_argu8] != NULL) ? RTOS_STATUS_SUCCESS : RTOS_STATUS_ERROR;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : RTOS_TaskDelete_st
*   Description   : Deletes an RTOS task and clears its handle
*   Parameters    : task_argu8 - Task index to delete
*   Return Value  : RTOS_Status_ten - Status of task deletion
*  --------------------------------------------------------------------------- */
RTOS_Status_ten RTOS_TaskDelete_st(U8 task_argu8)
{
    if (task_argu8 >= MAX_TASK)
    {
        return RTOS_STATUS_INVALID_PARAM;
    }

    if (taskHandle[task_argu8] == NULL)
    {
        return RTOS_STATUS_ERROR;
    }

    vTaskDelete(taskHandle[task_argu8]);
    taskHandle[task_argu8] = NULL;

    return RTOS_STATUS_SUCCESS;
}



/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : RTOS_TaskSuspend_st
*   Description   : Suspends execution of specified task
*   Parameters    : task_argu8 - Task index to suspend
*   Return Value  : RTOS_Status_ten - Status of suspend operation
*  --------------------------------------------------------------------------- */
RTOS_Status_ten RTOS_TaskSuspend_st(U8 task_argu8)
{
    if (task_argu8 >= MAX_TASK)
    {
        return RTOS_STATUS_INVALID_PARAM;
    }

    if (taskHandle[task_argu8] == NULL)
    {
        return RTOS_STATUS_ERROR;
    }

    vTaskSuspend(taskHandle[task_argu8]);
    return RTOS_STATUS_SUCCESS;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : RTOS_TaskResume_st
*   Description   : Resumes execution of suspended task
*   Parameters    : task_argu8 - Task index to resume
*   Return Value  : RTOS_Status_ten - Status of resume operation
*  --------------------------------------------------------------------------- */
RTOS_Status_ten RTOS_TaskResume_st(U8 task_argu8)
{
    if (task_argu8 >= MAX_TASK)
    {
        return RTOS_STATUS_INVALID_PARAM;
    }

    if (taskHandle[task_argu8] == NULL)
    {
        return RTOS_STATUS_ERROR;
    }

    vTaskResume(taskHandle[task_argu8]);
    return RTOS_STATUS_SUCCESS;
}
U32 RTOS_TaskGetStackHighWaterMark_st(U8 task_Idx_argu8)
{
    if (task_Idx_argu8 >= MAX_TASK)
    {
        return 0;
    }

    if (taskHandle[task_Idx_argu8] == NULL)
    {
        return 0;
    }

    return uxTaskGetStackHighWaterMark(taskHandle[task_Idx_argu8]);
}
/* ==================== QUEUE MANAGEMENT APIs ==================== */

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : RTOS_QueueCreateStatic_st
*   Description   : Creates a static queue using pre-allocated memory
*   Parameters    : QueueIdx_argu8 - Queue index in configuration table
*   Return Value  : RTOS_Status_ten - Status of queue creation
*  --------------------------------------------------------------------------- */
RTOS_Status_ten RTOS_QueueCreateStatic_st(U8 QueueIdx_argu8)
{
    if (QueueIdx_argu8 >= MAX_QUEUE)
    {
        return RTOS_STATUS_INVALID_PARAM;
    }

    const RTOS_QueueConfig_tst* config = &RTOS_QueueConfig_st[QueueIdx_argu8];

    if (config->queueStorage_u8 == NULL)
    {
        return RTOS_STATUS_INVALID_PARAM;
    }

    queueHandle[QueueIdx_argu8] = xQueueCreateStatic(
        config->Queue_size_u32,
        config->itemSize_u32,
        (uint8_t*)config->queueStorage_u8,
        &queueBuffer[QueueIdx_argu8]
    );

    return (queueHandle[QueueIdx_argu8] != NULL) ? RTOS_STATUS_SUCCESS : RTOS_STATUS_ERROR;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : RTOS_QueueSend_st
*   Description   : Sends an item to queue with timeout (task context)
*   Parameters    : QueueIdx_argu8 - Queue index
*                   item_argpv - Pointer to item to send
*                   timeoutMs_argu32 - Timeout in milliseconds
*   Return Value  : RTOS_Status_ten - Status of send operation
*  --------------------------------------------------------------------------- */
RTOS_Status_ten RTOS_QueueSend_st(U8 QueueIdx_argu8, const void* item_argpv, U32 timeoutMs_argu32)
{
    BaseType_t status_En;

    if ((QueueIdx_argu8 >= MAX_QUEUE) || (item_argpv == NULL))
    {
        return RTOS_STATUS_INVALID_PARAM;
    }

    if (queueHandle[QueueIdx_argu8] == NULL)
    {
        return RTOS_STATUS_ERROR;
    }

    status_En = xQueueSend(queueHandle[QueueIdx_argu8], item_argpv, RTOS_MS_TO_TICKS(timeoutMs_argu32));

    if (status_En == pdPASS)
    {
        return RTOS_STATUS_SUCCESS;
    }
    else if (status_En == errQUEUE_FULL)
    {
        return RTOS_STATUS_ERROR;
    }
    else
    {
        return RTOS_STATUS_TIMEOUT;
    }
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : RTOS_QueueSendFromISR_st
*   Description   : Sends an item to queue from ISR context
*   Parameters    : QueueIdx_argu8 - Queue index
*                   item_argpv - Pointer to item to send
*   Return Value  : RTOS_Status_ten - Status of send operation
*  --------------------------------------------------------------------------- */
RTOS_Status_ten RTOS_QueueSendFromISR_st(U8 QueueIdx_argu8, const void* item_argpv)
{
    BaseType_t status_En;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if ((QueueIdx_argu8 >= MAX_QUEUE) || (item_argpv == NULL))
    {
        return RTOS_STATUS_INVALID_PARAM;
    }

    if (queueHandle[QueueIdx_argu8] == NULL)
    {
        return RTOS_STATUS_ERROR;
    }

    status_En = xQueueSendFromISR(queueHandle[QueueIdx_argu8], item_argpv, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    return (status_En == pdPASS) ? RTOS_STATUS_SUCCESS : RTOS_STATUS_ERROR;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : RTOS_QueueReceive_st
*   Description   : Receives an item from queue with timeout (task context)
*   Parameters    : QueueIdx_argu8 - Queue index
*                   buffer_argpv - Pointer to receive buffer
*                   timeoutMs_argu32 - Timeout in milliseconds
*   Return Value  : RTOS_Status_ten - Status of receive operation
*  --------------------------------------------------------------------------- */
RTOS_Status_ten RTOS_QueueReceive_st(U8 QueueIdx_argu8, void* buffer_argpv, U32 timeoutMs_argu32)
{
    BaseType_t status_En;

    if ((QueueIdx_argu8 >= MAX_QUEUE) || (buffer_argpv == NULL))
    {
        return RTOS_STATUS_INVALID_PARAM;
    }

    if (queueHandle[QueueIdx_argu8] == NULL)
    {
        return RTOS_STATUS_ERROR;
    }

    status_En = xQueueReceive(queueHandle[QueueIdx_argu8], buffer_argpv, RTOS_MS_TO_TICKS(timeoutMs_argu32));
    return (status_En == pdPASS) ? RTOS_STATUS_SUCCESS : RTOS_STATUS_TIMEOUT;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : RTOS_QueueReceiveFromISR_st
*   Description   : Receives an item from queue from ISR context
*   Parameters    : QueueIdx_argu8 - Queue index
*                   buffer_argpv - Pointer to receive buffer
*   Return Value  : RTOS_Status_ten - Status of receive operation
*  --------------------------------------------------------------------------- */
RTOS_Status_ten RTOS_QueueReceiveFromISR_st(U8 QueueIdx_argu8, void* buffer_argpv)
{
    BaseType_t status_En;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if ((QueueIdx_argu8 >= MAX_QUEUE) || (buffer_argpv == NULL))
    {
        return RTOS_STATUS_INVALID_PARAM;
    }

    if (queueHandle[QueueIdx_argu8] == NULL)
    {
        return RTOS_STATUS_ERROR;
    }

    status_En = xQueueReceiveFromISR(queueHandle[QueueIdx_argu8], buffer_argpv, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    return (status_En == pdPASS) ? RTOS_STATUS_SUCCESS : RTOS_STATUS_ERROR;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : RTOS_QueueDelete_st
*   Description   : Deletes a queue and clears its handle
*   Parameters    : QueueIdx_argu8 - Queue index to delete
*   Return Value  : RTOS_Status_ten - Status of queue deletion
*  --------------------------------------------------------------------------- */
RTOS_Status_ten RTOS_QueueDelete_st(U8 QueueIdx_argu8)
{
    if (QueueIdx_argu8 >= MAX_QUEUE)
    {
        return RTOS_STATUS_INVALID_PARAM;
    }

    if (queueHandle[QueueIdx_argu8] == NULL)
    {
        return RTOS_STATUS_ERROR;
    }

    vQueueDelete(queueHandle[QueueIdx_argu8]);
    queueHandle[QueueIdx_argu8] = NULL;

    return RTOS_STATUS_SUCCESS;
}

/* ==================== SEMAPHORE MANAGEMENT APIs ==================== */

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : RTOS_BinarySemaphoreCreateStatic_st
*   Description   : Creates a static binary semaphore
*   Parameters    : SemaphoreIdx_argu8 - Semaphore index
*   Return Value  : RTOS_Status_ten - Status of semaphore creation
*  --------------------------------------------------------------------------- */
RTOS_Status_ten RTOS_BinarySemaphoreCreateStatic_st(U8 SemaphoreIdx_argu8)
{
    if (SemaphoreIdx_argu8 >= MAX_SEMAPHORE)
    {
        return RTOS_STATUS_INVALID_PARAM;
    }

    semaphoreHandle[SemaphoreIdx_argu8] = xSemaphoreCreateBinaryStatic(&semaphoreBuffer[SemaphoreIdx_argu8]);

    if (semaphoreHandle[SemaphoreIdx_argu8] == NULL)
    {
        return RTOS_STATUS_ERROR;
    }
    return RTOS_STATUS_SUCCESS;
}
/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : RTOS_MutexCreateStatic_st
*   Description   : Creates a static mutex semaphore
*   Parameters    : SemaphoreIdx_argu8 - Semaphore index
*   Return Value  : RTOS_Status_ten - Status of mutex creation
*  --------------------------------------------------------------------------- */
RTOS_Status_ten RTOS_MutexCreateStatic_st(U8 SemaphoreIdx_argu8)
{
    if (SemaphoreIdx_argu8 >= MAX_SEMAPHORE)
    {
        return RTOS_STATUS_INVALID_PARAM;
    }

    semaphoreHandle[SemaphoreIdx_argu8] = xSemaphoreCreateMutexStatic(&semaphoreBuffer[SemaphoreIdx_argu8]);

    if (semaphoreHandle[SemaphoreIdx_argu8] == NULL)
    {
        return RTOS_STATUS_ERROR;
    }

    return RTOS_STATUS_SUCCESS;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : RTOS_SemaphoreTake_st
*   Description   : Takes a semaphore with timeout
*   Parameters    : SemaphoreIdx_argu8 - Semaphore index
*                   timeoutMs_argu32 - Timeout in milliseconds
*   Return Value  : RTOS_Status_ten - Status of take operation
*  --------------------------------------------------------------------------- */
RTOS_Status_ten RTOS_SemaphoreTake_st(U8 SemaphoreIdx_argu8, U32 timeoutMs_argu32)
{
    BaseType_t status_En;

    if (SemaphoreIdx_argu8 >= MAX_SEMAPHORE)
    {
        return RTOS_STATUS_INVALID_PARAM;
    }

    if (semaphoreHandle[SemaphoreIdx_argu8] == NULL)
    {
        return RTOS_STATUS_ERROR;
    }

    status_En = xSemaphoreTake(semaphoreHandle[SemaphoreIdx_argu8], RTOS_MS_TO_TICKS(timeoutMs_argu32));

    if (status_En == pdPASS)
    {
        return RTOS_STATUS_SUCCESS;
    }
    else if (status_En == errQUEUE_EMPTY)
    {
        return RTOS_STATUS_TIMEOUT;
    }
    else
    {
        return RTOS_STATUS_ERROR;
    }
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : RTOS_SemaphoreGive_st
*   Description   : Gives a semaphore (task context)
*   Parameters    : SemaphoreIdx_argu8 - Semaphore index
*   Return Value  : RTOS_Status_ten - Status of give operation
*  --------------------------------------------------------------------------- */
RTOS_Status_ten RTOS_SemaphoreGive_st(U8 SemaphoreIdx_argu8)
{
    BaseType_t status_En;

    if (SemaphoreIdx_argu8 >= MAX_SEMAPHORE)
    {
        return RTOS_STATUS_INVALID_PARAM;
    }

    if (semaphoreHandle[SemaphoreIdx_argu8] == NULL)
    {
        return RTOS_STATUS_ERROR;
    }

    status_En = xSemaphoreGive(semaphoreHandle[SemaphoreIdx_argu8]);

    if (status_En == pdPASS)
    {
        return RTOS_STATUS_SUCCESS;
    }
    else
    {
        return RTOS_STATUS_ERROR;
    }
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : RTOS_SemaphoreGiveFromISR_st
*   Description   : Gives a semaphore from ISR context
*   Parameters    : SemaphoreIdx_argu8 - Semaphore index
*   Return Value  : RTOS_Status_ten - Status of give operation
*  --------------------------------------------------------------------------- */
RTOS_Status_ten RTOS_SemaphoreGiveFromISR_st(U8 SemaphoreIdx_argu8)
{
    BaseType_t status_En;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (SemaphoreIdx_argu8 >= MAX_SEMAPHORE)
    {
        return RTOS_STATUS_INVALID_PARAM;
    }

    if (semaphoreHandle[SemaphoreIdx_argu8] == NULL)
    {
        return RTOS_STATUS_ERROR;
    }

    status_En = xSemaphoreGiveFromISR(semaphoreHandle[SemaphoreIdx_argu8], &xHigherPriorityTaskWoken);

    if (xHigherPriorityTaskWoken == pdTRUE)
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }

    return (status_En == pdPASS) ? RTOS_STATUS_SUCCESS : RTOS_STATUS_ERROR;
}

/* ==================== TIMER MANAGEMENT APIs ==================== */

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : RTOS_TimerCreateStatic_st
*   Description   : Creates a static RTOS timer
*   Parameters    : TimerIdx_argu8 - Timer index in configuration table
*   Return Value  : RTOS_Status_ten - Status of timer creation
*  --------------------------------------------------------------------------- */
RTOS_Status_ten RTOS_TimerCreateStatic_st(U8 TimerIdx_argu8)
{
    if (TimerIdx_argu8 >= MAX_TIMER)
    {
        return RTOS_STATUS_INVALID_PARAM;
    }

    const RTOS_TimerConfig_tst* config = &RTOS_TimerConfig_st[TimerIdx_argu8];

    if ((config->timerName == NULL) || (config->callback == NULL))
    {
        return RTOS_STATUS_INVALID_PARAM;
    }

    timerHandle[TimerIdx_argu8] = xTimerCreateStatic(
        config->timerName,
        RTOS_MS_TO_TICKS(config->periodMs_u32),
        config->autoReload_u8,
        NULL,
        config->callback,
        &timerBuffer[TimerIdx_argu8]
    );

    return (timerHandle[TimerIdx_argu8] != NULL) ? RTOS_STATUS_SUCCESS : RTOS_STATUS_ERROR;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : RTOS_TimerStart_st
*   Description   : Starts an RTOS timer with timeout
*   Parameters    : TimerIdx_argu8 - Timer index
*                   timeoutMs_argu32 - Timeout in milliseconds
*   Return Value  : RTOS_Status_ten - Status of start operation
*  --------------------------------------------------------------------------- */
RTOS_Status_ten RTOS_TimerStart_st(U8 TimerIdx_argu8, U32 timeoutMs_argu32)
{
    BaseType_t status_En;

    if (TimerIdx_argu8 >= MAX_TIMER)
    {
        return RTOS_STATUS_INVALID_PARAM;
    }

    if (timerHandle[TimerIdx_argu8] == NULL)
    {
        return RTOS_STATUS_ERROR;
    }

    status_En = xTimerStart(timerHandle[TimerIdx_argu8], RTOS_MS_TO_TICKS(timeoutMs_argu32));
    return (status_En == pdPASS) ? RTOS_STATUS_SUCCESS : RTOS_STATUS_ERROR;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : RTOS_TimerStartFromISR_st
*   Description   : Starts an RTOS timer from ISR context
*   Parameters    : TimerIdx_argu8 - Timer index
*   Return Value  : RTOS_Status_ten - Status of start operation
*  --------------------------------------------------------------------------- */
RTOS_Status_ten RTOS_TimerStartFromISR_st(U8 TimerIdx_argu8)
{
    BaseType_t status_En;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (TimerIdx_argu8 >= MAX_TIMER)
    {
        return RTOS_STATUS_INVALID_PARAM;
    }

    if (timerHandle[TimerIdx_argu8] == NULL)
    {
        return RTOS_STATUS_ERROR;
    }

    status_En = xTimerStartFromISR(timerHandle[TimerIdx_argu8], &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    return (status_En == pdPASS) ? RTOS_STATUS_SUCCESS : RTOS_STATUS_ERROR;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : RTOS_TimerStop_st
*   Description   : Stops an RTOS timer with timeout
*   Parameters    : TimerIdx_argu8 - Timer index
*                   timeoutMs_argu32 - Timeout in milliseconds
*   Return Value  : RTOS_Status_ten - Status of stop operation
*  --------------------------------------------------------------------------- */
RTOS_Status_ten RTOS_TimerStop_st(U8 TimerIdx_argu8, U32 timeoutMs_argu32)
{
    BaseType_t status_En;

    if (TimerIdx_argu8 >= MAX_TIMER)
    {
        return RTOS_STATUS_INVALID_PARAM;
    }

    if (timerHandle[TimerIdx_argu8] == NULL)
    {
        return RTOS_STATUS_ERROR;
    }

    status_En = xTimerStop(timerHandle[TimerIdx_argu8], RTOS_MS_TO_TICKS(timeoutMs_argu32));
    return (status_En == pdPASS) ? RTOS_STATUS_SUCCESS : RTOS_STATUS_ERROR;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : RTOS_TimerStopFromISR_st
*   Description   : Stops an RTOS timer from ISR context
*   Parameters    : TimerIdx_argu8 - Timer index
*   Return Value  : RTOS_Status_ten - Status of stop operation
*  --------------------------------------------------------------------------- */
RTOS_Status_ten RTOS_TimerStopFromISR_st(U8 TimerIdx_argu8)
{
    BaseType_t status_En;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;

    if (TimerIdx_argu8 >= MAX_TIMER)
    {
        return RTOS_STATUS_INVALID_PARAM;
    }

    if (timerHandle[TimerIdx_argu8] == NULL)
    {
        return RTOS_STATUS_ERROR;
    }

    status_En = xTimerStopFromISR(timerHandle[TimerIdx_argu8], &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);

    return (status_En == pdPASS) ? RTOS_STATUS_SUCCESS : RTOS_STATUS_ERROR;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : RTOS_TimerDelete_st
*   Description   : Deletes an RTOS timer and clears its handle
*   Parameters    : TimerIdx_argu8 - Timer index to delete
*   Return Value  : RTOS_Status_ten - Status of timer deletion
*  --------------------------------------------------------------------------- */
RTOS_Status_ten RTOS_TimerDelete_st(U8 TimerIdx_argu8)
{
    if (TimerIdx_argu8 >= MAX_TIMER)
    {
        return RTOS_STATUS_INVALID_PARAM;
    }

    if (timerHandle[TimerIdx_argu8] == NULL)
    {
        return RTOS_STATUS_ERROR;
    }

    if (xTimerDelete(timerHandle[TimerIdx_argu8], 0) == pdPASS)
    {
        timerHandle[TimerIdx_argu8] = NULL;
        return RTOS_STATUS_SUCCESS;
    }

    return RTOS_STATUS_ERROR;
}

/* ==================== SYSTEM MANAGEMENT APIs ==================== */

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : RTOS_StartScheduler_gv
*   Description   : Starts the FreeRTOS scheduler
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void RTOS_StartScheduler_gv(void)
{
    vTaskStartScheduler();
}

/* ==================== FREERTOS HOOK FUNCTIONS ==================== */

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : vApplicationGetIdleTaskMemory
*   Description   : Provides memory for idle task (static allocation)
*   Parameters    : ppxIdleTaskTCBBuffer - Pointer to store TCB buffer
*                   ppxIdleTaskStackBuffer - Pointer to store stack buffer
*                   pulIdleTaskStackSize - Pointer to store stack size
*   Return Value  : None
*  --------------------------------------------------------------------------- */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                  StackType_t **ppxIdleTaskStackBuffer,
                                  uint32_t *pulIdleTaskStackSize)
{
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : vApplicationGetTimerTaskMemory
*   Description   : Provides memory for timer task (static allocation)
*   Parameters    : ppxTimerTaskTCBBuffer - Pointer to store TCB buffer
*                   ppxTimerTaskStackBuffer - Pointer to store stack buffer
*                   pulTimerTaskStackSize - Pointer to store stack size
*   Return Value  : None
*  --------------------------------------------------------------------------- */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                   StackType_t **ppxTimerTaskStackBuffer,
                                   uint32_t *pulTimerTaskStackSize)
{
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : vApplicationStackOverflowHook
*   Description   : Stack overflow handler - disables interrupts and halts
*   Parameters    : pxTask - Handle of offending task
*                   pcTaskName - Name of offending task
*   Return Value  : None
*  --------------------------------------------------------------------------- */
void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
    (void)pcTaskName;
    (void)pxTask;
    taskDISABLE_INTERRUPTS();
    for(;;);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : vApplicationIdleHook
*   Description   : Idle task hook function for monitoring
*   Parameters    : None
*   Return Value  : None
*  --------------------------------------------------------------------------- */
void vApplicationIdleHook(void)
{

}
/* ==================== TASK DELAY APIs ==================== */
/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : RTOS_GetTickCount_u32
*   Description   : Gets current RTOS tick count
*   Parameters    : None
*   Return Value  : U32 - Current tick count
*  --------------------------------------------------------------------------- */
U32 RTOS_GetTickCount_u32(void)
{
    return xTaskGetTickCount();
}
/* -----------------------------------------------------------------------------
 *  Function Name : RTOS_TaskDelayUntil_st
 *  Description   : Delays task until specified time (fixed period execution)
 *  Parameters    : lastWakeTime - Pointer to last wake time variable
 *                  periodMs     - Period between executions (milliseconds)
 *  Return Value  : RTOS_Status_ten - Operation status
 * --------------------------------------------------------------------------- */
RTOS_Status_ten RTOS_TaskDelayUntil_st(U32* lastWakeTime, U32 periodMs)
{
    if (lastWakeTime == NULL) {
        return RTOS_STATUS_INVALID_PARAM;
    }

    vTaskDelayUntil(lastWakeTime, RTOS_MS_TO_TICKS(periodMs));
    return RTOS_STATUS_SUCCESS;
}
/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : RTOS_TaskDelay_st
*   Description   : Delays task execution for specified milliseconds
*   Parameters    : delayMs_argu32 - Delay time in milliseconds
*   Return Value  : RTOS_Status_ten - Status of delay operation
*  --------------------------------------------------------------------------- */
RTOS_Status_ten RTOS_TaskDelay_st(U32 delayMs_argu32)
{
    if (delayMs_argu32 == 0)
    {
        taskYIELD();
    }
    else
    {
        vTaskDelay(RTOS_MS_TO_TICKS(delayMs_argu32));
    }

    return RTOS_STATUS_SUCCESS;
}


/* ==================== TASK TIMING APIs ==================== */
/* -----------------------------------------------------------------------------
 *  Function Name : RTOS_TaskTimingStart_st
 *  Description   : Starts task timing measurement and calculates period
 *  Parameters    : taskIdx_argu8 - Task index to measure
 *  Return Value  : None
 * --------------------------------------------------------------------------- */
void RTOS_TaskTimingStart_st(U8 taskIdx_argu8)
{
    if (taskIdx_argu8 < MAX_TASK) {
        U32 currentTime_u32 = RTOS_GetTickCount_u32();

        if (RTOS_PreviousstartTime_u32_arr[taskIdx_argu8] > 0) {
            U32 period = currentTime_u32 - RTOS_PreviousstartTime_u32_arr[taskIdx_argu8];
            RTOS_TaskTiming_tst* timing = NULL;
            switch(taskIdx_argu8) {
                case RTOS_DataProcessing:
                    timing = &taskTiming_St.RTOS_DataProcessing;
                    break;
                case RTOS_BMS_StateProcessing:
                    timing = &taskTiming_St.RTOS_BMS_StateProcessing;
                    break;
                case RTOS_SoCEstimation:
                    timing = &taskTiming_St.RTOS_SoCEstimation;
                    break;
                case RTOS_WDT:
                    timing = &taskTiming_St.RTOS_WDT;
                    RTOS_WatchdogTaskPeriod_u32 = period;
                    break;
                default:
                    break;
            }
                timing->taskPeriod = period;
                    RTOS_AccumulatedPeriod_arr[taskIdx_argu8] += period;
                    RTOS_PeriodSampleCount_arr[taskIdx_argu8]++;
                    if (RTOS_PeriodSampleCount_arr[taskIdx_argu8] >= 3) {
                        U32 averagePeriod = RTOS_AccumulatedPeriod_arr[taskIdx_argu8] / 3;
                        timing->taskCount = averagePeriod / RTOS_WatchdogTaskPeriod_u32;
                        RTOS_AccumulatedPeriod_arr[taskIdx_argu8] = 0;
                        RTOS_PeriodSampleCount_arr[taskIdx_argu8] = 0;
                        timing->taskPeriod = 0;
                        timing->taskExecutionTime = 0;
                        RTOS_PreviousstartTime_u32_arr[taskIdx_argu8] = 0;
                        RTOS_TaskstartTime_u32_arr[taskIdx_argu8] = 0;
                    }
        }
        RTOS_PreviousstartTime_u32_arr[taskIdx_argu8] = currentTime_u32;
        RTOS_TaskstartTime_u32_arr[taskIdx_argu8] = currentTime_u32;
    }
}
/* -----------------------------------------------------------------------------
 *  Function Name : RTOS_TaskTimingEnd_st
 *  Description   : Ends task timing measurement and calculates execution time
 *  Parameters    : taskIdx_argu8 - Task index to measure
 *  Return Value  : None
 * --------------------------------------------------------------------------- */
void RTOS_TaskTimingEnd_st(U8 taskIdx_argu8)
{
    if (taskIdx_argu8 >= MAX_TASK) return;

    U32 endTime_u32 = RTOS_GetTickCount_u32();
    U32 startTime_u32 = RTOS_TaskstartTime_u32_arr[taskIdx_argu8];
    if (startTime_u32 == 0) return;
    U32 executionTime = endTime_u32 - startTime_u32;
    RTOS_TaskTiming_tst* timing = NULL;

    switch(taskIdx_argu8) {
        case RTOS_DataProcessing:
            timing = &taskTiming_St.RTOS_DataProcessing;
            break;
        case RTOS_BMS_StateProcessing:
            timing = &taskTiming_St.RTOS_BMS_StateProcessing;
            break;
        case RTOS_SoCEstimation:
            timing = &taskTiming_St.RTOS_SoCEstimation;
            break;
        case RTOS_WDT:
            timing = &taskTiming_St.RTOS_WDT;
            break;
        default:
            return;
    }

    if (timing != NULL) {
        timing->taskExecutionTime = executionTime;
    }
}
/* ==================== STACK MONITORING APIs ==================== */
/* -----------------------------------------------------------------------------
 *  Function Name : RTOS_PeakStackUsed_st
 *  Description   : Calculates peak stack memory used by task
 *  Parameters    : taskIdx_argu8 - Task index to measure
 *  Return Value  : None
 * --------------------------------------------------------------------------- */
 void RTOS_PeakStackUsed_st(U8 taskIdx_argu8)
 {
     if (taskIdx_argu8 >= MAX_TASK) return;

     U32 stackHighWaterMark_u32 = RTOS_TaskGetStackHighWaterMark_st(taskIdx_argu8);
     U32 stackSize_u32 = RTOS_TaskConfig_st[taskIdx_argu8].Stack_Size_u32;
     U32 peakStackUsed_u32 = (stackSize_u32 - stackHighWaterMark_u32) * sizeof(StackType_t);

     switch(taskIdx_argu8) {
         case RTOS_DataProcessing: RTOS_PeakStack_St.RTOS_DataProccessing_PeakStack = peakStackUsed_u32; break;
         case RTOS_BMS_StateProcessing: RTOS_PeakStack_St.RTOS_BMS_StateProccesing_PeakStack = peakStackUsed_u32; break;
         case RTOS_SoCEstimation: RTOS_PeakStack_St.RTOS_SoCEstimation_PeakStack = peakStackUsed_u32; break;
         case RTOS_WDT: RTOS_PeakStack_St.RTOS_WdG_PeakStack = peakStackUsed_u32; break;
         default: break;
     }
 }
