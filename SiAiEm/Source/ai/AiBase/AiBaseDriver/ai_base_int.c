/**
 ******************************************************************************
 * file    ai_base_int.c
 * author  sunlingge
 * version V100
 * brief   the ai base int driver.
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
 * brief  	ai int disable.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseIntDisable(void) {
#if (AI_PRODUCT_OPTION_WIN32 == 0)
    if (AiBaseSystemIntCheck() == AI_ERROR_SUCCESS) {
        __disable_irq();
    }
#endif
    return AI_ERROR_SUCCESS;
}

/**
 * brief  	ai int enable.
 * param  	None
 * retval 	None
 * author	Sunlingge
 * comment  V100
 */
ERROR_T AiBaseIntEnable(void) {
#if (AI_PRODUCT_OPTION_WIN32 == 0)
    if (AiBaseSystemIntCheck() == AI_ERROR_SUCCESS) {
        __enable_irq();
    }
#endif
	return AI_ERROR_SUCCESS;
}

/*------------------------- End ---------------------------------------------*/
