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

#if (AI_PRODUCT_OPTION_ACCELERATOR_TYPE == AI_PRODUCT_ACCELERATOR_CPU)
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
	AiNetLayerContentInit(&layer_data->output, output_length, layer_rw, layer_index);
	delta_length = (batch * outputs * sizeof(FLOAT_AI_T));
	AiNetLayerContentInit(&layer_data->delta, delta_length, layer_rw, layer_index);

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

	input = net_state->input;
	output = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->output));

    AiBaseBoardX86AioFillCpu(outputs*batch, 0, output, 1);
    if(reverse > 0){
        AiBaseBoardX86AioUpsampleCpu(output, out_w, out_h, c, batch, stride, 0, scale, input);
    }else{
        AiBaseBoardX86AioUpsampleCpu(input, w, h, c, batch, stride, 1, scale, output);
    }

	net_state->output = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->output));

	AiBaseLogLayerData(layer_index, "Output", (UINT8_T *)(net_state->output), (layer_data->output.length));

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

/*------------------------- End ---------------------------------------------*/
#endif
/*------------------------- End ---------------------------------------------*/
