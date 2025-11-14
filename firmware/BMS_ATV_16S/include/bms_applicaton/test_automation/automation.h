/*
 * automation.h
 *
 *  Created on: 10-Nov-2025
 *      Author: PandurangaKarapothul
 */

#ifndef BMS_APPLICATON_AUTOMATION_H_
#define BMS_APPLICATON_AUTOMATION_H_
#include "common.h"
#include "can_comm.h"

#define TEST_RAM_ADDRESS 0x20000000

typedef struct {
	U16 TEST_datatype_u16;
	I16 TEST_datatype_i16;
	F32 TEST_datatype_f32;
}TEST_automationFramework_tst;
typedef struct {
	U16 TEST_datatype_u16;
	I16 TEST_datatype_i16;
	F32 TEST_datatype_f32;
}Response_automationFramework_tst;
typedef enum
{
	DATA_BYTES_U16,
	DATA_BYTES_I16,
	DATA_BYTES_F32
}TEST_DataBytes_ten;
typedef union {
	uint32_t hexVal;
	float fval;
} __attribute__((packed)) converter;

extern uint32_t input_data1;

extern TEST_automationFramework_tst dataVar;
extern Response_automationFramework_tst Responsedata_gst;
void TEST_ReadTestCases_gv(void);
void Test_ResponseFrame_gv(Response_automationFramework_tst* ResponseData_argst);
void Test_InpurDataUpdate_gv(void);
#endif /* BMS_APPLICATON_AUTOMATION_H_ */
