/**
 ******************************************************************************
 * file    ai_ident_sdk.c
 * author  sunlingge
 * version V100
 * brief   the ai ident sdk.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_ident_global.h"

#if ((AI_PRODUCT_SOFT_NAME == AI_PRODUCT_SOFT_NAME_MONITOR) || (AI_PRODUCT_SOFT_NAME == AI_PRODUCT_SOFT_NAME_SERVER))
/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/

/*------------------------- Function Definition -----------------------------*/
/**
 * brief    ai sdk data get.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiIdentSdkIndexDataGet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length) {
    return AI_ERROR_SUCCESS;
}

/**
 * brief    ai sdk data set.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiIdentSdkIndexDataSet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length) {
    return AI_ERROR_SUCCESS;
}
/*------------------------- End Definition --------------------------------*/
#endif
