/**
 ******************************************************************************
 * file    ai_server_tick_test.c
 * author  sunlingge
 * version V100
 * brief   the ai server tick test.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "StdAfx.h"
#include "SiAiGlobal.h"

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/
AI_SERVER_TICK_TEST g_ai_server_tick_test = { 0 };
UINT64_T g_ai_server_protect_start = 0x5a5a5a5a5a5a5a5a;
CSemaphore g_ai_server_tick_hSemphore(1, 1);
UINT64_T g_ai_server_protect_end = 0xa5a5a5a5a5a5a5a5;

/*------------------------- Function Definition -----------------------------*/
void AiServerTickTestAdd(CString string) {
	CString temp;

	g_ai_server_tick_hSemphore.Lock();
	temp = string.Left(AI_SERVER_TICKET_TEST_POSITION_LENGTH - 1);
	memset(g_ai_server_tick_test.data[(g_ai_server_tick_test.pos % AI_SERVER_TICKET_TEST_ARRAY_NUMBER)].file_line, 0, AI_SERVER_TICKET_TEST_POSITION_LENGTH);
	memcpy(g_ai_server_tick_test.data[(g_ai_server_tick_test.pos % AI_SERVER_TICKET_TEST_ARRAY_NUMBER)].file_line, temp.GetBuffer(), temp.GetLength());
	temp.ReleaseBuffer();
	g_ai_server_tick_test.data[(g_ai_server_tick_test.pos % AI_SERVER_TICKET_TEST_ARRAY_NUMBER)].tick = GetTickCount();
	g_ai_server_tick_test.pos = (g_ai_server_tick_test.pos + 1) % AI_SERVER_TICKET_TEST_ARRAY_NUMBER;
	g_ai_server_tick_hSemphore.Unlock();

	return;
}

/*------------------------- End ---------------------------------------------*/
