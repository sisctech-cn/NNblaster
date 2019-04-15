/**
 ******************************************************************************
 * file    ai_ident_data_rw.h
 * author  sunlingge
 * version V100
 * brief   the ai ident read write data.
 ******************************************************************************
 */
#ifndef AI_IDENT_DATA_RW_H
#define AI_IDENT_DATA_RW_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/


/*------------------------- Macro Definition --------------------------------*/


/*------------------------- Type Definition----------------------------------*/
typedef struct ai_ident_data_export_struct {
	UINT8_T reserved;
} AI_IDENT_DATA_EXPORT, *AI_IDENT_DATA_EXPORT_PTR;

#pragma pack(push, 16)
typedef struct ai_ident_data_rw_struct {
	AI_NET_IMAGE image_net;
#if (AI_PRODUCT_OPTION_ACCELERATOR_TYPE == AI_PRODUCT_ACCELERATOR_SISCTECH_CMODEL1)
	AI_NET_IMAGE_CMODEL1 image_net_cmodel1;
#endif
#if (AI_PRODUCT_OPTION_ACCELERATOR_TYPE == AI_PRODUCT_ACCELERATOR_SISCTECH_CMODEL2)
	AI_NET_IMAGE_CMODEL2 image_net_cmodel2;
#endif
	AI_BASE_IMAGE_ALPHABET alphabets[AI_BASE_IMAGE_ALPHABET_SET_N*AI_BASE_IMAGE_ALPHABET_N];
	AI_BASE_IMAGE_1080P image_1080p;
	INT32_T nboxes;
	FLOAT_AI_T thresh;
	FLOAT_AI_T hier_thresh;
	INT32_T letter_box;
	FLOAT_AI_T nms;
	INT32_T classes;
	AI_NET_STATE net_state;
	AI_IDENT_BOXES boxes;
	AI_IDENT_DETECTIONS detections;
	AI_IDENT_DRAW detection_with_class;
	AI_IDENT_DRAW_IMAGE_TITLE label;
} AI_IDENT_DATA_RW, *AI_IDENT_DATA_RW_PTR;
#pragma pack(pop)

/*------------------------- Function Definition -----------------------------*/

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

