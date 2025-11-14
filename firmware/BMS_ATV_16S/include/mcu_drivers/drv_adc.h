#ifndef DRIVERS_DRV_ADC_H_
#define DRIVERS_DRV_ADC_H_

#include <bsp_config.h>
#include <common.h>
#include "adc_pal.h"

/* ==================== ENUM DEFINITIONS ==================== */
typedef enum
{
    DRV_ADC_CHANNEL_0,
    DRV_ADC_CHANNEL_1,
    DRV_ADC_CHANNEL_2,
    DRV_ADC_MAX_CHANNEL
} DRV_AdcChannel_ten;

typedef enum
{
    DRV_ADC_INSTANCE_0,
    DRV_ADC_INSTANCE_1,
    DRV_ADC_MAX_INSTANCE
} DRV_AdcInstance_ten;

typedef enum
{
    DRV_ADC_GROUP_0,
    DRV_ADC_GROUP_1,
    DRV_ADC_GROUP_2,
    DRV_ADC_MAX_GROUP
} DRV_AdcGroup_ten;

typedef enum
{
    DRV_ADC_STATUS_OK,
    DRV_ADC_STATUS_ERROR,
    DRV_ADC_STATUS_BUSY,
    DRV_ADC_STATUS_TIMEOUT,
} DRV_AdcStatus_ten;

/* ==================== STRUCTURE DEFINITIONS ==================== */
typedef struct
{
    DRV_AdcInstance_ten DRV_AdcInstance_en;
    DRV_AdcGroup_ten DRV_AdcGroupIdx_en;
    U8 DRV_NumChannels_u8;
} DRV_AdcConfig_tst;

/* ==================== INITIALIZATION/CONFIGURATION ==================== */
DRV_AdcStatus_ten DRV_ADC_Init_gen(U8 adcPinIdx_argu8);
DRV_AdcStatus_ten DRV_ADC_DeInit_gen(U8 adcPinIdx_argu8);

/* ==================== CONVERSION CONTROL ==================== */
DRV_AdcStatus_ten DRV_ADC_StartConversion_gen(U8 adcPinIdx_argu8);
DRV_AdcStatus_ten DRV_ADC_StopConversion_gen(U8 adcPinIdx_argu8, U32 adcTimeout_argu32);

/* ==================== DATA ACQUISITION ==================== */
DRV_AdcStatus_ten DRV_ADC_ReadGroupBlocking_gen(U8 adcPinIdx_argu8, U16* result_argptru16);
DRV_AdcStatus_ten DRV_ADC_ReadAverageGroupBlocking_gen(U8 adcPinIdx_argu8, U16* result_argptru16);

/* ==================== INFORMATIONAL ==================== */
U16 DRV_ADC_GetResolution_gen(void);

#endif /* DRIVERS_DRV_ADC_H_ */
