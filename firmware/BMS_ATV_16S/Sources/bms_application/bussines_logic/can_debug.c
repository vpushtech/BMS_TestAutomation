
/*
 * CAN_Debug_Protocol.c
 *
 *  Created on: 14-Oct-2025
 *  Author: RushikeshNitinKamble
 *  Description: CAN Debug Protocol Implementation - Debug Message Transmission
 *  Version: 1.0
 *  Modification History:
 *  Date            Author          Description
 *  ----------------------------------------------------------------------------
 *  14-Oct-2025     Rushikesh       Initial CAN Debug Protocol Implementation
 *  16-Oct-2025     Rushikesh       Added Fault Message Processing
 *  18-Oct-2025     Rushikesh       Integrated Cell Voltage and Temperature Data
 ******************************************************************************/

#include <afe_data_proccessing.h>
#include <bms_protection.h>
#include <can_comm.h>

/* ==================== STATIC FUNCTION DECLARATIONS ==================== */
static void CAN_Frame_0x01_Massages_tx(void);
static void CAN_Frame_0x02_Massages_tx(void);
static void CAN_Frame_0x03_Massages_tx(void);
static void CAN_Frame_0x04_Massages_tx(void);
static void CAN_Frame_0x05_Massages_tx(void);
static void CAN_Frame_0x06_Massages_tx(void);
static void CAN_Frame_0x07_Massages_tx(void);
static void CAN_Frame_0x08_Massages_tx(void);

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : CAN_Massages_Tranmission
*   Description   : Main CAN message transmission function
*                   Transmits all debug frames when timer flag is set
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void CAN_Massages_Tranmission(void)
{
    if (CAN_Tx_Timer_b)
    {
        CAN_SetMode(CAN_TX_MODE);
        CAN_Frame_0x01_Massages_tx();
        CAN_Frame_0x02_Massages_tx();
        CAN_Frame_0x03_Massages_tx();
        CAN_Frame_0x04_Massages_tx();
        CAN_Frame_0x05_Massages_tx();
        CAN_Frame_0x06_Massages_tx();
        CAN_Frame_0x07_Massages_tx();
       CAN_Frame_0x08_Massages_tx();
        CAN_Tx_Timer_b = false;
    }
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : CAN_Frame_0x01_Massages_tx
*   Description   : Transmits CAN frame 0x01 with pack voltage, current, and SoC data
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
static void CAN_Frame_0x01_Massages_tx(void)
{
	U8 SoC_Real_u8=0;
	U8 SoH_Real_u8=0;
	U16 PackVoltage=AFE_InputeMeasument_St.AFE_Voltage_St.AFE_Stack_Voltage_u16/10;
	 U16 CurrentmA=AFE_InputeMeasument_St.AFE_Current_st.AFE_CurrentmA_I16;
	CAN_QueueDataFrame_St[CAN_ID_0x01].CAN_DataFrame_st.DRV_Data_arru8[0] = PackVoltage;
	CAN_QueueDataFrame_St[CAN_ID_0x01].CAN_DataFrame_st.DRV_Data_arru8[1] = PackVoltage>>8;
    CAN_QueueDataFrame_St[CAN_ID_0x01].CAN_DataFrame_st.DRV_Data_arru8[2] = (CurrentmA);
    CAN_QueueDataFrame_St[CAN_ID_0x01].CAN_DataFrame_st.DRV_Data_arru8[3] = (CurrentmA>>8);
    CAN_QueueDataFrame_St[CAN_ID_0x01].CAN_DataFrame_st.DRV_Data_arru8[4] = SoC_Real_u8;
    CAN_QueueDataFrame_St[CAN_ID_0x01].CAN_DataFrame_st.DRV_Data_arru8[5] = AFE_InputeMeasument_St.AFE_Voltage_St.AFE_Pack_Voltage_u16>>8;
    CAN_QueueDataFrame_St[CAN_ID_0x01].CAN_DataFrame_st.DRV_Data_arru8[6] = SoH_Real_u8;
    CAN_QueueDataFrame_St[CAN_ID_0x01].CAN_DataFrame_st.DRV_Data_arru8[7] = 0;
    RTOS_QueueSend_st(CAN_QUEUE, & CAN_QueueDataFrame_St[CAN_ID_0x01], portMAX_DELAY);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : CAN_PrimaryFaults_Messages
*   Description   : Processes primary protection faults and updates CAN message bits
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void CAN_PrimaryFaults_Messages(void)
{
/*	APP_FaultStatus_tst*  Primary_faultStatus=APP_GetFaultStatus_gst(APP_PRIMARY_PROTECTION,APP_PRIMARY_VOLT_ALERT);
	if(APP_CHECK_FAULT_BIT(Primary_faultStatus, APP_BATTERY_OVER_THRESHOLD_BIT))
	{
		  CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[0] =CAN_MessageSetBit_gu8(CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[0],1) ;
	}else if(!APP_CHECK_FAULT_BIT(Primary_faultStatus, APP_BATTERY_OVER_THRESHOLD_BIT))
	{
		CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[0] =CAN_MessageClearBit_gu8(CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[0],1) ;
	}else
	{
		;
	}
	if(APP_CHECK_FAULT_BIT(Primary_faultStatus, APP_BATTERY_UNDER_THRESHOLD_BIT))
	{
		  CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[0] =CAN_MessageSetBit_gu8(CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[0],2) ;
	}else if(!APP_CHECK_FAULT_BIT(Primary_faultStatus, APP_BATTERY_UNDER_THRESHOLD_BIT))
	{
		CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[0] =CAN_MessageClearBit_gu8(CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[0],2) ;
	}else
	{
		;
	}*/
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : CAN_SecondaryFaults_Messages
*   Description   : Processes secondary protection faults and updates CAN message bits
*                   Handles voltage, current, and temperature fault conditions
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
void CAN_SecondaryFaults_Messages(void)
{
	APP_FaultStatus_tst*  Secondary_faultStatus=APP_GetFaultStatus_gst(APP_SECONDARY_PROTECTION,APP_SECONDARY_VOLT_ALERT);
	if(APP_CHECK_FAULT_BIT(Secondary_faultStatus, APP_BATTERY_OVER_THRESHOLD_BIT))
	{
		  CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[2] =CAN_MessageSetBit_gu8(CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[2],1) ;
	}else if(!APP_CHECK_FAULT_BIT(Secondary_faultStatus, APP_BATTERY_OVER_THRESHOLD_BIT))
	{
		CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[2] =CAN_MessageClearBit_gu8(CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[2],1) ;
	}else
	{
		;
	}
	if(APP_CHECK_FAULT_BIT(Secondary_faultStatus, APP_BATTERY_UNDER_THRESHOLD_BIT))
	{
		  CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[2] =CAN_MessageSetBit_gu8(CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[2],2) ;
	}else if(!APP_CHECK_FAULT_BIT(Secondary_faultStatus, APP_BATTERY_UNDER_THRESHOLD_BIT))
	{
		CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[2] =CAN_MessageClearBit_gu8(CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[2],2) ;
	}else
	{
		;
	}
	Secondary_faultStatus=APP_GetFaultStatus_gst(APP_SECONDARY_PROTECTION,APP_SECONDARY_CURRENT_ALERT);
	if(APP_CHECK_FAULT_BIT(Secondary_faultStatus, APP_BATTERY_OVER_THRESHOLD_BIT))
	{
		  CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[2] =CAN_MessageSetBit_gu8(CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[2],3) ;
	}else if(!APP_CHECK_FAULT_BIT(Secondary_faultStatus, APP_BATTERY_OVER_THRESHOLD_BIT))
	{
		CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[2] =CAN_MessageClearBit_gu8(CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[2],3) ;
	}else
	{
		;
	}
	Secondary_faultStatus=APP_GetFaultStatus_gst(APP_SECONDARY_PROTECTION,APP_SECONDARY_TEMP_ALERT);
	if(APP_CHECK_FAULT_BIT(Secondary_faultStatus, APP_BATTERY_OVER_THRESHOLD_BIT))
	{
		  CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[2] =CAN_MessageSetBit_gu8(CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[2],4) ;
	}else if(!APP_CHECK_FAULT_BIT(Secondary_faultStatus, APP_BATTERY_OVER_THRESHOLD_BIT))
	{
		CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[2] =CAN_MessageClearBit_gu8(CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[2],4) ;
	}else
	{
		;
	}
	if(APP_CHECK_FAULT_BIT(Secondary_faultStatus, APP_BATTERY_UNDER_THRESHOLD_BIT))
	{
		  CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[2] =CAN_MessageSetBit_gu8(CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[2],5) ;
	}else if(!APP_CHECK_FAULT_BIT(Secondary_faultStatus, APP_BATTERY_UNDER_THRESHOLD_BIT))
	{
		CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[2] =CAN_MessageClearBit_gu8(CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[2],5) ;
	}else
	{
		;
	}
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : CAN_Frame_0x02_Massages_tx
*   Description   : Transmits CAN frame 0x02 with cell voltage and temperature extremes
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
static void CAN_Frame_0x02_Massages_tx(void)
{
	CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[0] =	 AFE_InputeMeasument_St.AFE_Voltage_St.AFE_MaxCellVoltage_u16;
	CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[1] =	 AFE_InputeMeasument_St.AFE_Voltage_St.AFE_MaxCellVoltage_u16>>8;
	CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[2] =	 AFE_InputeMeasument_St.AFE_Voltage_St.AFE_MinCellVoltage_u16;
	CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[3] =	 AFE_InputeMeasument_St.AFE_Voltage_St.AFE_MinCellVoltage_u16>>8;
	CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[4] =	 AFE_InputeMeasument_St.AFE_Temperature_St.AFE_MaxCellTemperature_i16;
	CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[5] =	 AFE_InputeMeasument_St.AFE_Temperature_St.AFE_MaxCellTemperature_i16>>8;
	CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[6] =	 AFE_InputeMeasument_St.AFE_Temperature_St.AFE_MinCellTemperature_i16;
	CAN_QueueDataFrame_St[CAN_ID_0x02].CAN_DataFrame_st.DRV_Data_arru8[7] =	 AFE_InputeMeasument_St.AFE_Temperature_St.AFE_MinCellTemperature_i16>>8;
    RTOS_QueueSend_st(CAN_QUEUE, & CAN_QueueDataFrame_St[CAN_ID_0x02], portMAX_DELAY);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : CAN_Frame_0x03_Massages_tx
*   Description   : Transmits CAN frame 0x03 with cell voltages 0-3
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
static void CAN_Frame_0x03_Massages_tx(void)
{

    CAN_QueueDataFrame_St[CAN_ID_0x03].CAN_DataFrame_st.DRV_Data_arru8[0] = ( AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[0]);
    CAN_QueueDataFrame_St[CAN_ID_0x03].CAN_DataFrame_st.DRV_Data_arru8[1] = ( AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[0]>>8);
    CAN_QueueDataFrame_St[CAN_ID_0x03].CAN_DataFrame_st.DRV_Data_arru8[2] =  AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[1];
    CAN_QueueDataFrame_St[CAN_ID_0x03].CAN_DataFrame_st.DRV_Data_arru8[3] =  AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[1]>>8;
    CAN_QueueDataFrame_St[CAN_ID_0x03].CAN_DataFrame_st.DRV_Data_arru8[4] = AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[2];
    CAN_QueueDataFrame_St[CAN_ID_0x03].CAN_DataFrame_st.DRV_Data_arru8[5] = AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[2]>>8;
	CAN_QueueDataFrame_St[CAN_ID_0x03].CAN_DataFrame_st.DRV_Data_arru8[6] = AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[3];
	CAN_QueueDataFrame_St[CAN_ID_0x03].CAN_DataFrame_st.DRV_Data_arru8[7] = AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[3]>>8;
    RTOS_QueueSend_st(CAN_QUEUE, & CAN_QueueDataFrame_St[CAN_ID_0x03], portMAX_DELAY);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : CAN_Frame_0x04_Massages_tx
*   Description   : Transmits CAN frame 0x04 with cell voltages 4-7
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
static void CAN_Frame_0x04_Massages_tx(void)
{

    CAN_QueueDataFrame_St[CAN_ID_0x04].CAN_DataFrame_st.DRV_Data_arru8[0] = (AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[4]);
    CAN_QueueDataFrame_St[CAN_ID_0x04].CAN_DataFrame_st.DRV_Data_arru8[1] = (AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[4]>>8);
    CAN_QueueDataFrame_St[CAN_ID_0x04].CAN_DataFrame_st.DRV_Data_arru8[2] = AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[5];
    CAN_QueueDataFrame_St[CAN_ID_0x04].CAN_DataFrame_st.DRV_Data_arru8[3] = AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[5]>>8;
    CAN_QueueDataFrame_St[CAN_ID_0x04].CAN_DataFrame_st.DRV_Data_arru8[4] = AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[6];
    CAN_QueueDataFrame_St[CAN_ID_0x04].CAN_DataFrame_st.DRV_Data_arru8[5] = AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[6]>>8;
	CAN_QueueDataFrame_St[CAN_ID_0x04].CAN_DataFrame_st.DRV_Data_arru8[6] = AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[7];
	CAN_QueueDataFrame_St[CAN_ID_0x04].CAN_DataFrame_st.DRV_Data_arru8[7] = AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[7]>>8;
	  RTOS_QueueSend_st(CAN_QUEUE, & CAN_QueueDataFrame_St[CAN_ID_0x04], portMAX_DELAY);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : CAN_Frame_0x05_Massages_tx
*   Description   : Transmits CAN frame 0x05 with cell voltages 8-11
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
static void CAN_Frame_0x05_Massages_tx(void)
{

    CAN_QueueDataFrame_St[CAN_ID_0x05].CAN_DataFrame_st.DRV_Data_arru8[0] = (AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[8]);
    CAN_QueueDataFrame_St[CAN_ID_0x05].CAN_DataFrame_st.DRV_Data_arru8[1] = (AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[8]>>8);
    CAN_QueueDataFrame_St[CAN_ID_0x05].CAN_DataFrame_st.DRV_Data_arru8[2] = AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[9];
    CAN_QueueDataFrame_St[CAN_ID_0x05].CAN_DataFrame_st.DRV_Data_arru8[3] = AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[9]>>8;
    CAN_QueueDataFrame_St[CAN_ID_0x05].CAN_DataFrame_st.DRV_Data_arru8[4] = AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[10];
    CAN_QueueDataFrame_St[CAN_ID_0x05].CAN_DataFrame_st.DRV_Data_arru8[5] = AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[10]>>8;
	CAN_QueueDataFrame_St[CAN_ID_0x05].CAN_DataFrame_st.DRV_Data_arru8[6] = AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[11];
	CAN_QueueDataFrame_St[CAN_ID_0x05].CAN_DataFrame_st.DRV_Data_arru8[7] = AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[11]>>8;
	 RTOS_QueueSend_st(CAN_QUEUE, & CAN_QueueDataFrame_St[CAN_ID_0x05], portMAX_DELAY);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : CAN_Frame_0x06_Massages_tx
*   Description   : Transmits CAN frame 0x06 with cell voltages 12-15
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
static void CAN_Frame_0x06_Massages_tx(void)
{

    CAN_QueueDataFrame_St[CAN_ID_0x06].CAN_DataFrame_st.DRV_Data_arru8[0] = (AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[12]);
    CAN_QueueDataFrame_St[CAN_ID_0x06].CAN_DataFrame_st.DRV_Data_arru8[1] = (AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[12]>>8);
    CAN_QueueDataFrame_St[CAN_ID_0x06].CAN_DataFrame_st.DRV_Data_arru8[2] = AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[13];
    CAN_QueueDataFrame_St[CAN_ID_0x06].CAN_DataFrame_st.DRV_Data_arru8[3] = AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[13]>>8;
    CAN_QueueDataFrame_St[CAN_ID_0x06].CAN_DataFrame_st.DRV_Data_arru8[4] = AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[14];
    CAN_QueueDataFrame_St[CAN_ID_0x06].CAN_DataFrame_st.DRV_Data_arru8[5] = AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[14]>>8;
	CAN_QueueDataFrame_St[CAN_ID_0x06].CAN_DataFrame_st.DRV_Data_arru8[6] = AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[15];
	CAN_QueueDataFrame_St[CAN_ID_0x06].CAN_DataFrame_st.DRV_Data_arru8[7] = AFE_InputeMeasument_St.AFE_Voltage_St.AFE_CellVoltage_u16[15]>>8;
	 RTOS_QueueSend_st(CAN_QUEUE, & CAN_QueueDataFrame_St[CAN_ID_0x06], portMAX_DELAY);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : CAN_Frame_0x07_Massages_tx
*   Description   : Transmits CAN frame 0x07 with temperature sensor data
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
static void CAN_Frame_0x07_Massages_tx(void)
{

	 U8 temp1=AFE_InputeMeasument_St.AFE_Temperature_St.AFE_Temp_st.AFE_TS1_Temp_f64*100;
	 U8 temp2=AFE_InputeMeasument_St.AFE_Temperature_St.AFE_Temp_st.AFE_TS3_Temp_f64*100;
	 U8 temp3=AFE_InputeMeasument_St.AFE_Temperature_St.AFE_Temp_st.AFE_Internal_Temp_f64*100;
	 U8 temp4=AFE_InputeMeasument_St.AFE_Temperature_St.AFE_FET_Temperature_i16*100;
    CAN_QueueDataFrame_St[CAN_ID_0x07].CAN_DataFrame_st.DRV_Data_arru8[0] =temp1;
    CAN_QueueDataFrame_St[CAN_ID_0x07].CAN_DataFrame_st.DRV_Data_arru8[1] =temp1>>8;
    CAN_QueueDataFrame_St[CAN_ID_0x07].CAN_DataFrame_st.DRV_Data_arru8[2] =temp2;
    CAN_QueueDataFrame_St[CAN_ID_0x07].CAN_DataFrame_st.DRV_Data_arru8[3] =temp2>>8;
    CAN_QueueDataFrame_St[CAN_ID_0x07].CAN_DataFrame_st.DRV_Data_arru8[4] =temp3;
    CAN_QueueDataFrame_St[CAN_ID_0x07].CAN_DataFrame_st.DRV_Data_arru8[5] =temp3>>8;
    CAN_QueueDataFrame_St[CAN_ID_0x07].CAN_DataFrame_st.DRV_Data_arru8[6] =temp4;
    CAN_QueueDataFrame_St[CAN_ID_0x07].CAN_DataFrame_st.DRV_Data_arru8[7] =temp4>>8;
    RTOS_QueueSend_st(CAN_QUEUE, & CAN_QueueDataFrame_St[CAN_ID_0x07], portMAX_DELAY);
}

/* -----------------------------------------------------------------------------
*  FUNCTION DESCRIPTION
*  -----------------------------------------------------------------------------
*   Function Name : CAN_Frame_0x08_Massages_tx
*   Description   : Transmits CAN frame 0x08 with internal temperature and FET status
*   Parameters    : None
*   Return Value  : void
*  --------------------------------------------------------------------------- */
static void CAN_Frame_0x08_Massages_tx(void)
{
	 U8 temp5=AFE_InputeMeasument_St.AFE_Temperature_St.AFE_Temp_st.AFE_Internal_Temp_f64*100;
	CAN_QueueDataFrame_St[CAN_ID_0x08].CAN_DataFrame_st.DRV_Data_arru8[0] = temp5;
	CAN_QueueDataFrame_St[CAN_ID_0x08].CAN_DataFrame_st.DRV_Data_arru8[1] = temp5>>8;
	CAN_QueueDataFrame_St[CAN_ID_0x08].CAN_DataFrame_st.DRV_Data_arru8[2] = AFE_Status_st.FET_Status_u8;
	CAN_QueueDataFrame_St[CAN_ID_0x08].CAN_DataFrame_st.DRV_Data_arru8[6] = 0;
	CAN_QueueDataFrame_St[CAN_ID_0x08].CAN_DataFrame_st.DRV_Data_arru8[7] = 0;
    RTOS_QueueSend_st(CAN_QUEUE, & CAN_QueueDataFrame_St[CAN_ID_0x08], portMAX_DELAY);
}
