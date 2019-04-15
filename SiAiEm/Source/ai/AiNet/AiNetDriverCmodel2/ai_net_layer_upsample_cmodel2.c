/**
 ******************************************************************************
 * file    ai_net_layer_upsample.c
 * author  sunlingge
 * version V100
 * brief   the ai net layer upsample driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_net_global.h"
#pragma warning(disable:4996)

#if (AI_PRODUCT_OPTION_ACCELERATOR_TYPE == AI_PRODUCT_ACCELERATOR_SISCTECH_CMODEL2)
/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/
extern FLOAT_AI_T g_ai_cmodel2_map_padding_input[AI_NET_NETWORK_BUFFER_LENGTH_MAX];
extern FLOAT_AI_T g_ai_cmodel2_map_padding_output[AI_NET_NETWORK_BUFFER_LENGTH_MAX];

/*------------------------- Variable Definition------------------------------*/

/*------------------------- Function Definition -----------------------------*/
/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLayerUpsampleDefault(AI_NET_CONFIG *net_config, AI_NET_NETWORK *net_network, UINT32_T section_index)
{
	AI_NET_CONFIG_SECTION *option;
	AI_NET_NETWORK_DATA *params;

	option = &(net_config->section[section_index]);
	params = &(net_network->network_data);

	AiNetConfigSetOptionValueInt32(&(option->stride), AI_NET_CONFIG_OPTION_VALUE_STRIDE_2, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueFloat(&(option->scale), (UINT8_T *)AI_NET_CONFIG_OPTION_VALUE_SCALE_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLayerUpsampleSetLayer(AI_NET_LAYER *net_layer, AI_NET_CONFIG *net_config, UINT32_T section_index, AI_NET_NETWORK *net_network)
{
	AI_NET_CONFIG_SECTION *option;
	AI_NET_NETWORK_DATA *params;
	AI_NET_LAYER_DATA *layer_data;
	AI_NET_LAYER_FUNC *layer_func;
	AI_NET_LAYER_BUFFER *layer_ro;
	AI_NET_LAYER_BUFFER *layer_rw;
	INT32_T layer_index;
	UINT32_T batch, reverse;
	INT32_T stride;
	UINT32_T h, w, c;
	UINT32_T out_h, out_w, out_c;
	UINT32_T outputs, inputs;
	UINT32_T output_length, delta_length;
	AI_NET_CONFIG_SECTION *option_next;

	option = &(net_config->section[section_index]);
	params = &(net_network->network_data);
	layer_index = params->layer_index;
	layer_data = &(net_layer->layer_data[layer_index]);
	layer_func = &(net_layer->layer_func[layer_index]);
	layer_data->layer_config = option;
	layer_ro = &(net_layer->layer_ro);
	if ((layer_index % 2) == 0) {
		layer_rw = &(net_layer->layer_rw0);
	} else {
		layer_rw = &(net_layer->layer_rw1);
	}
	if ((INT32_T)layer_index < (net_layer->layer_number - 1)) {
		option_next = &(net_config->section[section_index + 1]);
	}
	else {
		option_next = NULL;
	}
	if (option_next == NULL) {
		option->padding.value = 0;
		option->size.value = 1;
		option->filters.value = params->c;
	}
	else {
		option->padding.value = 0;
		option->size.value = 1;
		option->filters.value = params->c;
	}

	batch = params->batch;
	stride = option->stride.value;
	h = params->h;
	w = params->w;
	c = params->c;
	if (stride >= 0) {
		out_h = h * stride;
		out_w = w * stride;
		reverse = 0;
	} else {
		out_h = h / (0-stride);
		out_w = w / (0-stride);
		reverse = 1;
	}
	out_c = params->c;
	outputs = ((out_h) * (out_w) * (out_c));
	inputs = ((h) * (w) * (c));;

    layer_data->batch = batch;
    layer_data->h = h;
    layer_data->w = w;
    layer_data->c = c;
	layer_data->out_h = out_h;
	layer_data->out_w = out_w;
	layer_data->out_c = out_c;
	layer_data->outputs = outputs;
	layer_data->inputs = inputs;

	output_length = (batch * outputs * sizeof(FLOAT_AI_T));
	delta_length = (batch * outputs * sizeof(FLOAT_AI_T));

	layer_func->forward = AiNetLayerUpsampleForward;
	layer_func->backward = AiNetLayerUpsampleBackward;

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLayerUpsampleForward(AI_NET_STATE *net_state, UINT32_T layer_index) {
	AI_NET_NETWORK *net_network;
	AI_NET_LAYER *net_layer;
	AI_NET_CONFIG_SECTION *option;
	AI_NET_LAYER_DATA *layer_data;
	UINT32_T outputs, batch, w, h, c, out_w, out_h, out_c, stride, activation, reverse;
	FLOAT_AI_T scale;
	FLOAT_AI_T *output;
	FLOAT_AI_T *input;
	UINT32_T padding;
	UINT32_T padding_next;
	UINT32_T h_align, w_align, c_align;
	UINT32_T out_h_align, out_w_align, out_c_align;
	UINT32_T outputs_align, inputs_align;

	net_network = AiNetDataGetAiNetNetwork();
	net_layer = AiNetDataGetAiNetLayer();
	layer_data = &(net_layer->layer_data[layer_index]);
	option = layer_data->layer_config;	

	batch = layer_data->batch;
	h = layer_data->h;
	w = layer_data->w;
	c = layer_data->c;
	out_h = layer_data->out_h;
	out_w = layer_data->out_w;
	out_c = layer_data->out_c;
	stride = option->stride.value;
	scale = option->scale.value;
	activation = option->activation.value;
	reverse = layer_data->reverse;
	outputs = layer_data->outputs;

	/* cmodel 2*/
	padding = layer_data->padding;
	padding_next = layer_data->padding_next;
	h_align = layer_data->h_align;
	w_align = layer_data->w_align;
	c_align = layer_data->c_align;
	inputs_align = layer_data->inputs_align;
	out_h_align = layer_data->out_h_align;
	out_w_align = layer_data->out_w_align;
	out_c_align = layer_data->out_c_align;
	outputs_align = layer_data->outputs_align;

	input = net_state->input;
	output = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->output));

    AiBaseBoardX86AioFillCpu(outputs_align*batch, 0, output, 1);
    if(reverse > 0){
		AiBaseBoardCmodel2AioMapCopyCpu(1, out_w_align, out_h_align, out_c_align, padding_next, output, out_w, out_h, out_c, 0, g_ai_cmodel2_map_padding_output);
        AiBaseBoardX86AioUpsampleCpu(g_ai_cmodel2_map_padding_output, out_w, out_h, c, batch, stride, 0, scale, g_ai_cmodel2_map_padding_input);
		AiBaseBoardCmodel2AioMapCopyCpu(1, w, h, c, 0, g_ai_cmodel2_map_padding_input, w_align, h_align, c_align, padding, input);
    }else{
		AiBaseBoardCmodel2AioMapCopyCpu(1, w_align, h_align, c_align, padding, input, w, h, c, 0, g_ai_cmodel2_map_padding_input);
		AiBaseBoardX86AioUpsampleCpu(g_ai_cmodel2_map_padding_input, w, h, c, batch, stride, 1, scale, g_ai_cmodel2_map_padding_output);
		AiBaseBoardCmodel2AioMapCopyCpu(1, out_w, out_h, out_c, 0, g_ai_cmodel2_map_padding_output, out_w_align, out_h_align, out_c_align, padding_next, output);
    }

	net_state->output = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->output));

	AiBaseLogLayerData(layer_index, "OutputCmodel2", (UINT8_T *)(net_state->output), (layer_data->output.length));

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLayerUpsampleBackward(AI_NET_STATE *net_state, UINT32_T layer_index) {
	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLayerUpsampleSetLayerContent(AI_NET_LAYER *net_layer, AI_NET_CONFIG *net_config, UINT32_T section_index, AI_NET_NETWORK *net_network)
{
	AI_NET_CONFIG_SECTION *option;
	AI_NET_NETWORK_DATA *params;
	AI_NET_LAYER_DATA *layer_data;
	AI_NET_LAYER_FUNC *layer_func;
	AI_NET_LAYER_BUFFER *layer_ro;
	AI_NET_LAYER_BUFFER *layer_rw;
	INT32_T layer_index;
	UINT32_T batch, reverse;
	INT32_T stride;
	UINT32_T h, w, c;
	UINT32_T out_h, out_w, out_c;
	UINT32_T outputs, inputs;
	UINT32_T output_length, delta_length;
	UINT32_T padding;
	AI_NET_CONFIG_SECTION *option_next;
	AI_NET_LAYER_DATA *layer_data_next;
	UINT32_T padding_next;
	UINT32_T h_align, w_align, c_align;
	UINT32_T out_h_align, out_w_align, out_c_align;
	UINT32_T outputs_align, inputs_align;

	option = &(net_config->section[section_index]);
	params = &(net_network->network_data);
	layer_index = params->layer_index;
	layer_data = &(net_layer->layer_data[layer_index]);
	layer_func = &(net_layer->layer_func[layer_index]);
	layer_data->layer_config = option;
	layer_ro = &(net_layer->layer_ro);
	if ((layer_index % 2) == 0) {
		layer_rw = &(net_layer->layer_rw0);
	}
	else {
		layer_rw = &(net_layer->layer_rw1);
	}
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

	batch = params->batch;
	stride = option->stride.value;
	h = params->h;
	w = params->w;
	c = params->c;
	if (stride >= 0) {
		out_h = h * stride;
		out_w = w * stride;
		reverse = 0;
	}
	else {
		out_h = h / (0 - stride);
		out_w = w / (0 - stride);
		reverse = 1;
	}
	out_c = params->c;
	outputs = ((out_h) * (out_w) * (out_c));
	inputs = ((h) * (w) * (c));;

	layer_data->batch = batch;
	layer_data->h = h;
	layer_data->w = w;
	layer_data->c = c;
	layer_data->out_h = out_h;
	layer_data->out_w = out_w;
	layer_data->out_c = out_c;
	layer_data->outputs = outputs;
	layer_data->inputs = inputs;

	padding = layer_data->layer_config->padding.value;
	/* cmodel1 */
	h_align = h;
	w_align = ((w + 2 * padding + 7) / 8) * 8;
	c_align = c;
	inputs_align = (h_align * w_align * c_align);
	out_h_align = out_h;
	out_w_align = ((out_w + 2 * padding_next + 7) / 8) * 8;
	out_c_align = out_c;
	outputs_align = (out_h_align * out_w_align * out_c_align);
	layer_data->h_align = h_align;
	layer_data->w_align = w_align;
	layer_data->c_align = c_align;
	layer_data->inputs_align = inputs_align;
	layer_data->out_h_align = out_h_align;
	layer_data->out_w_align = out_w_align;
	layer_data->out_c_align = out_c_align;
	layer_data->outputs_align = outputs_align;
	layer_data->padding = padding;
	layer_data->padding_next = padding_next;

	output_length = (batch * outputs_align * sizeof(FLOAT_AI_T));
	AiNetLayerContentInit(&layer_data->output, output_length, layer_rw, layer_index);
	delta_length = (batch * outputs * sizeof(FLOAT_AI_T));
	AiNetLayerContentInit(&layer_data->delta, delta_length, layer_rw, layer_index);

	layer_func->forward = AiNetLayerUpsampleForward;
	layer_func->backward = AiNetLayerUpsampleBackward;

	return AI_ERROR_SUCCESS;
}

/*------------------------- End ---------------------------------------------*/
#endif
/*------------------------- End ---------------------------------------------*/
