/**
 ******************************************************************************
 * file    ai_base_image_alphabet.h
 * author  sunlingge
 * version V100
 * brief   the ai base image alphabet driver.
 ******************************************************************************
 */
#ifndef AI_BASE_IMAGE_ALPHABET_H
#define AI_BASE_IMAGE_ALPHABET_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/


/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_IMAGE_ALPHABET_SET_N                            (8)
#define AI_BASE_IMAGE_ALPHABET_N                                (128)
#define AI_BASE_IMAGE_ALPHABET_MIN                              (32)
#define AI_BASE_IMAGE_ALPHABET_MAX                              (126)
#define AI_BASE_IMAGE_ALPHABET_IMAGE_DATA_LEN_MAX				(128*128*3)
#define AI_BASE_IMAGE_ALPHABAT_CONTENT_LENGTH_MAX				(128*128*3*sizeof(FLOAT_AI_T))


/*------------------------- Type Definition----------------------------------*/
typedef struct ai_base_image_alphabet_struct {
    INT32_T w;
    INT32_T h;
    INT32_T c;
    UINT32_T data_len;
	FLOAT_AI_T data[AI_BASE_IMAGE_ALPHABET_IMAGE_DATA_LEN_MAX];
} AI_BASE_IMAGE_ALPHABET, *AI_BASE_IMAGE_ALPHABET_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiBaseImageLoadAlphabetPng(AI_BASE_IMAGE_ALPHABET *alphabets);
ERROR_T AiBaseImageSaveAlphabetPng(AI_BASE_IMAGE_ALPHABET *alphabets);
ERROR_T AiBaseImageLoadAlphabet(void);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

