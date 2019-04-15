/**
 ******************************************************************************
 * file    ai_server_data.c
 * author  sunlingge
 * version V100
 * brief   the ai server data.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_server_global.h"


#if (AI_PRODUCT_OPTION_SERVER == 1)
/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/
AI_SERVER_DATA_RW g_ai_server_data_rw;

#if (AI_PRODUCT_OPTION_WIN32 == 1)
AI_SERVER_DATA_RO g_ai_server_data_ro;

ERROR_T AiServerDataRoConstInit(void) {

	return AI_ERROR_SUCCESS;
}
#else
const AI_SERVER_DATA_RO g_ai_server_data_ro = {
};
#endif

/*------------------------- Function Definition -----------------------------*/
/**
 * brief    ai server data previous init.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiServerDataInitPrevious(void) {
#if (AI_PRODUCT_OPTION_WIN32 == 1)
	AiServerDataRoConstInit();
#endif

    return AI_ERROR_SUCCESS;
}

/**
 * brief    ai server data post init.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiServerDataInitPost(void) {
    return AI_ERROR_SUCCESS;
}

#endif
/*------------------------- End ---------------------------------------------*/
