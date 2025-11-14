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
 ******************************************************************************/

#include <vpush_rtos.h>

/* ==================== STATIC VARIABLES ==================== */
TaskHandle_t taskHandle[MAX_TASK] = {NULL};
StaticTask_t StaticTask[MAX_TASK];
StaticQueue_t queueBuffer[MAX_QUEUE];
QueueHandle_t queueHandle[MAX_QUEUE] = {NULL};
StaticSemaphore_t semaphoreBuffer[MAX_SEMAPHORE];
SemaphoreHandle_t semaphoreHandle[MAX_SEMAPHORE] = {NULL};
StaticTimer_t timerBuffer[MAX_TIMER];
TimerHandle_t timerHandle[MAX_TIMER] = {NULL};

/* ==================== EXTERNAL STACK AND BUFFER DECLARATIONS ==================== */
 StackType_t RTOS_DataProcessingThread_stack[RTOS_DATA_PROCESSING_THREAD_STACK_SIZE];
 StackType_t RTOS_SoCEstimationThread_stack[RTOS_SOC_ESTIMATION_THREAD_STACK_SIZE];
 StackType_t RTOS_BMS_StateProcessingThread_stack[RTOS_BMS_STATE_PROCESSING_THREAD_STACK_SIZE];
 StackType_t RTOS_QueueReceiveThread_stack[RTOS_QUEUE_RECEIVE_THREAD_STACK_SIZE];
StackType_t RTOS_WDT_Thread_stack[RTOS_WDT_THREAD_STACK_SIZE];

 U8 RTOS_CAN_Queue_memory_arru8[RTOS_CAN_QUEUE_LENGTH * RTOS_CAN_QUEUE_ITEM_SIZE];
 U8 RTOS_HSM_Queue_memory_arru8[RTOS_HSM_QUEUE_LENGTH * RTOS_HSM_QUEUE_ITEM_SIZE];

/* ==================== EXTERNAL FUNCTION DECLARATIONS ==================== */
void RTOS_DataProcessingThread_entry(void *pvParameters);
void RTOS_SoCEstimationThread_entry(void *pvParameters);
void RTOS_BMS_StateProccessingThread_entry(void *pvParameters);
void RTOS_QueueReceiveThread_entry(void *pvParameters);
void RTOS_WDGThread_entry(void *pvParameters);

/* ==================== CONFIGURATION DEFINITIONS ==================== */
const RTOS_TaskConfig_tst RTOS_TaskConfig_st[MAX_TASK] = {
    [RTOS_DataProcessing] = {
        .taskFunction_arg = RTOS_DataProcessingThread_entry,
        .taskName = "DataProc",
        .stackBuffer = RTOS_DataProcessingThread_stack,
        .Stack_Size_u32 = RTOS_DATA_PROCESSING_THREAD_STACK_SIZE,
        .TaskPriority_u8 = RTOS_PRIORITY_ABOVE_NORMAL
    },
    [RTOS_SoCEstimation] = {
        .taskFunction_arg = RTOS_SoCEstimationThread_entry,
        .taskName = "SOCEst",
        .stackBuffer = RTOS_SoCEstimationThread_stack,
        .Stack_Size_u32 = RTOS_SOC_ESTIMATION_THREAD_STACK_SIZE,
        .TaskPriority_u8 = RTOS_PRIORITY_NORMAL
    },
    [RTOS_BMS_StateProcessing] = {
        .taskFunction_arg = RTOS_BMS_StateProccessingThread_entry,
        .taskName = "BMSState",
        .stackBuffer = RTOS_BMS_StateProcessingThread_stack,
        .Stack_Size_u32 = RTOS_BMS_STATE_PROCESSING_THREAD_STACK_SIZE,
        .TaskPriority_u8 = RTOS_PRIORITY_HIGH
    },
    [RTOS_QueueReceive] = {
        .taskFunction_arg = RTOS_QueueReceiveThread_entry,
        .taskName = "QueueRx",
        .stackBuffer = RTOS_QueueReceiveThread_stack,
        .Stack_Size_u32 = RTOS_QUEUE_RECEIVE_THREAD_STACK_SIZE,
        .TaskPriority_u8 = RTOS_PRIORITY_LOW
    },
    [RTOS_WDT] = {
        .taskFunction_arg = RTOS_WDGThread_entry,
        .taskName = "WDT",
        .stackBuffer = RTOS_WDT_Thread_stack,
        .Stack_Size_u32 = RTOS_WDT_THREAD_STACK_SIZE,
        .TaskPriority_u8 = RTOS_PRIORITY_BELOW_NORMAL
    }
};

const RTOS_QueueConfig_tst RTOS_QueueConfig_st[MAX_QUEUE] = {
    [CAN_QUEUE] = {
        .Queue_size_u32 = RTOS_CAN_QUEUE_LENGTH,
        .itemSize_u32 = RTOS_CAN_QUEUE_ITEM_SIZE,
        .queueStorage_u8 = RTOS_CAN_Queue_memory_arru8
    },
    [HSM_QUEUE] = {
        .Queue_size_u32 = RTOS_HSM_QUEUE_LENGTH,
        .itemSize_u32 = RTOS_HSM_QUEUE_ITEM_SIZE,
        .queueStorage_u8 = RTOS_HSM_Queue_memory_arru8
    }
};

/*const RTOS_TimerConfig_tst RTOS_TimerConfig_st[MAX_TIMER] = {
};*/

/* ==================== BULK CREATION APIs ==================== */

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : VPUSH_CreateAllTasks_st
*   Description   : Creates all RTOS tasks defined in configuration table
*   Parameters    : None
*   Return Value  : RTOS_Status_ten - Overall status_en of task creation
*  --------------------------------------------------------------------------- */
RTOS_Status_ten VPUSH_CreateAllTasks_st(void)
{
    RTOS_Status_ten overallStatus_en = RTOS_STATUS_SUCCESS;
    U8 taskIndex;

    for (taskIndex = 0; taskIndex < MAX_TASK; taskIndex++)
    {
        RTOS_Status_ten status_en = RTOS_TaskCreateStatic_st(taskIndex);
        if (status_en != RTOS_STATUS_SUCCESS)
        {
        	overallStatus_en = status_en;
        }
    }

    return overallStatus_en;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : VPUSH_CreateAllQueues_st
*   Description   : Creates all RTOS queues defined in configuration table
*   Parameters    : None
*   Return Value  : RTOS_Status_ten - Overall status_en of queue creation
*  --------------------------------------------------------------------------- */
RTOS_Status_ten VPUSH_CreateAllQueues_st(void)
{
    RTOS_Status_ten overallStatus_en = RTOS_STATUS_SUCCESS;
    U8 queueIndex;

    for (queueIndex = 0; queueIndex < MAX_QUEUE; queueIndex++)
    {
        RTOS_Status_ten status_en = RTOS_QueueCreateStatic_st(queueIndex);
        if (status_en != RTOS_STATUS_SUCCESS)
        {
        	overallStatus_en = status_en;
        }
    }

    return overallStatus_en;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : VPUSH_CreateAllSemaphores_st
*   Description   : Creates all RTOS semaphores and mutexes
*   Parameters    : None
*   Return Value  : RTOS_Status_ten - Overall status_en of semaphore creation
*  --------------------------------------------------------------------------- */
RTOS_Status_ten VPUSH_CreateAllSemaphores_st(void)
{
    RTOS_Status_ten overallStatus_en = RTOS_STATUS_SUCCESS;
    U8 semaphoreIndex;

    for (semaphoreIndex = 0; semaphoreIndex < MAX_SEMAPHORE; semaphoreIndex++)
    {
        RTOS_Status_ten status_en;

        if (semaphoreIndex == I2C_MUTEX)
        {
            status_en = RTOS_MutexCreateStatic_st(semaphoreIndex);
        }
        else
        {
            status_en = RTOS_BinarySemaphoreCreateStatic_st(semaphoreIndex);
        }

        if (status_en != RTOS_STATUS_SUCCESS)
        {
        	overallStatus_en = status_en;
        }
    }

    return overallStatus_en;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : VPUSH_CreateAllObjects_st
*   Description   : Creates all RTOS objects (tasks, queues, semaphores)
*   Parameters    : None
*   Return Value  : RTOS_Status_ten - Overall status_en of object creation
*  --------------------------------------------------------------------------- */
RTOS_Status_ten VPUSH_CreateAllObjects_st(void)
{
    RTOS_Status_ten status_en;

    status_en = VPUSH_CreateAllTasks_st();
    if (status_en != RTOS_STATUS_SUCCESS)
    {
        return status_en;
    }

    status_en = VPUSH_CreateAllQueues_st();
    if (status_en != RTOS_STATUS_SUCCESS)
    {
        return status_en;
    }

    status_en = VPUSH_CreateAllSemaphores_st();
    if (status_en != RTOS_STATUS_SUCCESS)
    {
        return status_en;
    }
    return status_en;
}

/* ==================== APPLICATION INITIALIZATION ==================== */

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : VPUSH_RTOS_Init_gv
*   Description   : Initializes RTOS environment and starts scheduler
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void VPUSH_RTOS_Init_gv(void)
{
    /* Create all RTOS objects */
    VPUSH_CreateAllObjects_st();

    /* Start the scheduler */
    RTOS_StartScheduler_gv();
}
