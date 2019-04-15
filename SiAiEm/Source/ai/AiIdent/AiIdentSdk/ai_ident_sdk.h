/**
 ******************************************************************************
 * file    ai_ident_sdk.h
 * author  sunlingge
 * version V100
 * brief   the ai ident software development kit.
 ******************************************************************************
 */
#ifndef AI_IDENT_SDK_H
#define AI_IDENT_SDK_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Type Definition --------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiIdentSdkIndexDataGet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length);
ERROR_T AiIdentSdkIndexDataSet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
