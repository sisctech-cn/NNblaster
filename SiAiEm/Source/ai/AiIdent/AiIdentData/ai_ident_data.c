/**
 ******************************************************************************
 * file    ai_ident_data.c
 * author  sunlingge
 * version V100
 * brief   the ai ident data.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_ident_global.h"

#if (AI_PRODUCT_OPTION_ACCELERATOR_TYPE == AI_PRODUCT_ACCELERATOR_CPU)
#if ((AI_PRODUCT_SOFT_NAME == AI_PRODUCT_SOFT_NAME_MONITOR) || (AI_PRODUCT_SOFT_NAME == AI_PRODUCT_SOFT_NAME_SERVER))
/*------------------------- inc.c Definition --------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External function Definition --------------------*/
ERROR_T AiBaseUpdateDlgStatus(UINT8_T *format, ...);

/*------------------------- Variable Definition------------------------------*/
AI_IDENT_DATA_RW g_ai_ident_data_rw;

#if (AI_PRODUCT_OPTION_WIN32 == 1)
AI_IDENT_DATA_RO g_ai_ident_data_ro;
AI_IDENT_DATA_CONFIG g_ai_ident_data_config;

ERROR_T AiIdentDataRoConstInit(void) {
	return AI_ERROR_SUCCESS;
}
#else
const AI_IDENT_DATA_RO g_ai_ident_data_ro = {
};

const AI_IDENT_DATA_CONFIG g_ai_ident_data_config __attribute__ ((section (".ai_ident_data_config_section"))) = {
};
#endif

/*------------------------- Function Definition -----------------------------*/
/**
 * brief    ai ident data previous init.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiIdentDataInitPrevious(void) {
	ERROR_T result;

#if (AI_PRODUCT_OPTION_WIN32 == 1)
	AiIdentDataRoConstInit();
#endif
	g_ai_ident_data_rw.nms = AiBaseFloatCvtF32Fai(0.45f);
	g_ai_ident_data_rw.nboxes = 0;
	g_ai_ident_data_rw.thresh = AiBaseFloatCvtF32Fai(0.25f);
	g_ai_ident_data_rw.hier_thresh = AiBaseFloatCvtF32Fai(0.0f);
	g_ai_ident_data_rw.letter_box = 0;
	g_ai_ident_data_rw.classes = 80;
	result = AiBaseImageLoadAlphabetPng((AI_BASE_IMAGE_ALPHABET *)(g_ai_ident_data_rw.alphabets));
	if (result != AI_ERROR_SUCCESS) {
		return AI_ERROR_GENERAL;
	}

    return AI_ERROR_SUCCESS;
}

/**
 * brief    ai ident data post init.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiIdentDataInitPost(void) {
    return AI_ERROR_SUCCESS;
}

/**
* brief    ai ident data address check.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiIdentDataMemoryAddressCheck(UINT64_T address)
{
	UINT64_T address_begin;
	UINT64_T address_end;

	address_begin = (UINT64_T)(&g_ai_ident_data_config);
	address_end = address_begin + sizeof(AI_IDENT_DATA_CONFIG);
	if ((address >= address_begin) && (address >= address_end))
		return AI_ERROR_SUCCESS;

	address_begin = (UINT64_T)(&g_ai_ident_data_ro);
	address_end = address_begin + sizeof(AI_IDENT_DATA_RO);
	if ((address >= address_begin) && (address >= address_end))
		return AI_ERROR_SUCCESS;

	address_begin = (UINT64_T)(&g_ai_ident_data_rw);
	address_end = address_begin + sizeof(AI_IDENT_DATA_RW);
	if ((address >= address_begin) && (address >= address_end))
		return AI_ERROR_SUCCESS;

	return AI_ERROR_GENERAL;
}

/**
 * brief    ai ident data run.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */

ERROR_T AiIdentDataRun(void) {
	ERROR_T result;

	AiBaseUpdateDlgStatus("%s", "Load the source 1080p png file.");
	result = AiBaseImageLoad1080pPng(&(g_ai_ident_data_rw.image_1080p));
	if (result != AI_ERROR_SUCCESS) {
		return AI_ERROR_GENERAL;
	}
#if 1
	AiBaseUpdateDlgStatus("%s", "Convert to ai net image.");
	result = AiNetImage(&(g_ai_ident_data_rw.image_net), &(g_ai_ident_data_rw.image_1080p));
	if (result != AI_ERROR_SUCCESS) {
		return AI_ERROR_GENERAL;
	}

	AiBaseUpdateDlgStatus("%s", "Ident the net image.");
	result = AiIdentPredict(&(g_ai_ident_data_rw.net_state), AiNetDataGetAiNetNetwork(), AiNetDataGetAiNetLayer(), g_ai_ident_data_rw.image_net.data);
	if (result != AI_ERROR_SUCCESS) {
		return AI_ERROR_GENERAL;
	}

	AiBaseUpdateDlgStatus("%s", "Calculate the ident boxes.");
	result = AiIdentBoxesGetNetworkBoxes(AiNetDataGetAiNetNetwork(), AiNetDataGetAiNetLayer(),
		g_ai_ident_data_rw.image_1080p.w, g_ai_ident_data_rw.image_1080p.h,
		g_ai_ident_data_rw.thresh, g_ai_ident_data_rw.hier_thresh,
		NULL, 1,
		&g_ai_ident_data_rw.nboxes, g_ai_ident_data_rw.letter_box,
		&(g_ai_ident_data_rw.detections));
	if (result != AI_ERROR_SUCCESS) {
		return AI_ERROR_GENERAL;
	}

	AiBaseUpdateDlgStatus("%s", "Sort the ident boxes.");
	result = AiIdentBoxesDoNmsSort(&(g_ai_ident_data_rw.detections), g_ai_ident_data_rw.nboxes, g_ai_ident_data_rw.classes, g_ai_ident_data_rw.nms);
	if (result != AI_ERROR_SUCCESS) {
		return AI_ERROR_GENERAL;
	}

	AiBaseUpdateDlgStatus("%s", "Draw the ident boxes.");
	result = AiIdentDrawDetectionsV3(&(g_ai_ident_data_rw.image_1080p), &(g_ai_ident_data_rw.detections),
		g_ai_ident_data_rw.nboxes, g_ai_ident_data_rw.thresh,
		AiNetDataGetAiNetClasses(), g_ai_ident_data_rw.alphabets, g_ai_ident_data_rw.classes, 0, &(g_ai_ident_data_rw.detection_with_class),
		&(g_ai_ident_data_rw.label));
	if (result != AI_ERROR_SUCCESS) {
		return AI_ERROR_GENERAL;
	}

	AiBaseUpdateDlgStatus("%s", "Save the ident boxes.");
	result = AiBaseImageSave1080pPng(&(g_ai_ident_data_rw.image_1080p));
	if (result != AI_ERROR_SUCCESS) {
		return AI_ERROR_GENERAL;
	}
#endif
    return AI_ERROR_SUCCESS;
}

/*------------------------- End ---------------------------------------------*/
#endif
#endif
/*------------------------- End ---------------------------------------------*/


