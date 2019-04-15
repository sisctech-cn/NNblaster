/**
 ******************************************************************************
 * file    ai_sdk.c
 * author  sunlingge
 * version V100
 * brief   the ai sdk.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_server_global.h"

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/
#if (AI_PRODUCT_OPTION_MODBUS == 1)
AI_DAT_MODBUS_INDEX g_ai_data_modbus_index = { 0 };
#endif

const AI_DAT g_ai_data[AI_DAT_INDEX_N] = {
AI_DAT_DEFINE(UNIT_N),
AI_DAT_DEFINE(UNIT_VALID),
AI_DAT_DEFINE(UNIT_ADDR),
AI_DAT_DEFINE(UNIT_TYPE),
AI_DAT_DEFINE(PN),
AI_DAT_DEFINE(SN),
AI_DAT_DEFINE(HARD_VER),
AI_DAT_DEFINE(SOFT_VER),
};

/*------------------------- Function Definition -----------------------------*/
/**
 * brief    Ai sdk data type get.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiSdkIndexDataTypeCategoryGet(UINT16_T index, UINT8_T *category) {
	if (index >= AI_DAT_INDEX_N)
		return AI_ERROR_GENERAL;
	if (g_ai_data[index].index != index)
		return AI_ERROR_GENERAL;

	*category = g_ai_data[index].category;

	return AI_ERROR_SUCCESS;

}

#if ((AI_PRODUCT_OPTION_MENU_ENABLE == 1) || (AI_PRODUCT_OPTION_SERVER == 1))
/**
 * brief    Ai sdk data type get.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiSdkIndexDataTypeMenuGet(UINT16_T index, UINT8_T **name, UINT8_T **description, UINT8_T **unit, UINT8_T **comment) {
    return AI_ERROR_SUCCESS;
}
#endif

#if (AI_PRODUCT_OPTION_MENU_ENABLE == 2)
/**
 * brief    Ai sdk data type get.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiSdkIndexDataTypeMenuGet(UINT16_T index, UINT8_T **name, UINT8_T **description, UINT8_T **unit, UINT8_T **comment) {
    if (index >= AI_DAT_INDEX_N)
        return AI_ERROR_GENERAL;
    if (g_ai_data[index].index != index)
        return AI_ERROR_GENERAL;

    *name = g_ai_data[index].name;
    *description = NULL;
    *unit = g_ai_data[index].unit;
    *comment = NULL;

    return AI_ERROR_SUCCESS;
}
#endif


/**
 * brief    Ai sdk data get.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiSdkIndexUnitGet(UINT8_T address, UINT16_T index, UINT64_T sub_index, UINT8_T *unit) {
    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai sdk data type get.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiSdkIndexDataTypeGet(UINT16_T index, UINT64_T *sub_index, UINT8_T *sub_data_type, UINT64_T *value_min, UINT64_T *value_max, UINT8_T *category, UINT8_T *operation) {
    if (index >= AI_DAT_INDEX_N)
        return AI_ERROR_GENERAL;
    if (g_ai_data[index].index != index)
        return AI_ERROR_GENERAL;

    *sub_index = g_ai_data[index].sub_n;
    *sub_data_type = g_ai_data[index].sub_data_type;
    *value_min = g_ai_data[index].min;
    *value_max = g_ai_data[index].max;
    *category = g_ai_data[index].category;
    *operation = g_ai_data[index].operation;

    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai sdk data get.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiSdkIndexDataGet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length) {
    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai sdk data set.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiSdkIndexDataSet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length) {
    return AI_ERROR_SUCCESS;
}

#if (AI_PRODUCT_OPTION_MODBUS == 1)
/**
 * brief    Ai sdk data internal index to modbus index get.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiSdkInternalIndexToModbusIndex(UINT16_T internal_index, UINT64_T internal_sub_index, UINT16_T *modbus_index, UINT16_T *modbus_number)
{
    return AI_ERROR_SUCCESS;
}


/**
 * brief    Ai sdk data modbus index to internal index get.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiSdkModbusIndexToInternalIndex(UINT16_T modbus_index, UINT16_T *internal_index, UINT16_T *internal_sub_index, UINT16_T *internal_sub_offset)
{
	return AI_ERROR_GENERAL;
}

/**
 * brief    Ai sdk data modbus index to internal index get.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiSdkModbusIndexGet(UINT16_T modbus_index, UINT16_T *modbus_s_index, UINT16_T *modbus_e_index)
{
    return AI_ERROR_GENERAL;
}

/**
 * brief    Ai sdk data modbus internal address get.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiSdkModbusAddressGet(UINT16_T internal_index, UINT8_T sdk_category, UINT8_T source_address, UINT8_T dest_address, UINT8_T *internal_source_address, UINT8_T *internal_dest_address) {
	return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai sdk data type get.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiSdkModbusIndexDataTypeGet(UINT16_T index, UINT64_T *sub_index, UINT8_T *sub_data_type, UINT64_T *value_min, UINT64_T *value_max, UINT8_T *category, UINT8_T *operation) {
    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai sdk data get.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiSdkModbusIndexDataGet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length) {
    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai sdk data set.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiSdkModbusIndexDataSet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length) {
    return AI_ERROR_SUCCESS;
}
#endif

AI_DAT_PTR BucSdkGetAiDat(void) {
	return (AI_DAT_PTR)g_ai_data;
}

/*------------------------- END ------------------------------------------------*/

