/**
 ******************************************************************************
 * file    ai_net_layer_shortcut.c
 * author  sunlingge
 * version V100
 * brief   the ai net layer shortcut driver.
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
ERROR_T AiNetLayerShortcutDefault(AI_NET_CONFIG *net_config, AI_NET_NETWORK *net_network, UINT32_T section_index)
{
	AI_NET_CONFIG_SECTION *option;
	AI_NET_NETWORK_DATA *params;

	option = &(net_config->section[section_index]);
	params = &(net_network->network_data);

	AiNetConfigSetOptionValueInt32(&(option->from), AI_NET_CONFIG_OPTION_VALUE_FROM_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->activation), (UINT8_T *)AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LINEAR_NAME, AI_NET_CONFIG_SET_BY_DEFAULT);

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLayerShortcutSetLayer(AI_NET_LAYER *net_layer, AI_NET_CONFIG *net_config, UINT32_T section_index, AI_NET_NETWORK *net_network)
{
	AI_NET_CONFIG_SECTION *option;
	AI_NET_NETWORK_DATA *params;
	AI_NET_LAYER_DATA *layer_data;
	AI_NET_LAYER_FUNC *layer_func;
	AI_NET_LAYER_BUFFER *layer_ro;
	AI_NET_LAYER_BUFFER *layer_rw;
	INT32_T layer_index;
	INT32_T layer_index_shortcut;
	AI_NET_LAYER_DATA *layer_data_shortcut;
	UINT32_T batch;
	UINT32_T h, w, c;
	UINT32_T out_h, out_w, out_c;
	UINT32_T outputs, inputs;
	UINT32_T output_length, delta_length;
	UINT32_T layer_index_start, layer_index_end;
	AI_NET_LAYER_BUFFER *layer_rw_shortcut;

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

	layer_index_shortcut = layer_index;
	if (option->from.value < 0) {
		layer_index_shortcut = layer_index_shortcut + option->from.value;
	}
	layer_data_shortcut = &(net_layer->layer_data[layer_index_shortcut]);
	layer_index_start = layer_index_shortcut + 1;
	layer_index_end = layer_index;
	if ((layer_index_shortcut % 2) == 0) {
		layer_rw_shortcut = &(net_layer->layer_rw0);
	}
	else {
		layer_rw_shortcut = &(net_layer->layer_rw1);
	}
	AiNetLayerBufferLayerOffsetStack(layer_rw_shortcut, layer_index_start, layer_index_end);

	batch = params->batch;
	h = layer_data_shortcut->out_h;
	w = layer_data_shortcut->out_w;
	c = layer_data_shortcut->out_c;
	out_h = params->h;
	out_w = params->w;
	out_c = params->c;
	outputs = ((out_h) * (out_w) * (out_c));
	inputs = outputs;

	layer_data->layer_index = layer_index_shortcut;
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

	layer_func->forward = AiNetLayerShortcutForward;
	layer_func->backward = AiNetLayerShortcutBackward;

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLayerShortcutForward(AI_NET_STATE *net_state, UINT32_T layer_index) 
{
	AI_NET_NETWORK *net_network;
	AI_NET_LAYER *net_layer;
	AI_NET_CONFIG_SECTION *option;
	AI_NET_LAYER_DATA *layer_data;
	UINT32_T outputs, batch, w, h, c, out_w, out_h, out_c, activation;
	FLOAT_AI_T *output;
	FLOAT_AI_T *input;
	INT32_T layer_index_shortcut;
	AI_NET_LAYER_DATA *layer_data_shortcut;
	FLOAT_AI_T *layer_short_cut_output;

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
	activation = option->activation.value;
	outputs = layer_data->outputs;

	input = net_state->input;
	output = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->output));
	layer_index_shortcut = layer_data->layer_index;
	layer_data_shortcut = &(net_layer->layer_data[layer_index_shortcut]);
	layer_short_cut_output = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data_shortcut->output));

    AiBaseBoardX86AioCopyCpu(outputs*batch, input, 1, output, 1);
	AiBaseLogLayerData(layer_index, "CopyOutput", (UINT8_T *)(output), (layer_data->output.length));
    AiBaseBoardX86AioShortcutCpu(batch, w, h, c, layer_short_cut_output, out_w, out_h, out_c, output);
	AiBaseLogLayerData(layer_index, "ShortcutOutput", (UINT8_T *)(output), (layer_data->output.length));
    AiBaseBoardX86AioActivateArrayCpuCustom(output, outputs*batch, activation);
	AiBaseLogLayerData(layer_index, "ActivateOutput", (UINT8_T *)(output), (layer_data->output.length));

	net_state->output = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->output));

	AiBaseLogLayerData(layer_index, "Input", (UINT8_T *)(net_state->input), (layer_data->inputs * sizeof(FLOAT_AI_T)));
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
ERROR_T AiNetLayerShortcutBackward(AI_NET_STATE *net_state, UINT32_T layer_index) {
	return AI_ERROR_SUCCESS;
}

/*------------------------- End ---------------------------------------------*/
#endif
/*------------------------- End ---------------------------------------------*/
