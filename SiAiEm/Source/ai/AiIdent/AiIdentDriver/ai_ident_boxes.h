/**
 ******************************************************************************
 * file    ai_ident_box.h
 * author  sunlingge
 * version V100
 * brief   the ai ident box driver.
 ******************************************************************************
 */
#ifndef AI_IDENT_BOX_H
#define AI_IDENT_BOX_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AI_IDENT_DETECTTION_NUM_MAX			(10000)
#define AI_IDENT_DETECTION_CLASSES_MAX		(80)

/*------------------------- Macro Definition --------------------------------*/
typedef struct ai_ident_box_struct {
    FLOAT_AI_T x, y, w, h;
} AI_IDENT_BOX, *AI_IDENT_BOX_PTR;

typedef struct ai_ident_detection_struct {
	AI_IDENT_BOX bbox;
	UINT32_T classes;
	FLOAT_AI_T prob[AI_IDENT_DETECTION_CLASSES_MAX];
	FLOAT_AI_T *mask;
	FLOAT_AI_T objectness;
	UINT32_T sort_class;
} AI_IDENT_DETECTION, *AI_IDENT_DETECTION_PTR;

typedef struct ai_ident_detections_struct {
	UINT32_T num;
	AI_IDENT_DETECTION detection[AI_IDENT_DETECTTION_NUM_MAX];
} AI_IDENT_DETECTIONS, *AI_IDENT_DETECTIONS_PTR;

typedef struct ai_ident_boxes_struct {
	AI_IDENT_DETECTIONS detections;
} AI_IDENT_BOXES, *AI_IDENT_BOXES_PTR;


/*------------------------- Function Definition -----------------------------*/
ERROR_T AiIdentBoxesGetNetworkBoxes(AI_NET_NETWORK *net_network, AI_NET_LAYER *net_layer, INT32_T w, INT32_T h, FLOAT_AI_T thresh, FLOAT_AI_T hier, 
	INT32_T *map, INT32_T relative, INT32_T *num, INT32_T letter, AI_IDENT_DETECTIONS *detections);
ERROR_T AiIdentBoxesDoNmsSort(AI_IDENT_DETECTIONS *detections, INT32_T total, INT32_T classes, FLOAT_AI_T thresh);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

