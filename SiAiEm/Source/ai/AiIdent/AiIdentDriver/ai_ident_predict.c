/**
 ******************************************************************************
 * file    ai_ident_predict.c
 * author  sunlingge
 * version V100
 * brief   the ai net predict driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_ident_global.h"
#pragma warning(disable:4996)

#if (AI_PRODUCT_OPTION_ACCELERATOR_TYPE == AI_PRODUCT_ACCELERATOR_CPU)
/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- External function Definition --------------------*/
ERROR_T AiBaseUpdateDlgStatus(UINT8_T *format, ...);

/*------------------------- Variable Definition------------------------------*/

/*------------------------- Function Definition -----------------------------*/
/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
FLOAT_AI_T *AiIdentPredictGetNetworkOutput(AI_NET_NETWORK *net_network, AI_NET_LAYER *net_layer)
{
    INT32_T layer_index;

	for(layer_index = net_network->network_data.layer_index-1; layer_index > 0; layer_index--) {
		if (net_layer->layer_data[layer_index].layer_config->type.value != AI_NET_CONFIG_SECTION_COST_TYPE) 
			break;
	}
	return (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(net_layer->layer_data[layer_index].output));
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiIdentGetLayerOutputLengthAlign(AI_NET_STATE *net_state, UINT32_T layer_index, UINT32_T *output_length_align)
{
	AI_NET_NETWORK *net_network;
	AI_NET_LAYER *net_layer;
	AI_NET_CONFIG_SECTION *option;
	AI_NET_LAYER_DATA *layer_data;
	UINT32_T batch, h, w, c, m, size, stride, padding, activation;
	UINT32_T out_h, out_w, out_c;
	UINT32_T outputs;
#if 1
	AI_NET_CONFIG_SECTION *option_next;
	AI_NET_LAYER_DATA *layer_data_next;
	UINT32_T padding_next;
	UINT32_T h_align, w_align, c_align;
	UINT32_T out_h_align, out_w_align, out_c_align;
	UINT32_T outputs_align, inputs_align;
#endif

	net_network = AiNetDataGetAiNetNetwork();
	net_layer = AiNetDataGetAiNetLayer();
	layer_data = &(net_layer->layer_data[layer_index]);
	option = layer_data->layer_config;
#if 1
	/* get next layer */
	if ((INT32_T)layer_index < (net_layer->layer_number - 1)) {
		layer_data_next = &(net_layer->layer_data[layer_index + 1]);
		option_next = layer_data_next->layer_config;
	}
	else {
		layer_data_next = NULL;
		option_next = NULL;
	}
	/* get next padding */
	if (option_next == NULL) {
		padding_next = 0;
	}
	else {
		padding_next = option_next->padding.value;
	}
#endif

	batch = layer_data->batch;
	h = layer_data->h;
	w = layer_data->w;
	c = layer_data->c;
	m = option->filters.value;
	size = option->size.value;
	stride = option->stride.value;
	padding = option->padding.value;
	activation = option->activation.value;
	outputs = layer_data->outputs;

	out_h = (((h + 2 * padding - size) / stride) + 1);
	out_w = (((w + 2 * padding - size) / stride) + 1);
	out_c = m;

#if 1
	/* tna_ir */
	h_align = h;
	w_align = ((w + 2 * padding + 7) / 8) * 8;
	c_align = c;
	inputs_align = (h_align * w_align * c_align);
	out_h_align = out_h;
	out_w_align = ((out_w + 2 * padding_next + 7) / 8) * 8;
	out_c_align = out_c;
	outputs_align = (out_h_align * out_w_align * out_c_align);
#endif

	*output_length_align = outputs_align;
	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLayerPredictNetworkOutputF32Load(AI_NET_STATE *net_state, UINT32_T layer_index)
{
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	return AI_ERROR_GENERAL;
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
#if (AI_PRODUCT_OPTION_FP32_TEST_PRELOAD == 1)
	UINT8_T file_path[256];
	FILE *f;
	UINT32_T length;
	UINT32_T read_length;
	AI_NET_NETWORK *net_network;
	AI_NET_LAYER *net_layer;
	AI_NET_CONFIG_SECTION *option;
	AI_NET_LAYER_DATA *layer_data;

	net_network = AiNetDataGetAiNetNetwork();
	net_layer = AiNetDataGetAiNetLayer();
	layer_data = &(net_layer->layer_data[layer_index]);
	option = layer_data->layer_config;

	net_state->output = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->output));
	sprintf((char *)file_path, "%sLayer%d%s", AI_PRODUCT_FILE_PATH""AI_PRODUCT_YOLOV3_TEST_DTA_PATH, layer_index, AI_PRODUCT_YOLOV3_NET_LAYER_FILE);
	length = layer_data->output.length;
	f = fopen(file_path, "rb");
	if (!f) {
		return AI_ERROR_GENERAL;
	}

	read_length = (UINT32_T)fread((char *)net_state->output, 1, length, f);
	if (read_length != length) {
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
ERROR_T AiNetLayerPredictNetworkOutputF16Load(AI_NET_STATE *net_state, UINT32_T layer_index)
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
	AI_NET_NETWORK *net_network;
	AI_NET_LAYER *net_layer;
	AI_NET_CONFIG_SECTION *option;
	AI_NET_LAYER_DATA *layer_data;

	net_network = AiNetDataGetAiNetNetwork();
	net_layer = AiNetDataGetAiNetLayer();
	layer_data = &(net_layer->layer_data[layer_index]);
	option = layer_data->layer_config;

	net_state->output = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->output));
	sprintf((char *)file_path, "%sLayer%d%s", AI_PRODUCT_FILE_PATH""AI_PRODUCT_YOLOV3_TEST_DTA_PATH, layer_index, AI_PRODUCT_YOLOV3_NET_LAYER_FILE_FP16);
	length = layer_data->output.length;
	f = fopen(file_path, "rb");
	if (!f) {
		return AI_ERROR_GENERAL;
	}

	read_length = (UINT32_T)fread((char *)net_state->output, 1, length, f);
	if (read_length != length) {
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
ERROR_T AiIdentPredictForwardNetwork(AI_NET_STATE *net_state, AI_NET_NETWORK *net_network, AI_NET_LAYER *net_layer)
{
	int layer_index;
	AI_NET_LAYER_DATA *layer_data;
	AI_NET_LAYER_FUNC *layer_func;
	AI_NET_CONFIG_SECTION *option;
	FLOAT_AI_T *delta;

	net_state->workspace = (FLOAT_AI_T *)(net_network->network_data.workspace.network_buffer->buffer);
    for(layer_index = 0; layer_index < net_network->network_data.layer_index; layer_index++) {
		AiBaseUpdateDlgStatus("Forward: Layer%d", layer_index);

        net_state->index = layer_index;
		layer_data = &(net_layer->layer_data[layer_index]);
		layer_func = &(net_layer->layer_func[layer_index]);
		option = layer_data->layer_config;

#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
		if (AiNetLayerPredictNetworkOutputF32Load(net_state, layer_index) == AI_ERROR_SUCCESS) {
			net_state->input = net_state->output;
			continue;
		}
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
		if (AiNetLayerPredictNetworkOutputF16Load(net_state, layer_index) == AI_ERROR_SUCCESS) {
			net_state->input = net_state->output;
			continue;
		}
#endif

		if(layer_data->delta.length > 0){
			delta = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(net_layer->layer_data[layer_index].delta));
			AiBaseBoardX86AioScaleCpu(layer_data->outputs * layer_data->batch, AiBaseFloatCvtF32Fai(0.0f), delta, 1);
        }

		layer_func->forward(net_state, layer_index);
        net_state->input = net_state->output;

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
ERROR_T AiIdentPredict(AI_NET_STATE *net_state, AI_NET_NETWORK *net_network, AI_NET_LAYER *net_layer, FLOAT_AI_T *input)
{
    net_state->index = 0;
    net_state->input = input;
    net_state->truth = 0;
    net_state->train = 0;
    net_state->delta = 0;

    AiIdentPredictForwardNetwork(net_state, net_network, net_layer);
 	net_state->output = AiIdentPredictGetNetworkOutput(net_network, net_layer);
    
	return AI_ERROR_SUCCESS;
}


/*------------------------- End ---------------------------------------------*/
#endif
