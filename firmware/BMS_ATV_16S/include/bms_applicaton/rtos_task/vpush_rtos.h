/*
 * VPUSH_RTOS.h
 *
 *  Created on: 14-Oct-2025
 *  Author: RushikeshNitinKamble
 *  Description: BMS Application RTOS Configuration
 */

#ifndef VPUSH_RTOS_H_
#define VPUSH_RTOS_H_

/* ==================== INCLUDES ==================== */
#include <rtos_abstraction.h>

/* ==================== THREAD PRIORITY CONFIGURATIONS ==================== */
#define RTOS_PRIORITY_HIGH                (tskIDLE_PRIORITY + 5)
#define RTOS_PRIORITY_ABOVE_NORMAL        (tskIDLE_PRIORITY + 4)
#define RTOS_PRIORITY_NORMAL              (tskIDLE_PRIORITY + 3)
#define RTOS_PRIORITY_BELOW_NORMAL        (tskIDLE_PRIORITY + 2)
#define RTOS_PRIORITY_LOW                 (tskIDLE_PRIORITY + 1)

/* ==================== THREAD STACK SIZE CONFIGURATIONS ==================== */
#define RTOS_DATA_PROCESSING_THREAD_STACK_SIZE         (3072/4)  /* 768 words */
#define RTOS_SOC_ESTIMATION_THREAD_STACK_SIZE          (3072/4)  /* 768 words */
#define RTOS_BMS_STATE_PROCESSING_THREAD_STACK_SIZE    (1024/4)  /* 256 words */
#define RTOS_QUEUE_RECEIVE_THREAD_STACK_SIZE           (512/4)   /* 128 words */
#define RTOS_WDT_THREAD_STACK_SIZE                     (512/4)   /* 128 words */

/* ==================== QUEUE CONFIGURATIONS ==================== */
#define RTOS_CAN_QUEUE_LENGTH               30
#define RTOS_CAN_QUEUE_ITEM_SIZE            20
#define RTOS_HSM_QUEUE_LENGTH               20
#define RTOS_HSM_QUEUE_ITEM_SIZE            2

/* ==================== TYPE DEFINITIONS ==================== */
typedef struct {
    RTOS_TaskFunction_t taskFunction_arg;
    const char* taskName;
    void* stackBuffer;
    U32 Stack_Size_u32;
    U8 TaskPriority_u8;
} RTOS_TaskConfig_tst;

typedef struct {
    U32 Queue_size_u32;
    U32 itemSize_u32;
    U8* queueStorage_u8;
} RTOS_QueueConfig_tst;

typedef struct {
    const char* timerName;
    U32 periodMs_u32;
    U8 autoReload_u8;
    RTOS_TimerCallback_t callback;
} RTOS_TimerConfig_tst;

typedef enum {
    RTOS_DataProcessing,
    RTOS_SoCEstimation,
    RTOS_BMS_StateProcessing,
    RTOS_QueueReceive,
    RTOS_WDT,
    MAX_TASK
} RTOS_Task_En;

typedef enum {
    CAN_QUEUE,
    HSM_QUEUE,
    MAX_QUEUE
} RTOS_Queue_En;

typedef enum {
    SOC_SEMAPHORE,
    I2C_MUTEX,
    MAX_SEMAPHORE
} RTOS_Semaphore_En;

typedef enum {
	RTOS_TIMER,
    MAX_TIMER
} RTOS_Timer_En;

/* ==================== EXTERNAL VARIABLES ==================== */
extern TaskHandle_t taskHandle[MAX_TASK];
extern StaticTask_t StaticTask[MAX_TASK];
extern StaticQueue_t queueBuffer[MAX_QUEUE];
extern QueueHandle_t queueHandle[MAX_QUEUE];
extern StaticSemaphore_t semaphoreBuffer[MAX_SEMAPHORE];
extern SemaphoreHandle_t semaphoreHandle[MAX_SEMAPHORE];
extern StaticTimer_t timerBuffer[MAX_TIMER];
extern TimerHandle_t timerHandle[MAX_TIMER];

/* ==================== CONFIGURATION STRUCTURES ==================== */
extern const RTOS_TaskConfig_tst RTOS_TaskConfig_st[MAX_TASK];
extern const RTOS_QueueConfig_tst RTOS_QueueConfig_st[MAX_QUEUE];
extern const RTOS_TimerConfig_tst RTOS_TimerConfig_st[MAX_TIMER];

/* ==================== BULK CREATION APIs ==================== */
RTOS_Status_ten VPUSH_CreateAllTasks_st(void);
RTOS_Status_ten VPUSH_CreateAllQueues_st(void);
RTOS_Status_ten VPUSH_CreateAllSemaphores_st(void);
RTOS_Status_ten VPUSH_CreateAllTimers_st(void);
RTOS_Status_ten VPUSH_CreateAllObjects_st(void);

/* ==================== Task APIs ==================== */
extern void RTOS_DataProcessingThread_entry(void *pvParameters);
extern void RTOS_SoCEstimationThread_entry(void *pvParameters);
extern void RTOS_BMS_StateProccessingThread_entry(void *pvParameters);
extern void RTOS_QueueReceiveThread_entry(void *pvParameters);
extern void RTOS_WDGThread_entry(void *pvParameters);
/* ==================== APPLICATION INITIALIZATION ==================== */
void VPUSH_RTOS_Init_gv(void);

#endif /* VPUSH_RTOS_H_ */
