/*
 * drv_adc.c
 *
 *  Description     : ADC Driver
 *  Author          : Rushikesh
 *  Created On      : 08-Jul-2025
 *  Version         : 2.0
 *  Modification History:
 *  Date        Author      Description
 *  ----------------------------------------------------------------------------
 *  18-Jul-2025 RUSHIKESH   ADC Driver Architecture Imlimentation
 *  31-Jul_2025 RUSHIKESH	ADC Driver Testing completed (Tested with potentiameter)
 *  11-Aug-2025 RUSHIKESH   Guidlines Followed the naming Architecture Implimentation
 ******************************************************************************/
/* ==================== INCLUDE FILES ==================== */
#include <bsp_config.h>
#include "drv_adc.h"

/* ==================== STATIC VARIABLES ==================== */
static const adc_instance_t* DRV_AdcInstance_arrst[BSP_ADC_MAX_PIN] = {&adc_pal1_instance};
static const adc_config_t* DRV_AdcConfig_arrst[BSP_ADC_MAX_PIN] = {&adc_pal1_InitConfig0};

/* ==================== GLOBAL VARIABLES ==================== */
extern DRV_AdcConfig_tst DRV_AdcConfigTable_gst[BSP_ADC_MAX_PIN];

/* ==================== STATIC VOLATILE VARIABLES ==================== */
static volatile BIN DRV_ConversionComplete_mb = false;
static volatile U32 DRV_ResultLastOffset = 0;

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_Adc_pal1_callbackGroup0Channel0
*   Description   : ADC conversion complete callback function
*   Parameters    : callbackInfo - ADC callback info structure
*                   userData - User data pointer (unused)
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void DRV_Adc_pal1_callbackGroup0Channel0(const adc_callback_info_t *callbackInfo, void *userData)
{
    (void)userData;
    DRV_ConversionComplete_mb = true;
    DRV_ResultLastOffset = callbackInfo->resultBufferTail;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_ADC_Init_gen
*   Description   : Initializes ADC for specified pin
*   Parameters    : adcPinIdx_argu8 - ADC pin index
*   Return Value  : DRV_AdcStatus_ten - Status of initialization
*  --------------------------------------------------------------------------- */
DRV_AdcStatus_ten DRV_ADC_Init_gen(U8 adcPinIdx_argu8)
{
    DRV_AdcInstance_ten adcInstance_en = DRV_AdcConfigTable_gst[adcPinIdx_argu8].DRV_AdcInstance_en;
    status_t status = ADC_Init(DRV_AdcInstance_arrst[adcInstance_en],
                             DRV_AdcConfig_arrst[adcInstance_en]);
    if (status != STATUS_SUCCESS)
    {
        return DRV_ADC_STATUS_ERROR;
    }
    return DRV_ADC_STATUS_OK;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_ADC_DeInit_gen
*   Description   : Deinitializes ADC for specified pin
*   Parameters    : adcPinIdx_argu8 - ADC pin index
*   Return Value  : DRV_AdcStatus_ten - Status of deinitialization
*  --------------------------------------------------------------------------- */
DRV_AdcStatus_ten DRV_ADC_DeInit_gen(U8 adcPinIdx_argu8)
{
    DRV_AdcInstance_ten adcInstance_en = DRV_AdcConfigTable_gst[adcPinIdx_argu8].DRV_AdcInstance_en;
    status_t status = ADC_Deinit(DRV_AdcInstance_arrst[adcInstance_en]);
    if (status != STATUS_SUCCESS)
    {
        return DRV_ADC_STATUS_ERROR;
    }
    return DRV_ADC_STATUS_OK;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_ADC_StartConversion_gen
*   Description   : Starts ADC conversion for specified pin
*   Parameters    : adcPinIdx_argu8 - ADC pin index
*   Return Value  : DRV_AdcStatus_ten - Status of conversion start
*  --------------------------------------------------------------------------- */
DRV_AdcStatus_ten DRV_ADC_StartConversion_gen(U8 adcPinIdx_argu8)
{
    DRV_AdcInstance_ten adcInstance_en = DRV_AdcConfigTable_gst[adcPinIdx_argu8].DRV_AdcInstance_en;
    status_t status = ADC_StartGroupConversion(DRV_AdcInstance_arrst[adcInstance_en],
                                             DRV_AdcConfigTable_gst[adcPinIdx_argu8].DRV_AdcGroupIdx_en);
    if (status != STATUS_SUCCESS)
    {
        return DRV_ADC_STATUS_ERROR;
    }
    return DRV_ADC_STATUS_OK;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_ADC_StopConversion_gen
*   Description   : Stops ADC conversion for specified pin
*   Parameters    : adcPinIdx_argu8 - ADC pin index
*                   adcTimeout_argu32 - Timeout value
*   Return Value  : DRV_AdcStatus_ten - Status of conversion stop
*  --------------------------------------------------------------------------- */
DRV_AdcStatus_ten DRV_ADC_StopConversion_gen(U8 adcPinIdx_argu8, U32 adcTimeout_argu32)
{
    DRV_AdcInstance_ten adcInstance_en = DRV_AdcConfigTable_gst[adcPinIdx_argu8].DRV_AdcInstance_en;
    status_t status = ADC_StopGroupConversion(DRV_AdcInstance_arrst[adcInstance_en],
                                            DRV_AdcConfigTable_gst[adcPinIdx_argu8].DRV_AdcGroupIdx_en,
                                            adcTimeout_argu32);
    if (status != STATUS_SUCCESS)
    {
        return DRV_ADC_STATUS_ERROR;
    }
    return DRV_ADC_STATUS_OK;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_ADC_ReadGroupBlocking_gen
*   Description   : Reads ADC results in blocking mode
*   Parameters    : adcPinIdx_argu8 - ADC pin index
*                   result_argptru16 - Pointer to store results
*   Return Value  : DRV_AdcStatus_ten - Status of read operation
*  --------------------------------------------------------------------------- */
DRV_AdcStatus_ten DRV_ADC_ReadGroupBlocking_gen(U8 adcPinIdx_argu8, U16* result_argptru16)
{
    if (DRV_ConversionComplete_mb)
    {
        for (uint8_t i = 0; i < DRV_AdcConfigTable_gst[adcPinIdx_argu8].DRV_NumChannels_u8; i++)
        {
            result_argptru16[i] = adc_pal1_Results00[DRV_ResultLastOffset - DRV_AdcConfigTable_gst[adcPinIdx_argu8].DRV_NumChannels_u8 + 1 + i];
        }
    }
    return DRV_ADC_STATUS_OK;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_ADC_ReadAverageGroupBlocking_gen
*   Description   : Reads and averages ADC results
*   Parameters    : adcPinIdx_argu8 - ADC pin index
*                   result_argptru16 - Pointer to store averaged result
*   Return Value  : DRV_AdcStatus_ten - Status of read operation
*  --------------------------------------------------------------------------- */
DRV_AdcStatus_ten DRV_ADC_ReadAverageGroupBlocking_gen(U8 adcPinIdx_argu8, U16* result_argptru16)
{
    const U8 numChannels_u8 = DRV_AdcConfigTable_gst[adcPinIdx_argu8].DRV_NumChannels_u8;
    U16 results_arru16[4] = {0};
    U32 DRV_sum_u32 = 0;

    DRV_AdcStatus_ten status_en = DRV_ADC_ReadGroupBlocking_gen(adcPinIdx_argu8, results_arru16);
    if (status_en != DRV_ADC_STATUS_OK)
    {
        return status_en;
    }

    for (U8 i = 0; i < numChannels_u8; i++)
    {
        DRV_sum_u32 += results_arru16[i];
    }
    *result_argptru16 = (U16)(DRV_sum_u32 / numChannels_u8);
    DRV_ConversionComplete_mb = false;
    return status_en;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : DRV_ADC_GetResolution_gen
*   Description   : Gets ADC resolution in bits
*   Parameters    : None
*   Return Value  : U16 - ADC resolution (8, 10 or 12 bits)
*  --------------------------------------------------------------------------- */
U16 DRV_ADC_GetResolution_gen(void)
{
    U16 DRV_adcMax_u16;
    adc_resolution_t resolution_en = ((extension_adc_s32k1xx_t *)(adc_pal1_InitConfig0.extension))->resolution;

    DRV_adcMax_u16 = (U16)(1 << ((resolution_en == ADC_RESOLUTION_8BIT) ? 8 :
                            (resolution_en == ADC_RESOLUTION_10BIT) ? 10 : 12));
    return DRV_adcMax_u16;
}
