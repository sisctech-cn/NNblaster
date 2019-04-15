/**
 ******************************************************************************
 * file    ai_server_sdk.c
 * author  sunlingge
 * version V100
 * brief   the ai center sdk.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_server_global.h"

#if (AI_PRODUCT_OPTION_SERVER == 1)
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
ERROR_T AiServerSdkIndexDataGet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length) {
    switch (index) {
    default:
        break;
    }

    return AI_ERROR_SUCCESS;
}

/**
 * brief    ai sdk data get.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiServerSdkIndexDataSet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length) {
    switch (index) {
    default:
        break;
    }

    return AI_ERROR_SUCCESS;
}

/**
 * brief    ai sdk data get.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiServerBaseSdkIndexDataGet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length) {
    return AI_ERROR_SUCCESS;
}

/**
 * brief    ai sdk data get.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiServerBaseSdkIndexDataSet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length) {
    return AI_ERROR_SUCCESS;
}

/**
 * brief    ai sdk data get.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiServerManageSdkIndexDataGet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length) {
    return AI_ERROR_SUCCESS;
}

/**
 * brief    ai sdk data get.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiServerManageSdkIndexDataSet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length) {
    return AI_ERROR_SUCCESS;
}

#endif
