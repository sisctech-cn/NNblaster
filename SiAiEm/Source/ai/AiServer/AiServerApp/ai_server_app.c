/**
 ******************************************************************************
 * file    ai_server_app.c
 * author  sunlingge
 * version V100
 * brief   the ai server application.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_server_global.h"

#if (AI_PRODUCT_OPTION_SERVER == 1)
/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/
UINT8_T g_ai_server_init_flag = AI_SERVER_APP_INIT_NONE;

/*------------------------- Function Definition -----------------------------*/
/**
 * brief    ai server app main.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
#if (AI_PRODUCT_OPTION_WIN32 == 0)
int main(void) {
    AiServerAppInitPrevious();
    AiServerAppInitPost();
    AiServerAppRun();

    return 0;
}
#else
int AiMain(void) {
#if (AI_PRODUCT_OPTION_FP16_TEST == 1)
	AiBaseFloat16TestMain();
	return 0;
#endif
#if (AI_PRODUCT_OPTION_FP16_TEST != 1)
	//	if (g_ai_server_init_flag == AI_SERVER_APP_INIT_NONE) 
	{
		AiServerAppInitPrevious();
		AiServerAppInitPost();
		g_ai_server_init_flag = AI_SERVER_APP_INIT_DONE;
	}

	AiIdentDataRun();

	return 0;
#endif
}

/**
* brief    ai server app main.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
int AiWeightsConvert(void) {
	AiServerAppInitPrevious();
	AiServerAppInitPost();

	AiNetWeightsF16Save(AiNetDataGetAiNetNetwork(), AiNetDataGetAiNetLayer());
	AiBaseLogAccuracyWeights();

	return 0;
}
#endif

/**
 * brief    ai server app init previous.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiServerAppInitPrevious(void) {
	ERROR_T result;

    result = AiBaseDataInitPrevious();
	if (result != AI_ERROR_SUCCESS) {
		AiBaseLogErrorCritial();
		return result;
	}
	result = AiNetDataInitPrevious();
	if (result != AI_ERROR_SUCCESS) {
		AiBaseLogErrorCritial();
		return result;
	}
	result = AiIdentDataInitPrevious();
	if (result != AI_ERROR_SUCCESS) {
		AiBaseLogErrorCritial();
		return result;
	}
    result = AiManageDataInitPrevious();
	if (result != AI_ERROR_SUCCESS) {
		AiBaseLogErrorCritial();
		return result;
	}
	result = AiServerDataInitPrevious();
	if (result != AI_ERROR_SUCCESS) {
		AiBaseLogErrorCritial();
		return result;
	}

    return AI_ERROR_SUCCESS;
}

/**
 * brief    ai server app init previous.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiServerAppInitPost(void) {
	ERROR_T result;

    result = AiBaseDataInitPost();
	if (result != AI_ERROR_SUCCESS) {
		AiBaseLogErrorCritial();
		return result;
	}
	result = AiNetDataInitPost();
	if (result != AI_ERROR_SUCCESS) {
		AiBaseLogErrorCritial();
		return result;
	}
	result = AiIdentDataInitPost();
	if (result != AI_ERROR_SUCCESS) {
		AiBaseLogErrorCritial();
		return result;
	}
    result = AiManageDataInitPost();
	if (result != AI_ERROR_SUCCESS) {
		AiBaseLogErrorCritial();
		return result;
	}
	result = AiServerDataInitPost();
	if (result != AI_ERROR_SUCCESS) {
		AiBaseLogErrorCritial();
		return result;
	}

    return AI_ERROR_SUCCESS;
}

#endif

