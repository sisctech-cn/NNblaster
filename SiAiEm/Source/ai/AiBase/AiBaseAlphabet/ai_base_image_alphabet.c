/**
 ******************************************************************************
 * file    ai_base_image_alphabet.c
 * author  sunlingge
 * version V100
 * brief   the ai base image alphabet driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"
#pragma warning(disable:4996)

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/
INT32_T g_ai_base_image_alphabet_content_len = 0;
UINT8_T g_ai_base_image_alphabet_content[AI_BASE_IMAGE_ALPHABAT_CONTENT_LENGTH_MAX];
#if 0
AI_BASE_IMAGE_ALPHABET g_alphabets[AI_BASE_IMAGE_ALPHABET_SET_N*AI_BASE_IMAGE_ALPHABET_N];
#endif

/*------------------------- Function Definition -----------------------------*/
/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseImageLoadAlphabetPng(AI_BASE_IMAGE_ALPHABET *alphabets)
{
    INT32_T i, j;
    INT32_T nsize = AI_BASE_IMAGE_ALPHABET_SET_N;
    UINT8_T file_path[256];
	ERROR_T result;

    for(j = 0; j < nsize; ++j){
        for(i = AI_BASE_IMAGE_ALPHABET_MIN; i <= AI_BASE_IMAGE_ALPHABET_MAX; ++i){
            sprintf((char *)file_path, AI_PRODUCT_APHPHABET_CONFIG_PATH"/%d_%d.png", i, j);
			while(1) {
				g_ai_base_image_alphabet_content_len = AI_BASE_IMAGE_ALPHABAT_CONTENT_LENGTH_MAX;
				result = AiBaseImageLoadImageColor(file_path, 0, 0, (AI_BASE_IMAGE *)&(alphabets[j*AI_BASE_IMAGE_ALPHABET_N+i]), g_ai_base_image_alphabet_content, &g_ai_base_image_alphabet_content_len);
				if (result != AI_ERROR_SUCCESS) {
					AiBaseLogErrorCritial();
				} 
				break;
			}
        }
    }
#if (AI_PRODUCT_OPTION_LOG_ALPHABET == 1)
	AiBaseImageSaveAlphabetPng(alphabets);
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
ERROR_T AiBaseImageSaveAlphabetPng(AI_BASE_IMAGE_ALPHABET *alphabets)
{
    INT32_T i, j;
    INT32_T nsize = AI_BASE_IMAGE_ALPHABET_SET_N;
    UINT8_T file_path[256];
	ERROR_T result;

    for(j = 0; j < nsize; ++j){
        for(i = AI_BASE_IMAGE_ALPHABET_MIN; i <= AI_BASE_IMAGE_ALPHABET_MAX; ++i){
            sprintf((char *)file_path, AI_PRODUCT_APHPHABET_LOG_PATH"/%d_%d.png", i, j);
			g_ai_base_image_alphabet_content_len = AI_BASE_IMAGE_ALPHABAT_CONTENT_LENGTH_MAX;
			result = AiBaseImageSaveColor((AI_BASE_IMAGE *)&(alphabets[j*AI_BASE_IMAGE_ALPHABET_N+i]), file_path, g_ai_base_image_alphabet_content, &g_ai_base_image_alphabet_content_len);
			if (result != AI_ERROR_SUCCESS) {
				AiBaseLogErrorCritial();
			} 
        }
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
ERROR_T AiBaseImageLoadAlphabet(void)
{
	ERROR_T result;

	result = AiBaseImageLoadAlphabetPng((AI_BASE_IMAGE_ALPHABET *)g_alphabets);
	if (result != AI_ERROR_SUCCESS) {
		return AI_ERROR_GENERAL;
	}
	result = AiBaseImageSaveAlphabetPng((AI_BASE_IMAGE_ALPHABET *)g_alphabets);
	if (result != AI_ERROR_SUCCESS) {
		return AI_ERROR_GENERAL;
	}
	return AI_ERROR_SUCCESS;
}
#endif


/*------------------------- End ---------------------------------------------*/
