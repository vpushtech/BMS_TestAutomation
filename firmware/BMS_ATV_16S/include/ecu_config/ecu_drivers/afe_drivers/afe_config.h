#ifndef INC_BMS_DRIVERS_AFE_BQ76952_H_
#define INC_BMS_DRIVERS_AFE_BQ76952_H_

/* ==================== HEADER FILES ==================== */
#include <bsp_config.h>
#include <vpush_rtos.h>
#include"afe_config.h"
/* ==================== GLOBAL VARIABLES ==================== */
/* ==================== SYSTEM SETTINGS ==================== */


/* ==================== ENUM ==================== */

/* ==================== STATUS AND ERROR ENUMS ==================== */
typedef enum
{
    AFE_STATUS_SUCCESS = 0,
    AFE_STATUS_ERROR   = 1
} AFE_Status_ten;

/* ==================== DATA SIZE ENUMERATIONS ==================== */
typedef enum
{
    AFE_ONEBYTE = 1,
    AFE_TWOBYTE = 2
}AFE_DataSize_ten;
/* ==================== STRUCTURE ==================== */
/* ==================== TYPE DEFINITIONS ==================== */
typedef struct {
    U16 AFE_Address_u16;
    U32 AFE_Value_u32;
    U8 AFE_Data_size_u8;
} AFE_Config_tst;


/* ==================== MACROS ==================== */

/* ==================== FET CONTROL MASKS ==================== */
#define AFE_DSG_OFF_MASK                 (0x01U)     /* Bit 0: DSG_OFF - Discharge FET off mask */
#define AFE_PDSG_OFF_MASK                (0x02U)     /* Bit 1: PDSG_OFF - Parallel discharge FET off mask */
#define AFE_CHG_OFF_MASK                 (0x04U)     /* Bit 2: CHG_OFF - Charge FET off mask */
#define AFE_PCHG_OFF_MASK                (0x08U)     /* Bit 3: PCHG_OFF - Pre-charge FET off mask */


/* ==================== SYSTEM LIMIT SETTINGS ==================== */
#define AFE_MAXCMDRETIES                 (0x0AU)     /* Maximum command retry attempts */

/* ==================== COMMAND DEFINITIONS ==================== */
#define CMD_DIR_SUBCMD_LOW               0x3E        /* Sub command low byte */
#define CMD_DIR_RESP_CHKSUM              0x60        /* Response checksum command */
#define DEVICE_ID                        0x7695      /* Device identification number */

/* ==================== UTILITY MACROS ==================== */
#define LOW_BYTE(data)                   (U8)((data) & 0x00FF)
#define HIGH_BYTE(data)                  (U8)(((data) >> 8) & 0x00FF)
#define SECOND_LOW_BYTE(data)            (U8)(((data) >> 16) & 0x00FF)
#define SECOND_HIGH_BYTE(data)           (U8)(((data) >> 24) & 0x00FF)



/* ==================== AFE SUBCOMMAND ADDRESSES ==================== */
#define AFE_RESET_ADDR                          0x0012
#define AFE_CONFIG_UPDATE_MODE_ADDR             0x0090
#define AFE_CONFIG_UPDATE_EXIT_ADDR             0x0092
#define AFE_I2C_SWAP_ADDR                       0x29BC
#define AFE_CRC_CONFIG_ADDR                     0x29E7

/* ==================== THRESHOLD SETTING ADDRESSES ==================== */
#define AFE_CUV_THRESHOLD_ADDR                  0x9275
#define AFE_CUV_DELAY_ADDR                      0x9276
#define AFE_COV_THRESHOLD_ADDR                  0x9278
#define AFE_COV_DELAY_ADDR                      0x9279
#define AFE_CUV_RECOVERY_HYSTERESIS_ADDR        0x927B
#define AFE_COV_RECOVERY_HYSTERESIS_ADDR        0x927C
#define AFE_COVL_LATCH_LIMIT_ADDR               0x927D
#define AFE_COVL_CNTR_DEC_DELAY_ADDR            0x927E
#define AFE_COVL_RECOVERY_TIME_ADDR             0x927F
#define AFE_OCC_THRESHOLD_ADDR                  0x9280
#define AFE_OCC_DELAY_ADDR                      0x9281
#define AFE_OCD1_THRESHOLD_ADDR                 0x9282
#define AFE_OCD1_DELAY_ADDR                     0x9283
#define AFE_OCD2_THRESHOLD_ADDR                 0x9284
#define AFE_OCD2_DELAY_ADDR                     0x9285
#define AFE_SCD_THRESHOLD_ADDR                  0x9286
#define AFE_SCD_DELAY_ADDR                      0x9287
#define AFE_OCC_RECOVERY_THRESHOLD_ADDR         0x9288
#define AFE_OCD3_THRESHOLD_ADDR                 0x928A
#define AFE_OCD3_DELAY_ADDR                     0x928C
#define AFE_OCD_RECOVERY_THRESHOLD_ADDR         0x928D
#define AFE_OCDL_LATCH_LIMIT_ADDR               0x928F
#define AFE_OCDL_CNTR_DEC_DELAY_ADDR            0x9290
#define AFE_OCDL_RECOVERY_TIME_ADDR             0x9291
#define AFE_OCDL_RECOVERY_THRESHOLD_ADDR        0x9292
#define AFE_SCD_RECOVERY_TIME_ADDR              0x9294
#define AFE_SCDL_LATCH_LIMIT_ADDR               0x9295
#define AFE_SCDL_CNTR_DEC_DELAY_ADDR            0x9296
#define AFE_SCDL_RECOVERY_TIME_ADDR             0x9297
#define AFE_SCDL_RECOVERY_THRESHOLD_ADDR        0x9298
#define AFE_OTC_THRESHOLD_ADDR                  0x929A
#define AFE_OTC_DELAY_ADDR                      0x929B
#define AFE_OTC_RECOVERY_ADDR                   0x929C
#define AFE_OTD_THRESHOLD_ADDR                  0x929D
#define AFE_OTD_DELAY_ADDR                      0x929E
#define AFE_OTD_RECOVERY_ADDR                   0x929F
#define AFE_OTF_THRESHOLD_ADDR                  0x92A0
#define AFE_OTF_DELAY_ADDR                      0x92A1
#define AFE_OTF_RECOVERY_ADDR                   0x92A2
#define AFE_OTINT_THRESHOLD_ADDR                0x92A3
#define AFE_OTINT_DELAY_ADDR                    0x92A4
#define AFE_OTINT_RECOVERY_ADDR                 0x92A5
#define AFE_UTC_THRESHOLD_ADDR                  0x92A6
#define AFE_UTC_DELAY_ADDR                      0x92A7
#define AFE_UTC_RECOVERY_ADDR                   0x92A8
#define AFE_UTD_THRESHOLD_ADDR                  0x92A9
#define AFE_UTD_DELAY_ADDR                      0x92AA
#define AFE_UTD_RECOVERY_ADDR                   0x92AB
#define AFE_UTINT_THRESHOLD_ADDR                0x92AC
#define AFE_UTINT_DELAY_ADDR                    0x92AD
#define AFE_UTINT_RECOVERY_ADDR                 0x92AE
#define AFE_RECOVERY_TIME_ADDR                  0x92AF
#define AFE_OCC_PACK_TOS_DELTA_ADDR             0x92B0
#define AFE_HWD_DELAY_ADDR                      0x92B2
#define AFE_LD_ACTIVE_TIME_ADDR                 0x92B4
#define AFE_LD_RETRY_DELAY_ADDR                 0x92B5
#define AFE_LD_TIMEOUT_ADDR                     0x92B6
#define AFE_PTO_CHARGE_THRESHOLD_ADDR           0x92BA
#define AFE_PTO_DELAY_ADDR                      0x92BC
#define AFE_PTO_RESET_ADDR                      0x92BE

/* ==================== PROTECTION SETTING ADDRESSES ==================== */
#define AFE_PROTECTION_A_ADDR                  0x9261
#define AFE_PROTECTION_B_ADDR                  0x9262
#define AFE_PROTECTION_C_ADDR                  0x9263
#define AFE_CHG_PROTECTION_A_ADDR              0x9265
#define AFE_CHG_PROTECTION_B_ADDR              0x9266
#define AFE_CHG_PROTECTION_C_ADDR              0x9267
#define AFE_DSG_PROTECTION_A_ADDR              0x9269
#define AFE_DSG_PROTECTION_B_ADDR              0x926A
#define AFE_DSG_PROTECTION_C_ADDR              0x926B

/* ==================== OTHER PROTECTION SETTING ADDRESSES ==================== */
#define AFE_CELL_BALANCING_LEVEL_ADDR          0x0084
#define AFE_CC_GAIN_ADDR                       0x91A8
#define AFE_CAPACITY_GAIN_ADDR                 0x91AC
#define AFE_POWER_CONFIG_ADDR                  0x9234
#define AFE_REG12_CONFIG_ADDR                  0x9236
#define AFE_REG0_CONFIG_ADDR                   0x9237
#define AFE_SHUTDOWN_CELL_VOLTAGE_ADDR         0x923F
#define AFE_SHUTDOWN_STACK_VOLTAGE_ADDR        0x9241
#define AFE_SLEEP_CHARGE_VOLTAGE_ADDR          0x924E
#define AFE_PROTECTION_CONFIG_ADDR             0x925F
#define AFE_ALARM_DEFAULT_MASK_ADDR            0x926D
#define AFE_SF_ALERT_MASK_A_ADDR               0x926F
#define AFE_SF_ALERT_MASK_B_ADDR               0x9270
#define AFE_SF_ALERT_MASK_C_ADDR               0x9271
#define AFE_CFETOFF_PIN_CONFIG_ADDR            0x92FA
#define AFE_DFETOFF_PIN_CONFIG_ADDR            0x92FB
#define AFE_ALERT_PIN_CONFIG_ADDR              0x92FC
#define AFE_TS1_PIN_CONFIG_ADDR                0x92FD
#define AFE_TS2_CONFIG_ADDR                    0x92FE
#define AFE_TS3_PIN_CONFIG_ADDR                0x92FF
#define AFE_DCHG_PIN_CONFIG_ADDR               0x9301
#define AFE_DDSG_PIN_CONFIG_ADDR               0x9302
#define AFE_VCELL_MODE_CONFIG_ADDR             0x9304
#define AFE_FET_OPTIONS_ADDR                   0x9308
#define AFE_PRECHARGE_START_VOLTAGE_ADDR       0x930A
#define AFE_PRECHARGE_STOP_VOLTAGE_ADDR        0x930C
#define AFE_PREDISCHARGE_TIMEOUT_ADDR          0x930E
#define AFE_PREDISCHARGE_STOP_DELTA_ADDR       0x930F
#define AFE_DISCHARGE_CURRENT_THRESHOLD_ADDR   0x9310
#define AFE_CHARGE_CURRENT_THRESHOLD_ADDR      0x9312
#define AFE_OPEN_WIRE_CHECK_TIME_ADDR          0x9314
#define AFE_BALANCING_CONFIG_ADDR              0x9335
#define AFE_MIN_CELL_TEMP_ADDR                 0x9336
#define AFE_MAX_CELL_TEMP_ADDR                 0x9337
#define AFE_MAX_INTERNAL_TEMP_ADDR             0x9338
#define AFE_CELL_BALANCE_INTERVAL_ADDR         0x9339
#define AFE_MAX_CELL_BALANCE_ADDR              0x933A
#define AFE_MIN_CELL_BALANCE_CHARGE_ADDR       0x933B
#define AFE_MIN_CELL_BALANCE_DELTA_CHARGE_ADDR 0x933D
#define AFE_STOP_CELL_BALANCE_DELTA_CHARGE_ADDR 0x933E
#define AFE_MIN_CELL_BALANCE_RELAX_ADDR        0x933F
#define AFE_MIN_CELL_BALANCE_DELTA_RELAX_ADDR  0x9341
#define AFE_STOP_CELL_BALANCE_DELTA_RELAX_ADDR 0x9342
#define AFE_MFG_STATUS_INIT_ADDR               0x9343
#define AFE_ENABLED_PF_A_ADDR                  0x92C0
#define AFE_ENABLED_PF_B_ADDR                  0x92C1
#define AFE_ENABLED_PF_C_ADDR                  0x92C2
#define AFE_ENABLED_PF_D_ADDR                  0x92C3
#define AFE_PF_ALERT_MASK_A_ADDR               0x92C4
#define AFE_PF_ALERT_MASK_B_ADDR               0x92C5
#define AFE_PF_ALERT_MASK_C_ADDR               0x92C6
#define AFE_PF_ALERT_MASK_D_ADDR               0x92C7
/* ==================== AFE INITIALIZATION ADDRESSES ==================== */
#define AFE_RESET_VALUE                         0x0000
#define AFE_I2C_SWAP_VALUE                      0x0000
#define AFE_CRC_CONFIG_VALUE                    0x0000
#define AFE_CONFIG_UPDATE_MODE_VALUE            0x0000
#define AFE_CONFIG_UPDATE_EXIT_VALUE            0x0000


/* ==================== POWER MODE ADDRESSES ==================== */
#define AFE_EXIT_DEEP_SLEEP_CMD_ADDR   0x000E
#define AFE_DEEP_SLEEP_CMD_ADDR        0x000F
#define AFE_SHUTDOWN_CMD_ADDR          0x0010
#define AFE_PDSG_TEST_CMD_ADDR         0x001C
#define AFE_PCHG_TEST_CMD_ADDR         0x001E
#define AFE_CHG_TEST_CMD_ADDR          0x001F
#define AFE_DSG_TEST_CMD_ADDR          0x0020
#define AFE_FET_EN_CMD_ADDR            0x0022
#define AFE_SLEEP_CMD_ADDR             0x0099

/* ==================== FET CONTROL ADDRESSES ==================== */
#define AFE_DSG_PDSG_OFF_CMD_ADDR      0x0093
#define AFE_CHG_PCHG_OFF_CMD_ADDR      0x0094
#define AFE_ALL_FETS_OFF_CMD_ADDR      0x0095
#define AFE_ALL_FETS_ON_CMD_ADDR       0x0096
#define AFE_FET_CONTROL_CMD_ADDR       0x0097
/* ==================== FUNCTION DECLARATION ==================== */

/* ==================== INITIALIZATION FUNCTIONS ==================== */
AFE_Status_ten AFE_init_mv(void);
/* ==================== UTILITY FUNCTIONS ==================== */
U8 AFE_Checksum_gu8(U8 *data_argptru8, U8 len_argu8);
AFE_Status_ten AFE_WriteReg_gen(U8 *regdata_argptru8, U8 count_argsize);
/* ==================== DIRECT AND SUBCOMMAND FUNCTIONS ==================== */
AFE_Status_ten AFE_SubCommand_WriteOperation_gv(U16 SubCommand_argu16, U32 Subcommand_Data_u32, U8 SubcommandSize_u8);

#endif /* INC_BMS_DRIVERS_AFE_BQ76952_H_ */
