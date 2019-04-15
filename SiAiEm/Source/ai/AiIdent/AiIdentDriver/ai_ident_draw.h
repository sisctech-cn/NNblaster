/**
 ******************************************************************************
 * file    ai_ident_draw.h
 * author  sunlingge
 * version V100
 * brief   the ai ident draw driver.
 ******************************************************************************
 */
#ifndef AI_IDENT_DRAW_H
#define AI_IDENT_DRAW_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AI_IDENT_DETECTION_WITH_CLASS_NUM_MAX			(1000)
#define AI_IDENT_DRAW_IMAGE_TITLE_SIZE					(1)
#define AI_IDENT_DRAW_IMAGE_TITLE_CHAR_HEIGHT			(24)
#define AI_IDENT_DRAW_IMAGE_TITLE_CHAR_WIDTH			(24)
#define AI_IDENT_DRAW_IMAGE_TITLE_CHAR_WIDTH_EXPAND		(8)
#define AI_IDENT_DRAW_IMAGE_TITLE_CHAR_CHANNEL			(3)
#define AI_IDENT_DRAW_IMAGE_TITLE_IMAGE_DATA_LEN_MAX	(24*24*3*AI_NET_CLASSES_NAME_LENGTH_MAX)

/*------------------------- Macro Definition --------------------------------*/
typedef struct detection_with_class_struct {
	AI_IDENT_DETECTION detection;
	INT32_T best_class;
} AI_DETECTION_WITH_CLASS, *AI_DETECTION_WITH_CLASS_PTR;

typedef struct ai_ident_draw_struct {
	AI_DETECTION_WITH_CLASS detection_with_class[AI_IDENT_DETECTION_WITH_CLASS_NUM_MAX];
} AI_IDENT_DRAW, *AI_IDENT_DRAW_PTR;

typedef struct ai_ident_draw_image_title_struct {
    INT32_T w;
    INT32_T h;
    INT32_T c;
    UINT32_T data_len;
	FLOAT_AI_T data[AI_IDENT_DRAW_IMAGE_TITLE_IMAGE_DATA_LEN_MAX];
} AI_IDENT_DRAW_IMAGE_TITLE, *AI_IDENT_DRAW_IMAGE_TITLE_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiIdentDrawDetectionsV3(AI_BASE_IMAGE_1080P *im, AI_IDENT_DETECTIONS *detections, 
	INT32_T num, FLOAT_AI_T thresh, AI_NET_CLASSES *net_classes, AI_BASE_IMAGE_ALPHABET *alphabet, INT32_T classes, INT32_T ext_output, AI_IDENT_DRAW *detection_with_class, 
	AI_IDENT_DRAW_IMAGE_TITLE *label);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

