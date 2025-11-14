#include "automation.h"
#include <bms_protection.h>
#include "can_comm.h"

volatile uint16_t *ram_ptr = (volatile uint16_t *)TEST_RAM_ADDRESS;
U16 data = 0;
volatile uint16_t offset;
volatile uint8_t length;
volatile uint32_t cnt = 0x00;
converter var;
U32 response = 0;

uint32_t input_data1 = 0x00;

TEST_automationFramework_tst dataVar = {0};
Response_automationFramework_tst Responsedata_gst = {0};
TEST_DataBytes_ten TEST_DataBytes_argen = {0};
extern APP_GlobleProtectionLevelFaults_tst APP_GlobleProtectionLevelFaults_St;

void TEST_ReadTestCases_gv(void)
{
    converter var;
    offset = CAN_QueueDataFrame_St[CAN_ID_0x1C0].CAN_DataFrame_st.DRV_Data_arru8[0] |
             CAN_QueueDataFrame_St[CAN_ID_0x1C0].CAN_DataFrame_st.DRV_Data_arru8[1] << 8;
    length = CAN_QueueDataFrame_St[CAN_ID_0x1C0].CAN_DataFrame_st.DRV_Data_arru8[2];

    if(offset == 0x00)
    {
    	*ram_ptr =  CAN_QueueDataFrame_St[CAN_ID_0x1C0].CAN_DataFrame_st.DRV_Data_arru8[3] |
    				CAN_QueueDataFrame_St[CAN_ID_0x1C0].CAN_DataFrame_st.DRV_Data_arru8[4] << 8;
        ram_ptr++;
        cnt++;
    }
    else if(offset == 0x02)
	{
    	*(int16_t *)ram_ptr =  CAN_QueueDataFrame_St[CAN_ID_0x1C0].CAN_DataFrame_st.DRV_Data_arru8[3] |
    				CAN_QueueDataFrame_St[CAN_ID_0x1C0].CAN_DataFrame_st.DRV_Data_arru8[4] << 8;

    	ram_ptr++;
    	cnt++;
	}
    else if(offset == 0x04)
    {
        var.hexVal = CAN_QueueDataFrame_St[CAN_ID_0x1C0].CAN_DataFrame_st.DRV_Data_arru8[3] |
                     CAN_QueueDataFrame_St[CAN_ID_0x1C0].CAN_DataFrame_st.DRV_Data_arru8[4] << 8 |
                     CAN_QueueDataFrame_St[CAN_ID_0x1C0].CAN_DataFrame_st.DRV_Data_arru8[5] << 16 |
                     CAN_QueueDataFrame_St[CAN_ID_0x1C0].CAN_DataFrame_st.DRV_Data_arru8[6] << 24;
        *(float *)ram_ptr = var.fval;
        ram_ptr += 2;
        cnt++;
    }

    if(cnt == 0x03)
    {
        cnt = 0;
        input_data1 = 0x01;
        ram_ptr = (uint16_t *)TEST_RAM_ADDRESS;
        dataVar.TEST_datatype_u16 = *(uint16_t *)ram_ptr;
        ram_ptr++;
        dataVar.TEST_datatype_i16 = *(int16_t *)ram_ptr;
        ram_ptr++;
        dataVar.TEST_datatype_f32 = *(float *)ram_ptr;
        ram_ptr = (uint16_t *)TEST_RAM_ADDRESS;
        Test_InpurDataUpdate_gv();
    }
}

static void Test_FaultTypesResponseFrame_gv();
void Test_ResponseFrame_gv(Response_automationFramework_tst* ResponseData_argst)
{
    Test_FaultTypesResponseFrame_gv();
}

static void Test_FaultTypesResponseFrame_gv()
{
    Common_Memset_gv(CAN_QueueDataFrame_St[CAN_ID_0x1E0].CAN_DataFrame_st.DRV_Data_arru8, 0,
           sizeof(CAN_QueueDataFrame_St[CAN_ID_0x1E0].CAN_DataFrame_st.DRV_Data_arru8));

    APP_FaultStatus_tst* Secondary_faultStatus = APP_GetFaultStatus_gst(APP_SECONDARY_PROTECTION, APP_SECONDARY_VOLT_ALERT);
    if(APP_CHECK_FAULT_BIT(Secondary_faultStatus, APP_BATTERY_OVER_THRESHOLD_BIT))
    {
        CAN_QueueDataFrame_St[CAN_ID_0x1E0].CAN_DataFrame_st.DRV_Data_arru8[0] = 1;
    }
    else if(APP_CHECK_FAULT_BIT(Secondary_faultStatus, APP_BATTERY_UNDER_THRESHOLD_BIT))
    {
        CAN_QueueDataFrame_St[CAN_ID_0x1E0].CAN_DataFrame_st.DRV_Data_arru8[1] = 1;
    }

    Secondary_faultStatus = APP_GetFaultStatus_gst(APP_SECONDARY_PROTECTION, APP_SECONDARY_CURRENT_ALERT);
    if(APP_CHECK_FAULT_BIT(Secondary_faultStatus, APP_BATTERY_OVER_THRESHOLD_BIT))
    {
        CAN_QueueDataFrame_St[CAN_ID_0x1E0].CAN_DataFrame_st.DRV_Data_arru8[2] = 1;
    }
    else if(APP_CHECK_FAULT_BIT(Secondary_faultStatus, APP_BATTERY_UNDER_THRESHOLD_BIT))
    {
        CAN_QueueDataFrame_St[CAN_ID_0x1E0].CAN_DataFrame_st.DRV_Data_arru8[3] = 1;
    }

    Secondary_faultStatus = APP_GetFaultStatus_gst(APP_SECONDARY_PROTECTION, APP_SECONDARY_TEMP_ALERT);
    if(APP_CHECK_FAULT_BIT(Secondary_faultStatus, APP_BATTERY_OVER_THRESHOLD_BIT))
    {
        CAN_QueueDataFrame_St[CAN_ID_0x1E0].CAN_DataFrame_st.DRV_Data_arru8[4] = 1;
    }
    else if(APP_CHECK_FAULT_BIT(Secondary_faultStatus, APP_BATTERY_UNDER_THRESHOLD_BIT))
    {
        CAN_QueueDataFrame_St[CAN_ID_0x1E0].CAN_DataFrame_st.DRV_Data_arru8[5] = 1;
    }

    DRV_CAN_ConfigTxBuffer_gen(DRV_CAN_INSTANCE_1, CAN_QueueDataFrame_St[CAN_ID_0x1E0].CAN_BufferIdx_en, &CAN_QueueDataFrame_St[CAN_ID_0x1E0].CAN_DataFrame_st, CAN_QueueDataFrame_St[CAN_ID_0x1E0].CAN_DataFrame_st.DRV_CanId_u32);
    DRV_CAN_TransmitBlock_gen(DRV_CAN_INSTANCE_1, CAN_QueueDataFrame_St[CAN_ID_0x1E0].CAN_BufferIdx_en, &CAN_QueueDataFrame_St[CAN_ID_0x1E0].CAN_DataFrame_st);
}

void Test_InpurDataUpdate_gv(void)
{
    APP_GlobleProtectionLevelFaults_St.APP_InputMeasument_St[APP_SECONDARY_TEMP_ALERT].APP_MaxInputData_i16 = dataVar.TEST_datatype_f32;
    APP_GlobleProtectionLevelFaults_St.APP_InputMeasument_St[APP_SECONDARY_TEMP_ALERT].APP_MinInputData_i16 = dataVar.TEST_datatype_f32;
    APP_GlobleProtectionLevelFaults_St.APP_InputMeasument_St[APP_SECONDARY_VOLT_ALERT].APP_MaxInputData_i16 = dataVar.TEST_datatype_u16;
    APP_GlobleProtectionLevelFaults_St.APP_InputMeasument_St[APP_SECONDARY_VOLT_ALERT].APP_MinInputData_i16 = dataVar.TEST_datatype_u16;
    APP_GlobleProtectionLevelFaults_St.APP_InputMeasument_St[APP_SECONDARY_CURRENT_ALERT].APP_MaxInputData_i16 = dataVar.TEST_datatype_i16;
    APP_GlobleProtectionLevelFaults_St.APP_InputMeasument_St[APP_SECONDARY_CURRENT_ALERT].APP_MinInputData_i16 = 0;
}
