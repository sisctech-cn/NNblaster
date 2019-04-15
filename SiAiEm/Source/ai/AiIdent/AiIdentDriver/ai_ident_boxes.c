/**
 ******************************************************************************
 * file    ai_ident_box.c
 * author  sunlingge
 * version V100
 * brief   the ai net box driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_ident_global.h"
#pragma warning(disable:4996)

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/

/*------------------------- Function Definition -----------------------------*/
/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
UINT32_T AiIdentBoxesNumDetections(AI_NET_NETWORK *net_network, AI_NET_LAYER *net_layer, FLOAT_AI_T thresh)
{
	AI_NET_NETWORK_DATA *network_data;
	AI_NET_LAYER_DATA *layer_data;
	AI_NET_CONFIG_SECTION *option;
	INT32_T layer_index;
	UINT32_T type, w, h, n;
    UINT32_T s = 0;
	UINT32_T s1, s2, s3, s_t;
	UINT32_T s_times = 0;

	network_data = &(net_network->network_data);
	layer_data = &(net_layer->layer_data[network_data->layer_index-1]);
    for (layer_index = 0; layer_index < network_data->layer_index; layer_index++) {
        layer_data = &(net_layer->layer_data[layer_index]);
		option = layer_data->layer_config;
		type = option->type.value;
		h = layer_data->h;
		w = layer_data->w;
		n = option->num.value;

        if (type == AI_NET_CONFIG_SECTION_YOLO_TYPE) {
			s_times++;
			s_t = AiNetLayerYoloNumDetections(layer_index, thresh);
			if (s_times == 1)
				s1 = s_t;
			if (s_times == 2)
				s2 = s_t;
			if (s_times == 3)
				s3 = s_t;
			s = s + s_t;

        }
        if ((type == AI_NET_CONFIG_SECTION_DETECTION_TYPE) || (type == AI_NET_CONFIG_SECTION_REGION_TYPE)) {
            s += w*h*n;
        }
    }
    return s;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiIdentBoxesMakeNetworkBoxes(AI_NET_NETWORK *net_network, AI_NET_LAYER *net_layer, FLOAT_AI_T thresh, AI_IDENT_DETECTIONS *detections, INT32_T *num)
{
	AI_NET_NETWORK_DATA *network_data;
	AI_NET_LAYER_DATA *layer_data;
	UINT32_T i;
	UINT32_T nboxes;

	network_data = &(net_network->network_data);
	layer_data = &(net_layer->layer_data[network_data->layer_index-1]);
    nboxes = AiIdentBoxesNumDetections(net_network, net_layer, thresh);
	if (nboxes > AI_IDENT_DETECTTION_NUM_MAX) {
		nboxes = AI_IDENT_DETECTTION_NUM_MAX;
	}
	*num = nboxes;
	detections->num = nboxes;
    for (i = 0; i < nboxes; ++i) {
		detections->detection[i].mask = NULL;
    }
    return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiIdentBoxesGetYoloBox(FLOAT_AI_T *x, AI_NET_CONFIG_OPTION_FLOAT_AI_T *anchors, INT32_T n, INT32_T index,
	INT32_T i, INT32_T j, INT32_T lw, INT32_T lh, INT32_T w, INT32_T h, INT32_T stride, AI_IDENT_BOX *box)
{
	box->x = AiBaseFloatDiv(AiBaseFloatAdd(AiBaseFloatCvtF32Fai((FLOAT32_T)i), x[index + 0 * stride]), AiBaseFloatCvtF32Fai((FLOAT32_T)lw));
	box->y = AiBaseFloatDiv(AiBaseFloatAdd(AiBaseFloatCvtF32Fai((FLOAT32_T)j), x[index + 1 * stride]), AiBaseFloatCvtF32Fai((FLOAT32_T)lh));
	box->w = (FLOAT_AI_T)(AiBaseFloatDiv(AiBaseFloatMul(AiBaseFloatExp(x[index + 2 * stride]), (anchors[2 * n].value)), AiBaseFloatCvtF32Fai((FLOAT32_T)w)));
	box->h = (FLOAT_AI_T)(AiBaseFloatDiv(AiBaseFloatMul(AiBaseFloatExp(x[index + 3 * stride]), (anchors[2 * n + 1].value)), AiBaseFloatCvtF32Fai((FLOAT32_T)h)));
    return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiIdentBoxesCorrectYoloBoxes(AI_IDENT_DETECTIONS *detections, INT32_T n, INT32_T w, INT32_T h, INT32_T netw, INT32_T neth, INT32_T relative, INT32_T letter, INT32_T detection_index)
{
    INT32_T i;
    INT32_T new_w=0;
    INT32_T new_h=0;
	AI_IDENT_BOX *b;

	if (letter) {
		if (AiBaseFloatCmpLt(((FLOAT_AI_T)AiBaseFloatDiv(AiBaseFloatCvtF32Fai((FLOAT32_T)netw), AiBaseFloatCvtF32Fai((FLOAT32_T)w))),
								((FLOAT_AI_T)AiBaseFloatDiv(AiBaseFloatCvtF32Fai((FLOAT32_T)neth), AiBaseFloatCvtF32Fai((FLOAT32_T)h))))) {
			new_w = netw;
			new_h = (h * netw) / w;
		}
		else {
			new_h = neth;
			new_w = (w * neth) / h;
		}
	}
	else {
		new_w = netw;
		new_h = neth;
	}
	for (i = 0; i < n; ++i) {
		b = &(detections->detection[i + detection_index].bbox);
		b->x = (FLOAT_AI_T)AiBaseFloatDiv(AiBaseFloatSub(b->x, AiBaseFloatDiv(AiBaseFloatDiv(AiBaseFloatCvtF32Fai((FLOAT32_T)(netw - new_w)), 
					AiBaseFloatCvtF32Fai(2.)), AiBaseFloatCvtF32Fai((FLOAT32_T)netw))),
					((FLOAT_AI_T)AiBaseFloatDiv(AiBaseFloatCvtF32Fai((FLOAT32_T)new_w), AiBaseFloatCvtF32Fai((FLOAT32_T)netw))));
		b->y = (FLOAT_AI_T)AiBaseFloatDiv(AiBaseFloatSub(b->y, AiBaseFloatDiv(AiBaseFloatDiv(AiBaseFloatCvtF32Fai((FLOAT32_T)(neth - new_h)), 
					AiBaseFloatCvtF32Fai(2.)), AiBaseFloatCvtF32Fai((FLOAT32_T)neth))),
					((FLOAT_AI_T)AiBaseFloatDiv(AiBaseFloatCvtF32Fai((FLOAT32_T)new_h), AiBaseFloatCvtF32Fai((FLOAT32_T)neth))));
		b->w = AiBaseFloatMul(b->w, (FLOAT_AI_T)AiBaseFloatDiv(AiBaseFloatCvtF32Fai((FLOAT32_T)netw), AiBaseFloatCvtF32Fai((FLOAT32_T)new_w)));
		b->h = AiBaseFloatMul(b->h, (FLOAT_AI_T)AiBaseFloatDiv(AiBaseFloatCvtF32Fai((FLOAT32_T)neth), AiBaseFloatCvtF32Fai((FLOAT32_T)new_h)));
		if (!relative) {
			b->x = AiBaseFloatMul(b->x, AiBaseFloatCvtF32Fai((FLOAT32_T)w));
			b->w = AiBaseFloatMul(b->w, AiBaseFloatCvtF32Fai((FLOAT32_T)w));
			b->y = AiBaseFloatMul(b->y, AiBaseFloatCvtF32Fai((FLOAT32_T)h));
			b->h = AiBaseFloatMul(b->h, AiBaseFloatCvtF32Fai((FLOAT32_T)h));
		}
	}

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
INT32_T AiIdentBoxesGetYoloDetections(AI_NET_NETWORK *net_network, AI_NET_LAYER *net_layer, INT32_T layer_index, 
	INT32_T w, INT32_T h, INT32_T netw, INT32_T neth, FLOAT_AI_T thresh, INT32_T *map, INT32_T relative, AI_IDENT_DETECTIONS *detections, INT32_T letter, INT32_T detection_index)
{
	AI_NET_NETWORK_DATA *network_data;
	AI_NET_LAYER_DATA *layer_data;
	AI_NET_CONFIG_SECTION *option;
    UINT32_T i;
	UINT32_T j;
	UINT32_T n;
    FLOAT_AI_T *predictions;
	AI_NET_CONFIG_OPTION_FLOAT_AI_T *anchors;
	AI_NET_CONFIG_OPTION_UINT32_T *mask; 
	UINT32_T classes;
	INT32_T count;
	INT32_T row, col;
	UINT32_T num;
	INT32_T obj_index;
	FLOAT_AI_T objectness;
	INT32_T box_index;
	INT32_T class_index;
	FLOAT_AI_T prob;

	network_data = &(net_network->network_data);
	layer_data = &(net_layer->layer_data[layer_index]);
	option = layer_data->layer_config;
	predictions = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->output));
	anchors = (AI_NET_CONFIG_OPTION_FLOAT_AI_T *)option->anchors;
	mask = option->mask;
	classes = option->classes.value;
	num = layer_data->n;

	count = 0;
	for (i = 0; i < layer_data->w*layer_data->h; ++i) {
		row = i / layer_data->w;
		col = i % layer_data->w;
		for (n = 0; n < num; ++n) {
			obj_index = AiNetLayerYoloEntryIndex(layer_index, 0, n*layer_data->w*layer_data->h + i, 4);
			if (obj_index >(layer_data->output.length / sizeof(FLOAT_AI_T))) {
				continue;
			}
			objectness = predictions[obj_index];
			if (AiBaseFloatCmpGt(objectness, thresh)) {
				box_index = AiNetLayerYoloEntryIndex(layer_index, 0, n*layer_data->w*layer_data->h + i, 0);
				AiIdentBoxesGetYoloBox(predictions, anchors, mask[n].value, box_index, col, row, layer_data->w, layer_data->h, netw, neth,
					layer_data->w*layer_data->h, &(detections->detection[count + detection_index].bbox));
				detections->detection[count + detection_index].objectness = objectness;
				detections->detection[count + detection_index].classes = classes;
				for (j = 0; j < classes; ++j) {
					class_index = AiNetLayerYoloEntryIndex(layer_index, 0, n*layer_data->w*layer_data->h + i, 4 + 1 + j);
					prob = AiBaseFloatMul(objectness, predictions[class_index]);
					detections->detection[count + detection_index].prob[j] = (AiBaseFloatCmpGt(prob, thresh)) ? prob : AiBaseFloatCvtF32Fai(0.0f);
				}
				++count;
			}
		}
	}
	AiIdentBoxesCorrectYoloBoxes(detections, count, w, h, netw, neth, relative, letter, detection_index);

    return count;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiIdentBoxesFillNetworkBoxes(AI_NET_NETWORK *net_network, AI_NET_LAYER *net_layer, INT32_T w, INT32_T h, 
	FLOAT_AI_T thresh, FLOAT_AI_T hier, INT32_T *map, INT32_T relative, AI_IDENT_DETECTIONS *detections, INT32_T letter)
{
	AI_NET_NETWORK_DATA *network_data;
	AI_NET_LAYER_DATA *layer_data;
	AI_NET_CONFIG_SECTION *option;
    INT32_T prev_classes = -1;
    INT32_T layer_index;
	INT32_T yolo_t = 0;
	UINT32_T type;
	INT32_T count;
	INT32_T detection_index;
	UINT32_T classes;

	network_data = &(net_network->network_data);
	detection_index = 0;
    for (layer_index = 0; layer_index < network_data->layer_index; layer_index++) {
		layer_data = &(net_layer->layer_data[layer_index]);
		option = layer_data->layer_config;
		type = option->type.value;
		classes = option->classes.value;

        if (type == AI_NET_CONFIG_SECTION_YOLO_TYPE) {
			yolo_t++;
            count = AiIdentBoxesGetYoloDetections(net_network, net_layer, layer_index, w, h, network_data->w, network_data->h, thresh, map, relative, detections, letter, detection_index);
	        detection_index += count;
            if (prev_classes < 0) {
				prev_classes = classes;
			} else if (prev_classes != classes) {
                return AI_ERROR_GENERAL;
            }
        }
    }

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiIdentBoxesGetNetworkBoxes(AI_NET_NETWORK *net_network, AI_NET_LAYER *net_layer, INT32_T w, INT32_T h, FLOAT_AI_T thresh, FLOAT_AI_T hier, 
	INT32_T *map, INT32_T relative, INT32_T *num, INT32_T letter, AI_IDENT_DETECTIONS *detections)
{
	ERROR_T result;

    result = AiIdentBoxesMakeNetworkBoxes(net_network, net_layer, thresh, detections, num);
	if (result != AI_ERROR_SUCCESS)
		return result;
    
	result = AiIdentBoxesFillNetworkBoxes(net_network, net_layer, w, h, thresh, hier, map, relative, detections, letter);
    return result;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
INT32_T AiIdentBoxesNmsComparatorV3(const void *pa, const void *pb)
{
    AI_IDENT_DETECTION a = *(AI_IDENT_DETECTION *)pa;
    AI_IDENT_DETECTION b = *(AI_IDENT_DETECTION *)pb;
	FLOAT_AI_T diff = AiBaseFloatCvtF32Fai(0.0f);

	if (b.sort_class >= 0) {
		diff = AiBaseFloatSub(a.prob[b.sort_class], b.prob[b.sort_class]);
	}
	else {
		diff = AiBaseFloatSub(a.objectness, b.objectness);
	}
	if (AiBaseFloatCmpLt(diff, AiBaseFloatCvtF32Fai(0.0f)))
		return 1;
	else if (AiBaseFloatCmpGt(diff, AiBaseFloatCvtF32Fai(0.0f)))
		return -1;
	return 0;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiIdentBoxesOverlap(FLOAT_AI_T x1, FLOAT_AI_T w1, FLOAT_AI_T x2, FLOAT_AI_T w2)
{
	FLOAT_AI_T l1 = AiBaseFloatSub(x1, AiBaseFloatDiv(w1, AiBaseFloatCvtF32Fai(2.0f)));
	FLOAT_AI_T l2 = AiBaseFloatSub(x2, AiBaseFloatDiv(w2, AiBaseFloatCvtF32Fai(2.0f)));
	FLOAT_AI_T left = AiBaseFloatCmpGt(l1, l2) ? l1 : l2;
	FLOAT_AI_T r1 = AiBaseFloatAdd(x1, AiBaseFloatDiv(w1, AiBaseFloatCvtF32Fai(2.0f)));
	FLOAT_AI_T r2 = AiBaseFloatAdd(x2, AiBaseFloatDiv(w2, AiBaseFloatCvtF32Fai(2.0f)));
	FLOAT_AI_T right = AiBaseFloatCmpLt(r1, r2) ? r1 : r2;

	return AiBaseFloatSub(right, left);
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiIdentBoxesBoxIntersection(AI_IDENT_BOX a, AI_IDENT_BOX b)
{
    FLOAT_AI_T w = AiIdentBoxesOverlap(a.x, a.w, b.x, b.w);
    FLOAT_AI_T h = AiIdentBoxesOverlap(a.y, a.h, b.y, b.h);
	FLOAT_AI_T area;

	if (AiBaseFloatCmpLt(w, AiBaseFloatCvtF32Fai(0.0f)) || AiBaseFloatCmpLt(h, AiBaseFloatCvtF32Fai(0.0f)))
		return AiBaseFloatCvtF32Fai(0.0f);
	area = AiBaseFloatMul(w, h);
    return area;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiIdentBoxesBoxUnion(AI_IDENT_BOX a, AI_IDENT_BOX b)
{
    FLOAT_AI_T i = AiIdentBoxesBoxIntersection(a, b);
    FLOAT_AI_T u = AiBaseFloatSub(AiBaseFloatAdd(AiBaseFloatMul(a.w, a.h), AiBaseFloatMul(b.w, b.h)), i);
    return u;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiIdentBoxesBoxIou(AI_IDENT_BOX a, AI_IDENT_BOX b)
{
    return AiBaseFloatDiv(AiIdentBoxesBoxIntersection(a, b), AiIdentBoxesBoxUnion(a, b));
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiIdentBoxesDoNmsSort(AI_IDENT_DETECTIONS *detections, INT32_T total, INT32_T classes, FLOAT_AI_T thresh)
{
    INT32_T i, j, k;
	AI_IDENT_DETECTION swap;
	AI_IDENT_BOX a, b;

	k = total - 1;
	for (i = 0; i <= k; ++i) {
		if (AiBaseFloatCmpEq(detections->detection[i].objectness, AiBaseFloatCvtF32Fai(0.0f))) {
			memcpy((char *)&swap, (char *)&(detections->detection[i]), sizeof(AI_IDENT_DETECTION));
			memcpy((char *)&(detections->detection[i]), (char *)&(detections->detection[k]), sizeof(AI_IDENT_DETECTION));
			memcpy((char *)&(detections->detection[k]), (char *)&swap, sizeof(AI_IDENT_DETECTION));
			--k;
			--i;
		}
	}
	total = k + 1;

	classes = 80;
	for (k = 0; k < classes; ++k) {
		for (i = 0; i < total; ++i) {
			detections->detection[i].sort_class = k;
		}
		qsort(detections->detection, total, sizeof(AI_IDENT_DETECTION), AiIdentBoxesNmsComparatorV3);
		for (i = 0; i < total; ++i) {
			if (AiBaseFloatCmpEq(detections->detection[i].prob[k], AiBaseFloatCvtF32Fai(0.0f)))
				continue;
			memcpy((char *)&a, (char *)&(detections->detection[i].bbox), sizeof(AI_IDENT_BOX));
			for (j = i + 1; j < total; ++j) {
				memcpy((char *)&b, (char *)&(detections->detection[j].bbox), sizeof(AI_IDENT_BOX));
				if (AiBaseFloatCmpGt(AiIdentBoxesBoxIou(a, b), thresh)) {
					detections->detection[j].prob[k] = AiBaseFloatCvtF32Fai(0.0f);
				}
			}
		}
	}

	return AI_ERROR_SUCCESS;
}

/*------------------------- End ---------------------------------------------*/
