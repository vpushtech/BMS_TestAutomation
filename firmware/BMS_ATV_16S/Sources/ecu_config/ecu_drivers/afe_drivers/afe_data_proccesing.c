/*
 * afe_dataproccesing.c
 *
 *  Created on: 22-Oct-2025
 *      Author: RushikeshNitinKamble
 */

#include"afe_data_proccessing.h"
/* ==================== STRUCTURE AND ENUM DECLARATION ==================== */
AFE_InputeMeasument_tst AFE_InputeMeasument_St={0};
U16 deviceID_mu16 = 0x0000;
AFE_Status_tst AFE_Status_st={0};
/* ==================== STATIC  Function ==================== */
static void AFE_DATAStatus5_gv(U8* data);
/* ==================== FUNCTIONS  ==================== */
/* ==================== DEVICE ID & CONFIGURATION ==================== */
/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : AFE_DeviceID_mu16
*   Description   : Reads the device ID of the (AFE) by by sending 0x0001 command
*   Parameters    : None
*   Return Value  : U16 - 16-bit device ID formed by combining the two read bytes
*  ---------------------------------------------------------------------------*/
AFE_Status_ten AFE_DeviceID_mu16()
{
	AFE_Status_ten status_en = AFE_STATUS_SUCCESS;
	U8 afeIDByte_arru8[2]={0,0};
	status_en = AFE_Direct_WriteOperation_gv(0x0001, 0x0000, 0x03);
    BSP_TimerDelay(50, DRV_DELAY_UNITS_MILLISECOND);
	status_en = AFE_Direct_ReadOperation_gv(0x40, afeIDByte_arru8, 2);
	BSP_TimerDelay(50, DRV_DELAY_UNITS_MILLISECOND);
    deviceID_mu16=(afeIDByte_arru8[1] * (256) + afeIDByte_arru8[0]);
    status_en = (deviceID_mu16 == DEVICE_ID) ? AFE_STATUS_SUCCESS : AFE_STATUS_ERROR;
	return status_en;
}
/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : AFE_ProcessDAStatus5_gv
*   Description   : Processes DASTATUS5 system measurements data including voltages,
*                   temperatures, and current measurements from 32-byte data buffer
*   Parameters    : data - Pointer to 32-byte DASTATUS5 data buffer
*   Return Value  : None
*   Note          : Processes 16-bit values: VREG18, VSS, Max/Min Cell Voltage, Pack Voltage, CC1 Current
*                   Processes temperatures: Cell, FET, Max/Min/Avg Cell Temperature with conversion
*                   Processes 32-bit CC2/CC3 ADC counts and calculates currents
*                   Calculates Cell Voltage Difference from Max/Min values
*  ---------------------------------------------------------------------------*/
static void AFE_DATAStatus5_gv(U8* data) {
	struct {
		U8 offset;
		void* dest;
		U8 is_signed;
	} values_16bit[] = { { 0,&AFE_InputeMeasument_St.AFE_Voltage_St.AFE_VREG18_u16, 0 },
			{ 2,&AFE_InputeMeasument_St.AFE_Voltage_St.AFE_VSS_u16, 0 },
			{ 4,&AFE_InputeMeasument_St.AFE_Voltage_St.AFE_MaxCellVoltage_u16, 0 },
			{ 6, &AFE_InputeMeasument_St.AFE_Voltage_St.AFE_MinCellVoltage_u16,0 },
			{ 8,&AFE_InputeMeasument_St.AFE_Voltage_St.AFE_Pack_Voltage_u16,0 },
			{ 22,&AFE_InputeMeasument_St.AFE_Current_st.AFE_CC1_Current_i16,1 } };
	for (U8 i = 0; i < 6; i++) {
		U16 raw_value = (U16) ((data[values_16bit[i].offset + 1] << 8)| data[values_16bit[i].offset]);

		if (values_16bit[i].is_signed) {
			*((I16*) values_16bit[i].dest) = (I16) raw_value;
		} else {
			*((U16*) values_16bit[i].dest) = raw_value;
		}
	}
	struct {
		U8 offset;
		I16* dest;
	} temp_values[] =
			{
					{ 10,&AFE_InputeMeasument_St.AFE_Temperature_St.AFE_CellTemperature_i16 },
					{ 12,&AFE_InputeMeasument_St.AFE_Temperature_St.AFE_FET_Temperature_i16 },
					{ 14,&AFE_InputeMeasument_St.AFE_Temperature_St.AFE_MaxCellTemperature_i16 },
					{ 16,&AFE_InputeMeasument_St.AFE_Temperature_St.AFE_MinCellTemperature_i16 },
					{ 18,&AFE_InputeMeasument_St.AFE_Temperature_St.AFE_AvgCellTemperature_i16 }
			};

	for (U8 i = 0; i < 5; i++) {
		U16 raw_temp = (data[temp_values[i].offset + 1] << 8)
				| data[temp_values[i].offset];
		*temp_values[i].dest = (I16) (0.100 * raw_temp - 273.1500);
	}
	I32 CC2_ADC_Count_Temp_I32 = (I32) ((data[27] << 24) | (data[26] << 16)
			| (data[25] << 8) | data[24]);

	I32 CC3_ADC_Count_Temp_I32 = (I32) ((data[31] << 24) | (data[30] << 16)
			| (data[29] << 8) | data[28]);

	AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltageDifference_u16 =AFE_InputeMeasument_St.AFE_Voltage_St.AFE_MaxCellVoltage_u16- AFE_InputeMeasument_St.AFE_Voltage_St.AFE_MinCellVoltage_u16;
	AFE_InputeMeasument_St.AFE_Current_st.AFE_CC2_Current_i16 =
			(F64) ((CC2_ADC_Count_Temp_I32 * 1.24) / (5.0 * 8388608.0 * 0.001));

	AFE_InputeMeasument_St.AFE_Current_st.AFE_CC3_Current_i16 =
			(I16) ((CC3_ADC_Count_Temp_I32 * 1.24) / (5.0 * 8388608.0 * 0.001));

}


/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : AFE_ReadAllDAStatus_gv
*   Description   : Reads all DASTATUS registers (0x0071-0x0077) and cell balancing
*                   status_en from AFE device using subcommand read operations
*   Parameters    : None
*   Return Value  : AFE_Status_ten - Communication status_en
*   Note          : Reads DASTATUS1-4 (0x0071-0x0074): Cell current measurements for 16 cells
*                   Reads DASTATUS5 (0x0075): System measurements and processes data
*                   Reads Cell Balancing Status (0x0083): Active cell balancing bitmask
*                   Uses ADC conversion formula: (ADC_Counts * 1.24) / (5 * 8388608 * 0.001)
*  ---------------------------------------------------------------------------*/
AFE_Status_ten AFE_ReadAllDAStatus_gv(void)
{
    AFE_Status_ten status_En = AFE_STATUS_SUCCESS;
    U8 DAStatus_data[32] = {0};
    U8 CellBalacing_Status_arru8[2]={0};
	for (U8 cell_block = 0; cell_block < 4; cell_block++)
	{
		U8 da_status_reg = 0x0071 + cell_block;
		status_En = AFE_SubCommand_ReadOperation_gv(da_status_reg, DAStatus_data, 32);
			if (status_En == AFE_STATUS_SUCCESS)
			{
				for (U8 cell_in_block = 0; cell_in_block < 4; cell_in_block++) {
					U8 cell_index = (cell_block * 4) + cell_in_block;
					if (cell_index < 16)
					{
						I32 cell_current_counts =
								(I32) ((DAStatus_data[cell_in_block * 8 + 7]
										<< 24)
										| (DAStatus_data[cell_in_block * 8 + 6]
												<< 16)
										| (DAStatus_data[cell_in_block * 8 + 5]
												<< 8)
										| DAStatus_data[cell_in_block * 8 + 4]);
						AFE_InputeMeasument_St.AFE_Current_st.AFE_CellCurrent_i16[cell_index] =(I16) ((cell_current_counts * 1.24)/ (5.0 * 8388608.0 * 0.001));
					}
				}
			} else {
				break;
			}
	}

	/* Process DASTATUS5 */

	status_En = AFE_SubCommand_ReadOperation_gv(0x0075,DAStatus_data,13);
	if (status_En == AFE_STATUS_SUCCESS) {
		AFE_DATAStatus5_gv(DAStatus_data);
	}
	/* Process DASTATUS5 */
	status_En = AFE_SubCommand_ReadOperation_gv(0x0083, CellBalacing_Status_arru8, 2);
	if (status_En == AFE_STATUS_SUCCESS)
	{
		AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CB_ActiveCells_u16 =(U16) ((CellBalacing_Status_arru8[1] << 8) | CellBalacing_Status_arru8[0]);
	}
	return status_En;
}
/* ==================== DIRECT COMMANDS ==================== */
/* ==================== SYSTEM CONTROL ==================== */
/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : AFE_AlarmStatusReset_gen
*   Description   : Resets AFE alarm status_en by writing to alarm status_en register
*   Parameters    : value - 16-bit mask specifying which alarm flags to reset
*                   (each bit corresponds to a specific alarm type)
*   Return Value  : AFE_Status_ten - I2C communication status_en (success/error)
*  ---------------------------------------------------------------------------*/

AFE_Status_ten AFE_AlarmStatusReset_gen(U16 value_argu16)
{
	AFE_Status_ten status_en;
	status_en= AFE_Direct_WriteOperation_gv(0x62,value_argu16,0x04);
    BSP_TimerDelay (2, DRV_DELAY_UNITS_MILLISECOND);
    return status_en;
}
/* ==================== ALERTS AND ALARMS ==================== */
/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : AFE_ReadAllSafetyAlertStatus_gv
*   Description   : Reads all Safety Status, Alert, and Permanent Fail registers from AFE
*                   using mathematical patterns 0x02 + offset and 0x0A + offset
*   Parameters    : None
*   Return Value  : None
*   Note          : Register Map & Bit Functions:
*                   0x02-AlertA: Bit7:SCD(Short Circuit Discharge) Bit6:OCD2(Overcurrent Discharge Tier2)
*                   Bit5:OCD1(Overcurrent Discharge Tier1) Bit4:OCC(Overcurrent Charge) Bit3:COV(Cell Overvoltage) Bit2:CUV(Cell Undervoltage)
*                   0x03-SafetyA: Same bit mapping as AlertA - 0=No fault, 1=Fault triggered
*                   0x04-AlertB: Bit7:OTF(FET Overtemp) Bit6:OTINT(Internal Overtemp) Bit5:OTD(Discharge Overtemp)
*                   Bit4:OTC(Charge Overtemp) Bit2:UTINT(Internal Undertemp) Bit1:UTD(Discharge Undertemp) Bit0:UTC(Charge Undertemp)
*                   0x05-SafetyB: Same bit mapping as AlertB
*                   0x06-AlertC: Bit7:OCD3(Discharge Overcurrent Tier3) Bit6:SCDL(Short Circuit Discharge Latch)
*                   Bit5:OCDL(Overcurrent Discharge Latch) Bit4:COVL(Cell Overvoltage Latch) Bit3:PTOS(Precharge Timeout Suspend)
*                   0x07-SafetyC: Bit7:OCD3 Bit6:SCDL Bit5:OCDL Bit4:COVL Bit2:PTO(Precharge Timeout) Bit1:HWDF(Host Watchdog Fault)
*                   0x0A-PF AlertA: Bit7:CUDEP(Copper Deposition) Bit6:SOTF(Safety Overtemp FET) Bit4:SOT(Safety Overtemp)
*                   Bit3:SOCD(Safety Overcurrent Discharge) Bit2:SOCC(Safety Overcurrent Charge) Bit1:SOV(Safety Overvoltage) Bit0:SUV(Safety Undervoltage)
*                   0x0B-PF StatusA: Same bit mapping as PF AlertA
*                   0x0C-PF AlertB: Bit7:SCDL(Short Circuit Discharge Latch) Bit4:VIMA(Voltage Imbalance Active)
*                   Bit3:VIMR(Voltage Imbalance Rest) Bit2:2LVL(Second Level Protector) Bit1:DFETF(Discharge FET) Bit0:CFETF(Charge FET)
*                   0x0D-PF StatusB: Same bit mapping as PF AlertB
*                   0x0E-PF AlertC: Bit6:HWMX(Hardware Mux) Bit5:VSSF(Internal VSS) Bit4:VREF(Internal Voltage Ref) Bit3:LFOF(Internal LFO)
*                   0x0F-PF StatusC: Bit7:CMDF(Commanded) Bit6:HWMX Bit5:VSSF Bit4:VREF Bit3:LFOF Bit2:IRMF(Instruction ROM) Bit1:DRMF(Data ROM) Bit0:OTPF(OTP Memory)
*                   0x10-PF AlertD: Bit0:TOSF(Top of Stack vs Cell Sum)
*                   0x11-PF StatusD: Bit0:TOSF
*  ---------------------------------------------------------------------------*/
void AFE_ReadAllSafetyAlertStatus_gv(void)
{
    U8* AlertStatusRegister[] = {
        &AFE_Status_st.AFE_AlertA_u8,
        &AFE_Status_st.AFE_SafetyStatusA_u8,
        &AFE_Status_st.AFE_AlertB_u8,
        &AFE_Status_st.AFE_SafetyStatusB_u8,
        &AFE_Status_st.AFE_AlertC_u8,
        &AFE_Status_st.AFE_SafetyStatusC_u8,
    };

    for (U8 RegIdx_u8 = 0; RegIdx_u8 < 6; RegIdx_u8++)
    {
        AFE_Direct_ReadOperation_gv(0x02 + RegIdx_u8, AlertStatusRegister[RegIdx_u8], 1);
    }
    U8* PF_AlertStatusRegister[] = {
		&AFE_Status_st.AFE_PF_AlertA_u8,
		&AFE_Status_st.AFE_PF_StatusA_u8,
		&AFE_Status_st.AFE_PF_AlertB_u8,
		&AFE_Status_st.AFE_PF_StatusB_u8,
		&AFE_Status_st.AFE_PF_AlertC_u8,
		&AFE_Status_st.AFE_PF_StatusC_u8,
		&AFE_Status_st.AFE_PF_AlertD_u8,
		&AFE_Status_st.AFE_PF_StatusD_u8,
    };
    for (U8 RegIdx_u8 = 0; RegIdx_u8 < 8; RegIdx_u8++)
    {
        AFE_Direct_ReadOperation_gv(0x0A + RegIdx_u8, PF_AlertStatusRegister[RegIdx_u8], 1);
    }
}
/* ==================== VOLTAGE MEASUREMENT ==================== */
/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : AFE_ReadAllCellVolatages_gv
*   Description   : Reads all 16 cell voltage registers from AFE device and stores
*                   them in the global voltage structure
*   Note          : Register addresses follow mathematical pattern:
*                   0x14 + (cell_number × 2) where cell_number = 1 to 16
*                   Actual addresses: 0x14, 0x16, 0x18, 0x1A, 0x1C, 0x1E, 0x20,
*                   0x22, 0x24, 0x26, 0x28, 0x2A, 0x2C, 0x2E, 0x30, 0x32
*   Parameters    : None
*   Return Value  : None
*  ---------------------------------------------------------------------------*/
void AFE_ReadAllSystemMeasurements_gv(void)
{
    U8 Cell_channel_u8 = 1;
    for (U8 Cell_i_u8 = 0; Cell_i_u8 < 16; Cell_i_u8++)
    {
        AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[Cell_i_u8] =AFE_Read_Value_i32(((Cell_channel_u8 * 2) + (0x12)), AFE_MILLI_UNIT);
        Cell_channel_u8++;
    }

    U16* voltage_pointers[3] = {
        &AFE_InputeMeasument_St.AFE_Voltage_St.AFE_Stack_Voltage_u16,
        &AFE_InputeMeasument_St.AFE_Voltage_St.AFE_PackPin_Voltage_u16,
        &AFE_InputeMeasument_St.AFE_Voltage_St.AFE_LD_Voltage_u16
    };

    I16* current_pointer = &AFE_InputeMeasument_St.AFE_Current_st.AFE_CurrentmA_I16;

    AFE_MeasurementUnit_ten voltage_units[3] = {
        AFE_BASIC_UNIT,
        AFE_BASIC_UNIT,
        AFE_BASIC_UNIT
    };
    for (U8 channel = 0; channel < 3; channel++)
    {
        *voltage_pointers[channel] = AFE_Read_Value_i32(0x34 + (channel * 2), voltage_units[channel]);
    }

    *current_pointer = (I16)AFE_Read_Value_i32(0x3A, AFE_MILLI_UNIT);
}

/* ==================== TEMPERATURE MEASUREMENT ==================== */
/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : AFE_ReadTemperature_F64
*   Description   : Reads temperature from specified sensor channel via AFE and
*                   returns the value as 64-bit float
*   Parameters    : temp_channel - Temperature channel selection (0-9)
*                   Channel mapping:
*                   0: Internal Temperature (0x68)
*                   1: CFET Temperature     (0x6A)
*                   2: DFET Temperature     (0x6C)
*                   3: ALERT Temperature    (0x6E)
*                   4: TS1 Temperature      (0x70)
*                   5: TS2 Temperature      (0x72)
*                   6: TS3 Temperature      (0x74)
*                   7: HDQ Temperature      (0x76)
*                   8: DCHG Temperature     (0x78)
*                   9: DDSG Temperature     (0x7A)
*   Note          : Register addresses follow mathematical pattern:
*                   Address = 0x68 + (channel × 2)
*                   Temperature conversion: 0.1 × raw_value - 273.15 °C
*   Return Value  : F64 - Measured temperature value in degrees Celsius
*  ---------------------------------------------------------------------------*/
void AFE_ReadAllTemperatures_gv(void)
{
    F64* temp_pointers[10] = {
        &AFE_InputeMeasument_St.AFE_Temperature_St.AFE_Temp_st.AFE_Internal_Temp_f64,
        &AFE_InputeMeasument_St.AFE_Temperature_St.AFE_Temp_st.AFE_CFET_Temp_f64,
        &AFE_InputeMeasument_St.AFE_Temperature_St.AFE_Temp_st.AFE_DFET_Temp_f64,
        &AFE_InputeMeasument_St.AFE_Temperature_St.AFE_Temp_st.AFE_ALERT_Temp_f64,
        &AFE_InputeMeasument_St.AFE_Temperature_St.AFE_Temp_st.AFE_TS1_Temp_f64,
        &AFE_InputeMeasument_St.AFE_Temperature_St.AFE_Temp_st.AFE_TS2_Temp_f64,
        &AFE_InputeMeasument_St.AFE_Temperature_St.AFE_Temp_st.AFE_TS3_Temp_f64,
        &AFE_InputeMeasument_St.AFE_Temperature_St.AFE_Temp_st.AFE_HDQ_Temp_f64,
        &AFE_InputeMeasument_St.AFE_Temperature_St.AFE_Temp_st.AFE_DCHG_Temp_f64,
        &AFE_InputeMeasument_St.AFE_Temperature_St.AFE_Temp_st.AFE_DDSG_Temp_f64
    };

    for (U8 channel = 0; channel < 10; channel++)
    {
        *temp_pointers[channel] = (F64)  AFE_ReadTemperature_gf32(0x68 + (channel * 2));
    }
}
/* ==================== DEVICE STATUS & ALARMS ==================== */
/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : AFE_FETStatusRead_gv
*   Description   : Reads FET status_en from AFE device (register 0x7F) with retry
*                   mechanism and stores result in global variable FET_Status_gu8
*   Parameters    : None
*   Note		  :	6-ALRT_PIN (Alert Pin Status), 5-DDSG_PIN (Discharge Disable Pin), 4-DCHG_PIN (Charge Disable Pin),
*   				3-PDSG_FET (Pre-Discharge FET), 2-DSG_FET (Discharge FET), 1-PCHG_FET (Pre-Charge FET), 0-CHG_FET (Charge FET)
*   Return Value  : None
*   Note          : - Implements retry mechanism (AFE_MAXCMDRETIES attempts)
*                   - Sets AFE_COMM_ERROR fault if all retries fail
*                   - Stores result directly in FET_Status_gu8 global variable
*  ---------------------------------------------------------------------------*/
AFE_Status_ten AFE_FETStatusRead_gv(void)
{
	AFE_Status_ten Status=AFE_Direct_ReadOperation_gv(0x7F,&AFE_Status_st.FET_Status_u8,1);
	return Status;
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : AFE_ReadAllAlarmStatus_gv
*   Description   : Reads all alarm registers (Status, Raw Status, Enable) from AFE
*                   using mathematical pattern 0x62 + (channel × 2)
*   Parameters    : None
*   Return Value  : None
*   Note          : Register Map & Bit Functions:
*                   0x62-Alarm Status  | 0x64-Alarm Raw Status | 0x66-Alarm Enable
*                   Bit15:SSBC(Safety Status B/C) Bit14:SSA(Safety Status A) Bit13:PF(Permanent Failure) Bit12:MSK_SFALERT(Masked Safety Fault)
*                   Bit11:MSK_PFALERT(Masked Perm Fault) Bit10:INITSTART(Init Start) Bit9:INITCOMP(Init Complete) Bit7:FULLSCAN(Full ADC Scan)
*                   Bit6:XCHG(Charge FET Off) Bit5:XDSG(Discharge FET Off) Bit4:SHUTV(Shutdown Voltage) Bit3:FUSE(Fuse Pin)
*                   Bit2:CB(Cell Balancing) Bit1:ADSCAN(ADC Scan) Bit0:WAKE(Wake Event)
*                   Alarm Enable controls which alarms generate Status bits
*  ---------------------------------------------------------------------------*/
void AFE_ReadAllAlarmStatus_gv(void)
{
    volatile U16* alarm_pointers[3] = {
        &AFE_Status_st.APP_AlarmStatusValue_vu16,  /* 0x62 */
        &AFE_Status_st.AFE_AlarmRawStatus_u16,     /* 0x64 */
        &AFE_Status_st.AFE_AlarmEnable_u16         /* 0x66 */
    };

    for (U8 channel = 0; channel < 3; channel++)
    {
        *alarm_pointers[channel] = AFE_Read_Value_i32(0x62 + (channel * 2), AFE_BASIC_UNIT);
    }
}


 /* -----------------------------------------------------------------------------
 *  FUNCTION DESCRIPTION
 *  -----------------------------------------------------------------------------
 *   Function Name : AFE_ReadTemperature_gf32
 *   Description   : Reads temperature from specified AFE register address via I2C
 *                   with retry mechanism and converts raw data to temperature value
 *   Parameters    : Regaddr_argu8 - I2C register address to read temperature from
 *   Return Value  : F32 - Converted temperature value in degrees Celsius
 *   Note          : Implements retry mechanism (AFE_MAXCMDRETIES attempts)
 *                   Sets AFE_COMM_ERROR fault if communication fails
 *  ---------------------------------------------------------------------------*/
 F32 AFE_ReadTemperature_gf32(U8 Regaddr_argu8)
 {
     U8 RX_2Byte_u8[2] ={ 0x00, 0x00 };
     F32 tempMeasured_f32=0;
     AFE_Direct_ReadOperation_gv(Regaddr_argu8, RX_2Byte_u8, 2);
    tempMeasured_f32 = (F32) (0.100 * (RX_2Byte_u8[1] * 256 + RX_2Byte_u8[0]) - 273.1500);
     return tempMeasured_f32;
 }
 /* ==================== AFE Direct and Sub Commands ==================== */
 /* -----------------------------------------------------------------------------
  *  FUNCTION DESCRIPTION
  *  -----------------------------------------------------------------------------
  *   Function Name : AFE_Read_Value_i32
  *   Description   : Reads 16-bit value from AFE register and converts to 32-bit
  *                   integer with unit scaling applied
  *   Parameters    : RegAddr_argu8 - AFE register address to read from
  *                   Units_argen - Measurement unit scaling factor to apply
  *   Return Value  : I32 - Scaled 32-bit integer value after unit conversion
  *   Note          : Reads 2 bytes from specified register
  *                   Combines bytes as: (MSB * 256) + LSB
  *                   Applies unit scaling factor to the raw value
  *                   Used for voltage, current, and other measurements
  *  ---------------------------------------------------------------------------*/
 I32 AFE_Read_Value_i32(U8 RegAddr_argu8,AFE_MeasurementUnit_ten Units_argen)
 {
     U8 RX_2Byte_arru8[2] = { 0x00, 0x00 };
     I32 Read_value_i32=0;
     AFE_Direct_ReadOperation_gv(RegAddr_argu8, RX_2Byte_arru8, 2);
     Read_value_i32 = (I32) (Units_argen*(RX_2Byte_arru8[1] *256) + RX_2Byte_arru8[0]);
     return Read_value_i32;
 }
 /* ==================== FET CONTROL FUNCTIONS ==================== */

 /* -----------------------------------------------------------------------------
 *  FUNCTION DESCRIPTION
 *  -----------------------------------------------------------------------------
 *   Function Name : AFE_FetControl_gv
 *   Description   : Controls various FET operations using single unified function
 *   Parameters    : FetControlMode_argen - FET control mode to execute
 *   Return Value  : AFE_Status_ten - Communication status_en (success/error)
 *  ---------------------------------------------------------------------------*/
 AFE_Status_ten AFE_FetControl_gv(AFE_FetControl_ten FetControlMode_argen)
 {
     AFE_Status_ten status_en = AFE_STATUS_ERROR;
     U8 FET_Control_u8=0;
     switch (FetControlMode_argen)
     {
     	case AFE_FET_MODE_CONTROL:
     		status_en = AFE_Direct_WriteOperation_gv(AFE_FET_EN_CMD_ADDR, 0x0000, 0x05);
     		break;
         case AFE_FET_PDSG_TEST_MODE_ON:
             status_en = AFE_Direct_WriteOperation_gv(AFE_PDSG_TEST_CMD_ADDR, 0x0000, 0x05);
             break;

         case AFE_FET_PCHG_TEST_MODE_ON:
             status_en = AFE_Direct_WriteOperation_gv(AFE_PCHG_TEST_CMD_ADDR, 0x0000, 0x05);
             break;

         case AFE_FET_CHG_TEST_MODE_ON:
             status_en = AFE_Direct_WriteOperation_gv(AFE_CHG_TEST_CMD_ADDR, 0x0000, 0x05);
             break;

         case AFE_FET_DSG_TEST_MODE_ON:
             status_en = AFE_Direct_WriteOperation_gv(AFE_DSG_TEST_CMD_ADDR, 0x0000, 0x05);
             break;
         case AFE_FET_DSG_PDSG_OFF:
             status_en = AFE_Direct_WriteOperation_gv(AFE_DSG_PDSG_OFF_CMD_ADDR, 0x0000, 0x05);
             break;

         case AFE_FET_CHG_PCHG_OFF:
             status_en = AFE_Direct_WriteOperation_gv(AFE_CHG_PCHG_OFF_CMD_ADDR, 0x0000, 0x05);
             break;

         case AFE_FET_ENABLE_CHARGING:
             FET_Control_u8 = ((U8) ~(AFE_CHG_OFF_MASK | AFE_PCHG_OFF_MASK));
             FET_Control_u8 = FET_Control_u8 & 0x0F;
             status_en = AFE_SubCommand_WriteOperation_gv(AFE_FET_CONTROL_CMD_ADDR, FET_Control_u8, 0x05);
             break;

         case AFE_FET_ENABLE_DISCHARGING:
         {
              FET_Control_u8 = ((U8) ~(AFE_DSG_OFF_MASK | AFE_PDSG_OFF_MASK));
             FET_Control_u8 = FET_Control_u8 & 0x0F;
             status_en = AFE_SubCommand_WriteOperation_gv(AFE_FET_CONTROL_CMD_ADDR, FET_Control_u8, 0x05);
             break;
         }
         case AFE_FET_DISABLE_CHARGING:
             FET_Control_u8 = ((U8) (AFE_CHG_OFF_MASK | AFE_PCHG_OFF_MASK));
             FET_Control_u8 = FET_Control_u8 & 0x0F;
             status_en = AFE_Direct_WriteOperation_gv(AFE_FET_CONTROL_CMD_ADDR, FET_Control_u8, 0x05);
             break;

         case AFE_FET_DISABLE_DISCHARGING:
             FET_Control_u8 = ((U8) (AFE_DSG_OFF_MASK | AFE_PDSG_OFF_MASK));
            FET_Control_u8 = FET_Control_u8 & 0x0F;
             status_en = AFE_Direct_WriteOperation_gv(AFE_FET_CONTROL_CMD_ADDR, FET_Control_u8, 0x05);
             break;
         case AFE_FET_ALL_ON:
             status_en = AFE_Direct_WriteOperation_gv(AFE_ALL_FETS_ON_CMD_ADDR, 0x0000, 0x05);
             break;

         case AFE_FET_ALL_OFF:
             status_en = AFE_Direct_WriteOperation_gv(AFE_ALL_FETS_OFF_CMD_ADDR, 0x0000, 0x03);
             break;
         default:
             return AFE_STATUS_ERROR;
     }

     BSP_TimerDelay(10, DRV_DELAY_UNITS_MILLISECOND);
     return status_en;
 }

 /* -----------------------------------------------------------------------------
 *  FUNCTION DESCRIPTION
 *  -----------------------------------------------------------------------------
 *   Function Name : AFE_FetControlWithRetry_gv
 *   Description   : FET control with retry mechanism for critical operations
 *   Parameters    : FetControlMode_argen - FET control mode to execute
 *   Return Value  : AFE_Status_ten - Communication status_en
 *  ---------------------------------------------------------------------------*/
 AFE_Status_ten AFE_FetControlWithRetry_gv(AFE_FetControl_ten FetControlMode_argen)
 {
     AFE_Status_ten status_en = AFE_STATUS_ERROR;
     U8 retryCounts = AFE_MAXCMDRETIES;
     U8 isConfigSuccFlag = 0;

     do
     {
         status_en = AFE_FetControl_gv(FetControlMode_argen);
         if (status_en == AFE_STATUS_SUCCESS)
         {

         	if(AFE_FET_ALL_OFF==FetControlMode_argen)
         	{
                 if (AFE_Status_st.FET_Status_u8 == 0x70)
                 {

                     isConfigSuccFlag = 1;
                 }
         	}else{
                 if (AFE_Status_st.FET_Status_u8 != 0x70)
                 {

                     isConfigSuccFlag = 1;
                 }
         	}
         }

         if (!isConfigSuccFlag)
         {
             retryCounts--;
         }

     } while (retryCounts && !isConfigSuccFlag);

     return status_en;
 }
 /* ==================== POWER MODE CONTROL ==================== */
 /* -----------------------------------------------------------------------------
 *  FUNCTION DESCRIPTION
 *  -----------------------------------------------------------------------------
 *   Function Name : AFE_PowerModeControl_gv
 *   Description   : Controls AFE power modes (Shutdown, Deep Sleep, Exit Deep Sleep, Sleep)
 *                   using a single unified function with mode parameter
 *   Parameters    : powerMode - AFE_PowerMode_ten enumeration specifying the power mode
 *   Return Value  : AFE_Status_ten - I2C communication status_en (success/error)
 *  ---------------------------------------------------------------------------*/
 AFE_Status_ten AFE_PowerModeControl_gv(AFE_PowerMode_ten PowerMode_argen)
 {
     AFE_Status_ten status_en = AFE_STATUS_ERROR;
     switch (PowerMode_argen)
     {
         case AFE_POWER_MODE_SHUTDOWN:
             status_en = AFE_Direct_WriteOperation_gv(AFE_SHUTDOWN_CMD_ADDR, 0x0000, 0x03);
             BSP_TimerDelay(500, DRV_DELAY_UNITS_MILLISECOND);
             break;

         case AFE_POWER_MODE_DEEP_SLEEP:
             status_en = AFE_Direct_WriteOperation_gv(AFE_DEEP_SLEEP_CMD_ADDR, 0x0000, 0x03);
             BSP_TimerDelay(500, DRV_DELAY_UNITS_MILLISECOND);
             break;

         case AFE_POWER_MODE_EXIT_DEEP_SLEEP:
         	 status_en = AFE_Direct_WriteOperation_gv(AFE_EXIT_DEEP_SLEEP_CMD_ADDR, 0x0000, 0x03);
         	 BSP_TimerDelay(500, DRV_DELAY_UNITS_MILLISECOND);
             break;

         case AFE_POWER_MODE_SLEEP:
         	 status_en = AFE_Direct_WriteOperation_gv(AFE_SLEEP_CMD_ADDR, 0x0000, 0x03);
         	 BSP_TimerDelay(500, DRV_DELAY_UNITS_MILLISECOND);
             break;

         default:
             return AFE_STATUS_ERROR;
     }
     return status_en;
 }

 /* ==================== DIRECT AND SUBCOMMAND FUNCTIONS ==================== */
 /* -----------------------------------------------------------------------------
   *  FUNCTION DESCRIPTION
   *  -----------------------------------------------------------------------------
   *   Function Name : AFE_SubCommand_ReadOperation_gv
   *   Description   : Performs subcommand-based read operation by first writing
   *                   subcommand then reading response data from response register
   *   Parameters    : SubCommand_argu16 - 16-bit subcommand to execute for read
   *                   Data_argu8 - Pointer to buffer for storing read data
   *                   DataSize_argu8 - Number of bytes to read from response
   *   Return Value  : AFE_Status_ten - Communication status_en (success/error)
   *   Note          : Two-step process: subcommand write followed by data read
   *                   Reads from fixed response register address 0x40
   *  ---------------------------------------------------------------------------*/
  AFE_Status_ten AFE_SubCommand_ReadOperation_gv(U16 SubCommand_argu16, U8* Data_argu8, U8 DataSize_argu8) {
      AFE_Status_ten status_en;
      status_en = AFE_SubCommand_WriteOperation_gv(SubCommand_argu16, 0x00, 3);
      RTOS_TaskDelay_st(2);
      if (status_en == AFE_STATUS_SUCCESS) {
          status_en = AFE_Direct_ReadOperation_gv(0x40, Data_argu8, DataSize_argu8);
      }
      return status_en;
  }

  /* -----------------------------------------------------------------------------
   *  FUNCTION DESCRIPTION
   *  -----------------------------------------------------------------------------
   *   Function Name : AFE_Direct_ReadOperation_gv
   *   Description   : Performs direct read operation from AFE register with retry
   *                   mechanism and fault handling
   *   Parameters    : RegAddr_argu8 - Register address to read from
   *                   ReadData_argu8 - Pointer to buffer for storing read data
   *                   DataSize_argu8 - Number of bytes to read
   *   Return Value  : AFE_Status_ten - Communication status_en (success/error)
   *   Note          : Implements retry mechanism (AFE_MAXCMDRETIES attempts)
   *                   Sets AFE_COMM_ERROR fault if all retries fail
   *  ---------------------------------------------------------------------------*/
  AFE_Status_ten AFE_Direct_ReadOperation_gv(U8 RegAddr_argu8, U8* ReadData_argu8, U8 DataSize_argu8) {
      AFE_Status_ten status_en;
      U8 retryCounts_u8 = AFE_MAXCMDRETIES;
      BIN isConfigSuccFlag_b = false;
      do {
          status_en = AFE_ReadReg_gen(RegAddr_argu8, ReadData_argu8, DataSize_argu8);
          RTOS_TaskDelay_st(10);
          if (status_en == AFE_STATUS_SUCCESS) {

              isConfigSuccFlag_b = true;
          }
      } while (retryCounts_u8-- && !isConfigSuccFlag_b);

      if (!isConfigSuccFlag_b) {

      }
      return status_en;
  }
  /* -----------------------------------------------------------------------------
   *  FUNCTION DESCRIPTION
   *  -----------------------------------------------------------------------------
   *   Function Name : AFE_ReadReg_gen
   *   Description   : Low-level I2C register read operation with address phase
   *   Parameters    : regaddr_argu8 - Register address to read from
   *                   regdata_argptru8 - Pointer to buffer for storing read data
   *                   count_argu8 - Number of bytes to read
   *   Return Value  : AFE_Status_ten - Communication status_en (success/error)
   *   Note          : Two-phase I2C operation: write address then read data
   *                   Uses repeated start condition for efficient read
   *                   Converts I2C status_en to AFE status_en codes
   *  ---------------------------------------------------------------------------*/
  AFE_Status_ten AFE_ReadReg_gen(U8 regaddr_argu8, U8 *regdata_argptru8, U8 count_argu8)
  {
      DRV_i2cStatus_ten status_en;

      status_en = DRV_I2C_WriteBlock_gen(BSP_BQ76952Afe, &regaddr_argu8, 1, true);
      if (status_en != DRV_I2C_SUCCESS)
      {
          return DRV_I2C_FAILED;
      }
      status_en = DRV_I2C_ReadBlock_gen(BSP_BQ76952Afe,(U8* const ) &regdata_argptru8[0], count_argu8, false);

      if (status_en != DRV_I2C_SUCCESS)
      {
          return DRV_I2C_FAILED;
      }

      return DRV_I2C_SUCCESS;
  }
  /* -----------------------------------------------------------------------------
   *  FUNCTION DESCRIPTION
   *  -----------------------------------------------------------------------------
   *   Function Name : AFE_Direct_WriteOperation_gv
   *   Description   : Performs direct write operation to AFE register with retry
   *                   mechanism and fault handling
   *   Parameters    : DirectCommand_argu8 - Direct command byte
   *                   DirectCommandData_argu8 - Data to write
   *                   DirectCmd_len_argu8 - Length of command data
   *   Return Value  : AFE_Status_ten - Communication status_en (success/error)
   *   Note          : Implements retry mechanism (AFE_MAXCMDRETIES attempts)
   *                   Sets AFE_COMM_ERROR fault if all retries fail
   *  ---------------------------------------------------------------------------*/
  AFE_Status_ten AFE_Direct_WriteOperation_gv(U8 DirectCommand_argu8, U8 DirectCommandData_argu8, U8 DirectCmd_len_argu8) {
      U8 messagePayLoad_arru8[4];
      AFE_Status_ten status_en;
      U8 retryCounts = AFE_MAXCMDRETIES;
      U8 isConfigSuccFlag = 0;

      messagePayLoad_arru8[0] = CMD_DIR_SUBCMD_LOW;
      messagePayLoad_arru8[1] = DirectCommand_argu8;
      messagePayLoad_arru8[2] = LOW_BYTE(DirectCommandData_argu8);
      messagePayLoad_arru8[3] = HIGH_BYTE(DirectCommandData_argu8);

      do {
          status_en = AFE_WriteReg_gen(messagePayLoad_arru8, DirectCmd_len_argu8);
          RTOS_TaskDelay_st(10);
          if (status_en == AFE_STATUS_SUCCESS) {

              isConfigSuccFlag = 0x01;
          }
      } while (retryCounts-- && !isConfigSuccFlag);

      if (!isConfigSuccFlag) {

      }
      return status_en;
  }
