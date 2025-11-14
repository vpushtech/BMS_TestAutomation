/*
 * AFE_Board_Config.h
 *
 *  Created on: 30-Aug-2025
 *      Author: RushikeshNitinKamble
 */

#ifndef INC_BMS_BOARD_CONFIG_AFE_BOARD_CONFIG_H_
#define INC_BMS_BOARD_CONFIG_AFE_BOARD_CONFIG_H_

#include <afe_config.h>

/* ==================== AFE CONFIGURATION SETTINGS ==================== */
/* ==================== VOLTAGE PROTECTION SETTINGS ==================== */
/* Cell Under voltage Protection (CUV) */
#define AFE_CUV_THR_VALUE           2500    /* Threshold in mV (1012-4048 range) */
#define AFE_CUV_THR_REG_VALUE       (U8)((AFE_CUV_THR_VALUE * 10) / 506)
#define AFE_CUV_DELAY_VALUE         198.0   /* Delay in ms (10-6761 range) */
#define AFE_CUV_DELAY_REG_VALUE     (I16)(((AFE_CUV_DELAY_VALUE * 10) / 33) - 2)
#define AFE_CUV_HYS_VALUE           101.2   /* Hysteresis in mV (101.2-1012 range) */
#define AFE_CUV_HYS_REG_VALUE       (uint8_t)((AFE_CUV_HYS_VALUE * 10) / 506)

/* Cell Over voltage Protection (COV) */
#define AFE_COV_THR_VALUE           3650    /* Threshold in mV (1012-5566 range) */
#define AFE_COV_THR_REG_VALUE       (U8)((AFE_COV_THR_VALUE * 10) / 506)
#define AFE_COV_DELAY_VALUE         198.0   /* Delay in ms (10-6761 range) */
#define AFE_COV_DELAY_REG_VALUE     (U16)(((AFE_COV_DELAY_VALUE * 10) / 33) - 2)
#define AFE_COV_HYS_VALUE           101.2   /* Hysteresis in mV (101.2-1012 range) */
#define AFE_COV_HYS_REG_VALUE       (U8)((AFE_COV_HYS_VALUE * 10) / 506)

/* COV Latch Settings */
#define AFE_COVL_LMTCNT_VALUE       2       /* Latch limit count (0-255) */
#define AFE_COVL_LMTDECDEL_VALUE    10      /* Count decrement delay in sec */
#define AFE_COVL_LMTRECOVERY_VALUE  15      /* Recovery time in sec */

/* ==================== CURRENT PROTECTION SETTINGS ==================== */
/* Charge Over current (OCC) */
#define AFE_OCC_THR_VALUE           40      /* Threshold in 2mV units (2-62 range) */
#define AFE_OCC_THR_REG_VALUE       (U8)(AFE_OCC_THR_VALUE / 2)
#define AFE_OCC_DELAY_VALUE         198.0   /* Delay in ms (10-127 range) */
#define AFE_OCC_DELAY_REG_VALUE     (U8)(((AFE_OCC_DELAY_VALUE * 10) / 33) - 2)
#define AFE_OCC_HYS_VALUE           10000   /* Hysteresis in mA */
#define AFE_OCC_HYS_REG_VALUE       (I16)(AFE_OCC_HYS_VALUE)
#define AFE_OCC_PACKTOS_VALUE       200     /* Voltage delta in 10mV units */
#define AFE_OCC_PACKTOS_REG_VALUE   (U16)(AFE_OCC_PACKTOS_VALUE)

/* Discharge Over current Level 1 (OCD1) */
#define AFE_OCD1_THR_VALUE          150     /* Threshold in 2mV units (2-100 range) */
#define AFE_OCD1_THR_REG_VALUE      (U8)(AFE_OCD1_THR_VALUE / 2)
#define AFE_OCD1_DELAY_VALUE        396     /* Delay in ms (10-127 range) */
#define AFE_OCD1_DELAY_REG_VALUE    (U8)(((AFE_OCD1_DELAY_VALUE * 10) / 33) - 2)

/* Discharge Over current Level 2 (OCD2) */
#define AFE_OCD2_THR_VALUE          250     /* Threshold in 2mV units (2-100 range) */
#define AFE_OCD2_THR_REG_VALUE      (U8)(AFE_OCD2_THR_VALUE / 2)
#define AFE_OCD2_DELAY_VALUE        297.0   /* Delay in ms (10-127 range) */
#define AFE_OCD2_DELAY_REG_VALUE    (U8)(((AFE_OCD2_DELAY_VALUE * 10) / 33) - 2)

/* Short Circuit Protection (SCD) */
#define AFE_SCD_THR_VALUE           15      /* Threshold (0-15 = 10-500mV) */
#define AFE_SCD_THR_REG_VALUE       (U8)(AFE_SCD_THR_VALUE)
#define AFE_SCD_DELAY_VALUE         1       /* Delay in 15μs units (1-31) */
#define AFE_SCD_DELAY_REG_VALUE     (U8)(AFE_SCD_DELAY_VALUE)
#define AFE_SCD_HYS_VALUE           30      /* Recovery time in sec */
#define AFE_SCD_HYS_REG_VALUE       (uint8_t)(AFE_SCD_HYS_VALUE)

/* Discharge Over current Level 3 (OCD3) */
#define AFE_OCD3_THR_VALUE          -32000  /* Threshold in mA */
#define AFE_OCD3_THR_REG_VALUE      (I16)(AFE_OCD3_THR_VALUE)
#define AFE_OCD3_DELAY_VALUE        255     /* Delay in sec */
#define AFE_OCD3_DELAY_REG_VALUE    (U8)(AFE_OCD3_DELAY_VALUE)

/* OCD Common Settings */
#define AFE_OCD_HYS_VALUE           -1000   /* Hysteresis in mA */
#define AFE_OCD_HYS_REG_VALUE       (I16)(AFE_OCD_HYS_VALUE)

/* OCDL Common Settings */
#define AFE_OCDL_LMTCNT_VALUE       10      /* Latch limit count */
#define AFE_OCDL_LMTDECDEL_VALUE    20      /* Count decrement delay in sec */
#define AFE_OCDL_LMTRECOVERY_VALUE  5       /* Recovery time in sec */
#define AFE_OCDL_HYS_VALUE          -1000   /* Hysteresis in mA */
#define AFE_OCDL_HYS_REG_VALUE      (I16)(AFE_OCDL_HYS_VALUE)

/* SCDL Latch Settings */
#define AFE_SCDL_LMTCNT_VALUE       10      /* Latch limit count */
#define AFE_SCDL_LMTDECDEL_VALUE    10      /* Count decrement delay in sec */
#define AFE_SCDL_LMTRECOVERY_VALUE  15      /* Recovery time in sec */
#define AFE_SCDL_HYS_VALUE          -1000   /* Hysteresis in mA */
#define AFE_SCDL_HYS_REG_VALUE      (I16)(AFE_SCDL_HYS_VALUE)

/* ==================== TEMPERATURE PROTECTION SETTINGS ==================== */
/* Charge Overtemperature (OTC) */
#define AFE_OTC_THR_VALUE           60      /* Threshold in °C (-40-120 range) */
#define AFE_OTC_THR_REG_VALUE       (I8)(AFE_OTC_THR_VALUE)
#define AFE_OTC_DELAY_VALUE         5       /* Delay in sec */
#define AFE_OTC_DELAY_REG_VALUE     (U8)(AFE_OTC_DELAY_VALUE)
#define AFE_OTC_HYS_VALUE           (AFE_OTC_THR_VALUE - 5) /* Hysteresis */
#define AFE_OTC_HYS_REG_VALUE       (int8_t)(AFE_OTC_HYS_VALUE)

/* Discharge Overtemperature (OTD) */
#define AFE_OTD_THR_VALUE           60      /* Threshold in °C (-40-120 range) */
#define AFE_OTD_THR_REG_VALUE       (I8)(AFE_OTD_THR_VALUE)
#define AFE_OTD_DELAY_VALUE         5       /* Delay in sec */
#define AFE_OTD_DELAY_REG_VALUE     (U8)(AFE_OTD_DELAY_VALUE)
#define AFE_OTD_HYS_VALUE           (AFE_OTD_THR_VALUE - 10) /* Hysteresis */
#define AFE_OTD_HYS_REG_VALUE       (I8)(AFE_OTD_HYS_VALUE)

/* FET Overtemperature (OTF) */
#define AFE_OTF_THR_VALUE           60      /* Threshold in °C (0-150 range) */
#define AFE_OTF_THR_REG_VALUE       (U8)(AFE_OTF_THR_VALUE)
#define AFE_OTF_DELAY_VALUE         5       /* Delay in sec */
#define AFE_OTF_DELAY_REG_VALUE     (U8)(AFE_OTF_DELAY_VALUE)
#define AFE_OTF_HYS_VALUE           (AFE_OTF_THR_VALUE - 15) /* Hysteresis */
#define AFE_OTF_HYS_REG_VALUE       (I8)(AFE_OTF_HYS_VALUE)

/* Internal Over temperature (OTINT) */
#define AFE_OTINT_THR_VALUE         60      /* Threshold in °C (-40-120 range) */
#define AFE_OTINT_THR_REG_VALUE     (U8)(AFE_OTINT_THR_VALUE)
#define AFE_OTINT_DELAY_VALUE       5       /* Delay in sec */
#define AFE_OTINT_DELAY_REG_VALUE   (U8)(AFE_OTINT_DELAY_VALUE)
#define AFE_OTINT_HYS_VALUE         (AFE_OTINT_THR_VALUE - 10) /* Hysteresis */
#define AFE_OTINT_HYS_REG_VALUE     (I8)(AFE_OTINT_HYS_VALUE)

/* Charge Under temperature (UTC) */
#define AFE_UTC_THR_VALUE           0       /* Threshold in °C (-40-120 range) */
#define AFE_UTC_THR_REG_VALUE       (U8)(AFE_UTC_THR_VALUE)
#define AFE_UTC_DELAY_VALUE         5       /* Delay in sec */
#define AFE_UTC_DELAY_REG_VALUE     (U8)(AFE_UTC_DELAY_VALUE)
#define AFE_UTC_HYS_VALUE           5       /* Hysteresis in °C */
#define AFE_UTC_HYS_REG_VALUE       (I8)(AFE_UTC_HYS_VALUE)

/* Discharge Under temperature (UTD) */
#define AFE_UTD_THR_VALUE           -10     /* Threshold in °C (-40-120 range) */
#define AFE_UTD_THR_REG_VALUE       (U8)(AFE_UTD_THR_VALUE)
#define AFE_UTD_DELAY_VALUE         5       /* Delay in sec */
#define AFE_UTD_DELAY_REG_VALUE     (U8)(AFE_UTD_DELAY_VALUE)
#define AFE_UTD_HYS_VALUE           0       /* Hysteresis in °C */
#define AFE_UTD_HYS_REG_VALUE       (I8)(AFE_UTD_HYS_VALUE)

/* Internal Under temperature (UTINT) */
#define AFE_UTINT_THR_VALUE         -10     /* Threshold in °C (-40-120 range) */
#define AFE_UTINT_THR_REG_VALUE     (I8)(AFE_UTINT_THR_VALUE)
#define AFE_UTINT_DELAY_VALUE       5       /* Delay in sec */
#define AFE_UTINT_DELAY_REG_VALUE   (U8)(AFE_UTINT_DELAY_VALUE)
#define AFE_UTINT_HYS_VALUE         (AFE_UTINT_THR_VALUE + 5) /* Hysteresis */
#define AFE_UTINT_HYS_REG_VALUE     (I8)(AFE_UTINT_HYS_VALUE)

/* ==================== SYSTEM TIMING SETTINGS ==================== */
#define AFE_PROT_RECOVERY_TIME_VALUE        1       /* Protection recovery in sec */
#define AFE_PROT_RECOVERY_TIME_REG_VALUE    (U8)(AFE_PROT_RECOVERY_TIME_VALUE)
#define AFE_PROT_WDG_TIME_VALUE             60      /* Watchdog timeout in sec */
#define AFE_PROT_WDG_TIME_REG_VALUE         (U16)(AFE_PROT_WDG_TIME_VALUE)
#define AFE_PROT_LD_TIME_VALUE              0       /* Load detect time in sec */
#define AFE_PROT_LD_TIME_REG_VALUE          (U8)(AFE_PROT_LD_TIME_VALUE)
#define AFE_PROT_LDRETRY_TIME_VALUE         255     /* Load retry time in sec */
#define AFE_PROT_LDRETRY_TIME_REG_VALUE     (U8)(AFE_PROT_LDRETRY_TIME_VALUE)
#define AFE_PROT_LD_TIMEOUT_VALUE           0       /* Load timeout in hours */
#define AFE_PROT_LD_TIMEOUT_REG_VALUE       (U16)(AFE_PROT_LD_TIMEOUT_VALUE)

/* ==================== PRECHARGE TIMEOUT SETTINGS ==================== */
#define AFE_PTO_THR_VALUE           250     /* Threshold in mA */
#define AFE_PTO_THR_REG_VALUE       (I16)(AFE_PTO_THR_VALUE)
#define AFE_PTO_DELAY_VALUE         60      /* Delay in sec */
#define AFE_PTO_DELAY_REG_VALUE     (U16)(AFE_PTO_DELAY_VALUE)
#define AFE_PTO_RESET_VALUE         2       /* Reset in Ah */
#define AFE_PTO_RESET_REG_VALUE     (U16)(AFE_PTO_RESET_VALUE)
/* ==================== CELL VOLTAGE SETTINGS ==================== */
#define AFE_CELL_MODE_CONFIG                    0xFFFF  /* 16-cell mode */
#define AFE_CB_MAX_CELL_VOLTAGE                 0x0E10  /* 3600mV - Max cell voltage for balancing */
#define AFE_CB_MIN_CELL_VOLTAGE_CHG             0x0BB8  /* 3000mV - Min cell voltage for charge balancing */
#define AFE_CB_MIN_CELL_VOLTAGE_RELAX           0x0BB8  /* 3000mV - Min cell voltage for relax balancing */
#define AFE_SHUTDOWN_STACK_CELL_VOLATGE         0x05DC  /* 1500mV - Stack voltage cell shutdown threshold */
#define AFE_SHUTDOWN_STACK_VOLATGE              0x05DC  /* 1500mV - Stack voltage shutdown threshold */
#define AFE_SLEEP_CHG_VOLTAGE                   0x0708  /* 1800mV - Sleep charge voltage threshold */
#define AFE_FET_PROTECTION_VOLTAGE              0x0000  /* 0mV - FET protection voltage threshold */

/* ==================== TEMPERATURE SETTINGS ==================== */
#define AFE_MIN_CELL_TEMP                       0x00    /* 0°C - Minimum allowed cell temperature */
#define AFE_MAX_CELL_TEMP                       0x3C    /* 60°C - Maximum allowed cell temperature */
#define AFE_MAX_TEMPERATURE                     0x46    /* 70°C - Absolute maximum temperature limit */

/* ==================== CURRENT SETTINGS ==================== */
#define AFE_CHG_CURRENT_THRESHOLD               0x0064  /* 100mA - Charge current threshold */
#define AFE_DHG_CURRENT_THRESHOLD               0x0064  /* 100mA - Discharge current threshold */

/* ==================== CELL BALANCING SETTINGS ==================== */
#define AFE_MAX_CB_NUMBER                       0x10    /* 16 cells - Maximum number of cells to balance */
#define AFE_CB_MIN_DELTA                        0x32    /* 50mV - Minimum voltage delta to start balancing */
#define AFE_CB_STOP_DELTA                       0x0A    /* 10mV - Voltage delta to stop balancing */
#define AFE_CB_TIME_INTERVAL                    0xFF    /* 255 seconds - Balancing time interval */

/* ==================== PRECHARGE/PREDISCHARGE SETTINGS ==================== */
#define AFE_PRECHARGE_START_VOLTAGE             0x0AF0  /* 2800mV - Precharge start voltage threshold */
#define AFE_PRECHARGE_STOP_VOLTAGE              0x092E  /* 2350mV - Precharge stop voltage threshold */
#define AFE_PRE_DHG_TIMEOUT                     0x32    /* 500ms - Predischarge timeout value */
#define AFE_PREDISCHARGE_STOP_DELTA_VALUE       0x0000  /* 0mV - Predischarge stop delta */

/* ==================== PROTECTION SETTINGS ==================== */
#define AFE_PROTECTION_A_VALUE                  0x00FC  /* Protection Set A configuration */
#define AFE_CHG_PROTECTION_A_VALUE              0x0092  /* CHG Protection Set A configuration */
#define AFE_DSG_PROTECTION_A_VALUE              0x00E4  /* DSG Protection Set A configuration */
#define AFE_PROTECTION_B_VALUE                  0x0044  /* Protection Set B configuration */
#define AFE_CHG_PROTECTION_B_VALUE              0x0044  /* CHG Protection Set B configuration */
#define AFE_DSG_PROTECTION_B_VALUE              0x0044  /* DSG Protection Set B configuration */
#define AFE_PROTECTION_C_VALUE                  0x0076  /* Protection Set C configuration */
#define AFE_CHG_PROTECTION_C_VALUE              0x00D5  /* CHG Protection Set C configuration */
#define AFE_DSG_PROTECTION_C_VALUE              0x0076  /* DSG Protection Set C configuration */

/* ==================== ALARM AND ALERT SETTINGS ==================== */
#define AFE_ALARM_DEFAULT_MASK_VALUE            0xD086  /* Default alarm mask configuration */
#define AFE_PF_ALERT_MASK_VALUE                 0x0000  /* Permanent failure alert mask */
#define AFE_ENABLED_PF_VALUE                    0x0000  /* Enabled permanent failure settings */
#define AFE_SF_ALERT_MASK_A_VALUE               0x00FC  /* Safety fault alert mask A */
#define AFE_SF_ALERT_MASK_B_VALUE               0x00F7  /* Safety fault alert mask B */
#define AFE_SF_ALERT_MASK_C_VALUE               0x00F4  /* Safety fault alert mask C */

/* ==================== PIN CONFIGURATION SETTINGS ==================== */
#define AFE_DDSG_PIN_CONFIG_VALUE               0x0002  /* DDSG pin configuration */
#define AFE_DCHG_PIN_CONFIG_VALUE               0x0002  /* DCHG pin configuration */
#define AFE_CFETOFF_PIN_CONFIG_VALUE            0x0007  /* CFETOFF pin configuration */
#define AFE_DFETOFF_PIN_CONFIG_VALUE            0x0007  /* DFETOFF pin configuration */
#define AFE_TS3_PIN_CONFIG_VALUE                0x0007  /* TS3 pin configuration */
#define AFE_TS1_PIN_CONFIG_VALUE                0x0007  /* TS1 pin configuration */
#define AFE_TS2_CONFIG_VALUE                    0x000B  /* TS2 pin configuration */
#define AFE_ALERT_PIN_CONFIG_VALUE              0x002A  /* ALERT pin configuration */

/* ==================== SYSTEM CONFIGURATION SETTINGS ==================== */
#define AFE_FET_OPTIONS_VALUE                   0x003F  /* FET options configuration */
#define AFE_MFG_STATUS_INIT_VALUE               0x0010  /* Manufacturing status initialization */
#define AFE_BALANCING_CONFIG_VALUE              0x000B  /* Balancing configuration */
#define AFE_OPEN_WIRE_CHECK_TIME_VALUE          0x0000  /* Open wire check time (disabled) */

/* ==================== INITIALIZATION SETTINGS ==================== */
#define AFE_POWER_CONFIG_VALUE                  0x2EB2  /* Power configuration */
#define AFE_REG0_CONFIG_VALUE                   0x0001  /* REG0 configuration */
#define AFE_REG12_CONFIG_VALUE                  0x00DD  /* REG12 configuration */

#endif /* INC_BMS_BOARD_CONFIG_AFE_BOARD_CONFIG_H_ */
