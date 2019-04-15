/**
 ******************************************************************************
 * file    ai_base_sdk.c
 * author  sunlingge
 * version V100
 * brief   the ai base sdk.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/
#if (AI_PRODUCT_OPTION_MODBUS == 1)
AI_DAT_MODBUS_INDEX g_ai_data_modbus_index;
#endif

/*------------------------- Variable Definition------------------------------*/

/*------------------------- Function Definition -----------------------------*/
/**
 * brief    ai sdk data get.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseSdkIndexDataGet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length) {
    return AI_ERROR_SUCCESS;
}


/**
 * brief    ai sdk data get.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseSdkIndexDataSet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length) {
    return AI_ERROR_SUCCESS;
}



