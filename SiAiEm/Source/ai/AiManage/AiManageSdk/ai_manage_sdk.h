/**
 ******************************************************************************
 * file    ai_manage_sdk.h
 * author  sunlingge
 * version V100
 * brief   the ai manage software development kit.
 ******************************************************************************
 */
#ifndef AI_MANAGE_SDK_H
#define AI_MANAGE_SDK_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Type Definition --------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiManageSdkIndexDataGet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length);
ERROR_T AiManageSdkIndexDataSet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
