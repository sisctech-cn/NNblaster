/**
 ******************************************************************************
 * file    ai_ident_draw.c
 * author  sunlingge
 * version V100
 * brief   the ai net draw driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_ident_global.h"
#pragma warning(disable:4996)

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/
FLOAT32_T g_ai_ident_draw_colors[6][3] = { {1,0,1}, {0,0,1},{0,1,1},{0,1,0},{1,1,0},{1,0,0} };

/*------------------------- Function Definition -----------------------------*/
/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiIdentDrawGetActualDetections(AI_IDENT_DETECTIONS *detections, INT32_T dets_num, FLOAT_AI_T thresh, 
	INT32_T* selected_detections_num, AI_IDENT_DRAW *detection_with_class)
{
    INT32_T selected_num = 0;
	AI_IDENT_DRAW* result_arr;
	INT32_T i;
	INT32_T best_class;
	FLOAT_AI_T best_class_prob;
	UINT32_T j;
	
	if (dets_num > AI_IDENT_DETECTION_WITH_CLASS_NUM_MAX) {
		dets_num = AI_IDENT_DETECTION_WITH_CLASS_NUM_MAX;
	}
    result_arr = detection_with_class;

	for (i = 0; i < dets_num; ++i) {
        best_class = -1;
        best_class_prob = thresh;
        for (j = 0; j < detections->detection[i].classes; ++j) {
            if (AiBaseFloatCmpGt(detections->detection[i].prob[j], best_class_prob)) {
                best_class = j;
                best_class_prob = detections->detection[i].prob[j];
            }
        }
        if (best_class >= 0) {
            memcpy((char *)&(result_arr->detection_with_class[selected_num].detection), (char *)&(detections->detection[i]), sizeof(AI_IDENT_DETECTION));
            result_arr->detection_with_class[selected_num].best_class = best_class;
            ++selected_num;
        }
    }
    *selected_detections_num = selected_num;

    return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
INT32_T AiIdentDrawCompareByLefts(const void *a_ptr, const void *b_ptr) {
	AI_DETECTION_WITH_CLASS* a = (AI_DETECTION_WITH_CLASS*)a_ptr;
	AI_DETECTION_WITH_CLASS* b = (AI_DETECTION_WITH_CLASS*)b_ptr;
	FLOAT_AI_T delta = AiBaseFloatSub(AiBaseFloatSub(a->detection.bbox.x, AiBaseFloatDiv(a->detection.bbox.w, AiBaseFloatCvtF32Fai(2.0f))),
										AiBaseFloatSub(b->detection.bbox.x, AiBaseFloatDiv(b->detection.bbox.w, AiBaseFloatCvtF32Fai(2.0f))));
	return AiBaseFloatCmpLt(delta, AiBaseFloatCvtF32Fai(0.0f)) ? 
		-1 : 
		AiBaseFloatCmpGt(delta, AiBaseFloatCvtF32Fai(0.0f)) ? 
		1 : 
		0;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
INT32_T AiIdentDrawCompareByProbs(const void *a_ptr, const void *b_ptr) {
	AI_DETECTION_WITH_CLASS* a = (AI_DETECTION_WITH_CLASS*)a_ptr;
	AI_DETECTION_WITH_CLASS* b = (AI_DETECTION_WITH_CLASS*)b_ptr;

	FLOAT_AI_T delta = AiBaseFloatSub(a->detection.prob[a->best_class], b->detection.prob[b->best_class]);
	return AiBaseFloatCmpLt(delta, AiBaseFloatCvtF32Fai(0.0f)) ?
		-1 :
		AiBaseFloatCmpGt(delta, AiBaseFloatCvtF32Fai(0.0f)) ?
		1 :
		0;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T AiIdentDrawGetColor(INT32_T c, INT32_T x, INT32_T max)
{
    FLOAT_AI_T ratio = AiBaseFloatMul(AiBaseFloatDiv(AiBaseFloatCvtF32Fai((FLOAT32_T)x),AiBaseFloatCvtF32Fai((FLOAT32_T)max)),AiBaseFloatCvtF32Fai(5.0f));
    INT32_T i = (INT32_T)AiBaseFloatCvtFaiF32(AiBaseFloatFloor((ratio)));
    INT32_T j = (INT32_T)AiBaseFloatCvtFaiF32(AiBaseFloatCeil((ratio)));
	FLOAT_AI_T r;

	ratio = AiBaseFloatSub(ratio, AiBaseFloatCvtF32Fai((FLOAT32_T)i));
    r = AiBaseFloatAdd(AiBaseFloatMul(AiBaseFloatSub(AiBaseFloatCvtF32Fai(1.0f), ratio), AiBaseFloatCvtF32Fai(g_ai_ident_draw_colors[i][c])), AiBaseFloatMul(ratio, AiBaseFloatCvtF32Fai(g_ai_ident_draw_colors[j][c])));

	return r;
}

ERROR_T AiIdentDrawBox(AI_BASE_IMAGE_1080P *a, INT32_T x1, INT32_T y1, INT32_T x2, INT32_T y2, FLOAT_AI_T r, FLOAT_AI_T g, FLOAT_AI_T b)
{
    INT32_T i;

	if (x1 < 0) 
		x1 = 0;
    if (x1 >= a->w) 
		x1 = a->w - 1;
    if (x2 < 0) 
		x2 = 0;
    if (x2 >= a->w) 
		x2 = a->w - 1;

    if (y1 < 0) 
		y1 = 0;
    if (y1 >= a->h) 
		y1 = a->h - 1;
    if (y2 < 0) 
		y2 = 0;
    if (y2 >= a->h) 
		y2 = a->h - 1;

    for(i = x1; i <= x2; ++i){
        a->data[i + y1*a->w + 0*a->w*a->h] = r;
        a->data[i + y2*a->w + 0*a->w*a->h] = r;
        a->data[i + y1*a->w + 1*a->w*a->h] = g;
        a->data[i + y2*a->w + 1*a->w*a->h] = g;
        a->data[i + y1*a->w + 2*a->w*a->h] = b;
        a->data[i + y2*a->w + 2*a->w*a->h] = b;
    }
    for(i = y1; i <= y2; ++i){
        a->data[x1 + i*a->w + 0*a->w*a->h] = r;
        a->data[x2 + i*a->w + 0*a->w*a->h] = r;
        a->data[x1 + i*a->w + 1*a->w*a->h] = g;
        a->data[x2 + i*a->w + 1*a->w*a->h] = g;
        a->data[x1 + i*a->w + 2*a->w*a->h] = b;
        a->data[x2 + i*a->w + 2*a->w*a->h] = b;
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
ERROR_T AiIdentDrawBoxWidth(AI_BASE_IMAGE_1080P *a, INT32_T x1, INT32_T y1, INT32_T x2, INT32_T y2, INT32_T w, FLOAT_AI_T r, FLOAT_AI_T g, FLOAT_AI_T b)
{
    INT32_T i;

    for(i = 0; i < w; ++i){
        AiIdentDrawBox(a, x1+i, y1+i, x2-i, y2-i, r, g, b);
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
FLOAT_AI_T AiIdentDrawGetPixelExtend(AI_IDENT_DRAW_IMAGE_TITLE *m, INT32_T x, INT32_T y, INT32_T c)
{
    if ((x < 0) || (x >= m->w) || (y < 0) || (y >= m->h)) 
		return 0;
     if (c < 0 || c >= m->c) 
		 return 0;
    return AiBaseImageGetPixel(m->data, m->w, m->h, m->c, x, y, c);
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiIdentDrawBorderImage(AI_IDENT_DRAW_IMAGE_TITLE *a, INT32_T border)
{
    INT32_T x,y,k;
	FLOAT_AI_T val;

    for(k = 0; k < a->c; ++k){
        for(y = 0; y < a->h; ++y){
            for(x = 0; x < a->w; ++x){
                val = AiIdentDrawGetPixelExtend(a, x, y, k);
				if (((x - border) < 0) || ((x + border) >= (a->w)) || ((y - border) < 0) || ((y + border) >= (a->h))) {
					val = 1;
					AiBaseImageSetPixel(a->data, a->w, a->h, a->c, x, y, k, val);
				}
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
ERROR_T AiIdentDrawGetLabelV3(AI_BASE_IMAGE_ALPHABET *characters, UINT8_T *string, INT32_T size, AI_IDENT_DRAW_IMAGE_TITLE *label)
{
	AI_BASE_IMAGE_ALPHABET *l;
	UINT32_T str_len;
	UINT32_T char_index;
	UINT32_T w;
	UINT32_T h;
	UINT32_T c;
	UINT32_T l_w;
	UINT32_T l_h;
	UINT32_T l_c;
	UINT32_T pos_x1;
	UINT32_T pos_y1;
	UINT32_T alpha_index;
	FLOAT_AI_T val;
	INT32_T index;
	UINT32_T width_offset;

	memset((char *)label, 0, sizeof(AI_IDENT_DRAW_IMAGE_TITLE));
	size = AI_IDENT_DRAW_IMAGE_TITLE_SIZE;
	str_len = (UINT32_T)strlen((char *)string);
	label->w = AI_IDENT_DRAW_IMAGE_TITLE_CHAR_WIDTH_EXPAND;
	for (index = 0; index < (INT32_T)str_len; index++) {
		alpha_index = size*AI_BASE_IMAGE_ALPHABET_N + (((int)(string[index])));
		l = &(characters[alpha_index]);
		label->w = label->w  + l->w;
	}
	label->h = AI_IDENT_DRAW_IMAGE_TITLE_CHAR_HEIGHT;
	label->c = 3;
	label->data_len = (label->w * label->h * label->c * sizeof(FLOAT_AI_T));
	for (index = 0; index < (label->w * label->h * label->c); index++) {
		label->data[index] = (FLOAT_AI_T)AiBaseFloatCvtF32Fai(1.0f);
	}
	char_index = 0;
	width_offset = AI_IDENT_DRAW_IMAGE_TITLE_CHAR_WIDTH_EXPAND/2;
    while (*string) {
		alpha_index = size*AI_BASE_IMAGE_ALPHABET_N + (((int)*string));
        l = &(characters[alpha_index]);
		l_w = l->w;
		l_h = l->h;
		l_c = l->c;
		pos_x1 = width_offset;
		width_offset = width_offset + l_w;
		pos_y1 = (AI_IDENT_DRAW_IMAGE_TITLE_CHAR_HEIGHT - l_h) / 2;
		for (c = 0; c<l_c; c++) {
			for (h = 0; h < l_h; h++) {
				for (w = 0; w<l_w; w++) {
					val = AiBaseImageGetPixel(l->data, l->w, l->h, l->c, w, h, c);
					AiBaseImageSetPixel(label->data, label->w, label->h, label->c, w + pos_x1, h + pos_y1, c, val);
				}
			}
		}
		char_index++;
        ++string;
    }
	AiIdentDrawBorderImage(label, (INT32_T)(1));
    return AI_ERROR_SUCCESS;
}

ERROR_T AiIdentDrawLabel(AI_BASE_IMAGE_1080P *a, INT32_T r, INT32_T c, AI_IDENT_DRAW_IMAGE_TITLE *label, FLOAT_AI_T *rgb)
{
    INT32_T w = label->w;
    INT32_T h = label->h;
    INT32_T i, j, k;
	FLOAT_AI_T val;

    if (r - h >= 0) 
		r = r - h;

    for(j = 0; (j < h) && ((j + r) < a->h); ++j){
        for(i = 0; (i < w) && ((i + c) < a->w); ++i){
            for(k = 0; k < label->c; ++k){
                val = AiBaseImageGetPixel(label->data, label->w, label->h, label->c, i, j, k);
				AiBaseImageSetPixel(a->data, a->w, a->h, a->c, i + c, j + r, k, AiBaseFloatMul(rgb[k], val));
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
ERROR_T AiIdentDrawDetectionsV3(AI_BASE_IMAGE_1080P *im, AI_IDENT_DETECTIONS *detections,
	INT32_T num, FLOAT_AI_T thresh, AI_NET_CLASSES *net_classes, AI_BASE_IMAGE_ALPHABET *alphabet, INT32_T classes, INT32_T ext_output, AI_IDENT_DRAW *detection_with_class,
	AI_IDENT_DRAW_IMAGE_TITLE *label)
{
	INT32_T selected_detections_num;
	AI_DETECTION_WITH_CLASS* selected_detections;
	INT32_T i;
	INT32_T width;
	INT32_T offset;
	FLOAT_AI_T red;
	FLOAT_AI_T green;
	FLOAT_AI_T blue;
	FLOAT_AI_T rgb[3];
	AI_IDENT_BOX b;
	INT32_T left;
	INT32_T right;
	INT32_T top;
	INT32_T bot;
	UINT8_T labelstr[4096];
	INT32_T j;

	AiIdentDrawGetActualDetections(detections, num, thresh, &selected_detections_num, detection_with_class);
	selected_detections = detection_with_class->detection_with_class;

	qsort(selected_detections, selected_detections_num, sizeof(AI_DETECTION_WITH_CLASS), AiIdentDrawCompareByLefts);
	qsort(selected_detections, selected_detections_num, sizeof(AI_DETECTION_WITH_CLASS), AiIdentDrawCompareByProbs);
	for (i = 0; i < selected_detections_num; ++i) {
		width = (INT32_T)AiBaseFloatCvtFaiF32(AiBaseFloatMul(AiBaseFloatCvtF32Fai((FLOAT32_T)(im->h)), AiBaseFloatCvtF32Fai(.006f)));
		if (width < 1)
			width = 1;

		offset = (selected_detections[i].best_class * 123457) % classes;
		red = AiIdentDrawGetColor(2, offset, classes);
		green = AiIdentDrawGetColor(1, offset, classes);
		blue = AiIdentDrawGetColor(0, offset, classes);
		rgb[0] = red;
		rgb[1] = green;
		rgb[2] = blue;
		memcpy((char *)&b, (char *)&(selected_detections[i].detection.bbox), sizeof(AI_IDENT_BOX));

		left = (INT32_T)AiBaseFloatCvtFaiF32(AiBaseFloatMul(AiBaseFloatSub(b.x, AiBaseFloatDiv(b.w, AiBaseFloatCvtF32Fai(2.0f))), AiBaseFloatCvtF32Fai((FLOAT32_T)(im->w))));
		right = (INT32_T)AiBaseFloatCvtFaiF32(AiBaseFloatMul(AiBaseFloatAdd(b.x, AiBaseFloatDiv(b.w, AiBaseFloatCvtF32Fai(2.0f))), AiBaseFloatCvtF32Fai((FLOAT32_T)(im->w))));
		top = (INT32_T)AiBaseFloatCvtFaiF32(AiBaseFloatMul(AiBaseFloatSub(b.y, AiBaseFloatDiv(b.h, AiBaseFloatCvtF32Fai(2.0f))), AiBaseFloatCvtF32Fai((FLOAT32_T)(im->h))));
		bot = (INT32_T)AiBaseFloatCvtFaiF32(AiBaseFloatMul(AiBaseFloatAdd(b.y, AiBaseFloatDiv(b.h, AiBaseFloatCvtF32Fai(2.0f))), AiBaseFloatCvtF32Fai((FLOAT32_T)(im->h))));

		if (left < 0)
			left = 0;
		if (right > im->w - 1)
			right = im->w - 1;
		if (top < 0)
			top = 0;
		if (bot > im->h - 1)
			bot = im->h - 1;

		AiIdentDrawBoxWidth(im, left, top, right, bot, width, red, green, blue);
		if (alphabet != NULL) {
			memset((char *)labelstr, 0, 4096);
			strcat((char *)labelstr, (char *)(net_classes->name[selected_detections[i].best_class]));
			strcat((char *)labelstr, "("); 
			sprintf((char *)(labelstr + strlen(labelstr)), "%.2f", (AiBaseFloatCvtFaiF32(selected_detections[i].detection.prob[selected_detections[i].best_class])));
			strcat((char *)labelstr, ")");
			for (j = 0; j < classes; ++j) {
				if (AiBaseFloatCmpGt(selected_detections[i].detection.prob[j], thresh) && j != selected_detections[i].best_class) {
					strcat((char *)labelstr, ", ");
					strcat((char *)labelstr, (char *)(net_classes->name[j]));
				}
			}
			AiIdentDrawGetLabelV3(alphabet, labelstr, (INT32_T)(AiBaseFloatCvtFaiF32(AiBaseFloatMul(AiBaseFloatCvtF32Fai((FLOAT32_T)(im->h)),AiBaseFloatCvtF32Fai(.03f)))), label);
			AiIdentDrawLabel(im, top + width, left, label, rgb);
		}
	}

	return AI_ERROR_SUCCESS;
}


/*------------------------- End ---------------------------------------------*/
