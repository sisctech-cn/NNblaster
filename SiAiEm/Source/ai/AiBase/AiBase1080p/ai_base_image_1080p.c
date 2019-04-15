/**
 ******************************************************************************
 * file    ai_base_image_1080p.c
 * author  sunlingge
 * version V100
 * brief   the ai base image 1080p driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"
#pragma warning(disable:4996)

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/
INT32_T g_ai_base_image_1080p_content_len;
#pragma pack(push, 16)
UINT8_T g_ai_base_image_1080p_content[AI_BASE_IMAGE_1080P_CONTENT_LENGTH_MAX];
#pragma pack(pop)
#if 0
AI_BASE_IMAGE_1080P g_image_1080p;
#endif

/*------------------------- Function Definition -----------------------------*/
/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiBaseImageF16Load1080pPng(AI_BASE_IMAGE_1080P *image_1080p)
{
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	return AI_ERROR_GENERAL;
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
#if (AI_PRODUCT_OPTION_FP16_TEST_PRELOAD == 1)
	UINT8_T file_path[256];
	FILE *f;
	UINT32_T length;
	UINT32_T read_length;

	sprintf((char *)file_path, AI_PRODUCT_FILE_PATH""AI_PRODUCT_YOLOV3_TEST_DTA_PATH""AI_PRODUCT_YOLOV3_1080P_PNG_FILE_FP16);
	f = fopen(file_path, "rb");
	if (!f) {
		return AI_ERROR_GENERAL;
	}

	length = (1920 * 1080 * 3);
	image_1080p->w = 1920;
	image_1080p->h = 1080;
	image_1080p->c = 3;
	image_1080p->data_len = length;

	read_length = (UINT32_T)fread((char *)image_1080p->data, 1, sizeof(FLOAT_AI_T)*length, f);
	if (read_length != sizeof(FLOAT_AI_T)*length) {
		fclose(f);
		return AI_ERROR_GENERAL;
	}

	fclose(f);
	return AI_ERROR_SUCCESS;
#else
	return AI_ERROR_GENERAL;
#endif
#endif
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageLoad1080pPng(AI_BASE_IMAGE_1080P *image_1080p)
{
    UINT8_T file_path[256];
	ERROR_T result;

	result = AiBaseImageF16Load1080pPng(image_1080p);
	if (result == AI_ERROR_SUCCESS)
		return AI_ERROR_SUCCESS;

    sprintf((char *)file_path, AI_PRODUCT_FILE_PATH"/Source/ai/AiBase/AiBaseAlphabetData/1080p.png");
	g_ai_base_image_1080p_content_len = AI_BASE_IMAGE_1080P_CONTENT_LENGTH_MAX;
	result = AiBaseImageLoadImageColor(file_path, 0, 0, (AI_BASE_IMAGE *)image_1080p, g_ai_base_image_1080p_content, &g_ai_base_image_1080p_content_len);
	if (result != AI_ERROR_SUCCESS) {
		return AI_ERROR_GENERAL;
	}
#if (AI_PRODUCT_OPTION_LOG_1080P == 1)
	AiBaseImageSave1080pPng(image_1080p);
#endif
#if (AI_PRODUCT_OPTION_LOG_1080P == 1)
	AiBaseLogLayerData(0, "1080pPng", (char *)(image_1080p->data), (image_1080p->data_len*sizeof(FLOAT_AI_T)));
#endif
    return AI_ERROR_SUCCESS;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageSave1080pPng(AI_BASE_IMAGE_1080P *image_1080p)
{
    UINT8_T file_path[256];
	ERROR_T result;

    sprintf((char *)file_path, AI_PRODUCT_LOG_IDENT_PATH"/1080p.png");
	g_ai_base_image_1080p_content_len = AI_BASE_IMAGE_1080P_CONTENT_LENGTH_MAX;
	result = AiBaseImageSaveColor((AI_BASE_IMAGE *)image_1080p, file_path, g_ai_base_image_1080p_content, &g_ai_base_image_1080p_content_len);
	if (result != AI_ERROR_SUCCESS) {
		return AI_ERROR_GENERAL;
	}
    return AI_ERROR_SUCCESS;
}


#if 0
/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageLoad1080p(void)
{
	ERROR_T result;

	result = AiBaseImageLoad1080pPng((AI_BASE_IMAGE_1080P *)&g_image_1080p);
	if (result != AI_ERROR_SUCCESS) {
		return AI_ERROR_GENERAL;
	}
	result = AiBaseImageSave1080pPng((AI_BASE_IMAGE_1080P *)&g_image_1080p);
	if (result != AI_ERROR_SUCCESS) {
		return AI_ERROR_GENERAL;
	}

	return AI_ERROR_SUCCESS;
}
#endif

/*------------------------- End ---------------------------------------------*/
