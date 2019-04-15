/**
 ******************************************************************************
 * file    ai_base_data.c
 * author  sunlingge
 * version V100
 * brief   the ai base data.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/
AI_BASE_DATA_RW g_ai_base_data_rw;

#if (AI_PRODUCT_OPTION_WIN32 == 1)
AI_BASE_DATA_RO g_ai_base_data_ro;
AI_BASE_DATA_CONFIG g_ai_base_data_config;

ERROR_T AiBaseDataRoConstInit(void)  {
	return AI_ERROR_SUCCESS;
}
#else
const AI_BASE_DATA_RO g_ai_base_data_ro = {
};
#if (AI_PRODUCT_SOFT_NAME == AI_PRODUCT_SOFT_NAME_BOOT)
const AI_BASE_DATA_CONFIG g_ai_base_data_config __attribute__ ((section (".ai_boot_base_data_config_section"))) = {
#endif
#if (AI_PRODUCT_SOFT_NAME == AI_PRODUCT_SOFT_NAME_MONITOR)
const AI_BASE_DATA_CONFIG g_ai_base_data_config __attribute__ ((section (".ai_base_data_config_section"))) = {
#endif
};
#endif

/*------------------------- Function Definition -----------------------------*/
/**
 * brief    ai base data previous init.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseDataInitPrevious(void) {
#if (AI_PRODUCT_OPTION_WIN32 == 1)
	AiBaseDataRoConstInit();
#endif
    return AI_ERROR_SUCCESS;
}

/**
 * brief    ai base data post init.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseDataInitPost(void) {
    return AI_ERROR_SUCCESS;
}



/*------------------------- End ---------------------------------------------*/
