/**
 ******************************************************************************
 * file    SiAiSeBaseTickTest.h
 * author  sunlingge
 * version V100
 * brief   the ai base tick test driver.
 ******************************************************************************
 */
#ifndef AI_BASE_TICK_TEST_H
#define AI_BASE_TICK_TEST_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/
typedef struct ai_server_tick_test_data_struct {
	UINT8_T file_line[AI_SERVER_TICKET_TEST_POSITION_LENGTH];
	UINT64_T tick;
} AI_SERVER_TICK_TEST_DATA, *AI_SERVER_TICK_TEST_DATA_PTR;

typedef struct ai_server_tick_test_struct {
	UINT32_T pos;
	AI_SERVER_TICK_TEST_DATA data[AI_SERVER_TICKET_TEST_ARRAY_NUMBER]; 
} AI_SERVER_TICK_TEST, *AI_SERVER_TICL_TEST_PTR;

/*------------------------- Function Definition -----------------------------*/
void AiServerTickTestAdd(CString string);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
