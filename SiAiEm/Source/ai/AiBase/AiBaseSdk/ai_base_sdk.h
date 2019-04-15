/**
 ******************************************************************************
 * file    ai_base_sdk.h
 * author  sunlingge
 * version V100
 * brief   the ai base software development kit.
 ******************************************************************************
 */
#ifndef AI_BASE_SDK_H
#define AI_BASE_SDK_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Type Definition --------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiBaseSdkIndexDataGet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length);
ERROR_T AiBaseSdkIndexDataSet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
