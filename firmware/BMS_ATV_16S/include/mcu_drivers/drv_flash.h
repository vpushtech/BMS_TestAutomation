#ifndef DRIVERS_DRV_FLASH_H_
#define DRIVERS_DRV_FLASH_H_

/*******************************************************************************
 *  Description     : Flash Driver
 *  Author          : Rushikesh
 *  Created On      : 08-Jul-2025
 *  Version         : 2.0
 *  Modification History:
 *  Date        Author      Description
 *  ----------------------------------------------------------------------------
 *  18-Jul-2025 RUSHIKESH   Flash Driver Architecture Implementation
 *  31-Jul-2025 RUSHIKESH   Flash Driver Testing completed
 *  11-Aug-2025 RUSHIKESH   Guidelines Followed the naming Architecture Implementation
 ******************************************************************************/

/* ==================== INCLUDE ==================== */
#include <common.h>
#include "flash_driver.h"
#include "bsp_config.h"

/* ==================== DEFINE ==================== */
#define DRV_FLASH_TIMEOUT_MS               (1000U)
#define DRV_FLASH_SECTOR_SIZE              FEATURE_FLS_PF_BLOCK_SECTOR_SIZE
#define DRV_FLASH_PHRASE_SIZE              (8U)
/* ==================== ENUMS ==================== */
typedef enum
{
    DRV_FLASH_SUCCESS,
    DRV_FLASH_FAILED,
    DRV_FLASH_WRITE_ERROR,
    DRV_FLASH_ERASE_ERROR,
    DRV_FLASH_INVALID_ADDRESS,
    DRV_FLASH_INVALID_SIZE,
    DRV_FLASH_INVALID_ALIGNMENT
} DRV_flashStatus_ten;

/* ==================== INITIALIZATION/CONFIGURATION ==================== */
DRV_flashStatus_ten DRV_FLASH_Init_gen(void);

/* ==================== ERASE OPERATIONS ==================== */
DRV_flashStatus_ten DRV_FLASH_EraseSector_gen(U32 address_argu32, U32 size_argu32);

/* ==================== PROGRAM/READ OPERATIONS ==================== */
DRV_flashStatus_ten DRV_FLASH_WriteBlock_gen(U32 address_argu32, const U8 *data_argptru8, U32 size_argu32);
DRV_flashStatus_ten DRV_FLASH_ReadBlock_gen(U32 address_argu32, U8 *data_argptru8, U32 size_argu32);

/* ==================== INTERRUPT HANDLERS ==================== */
void CCIF_Handler(void);
void CCIF_Callback(void);

#endif /* DRIVERS_DRV_FLASH_H_ */
