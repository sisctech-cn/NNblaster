/**
 ******************************************************************************
 * file    ai_server_sdk.h
 * author  sunlingge
 * version V100
 * brief   the ai server software development kit.
 ******************************************************************************
 */
#ifndef AI_SERVER_SDK_H
#define AI_SERVER_SDK_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Type Definition --------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiServerSdkIndexDataGet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length);
ERROR_T AiServerSdkIndexDataSet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length);
ERROR_T AiServerBaseSdkIndexDataGet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length);
ERROR_T AiServerBaseSdkIndexDataSet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length);
ERROR_T AiServerManageSdkIndexDataGet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length);
ERROR_T AiServerManageSdkIndexDataSet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
