/**
 ******************************************************************************
 * file    ai_manage_data.c
 * author  sunlingge
 * version V100
 * brief   the ai manage data.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_manage_global.h"

#if ((AI_PRODUCT_SOFT_NAME == AI_PRODUCT_SOFT_NAME_MONITOR) || (AI_PRODUCT_SOFT_NAME == AI_PRODUCT_SOFT_NAME_SERVER))
/*------------------------- inc.c Definition --------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/
AI_MANAGE_DATA_RW g_ai_manage_data_rw;

#if (AI_PRODUCT_OPTION_WIN32 == 1)
AI_MANAGE_DATA_RO g_ai_manage_data_ro;
AI_MANAGE_DATA_CONFIG g_ai_manage_data_config;

ERROR_T AiManageDataRoConstInit(void) {
	g_ai_manage_data_ro.root.func_code = AI_BASE_DETECT_ROOT_DETECTOR;
	g_ai_manage_data_ro.detect.phase = AI_BASE_DETECT_PHASE_IDENT;
	g_ai_manage_data_ro.ident.data_sets = AI_BASE_DETECT_DATASETS_COCO;
	g_ai_manage_data_ro.ident.net = AI_BASE_DETECT_NET_YOLOV3;
	g_ai_manage_data_ro.ident.weights = AI_BASE_DETECT_WEIGHTS_YOLOV3;
	g_ai_manage_data_ro.ident.source = AI_BASE_DETECT_SOURCE_IMAGE;
	g_ai_manage_data_ro.ident.confidence_threshold = AiBaseFloatCvtF32Fai(AI_BASE_DETECT_IDENT_CONFIDENCE_THRESHOLD_DEFAULT);

	return AI_ERROR_SUCCESS;
}
#else
const AI_MANAGE_DATA_RO g_ai_manage_data_ro = {
	.root = {
		.func_code = AI_BASE_DETECT_ROOT_DETECTOR,
	},
	.detect = {
		.phase = AI_BASE_DETECT_PHASE_IDENT,
	},
	.ident = {
		.data_sets = AI_BASE_DETECT_DATASETS_COCO,
		.net = AI_BASE_DETECT_NET_YOLOV3,
		.weigths = AI_BASE_DETECT_WEIGHTS_YOLOV3_TINY,
		.source = AI_BASE_DETECT_SOURCE_IMAGE,
		.confidence_threshold = AI_BASE_DETECT_IDENT_CONFIDENCE_THRESHOLD_DEFAULT,
	},
};

const AI_MANAGE_DATA_CONFIG g_ai_manage_data_config __attribute__ ((section (".ai_manage_data_config_section"))) = {
};
#endif

/*------------------------- Function Definition -----------------------------*/
/**
 * brief    ai manage data previous init.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiManageDataInitPrevious(void) {
#if (AI_PRODUCT_OPTION_WIN32 == 1)
	AiManageDataRoConstInit();
#endif
    return AI_ERROR_SUCCESS;
}

/**
 * brief    ai manage data post init.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiManageDataInitPost(void) {
    return AI_ERROR_SUCCESS;
}

/**
 * brief    ai manage data run.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiManageDataRun(void) {
    return AI_ERROR_SUCCESS;
}

/*------------------------- End ---------------------------------------------*/
#endif
/*------------------------- End ---------------------------------------------*/


