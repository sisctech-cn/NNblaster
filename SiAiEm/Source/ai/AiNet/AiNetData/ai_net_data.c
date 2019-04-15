/**
 ******************************************************************************
 * file    ai_net_data.c
 * author  sunlingge
 * version V100
 * brief   the ai net data.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_net_global.h"

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/
AI_NET_DATA_RW g_ai_net_data_rw;

#if (AI_PRODUCT_OPTION_WIN32 == 1)
AI_NET_DATA_RO g_ai_net_data_ro;
AI_NET_DATA_CONFIG g_ai_net_data_config;

ERROR_T AiNetDataRoConstInit(void)  {
	return AI_ERROR_SUCCESS;
}
#else
const AI_NET_DATA_RO g_ai_net_data_ro = {
};
const AI_NET_DATA_CONFIG g_ai_net_data_config __attribute__ ((section (".ai_net_data_config_section"))) = {
};
#endif

/*------------------------- Function Definition -----------------------------*/
/**
 * brief    ai net data previous init.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiNetDataInitPrevious(void) {
	ERROR_T result;

#if (AI_PRODUCT_OPTION_WIN32 == 1)
	AiNetDataRoConstInit();
#endif
	result = AiNetClassesLoad(AiNetDataGetAiNetClasses());
	if (result != AI_ERROR_SUCCESS) {
		AiBaseLogErrorCritial();
		return result;
	}
	

	result = AiNetConfigLoad(AiNetDataGetAiNetConfig());
	if (result != AI_ERROR_SUCCESS) {
		AiBaseLogErrorCritial();
		return result;
	}

	result = AiNetNetworkInit(AiNetDataGetAiNetNetwork(), AiNetDataGetAiNetConfig());
	if (result != AI_ERROR_SUCCESS) {
		AiBaseLogErrorCritial();
		return result;
	}

	result = AiNetLayerInit(AiNetDataGetAiNetLayer(), AiNetDataGetAiNetConfig());
	if (result != AI_ERROR_SUCCESS) {
		AiBaseLogErrorCritial();
		return result;
	}

#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	result = AiNetWeightsLoad(AiNetDataGetAiNetNetwork(), AiNetDataGetAiNetLayer());
	if (result != AI_ERROR_SUCCESS) {
		AiBaseLogErrorCritial();
		return result;
	}
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	result = AiNetWeightsF16Load(AiNetDataGetAiNetNetwork(), AiNetDataGetAiNetLayer());
	if (result != AI_ERROR_SUCCESS) {
		AiBaseLogErrorCritial();
		return result;
	}
#endif

    return AI_ERROR_SUCCESS;
}

/**
 * brief    ai net data post init.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiNetDataInitPost(void) {
    return AI_ERROR_SUCCESS;
}

/**
* brief    ai net data address check.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetDataMemoryAddressCheck(UINT64_T address) 
{
	UINT64_T address_begin;
	UINT64_T address_end;

	address_begin = (UINT64_T)(&g_ai_net_data_config);
	address_end = address_begin + sizeof(AI_NET_DATA_CONFIG);
	if ((address >= address_begin) && (address >= address_end))
		return AI_ERROR_SUCCESS;

	address_begin = (UINT64_T)(&g_ai_net_data_ro);
	address_end = address_begin + sizeof(AI_NET_DATA_RO);
	if ((address >= address_begin) && (address >= address_end))
		return AI_ERROR_SUCCESS;

	address_begin = (UINT64_T)(&g_ai_net_data_rw);
	address_end = address_begin + sizeof(AI_NET_DATA_RW);
	if ((address >= address_begin) && (address >= address_end))
		return AI_ERROR_SUCCESS;

	return AI_ERROR_GENERAL;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
AI_NET_CLASSES *AiNetDataGetAiNetClasses(void) {
	return (AI_NET_CLASSES *)&(g_ai_net_data_config.net_classes);
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
AI_NET_CONFIG *AiNetDataGetAiNetConfig(void) {
	return (AI_NET_CONFIG *)&(g_ai_net_data_config.net_config);
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
AI_NET_NETWORK *AiNetDataGetAiNetNetwork(void) {
	return (AI_NET_NETWORK *)&(g_ai_net_data_config.net_network);
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
AI_NET_LAYER *AiNetDataGetAiNetLayer(void) {
	return (AI_NET_LAYER *)&(g_ai_net_data_config.net_layer);
}


/*------------------------- End ---------------------------------------------*/
