/**
 ******************************************************************************
 * file    ai_net_log_cmode1.c
 * author  sunlingge
 * version V100
 * brief   the ai net log driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_net_global.h"
#pragma warning(disable:4996)

#if (AI_PRODUCT_OPTION_ACCELERATOR_TYPE == AI_PRODUCT_ACCELERATOR_SISCTECH_CMODEL2)
/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/
UINT8_T g_ai_net_log_cmodel2_layer_data_string[AI_NET_LOG_CMODEL2_LAYER_DATA_STRING_LENGTH_MAX];
AI_NET_IMAGE_LOG_CMODEL2 g_ai_net_log_cmodel2_source;
AI_NET_IMAGE_LOG_CMODEL2 g_ai_net_log_cmodel2_dest;

/*------------------------- Function Definition -----------------------------*/
/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLogCmodel2LayerDataFloat16(INT32_T layer_index, UINT8_T *name, UINT8_T *buffer, UINT32_T length)
{
	UINT8_T file_path[256];
	FILE *f;
	UINT32_T index;
	FLOAT16_T fp16;
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	FLOAT32_T fp32;
#endif

	sprintf((char *)file_path, "%sLayer%d%s.f16.dat", AI_PRODUCT_LOG_IDENT_PATH"/", layer_index, name);
	f = fopen(file_path, "wb");
	if (!f) {
		return AI_ERROR_GENERAL;
	}
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	for (index = 0; index < length; index = index + 4) {
		fp32 = (FLOAT32_T)(*((FLOAT_AI_T *)(buffer + index)));
		fp16 = AiBaseFloat16CvtF32F16(fp32);
		fwrite((char *)&fp16, 1, sizeof(FLOAT16_T), f);
	}
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	for (index = 0; index < length; index = index + 2) {
		fp16 = (FLOAT16_T)(*((FLOAT16_T *)(buffer + index)));
		fwrite((char *)&fp16, 1, sizeof(FLOAT16_T), f);
	}
#endif

	fclose(f);

	return AI_ERROR_SUCCESS;
}


/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLogCmodel2LayerInfo(INT32_T layer_index, UINT8_T *name, UINT8_T *buffer, UINT32_T length)
{
//#if (AI_PRODUCT_OPTION_LOG == 1)
	UINT8_T file_path[256];
	FILE *f;

	sprintf((char *)file_path, "%sLayer%d%s.dat", AI_PRODUCT_LOG_IDENT_PATH"/", layer_index, name);
	f = fopen(file_path, "wb");
	if (!f) {
		return AI_ERROR_GENERAL;
	}

	fwrite(buffer, 1, length, f);
	fclose(f);
//#endif

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLogCmodel2LayerInfoAppend(INT32_T layer_index, UINT8_T *name, UINT8_T *buffer, UINT32_T length)
{
//#if (AI_PRODUCT_OPTION_LOG == 1)
	UINT8_T file_path[256];
	FILE *f;

	sprintf((char *)file_path, "%sLayer%d%s.dat", AI_PRODUCT_LOG_IDENT_PATH"/", layer_index, name);
	f = fopen(file_path, "ab+");
	if (!f) {
		return AI_ERROR_GENERAL;
	}

	fwrite(buffer, 1, length, f);
	fclose(f);
//#endif

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLogCmodel2LayerDataFormat(INT32_T layer_index, UINT8_T *name, UINT8_T *format, ...) {
//#if (AI_PRODUCT_OPTION_LOG == 1)
	UINT8_T *str_buffer = g_ai_net_log_cmodel2_layer_data_string;

	va_list vlArgs;
	va_start(vlArgs, format);
	//_vsnprintf_s(str_buffer, sizeof(str_buffer) - 1, format, vlArgs); 
	//vsprintf(str_buffer, format, vlArgs);
	memset((char *)str_buffer, 0, AI_NET_LOG_CMODEL2_LAYER_DATA_STRING_LENGTH_MAX);
	vsnprintf(str_buffer, AI_NET_LOG_CMODEL2_LAYER_DATA_STRING_LENGTH_MAX - 1, format, vlArgs);
	va_end(vlArgs);

	AiBaseLogLayerInfo(layer_index, name, str_buffer, (UINT32_T)strlen(str_buffer));
//#endif
	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLogCmodel2LayerDataAppendFormat(INT32_T layer_index, UINT8_T *name, UINT8_T *format, ...) {
//#if (AI_PRODUCT_OPTION_LOG == 1)
	UINT8_T *str_buffer = g_ai_net_log_cmodel2_layer_data_string;

	va_list vlArgs;
	va_start(vlArgs, format);
	//_vsnprintf_s(str_buffer, sizeof(str_buffer) - 1, format, vlArgs); 
	//vsprintf(str_buffer, format, vlArgs);
	memset((char *)str_buffer, 0, AI_NET_LOG_CMODEL2_LAYER_DATA_STRING_LENGTH_MAX);
	vsnprintf(str_buffer, AI_NET_LOG_CMODEL2_LAYER_DATA_STRING_LENGTH_MAX - 1, format, vlArgs);
	va_end(vlArgs);

	AiNetLogCmodel2LayerInfoAppend(layer_index, name, str_buffer, (UINT32_T)strlen(str_buffer));
//#endif
	return AI_ERROR_SUCCESS;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiNetLogCmodel2LayerWeights(INT32_T layer_index, UINT8_T *name, UINT8_T *buffer, UINT32_T length)
{
	AiNetLogCmodel2LayerDataFloat16(layer_index, name, buffer, length);
	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLogCmodel2LayerInput(INT32_T layer_index, UINT8_T *name, UINT8_T *buffer, UINT32_T length)
{
	AiNetLogCmodel2LayerDataFloat16(layer_index, name, buffer, length);
	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLogCmodel2LayerOutput(INT32_T layer_index, UINT8_T *name, UINT8_T *buffer, UINT32_T length)
{
	AiNetLogCmodel2LayerDataFloat16(layer_index, name, buffer, length);
	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLogCmodel2LayerAddress(INT32_T layer_index, UINT64_T net_state_address, UINT8_T *name)
{
//#if (AI_PRODUCT_OPTION_LOG == 1)
	AI_NET_NETWORK *net_network;
	AI_NET_LAYER *net_layer;
	AI_NET_LAYER_DATA *layer_data;
	AI_NET_CLASSES *net_class;
	AI_NET_STATE *net_state = (AI_NET_STATE *)net_state_address;

	net_network = AiNetDataGetAiNetNetwork();
	net_layer = AiNetDataGetAiNetLayer();
	layer_data = &(net_layer->layer_data[layer_index]);

	AiNetLogCmodel2LayerDataFormat(layer_index, name, "%d ", layer_data->w);
	AiNetLogCmodel2LayerDataAppendFormat(layer_index, name, "%d ", layer_data->h);
	AiNetLogCmodel2LayerDataAppendFormat(layer_index, name, "%d ", layer_data->c);
	AiNetLogCmodel2LayerDataAppendFormat(layer_index, name, "%d ", layer_data->out_c_align);
	if (layer_data->padding == 1) {
		AiNetLogCmodel2LayerDataAppendFormat(layer_index, name, "%d ", 3);
	} else {
		AiNetLogCmodel2LayerDataAppendFormat(layer_index, name, "%d ", 1);
	}
	AiNetLogCmodel2LayerDataAppendFormat(layer_index, name, "%d ", layer_data->layer_config->stride.value);
	if (layer_data->padding_next == 1) {
		AiNetLogCmodel2LayerDataAppendFormat(layer_index, name, "%d ", 3);
	} else {
		AiNetLogCmodel2LayerDataAppendFormat(layer_index, name, "%d ", 1);
	}
	net_class = AiNetDataGetAiNetClasses();
	if ((&(layer_data->weights))->layer_buffer == NULL) {
		AiNetLogCmodel2LayerDataAppendFormat(layer_index, name, "%u ", 0);
	} else {
		AiNetLogCmodel2LayerDataAppendFormat(layer_index, name, "%u ", (UINT32_T)((UINT64_T)((FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->weights)))) - (UINT64_T)(net_class));
	}
	AiNetLogCmodel2LayerDataAppendFormat(layer_index, name, "%u ", (UINT32_T)((UINT64_T)net_state->input) - (UINT64_T)(net_class));
	AiNetLogCmodel2LayerDataAppendFormat(layer_index, name, "%u ", (UINT32_T)((UINT64_T)net_state->output) - (UINT64_T)(net_class));
	AiNetLogCmodel2LayerDataAppendFormat(layer_index, name, "%u ", (UINT32_T)((UINT64_T)net_state->workspace) - (UINT64_T)(net_class));

	AiNetLogCmodel2LayerDataAppendFormat(layer_index, name, "\r\n");

	return AI_ERROR_SUCCESS;
}

/*------------------------- End ---------------------------------------------*/
#endif
