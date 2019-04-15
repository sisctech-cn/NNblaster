/**
 ******************************************************************************
 * file    ai_net_layer.c
 * author  sunlingge
 * version V100
 * brief   the ai net layer driver.
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
ERROR_T AiNetLayerInit(AI_NET_LAYER *net_layer, AI_NET_CONFIG *net_config)
{
	UINT32_T section_index;
	UINT32_T layer_index;
	AI_NET_NETWORK *net_network;
	UINT32_T inputs_save;
	UINT32_T h_save;
	UINT32_T w_save;
	UINT32_T c_save;
	UINT32_T layer_type;
	AI_NET_LAYER_BUFFER *layer_ro;
	AI_NET_LAYER_BUFFER *layer_rw0;
	AI_NET_LAYER_BUFFER *layer_rw1;
	ERROR_T result = AI_ERROR_SUCCESS;

	net_network = AiNetDataGetAiNetNetwork();

	inputs_save = net_network->network_data.inputs;
	h_save = net_network->network_data.h;
	w_save = net_network->network_data.w;
	c_save = net_network->network_data.c;

	layer_ro = &(net_layer->layer_ro);
	AiNetLayerBufferInit(layer_ro, net_layer->layer_ro_buffer, AI_NET_LAYER_RO_MEM_LENGTH_MAX);
	layer_rw0 = &(net_layer->layer_rw0);
	AiNetLayerBufferInit(layer_rw0, net_layer->layer_rw0_buffer, AI_NET_LAYER_RW0_MEM_LENGTH_MAX);
	layer_rw1 = &(net_layer->layer_rw1);
	AiNetLayerBufferInit(layer_rw1, net_layer->layer_rw1_buffer, AI_NET_LAYER_RW1_MEM_LENGTH_MAX);
	
	net_layer->layer_number = (net_config->section_number - 1);
	net_network->network_data.layer_index = 0;
	for (section_index = 0; section_index < net_config->section_number; section_index++) {
		layer_index = net_network->network_data.layer_index;
		AiNetLayerBufferSetLayerOffset(layer_ro, layer_index);
		AiNetLayerBufferInitOffset(layer_ro);
		AiNetLayerBufferInitOffset(layer_rw0);
		AiNetLayerBufferInitOffset(layer_rw1);

		switch (net_config->section[section_index].type.value) {
			case AI_NET_CONFIG_SECTION_CONVOLUTIONAL_TYPE:
				result = AiNetLayerConvolutionalDefault(net_config, net_network, section_index);
				if (result != AI_ERROR_SUCCESS)
					return result;
				result = AiNetLayerConvolutionalSetLayer(net_layer, net_config, section_index, net_network);
				if (result != AI_ERROR_SUCCESS)
					return result;
				break;
			case AI_NET_CONFIG_SECTION_SHORTCUT_TYPE:
				result = AiNetLayerShortcutDefault(net_config, net_network, section_index);
				if (result != AI_ERROR_SUCCESS)
					return result;
				result = AiNetLayerShortcutSetLayer(net_layer, net_config, section_index, net_network);
				if (result != AI_ERROR_SUCCESS)
					return result;
				break;
			case AI_NET_CONFIG_SECTION_YOLO_TYPE:
				result = AiNetLayerYoloDefault(net_config, net_network, section_index);
				if (result != AI_ERROR_SUCCESS)
					return result;
				result = AiNetLayerYoloSetLayer(net_layer, net_config, section_index, net_network);
				if (result != AI_ERROR_SUCCESS)
					return result;
				break;
			case AI_NET_CONFIG_SECTION_UPSAMPLE_TYPE:
				result = AiNetLayerUpsampleDefault(net_config, net_network, section_index);
				if (result != AI_ERROR_SUCCESS)
					return result;
				result = AiNetLayerUpsampleSetLayer(net_layer, net_config, section_index, net_network);
				if (result != AI_ERROR_SUCCESS)
					return result;
				break;
			case AI_NET_CONFIG_SECTION_ROUTE_TYPE:
				result = AiNetLayerRouteDefault(net_config, net_network, section_index);
				if (result != AI_ERROR_SUCCESS)
					return result;
				result = AiNetLayerRouteSetLayer(net_layer, net_config, section_index, net_network);
				if (result != AI_ERROR_SUCCESS)
					return result;
				break;
			default:
				result = AI_ERROR_GENERAL;
				break;
		}

		if (result != AI_ERROR_SUCCESS)
			continue;
		
		AiNetLayerDefault(net_config, section_index); 
		AiNetLayerSetNetwork(net_network, net_layer, layer_index);
		(net_network->network_data.layer_index)++;

	}

	net_network->network_data.inputs = inputs_save;
	net_network->network_data.h = h_save;
	net_network->network_data.w = w_save;
	net_network->network_data.c = c_save;

	for (layer_index = (net_network->network_data.layer_index -1); layer_index > 0; layer_index--) {
		if (net_layer->layer_data[layer_index].layer_config->type.value != AI_NET_CONFIG_SECTION_COST_TYPE) {
			break;
		}
	}
	net_network->network_data.outputs = net_layer->layer_data[layer_index].outputs;
	memcpy((char *)&(net_network->network_data.output),  (char *)&(net_layer->layer_data[layer_index].output), sizeof(AI_NET_LAYER_CONTENT));
	AiNetNetworkBufferInit(&(net_network->network_data.network_rw));
	AiNetNetworkContentInit(&(net_network->network_data.workspace), net_layer->layer_report.workspace_size_max, &(net_network->network_data.network_rw));

	layer_type = net_layer->layer_data[net_network->network_data.layer_index-1].layer_config->type.value;
	if (((net_network->network_data.w % 32 != 0) || (net_network->network_data.h % 32 != 0)) && 
		((layer_type == AI_NET_CONFIG_SECTION_YOLO_TYPE) || (layer_type == AI_NET_CONFIG_SECTION_REGION_TYPE) || (layer_type == AI_NET_CONFIG_SECTION_DETECTION_TYPE))) {
        result = AI_ERROR_GENERAL;
    }

	AiNetLayerReportSet(net_layer, net_network);

	return result;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLayerDefault(AI_NET_CONFIG *net_config, UINT32_T section_index)
{
	AI_NET_CONFIG_SECTION *option;

	option = &(net_config->section[section_index]);
	AiNetConfigSetOptionValueUint32(&(option->onlyforward), AI_NET_CONFIG_OPTION_VALUE_ONLYFORWARD_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->stopbackward), AI_NET_CONFIG_OPTION_VALUE_STOPBACKWARD_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->dontload), AI_NET_CONFIG_OPTION_VALUE_DONTLOAD_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->dontloadscales), AI_NET_CONFIG_OPTION_VALUE_DONTLOADSCALES_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);

	return AI_ERROR_GENERAL;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLayerSetNetwork(AI_NET_NETWORK *net_network, AI_NET_LAYER *net_layer, UINT32_T layer_index)
{
	AI_NET_LAYER_DATA *layer_data;
	AI_NET_NETWORK_DATA *network_data;

	layer_data = &(net_layer->layer_data[layer_index]);
	network_data = &(net_network->network_data);

	network_data->inputs = layer_data->outputs;
	network_data->h = layer_data->out_h;
	network_data->w = layer_data->out_w;
	network_data->c = layer_data->out_c;

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLayerReportInit(AI_NET_LAYER *net_layer) {
	AI_NET_LAYER_REPORT *layer_report;

	layer_report = &(net_layer->layer_report);
	layer_report->ro_mem_max = 0;
	layer_report->ro_mem_total = 0;
	layer_report->rw0_mem_max = 0;
	layer_report->rw0_mem_total = 0;
	layer_report->rw1_mem_max = 0;
	layer_report->rw1_mem_total = 0;
	layer_report->workspace_size_max = 0;
	layer_report->max_inputs = 0;
	layer_report->max_outputs = 0;
	layer_report->bflops_total = 0.0;

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLayerReportSet(AI_NET_LAYER *net_layer, AI_NET_NETWORK *net_network) {
	AI_NET_LAYER_REPORT *layer_report;
	INT32_T layer_index;
	AI_NET_LAYER_BUFFER_STACK *ro_stack;
	AI_NET_LAYER_BUFFER_STACK *rw0_stack;
	AI_NET_LAYER_BUFFER_STACK *rw1_stack;

	AiNetLayerReportInit(net_layer);

	layer_report = &(net_layer->layer_report);
	for (layer_index = 0; layer_index < net_network->network_data.layer_index; layer_index++) {
		ro_stack = &(net_layer->layer_ro.stack[layer_index]);
		if ((ro_stack->layer_length)  > layer_report->ro_mem_max) {
			layer_report->ro_mem_max = (ro_stack->layer_length); 
		}
		layer_report->ro_mem_total += ro_stack->layer_length;

		rw0_stack = &(net_layer->layer_rw0.stack[layer_index]);
		if ((rw0_stack->layer_offset + rw0_stack->layer_length)  > layer_report->rw0_mem_max) {
			layer_report->rw0_mem_max = (rw0_stack->layer_offset + rw0_stack->layer_length); 
		}
		layer_report->rw0_mem_total += rw0_stack->layer_length;
		
		rw1_stack = &(net_layer->layer_rw1.stack[layer_index]);
		if ((rw1_stack->layer_offset + rw1_stack->layer_length)  > layer_report->rw1_mem_max) {
			layer_report->rw1_mem_max = (rw1_stack->layer_offset + rw1_stack->layer_length); 
		}
		layer_report->rw1_mem_total += rw1_stack->layer_length;

		if (net_layer->layer_data[layer_index].workspace_size > layer_report->workspace_size_max) {
			layer_report->workspace_size_max = net_layer->layer_data[layer_index].workspace_size;
		}
		if (net_layer->layer_data[layer_index].inputs > layer_report->max_inputs) {
			layer_report->max_inputs = net_layer->layer_data[layer_index].inputs;
		}
		if (net_layer->layer_data[layer_index].outputs > layer_report->max_outputs) {
			layer_report->max_outputs = net_layer->layer_data[layer_index].outputs;
		}
		layer_report->bflops_total += net_layer->layer_data[layer_index].bflops;
	}

	return AI_ERROR_SUCCESS;
}

/*------------------------- End ---------------------------------------------*/
#endif
/*------------------------- End ---------------------------------------------*/
