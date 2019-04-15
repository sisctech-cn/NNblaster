/**
 ******************************************************************************
 * file    ai_base_time.c
 * author  sunlingge
 * version V100
 * brief   the ai base time driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/

/*------------------------- Function Definition -----------------------------*/
/**
 * brief    get the tick.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
#if (AI_PRODUCT_SOFT_NAME == AI_PRODUCT_SOFT_NAME_SERVER)
UINT64_T AiServerTickGet(void);
#endif
UINT64_T AiBaseTickGet(void) {
#if (AI_PRODUCT_SOFT_NAME == AI_PRODUCT_SOFT_NAME_SERVER)
	return AiServerTickGet();
#else
    return (*g_sys_tick_ptr);
#endif
}

/*------------------------- End ---------------------------------------------*/
