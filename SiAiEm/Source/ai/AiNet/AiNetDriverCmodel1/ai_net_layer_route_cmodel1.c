/**
 ******************************************************************************
 * file    ai_net_layer_route.c
 * author  sunlingge
 * version V100
 * brief   the ai net layer route driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_net_global.h"
#pragma warning(disable:4996)

#if (AI_PRODUCT_OPTION_ACCELERATOR_TYPE == AI_PRODUCT_ACCELERATOR_SISCTECH_CMODEL1)
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
ERROR_T AiNetLayerRouteDefault(AI_NET_CONFIG *net_config, AI_NET_NETWORK *net_network, UINT32_T section_index)
{
	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLayerRouteSetLayer(AI_NET_LAYER *net_layer, AI_NET_CONFIG *net_config, UINT32_T section_index, AI_NET_NETWORK *net_network)
{
	AI_NET_CONFIG_SECTION *option;
	AI_NET_NETWORK_DATA *params;
	AI_NET_LAYER_DATA *layer_data;
	AI_NET_LAYER_FUNC *layer_func;
	AI_NET_LAYER_BUFFER *layer_ro;
	AI_NET_LAYER_BUFFER *layer_rw;
	INT32_T layer_index;
	UINT32_T batch;
	UINT32_T n;
	UINT32_T outputs, inputs;
	UINT32_T output_length, delta_length;
	INT32_T layer_index_route;
	UINT32_T layer_index_start, layer_index_end;
	AI_NET_LAYER_BUFFER *layer_rw_route;
	UINT32_T out_w, out_h, out_c;
	UINT32_T first_out_w, first_out_h, first_out_c;
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
	
	batch = params->batch;
	out_w = 0;
	out_h = 0;
	out_c = 0;
	outputs = 0;
	for(n=0; n<AI_NET_CONFIG_OPTION_VALUE_LAYERS_ARRAY_LENGTH_MAX; n++) {
		if (option->layers[n].set_type == AI_NET_CONFIG_SET_BY_NONE) {
			break;
		}
		layer_index_route = option->layers[n].value;
		if (layer_index_route < 0) {
			layer_index_route += params->layer_index;
		} 
		outputs += net_layer->layer_data[layer_index_route].outputs;

		layer_index_start = layer_index_route + 1;
		layer_index_end = layer_index;
		if ((layer_index_route % 2) == 0) {
			layer_rw_route = &(net_layer->layer_rw0);
		}
		else {
			layer_rw_route = &(net_layer->layer_rw1);
		}

		if (n == 0) {
			first_out_w = net_layer->layer_data[layer_index_route].out_w;
			first_out_h = net_layer->layer_data[layer_index_route].out_h;
			first_out_c = net_layer->layer_data[layer_index_route].out_c;
			out_w = first_out_w;
			out_h = first_out_h;
			out_c = first_out_c;
		} else {
			if((net_layer->layer_data[layer_index_route].out_w == first_out_w) && (net_layer->layer_data[layer_index_route].out_h == first_out_h)){
				out_c += net_layer->layer_data[layer_index_route].out_c;
			}else{
				out_h = 0;
				out_w = 0;
				out_c = 0;
				return AI_ERROR_GENERAL;
			}
		}
	}
	inputs = outputs;

    layer_data->batch = batch;
	layer_data->outputs = outputs;
	layer_data->inputs = inputs;
	layer_data->out_w = out_w;
	layer_data->out_h = out_h;
	layer_data->out_c = out_c;

	if ((INT32_T)layer_index < (net_layer->layer_number - 1)) {
		option_next = &(net_config->section[section_index + 1]);
	}
	else {
		option_next = NULL;
	}
	if (option_next == NULL) {
		option->padding.value = 0;
		option->size.value = 1;
		option->stride.value = 1;
		option->filters.value = 0;
	}
	else {
		option->padding.value = option_next->padding.value;
		option->size.value = option_next->size.value;
		option->stride.value = option_next->stride.value;
		option->filters.value = out_c;
	}

	output_length = (batch * outputs * sizeof(FLOAT_AI_T));
	delta_length = (batch * outputs * sizeof(FLOAT_AI_T));

	layer_func->forward = AiNetLayerRouteForward;
	layer_func->backward = AiNetLayerRouteBackward;


	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLayerRouteForward(AI_NET_STATE *net_state, UINT32_T layer_index) {
	AI_NET_NETWORK *net_network;
	AI_NET_LAYER *net_layer;
	AI_NET_CONFIG_SECTION *option;
	AI_NET_LAYER_DATA *layer_data;
	UINT32_T outputs, batch;
	FLOAT_AI_T *output;
	UINT32_T n;
	INT32_T layer_index_route;
	AI_NET_LAYER_DATA *layer_data_route;
	INT32_T layer_data_route_outputs;
	FLOAT_AI_T *layer_data_route_output;
	UINT32_T padding;
	UINT32_T padding_next;
	UINT32_T h_align, w_align, c_align;
	UINT32_T out_h_align, out_w_align, out_c_align;
	UINT32_T outputs_align;
	FLOAT_AI_T *output_copy;
	UINT8_T route_log_name[64];

	net_network = AiNetDataGetAiNetNetwork();
	net_layer = AiNetDataGetAiNetLayer();
	layer_data = &(net_layer->layer_data[layer_index]);
	option = layer_data->layer_config;	

	batch = layer_data->batch;
	outputs = layer_data->outputs;
	output = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->output));

	padding_next = layer_data->padding_next;
	out_h_align = layer_data->out_h_align;
	out_w_align = layer_data->out_w_align;
	out_c_align = layer_data->out_c_align;
	outputs_align = layer_data->outputs_align;
	output_copy = output;

	for(n=0; n<AI_NET_CONFIG_OPTION_VALUE_LAYERS_ARRAY_LENGTH_MAX; n++) {
		if (option->layers[n].set_type == AI_NET_CONFIG_SET_BY_NONE) {
			break;
		}
		layer_index_route = option->layers[n].value;
		if (layer_index_route < 0) {
			layer_index_route += layer_index;
		}
		layer_data_route = &(net_layer->layer_data[layer_index_route]);

        layer_data_route_output = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data_route->output));
		layer_data_route_outputs = layer_data_route->outputs;
		padding = (&(net_layer->layer_data[layer_index_route+1]))->layer_config->padding.value;
		/* cmodel1 */
		h_align = layer_data_route->out_h;
		w_align = ((layer_data_route->out_w + 2 * padding + 7) / 8) * 8;
		c_align = layer_data_route->out_c;
		sprintf((char *)route_log_name, "Route%dCmodel1", layer_index_route);
		AiBaseLogLayerData(layer_index, route_log_name, (UINT8_T *)(layer_data_route_output), (layer_data_route->output.length));
		AiBaseBoardCmodel1AioRouteCopyCpu(batch, w_align, h_align, c_align, padding, layer_data_route_output, out_w_align, out_h_align, out_c_align, padding_next, output_copy);
		output_copy = output_copy + out_w_align*out_h_align*c_align;
    }

	net_state->output = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->output));

	AiBaseLogLayerData(layer_index, "OutputCmodel1", (UINT8_T *)(net_state->output), (layer_data->output.length));

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLayerRouteBackward(AI_NET_STATE *net_state, UINT32_T layer_index) {
	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLayerRouteSetLayerContent(AI_NET_LAYER *net_layer, AI_NET_CONFIG *net_config, UINT32_T section_index, AI_NET_NETWORK *net_network)
{
	AI_NET_CONFIG_SECTION *option;
	AI_NET_NETWORK_DATA *params;
	AI_NET_LAYER_DATA *layer_data;
	AI_NET_LAYER_FUNC *layer_func;
	AI_NET_LAYER_BUFFER *layer_ro;
	AI_NET_LAYER_BUFFER *layer_rw;
	INT32_T layer_index;
	UINT32_T batch;
	UINT32_T n;
	UINT32_T outputs, inputs;
	UINT32_T output_length, delta_length;
	INT32_T layer_index_route;
	UINT32_T layer_index_start, layer_index_end;
	AI_NET_LAYER_BUFFER *layer_rw_route;
	UINT32_T out_w, out_h, out_c;
	UINT32_T first_out_w, first_out_h, first_out_c;
	AI_NET_CONFIG_SECTION *option_next;
	AI_NET_LAYER_DATA *layer_data_next;
	UINT32_T padding_next;
	UINT32_T out_h_align, out_w_align, out_c_align;
	UINT32_T outputs_align;

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
	if (layer_index < (net_layer->layer_number - 1)) {
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
	out_w = 0;
	out_h = 0;
	out_c = 0;
	outputs = 0;
	for (n = 0; n<AI_NET_CONFIG_OPTION_VALUE_LAYERS_ARRAY_LENGTH_MAX; n++) {
		if (option->layers[n].set_type == AI_NET_CONFIG_SET_BY_NONE) {
			break;
		}
		layer_index_route = option->layers[n].value;
		if (layer_index_route < 0) {
			layer_index_route += params->layer_index;
		}
		outputs += net_layer->layer_data[layer_index_route].outputs;

		layer_index_start = layer_index_route + 1;
		layer_index_end = layer_index;
		if ((layer_index_route % 2) == 0) {
			layer_rw_route = &(net_layer->layer_rw0);
		}
		else {
			layer_rw_route = &(net_layer->layer_rw1);
		}
		AiNetLayerBufferLayerOffsetStack(layer_rw_route, layer_index_start, layer_index_end);

		if (n == 0) {
			first_out_w = net_layer->layer_data[layer_index_route].out_w;
			first_out_h = net_layer->layer_data[layer_index_route].out_h;
			first_out_c = net_layer->layer_data[layer_index_route].out_c;
			out_w = first_out_w;
			out_h = first_out_h;
			out_c = first_out_c;
		}
		else {
			if ((net_layer->layer_data[layer_index_route].out_w == first_out_w) && (net_layer->layer_data[layer_index_route].out_h == first_out_h)) {
				out_c += net_layer->layer_data[layer_index_route].out_c;
			}
			else {
				out_h = 0;
				out_w = 0;
				out_c = 0;
				return AI_ERROR_GENERAL;
			}
		}
	}
	inputs = outputs;

	layer_data->batch = batch;
	layer_data->outputs = outputs;
	layer_data->inputs = inputs;
	layer_data->out_w = out_w;
	layer_data->out_h = out_h;
	layer_data->out_c = out_c;

	out_h_align = out_h;
	out_w_align = ((out_w + 2 * padding_next + 7) / 8) * 8;
	out_c_align = out_c;
	outputs_align = (out_h_align * out_w_align * out_c_align);
	layer_data->padding_next = padding_next;
	layer_data->out_h_align = out_h_align;
	layer_data->out_w_align = out_w_align;
	layer_data->out_c_align = out_c_align;
	layer_data->outputs_align = outputs_align;

	output_length = (batch * outputs_align * sizeof(FLOAT_AI_T));
	AiNetLayerContentInit(&layer_data->output, output_length, layer_rw, layer_index);
	delta_length = (batch * outputs * sizeof(FLOAT_AI_T));
	AiNetLayerContentInit(&layer_data->delta, delta_length, layer_rw, layer_index);

	layer_func->forward = AiNetLayerRouteForward;
	layer_func->backward = AiNetLayerRouteBackward;


	return AI_ERROR_SUCCESS;
}

/*------------------------- End ---------------------------------------------*/
#endif
/*------------------------- End ---------------------------------------------*/
