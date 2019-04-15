/**
 ******************************************************************************
 * file    ai_net_layer_yolo.c
 * author  sunlingge
 * version V100
 * brief   the ai net layer yolo driver.
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
ERROR_T AiNetLayerYoloDefault(AI_NET_CONFIG *net_config, AI_NET_NETWORK *net_network, UINT32_T section_index)
{
	AI_NET_CONFIG_SECTION *option;
	AI_NET_NETWORK_DATA *params;
	UINT8_T_PTR option_array_ptr[AI_NET_CONFIG_OPTION_VALUE_ARRAY_NUMBER_MAX];
	UINT8_T option_value[AI_NET_CONFIG_LINE_LENGTH_MAX];
	UINT32_T index;

	option = &(net_config->section[section_index]);
	params = &(net_network->network_data);

	AiNetConfigSetOptionValueUint32(&(option->classes), AI_NET_CONFIG_OPTION_VALUE_CLASSES_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->num), (UINT8_T *)AI_NET_CONFIG_OPTION_VALUE_NUM_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);

	memset((char *)option_array_ptr, 0, sizeof(UINT8_T *)*AI_NET_CONFIG_OPTION_VALUE_ARRAY_NUMBER_MAX);
	strcpy((char *)option_value, AI_NET_CONFIG_OPTION_VALUE_MASK_DEFAULT);
	AiNetConfigReadOptionValue(option_value, (UINT8_T_PTR *)option_array_ptr);
	for(index = 0; index < AI_NET_CONFIG_OPTION_VALUE_MASK_ARRAY_LENGTH_MAX; index++) {
		if (option_array_ptr[index] == NULL)
			break;
		AiNetConfigSetOptionValueUint32(&(option->mask[index]), option_array_ptr[index], AI_NET_CONFIG_SET_BY_DEFAULT);
	}

	AiNetConfigSetOptionValueUint32(&(option->max), (UINT8_T *)AI_NET_CONFIG_OPTION_VALUE_MAX_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueFloat(&(option->jitter), (UINT8_T *)AI_NET_CONFIG_OPTION_VALUE_JITTER_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->focal_loss), (UINT8_T *)AI_NET_CONFIG_OPTION_VALUE_FOCAL_LOSS_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueFloat(&(option->ignore_thresh), (UINT8_T *)AI_NET_CONFIG_OPTION_VALUE_IGNORE_THRESH_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueFloat(&(option->truth_thresh), (UINT8_T *)AI_NET_CONFIG_OPTION_VALUE_TRUTH_THRESH_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->random), (UINT8_T *)AI_NET_CONFIG_OPTION_VALUE_RANDOM_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);

	memset((char *)option_array_ptr, 0, sizeof(UINT8_T *)*AI_NET_CONFIG_OPTION_VALUE_ARRAY_NUMBER_MAX);
	strcpy((char *)option_value, AI_NET_CONFIG_OPTION_VALUE_ANCHORS_DEFAULT);
	AiNetConfigReadOptionValue(option_value, (UINT8_T_PTR *)option_array_ptr);
	for(index = 0; index < AI_NET_CONFIG_OPTION_VALUE_ANCHORS_ARRAY_LENGTH_MAX; index++) {
		if (option_array_ptr[index] == NULL)
			break;
		AiNetConfigSetOptionValueFloat(&(option->anchors[index]), option_array_ptr[index], AI_NET_CONFIG_SET_BY_DEFAULT);
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
ERROR_T AiNetLayerYoloSetLayer(AI_NET_LAYER *net_layer, AI_NET_CONFIG *net_config, UINT32_T section_index, AI_NET_NETWORK *net_network)
{
	AI_NET_CONFIG_SECTION *option;
	AI_NET_NETWORK_DATA *params;
	AI_NET_LAYER_DATA *layer_data;
	AI_NET_LAYER_FUNC *layer_func;
	AI_NET_LAYER_BUFFER *layer_ro;
	AI_NET_LAYER_BUFFER *layer_rw;
	INT32_T layer_index;
	UINT32_T batch;
	UINT32_T n, total, h, w, classes, c, max_boxes;
	UINT32_T out_h, out_w, out_c;
	UINT32_T cost_length, biases_length, biases_update_length;
	UINT32_T outputs, inputs;
	UINT32_T output_length, delta_length;
	UINT32_T layer_index_start, layer_index_end;
	UINT32_T index;

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

	n = 0;
	for (index = 0; index < AI_NET_CONFIG_OPTION_VALUE_MASK_ARRAY_LENGTH_MAX; index++) {
		if (option->mask[index].set_type == AI_NET_CONFIG_SET_BY_NONE)
			break;
		n++;
	}
	total = option->num.value;
	batch = params->batch;
	h = params->h;
	w = params->w;
	classes = option->classes.value;
	c = (n * (classes + 4 + 1));
	out_h = params->h;
	out_w = params->w;
	out_c = (n * (classes + 4 + 1));
	max_boxes = option->max.value;

    layer_data->batch = batch;
    layer_data->h = h;
    layer_data->w = w;
    layer_data->c = c;
	layer_data->out_h = out_h;
	layer_data->out_w = out_w;
	layer_data->out_c = out_c;
	layer_data->n = n;

	cost_length = 1*sizeof(FLOAT_AI_T);
	AiNetLayerContentInit(&layer_data->cost, cost_length, layer_rw, layer_index);
	biases_length = (total * 2) * sizeof(FLOAT_AI_T);
	AiNetLayerContentInit(&layer_data->biases, biases_length, layer_rw, layer_index);
	biases_update_length = (n * 2) * sizeof(FLOAT_AI_T);
	AiNetLayerContentInit(&layer_data->biases_update, biases_update_length, layer_rw, layer_index);

	outputs = (h * w * n * (classes + 4 + 1));
	inputs = outputs;
	layer_data->outputs = outputs;
	layer_data->inputs = inputs;

	layer_data->truths = (max_boxes * (4 + 1));
	delta_length = (batch * outputs * sizeof(FLOAT_AI_T));
	AiNetLayerContentInit(&layer_data->delta, delta_length, layer_rw, layer_index);

	output_length = (batch * outputs * sizeof(FLOAT_AI_T));
	AiNetLayerContentInit(&layer_data->output, output_length, layer_rw, layer_index);

	layer_func->forward = AiNetLayerYoloForward;
	layer_func->backward = AiNetLayerYoloBackward;

	layer_index_start = layer_index + 1;
	layer_index_end = AI_NET_LAYER_MAX - 1;
	AiNetLayerBufferLayerOffsetStack(layer_rw, layer_index_start, layer_index_end);

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLayerYoloForward(AI_NET_STATE *net_state, UINT32_T layer_index) {
	AI_NET_NETWORK *net_network;
	AI_NET_LAYER *net_layer;
	AI_NET_CONFIG_SECTION *option;
	AI_NET_LAYER_DATA *layer_data;
	UINT32_T outputs, batch, w, h, c, out_w, out_h, out_c, stride, activation, num;
	FLOAT_AI_T *delta;
	FLOAT_AI_T *output;
	FLOAT_AI_T *input;
	UINT32_T b, n;
	INT32_T index;
	UINT32_T classes;

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
	activation = option->activation.value;
	num = layer_data->n;
	classes = option->classes.value;
	outputs = layer_data->outputs;

	input = net_state->input;
	output = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->output));

    memcpy(output, input, outputs*batch*sizeof(FLOAT_AI_T));

    for (b = 0; b < batch; ++b){
        for(n = 0; n < num; ++n){
            index = AiNetLayerYoloEntryIndex(layer_index, b, n*w*h, 0);
            AiBaseBoardX86AioActivateArrayCpuCustom(output + index, 2*w*h, AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LOGISTIC_VALUE);
            index = AiNetLayerYoloEntryIndex(layer_index, b, n*w*h, 4);
            AiBaseBoardX86AioActivateArrayCpuCustom(output + index, (1+classes)*w*h, AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LOGISTIC_VALUE);
        }
    }

	delta = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->delta));
    memset(delta, 0, outputs * batch * sizeof(FLOAT_AI_T));

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
ERROR_T AiNetLayerYoloBackward(AI_NET_STATE *net_state, UINT32_T layer_index) {
	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
INT32_T AiNetLayerYoloEntryIndex(UINT32_T layer_index, INT32_T batch, INT32_T location, INT32_T entry)
{
	AI_NET_LAYER *net_layer;
	AI_NET_LAYER_DATA *layer_data;
	UINT32_T outputs, w, h;
	INT32_T n, loc;
	INT32_T entry_index;
	UINT32_T classes;

	net_layer = AiNetDataGetAiNetLayer();
	layer_data = &(net_layer->layer_data[layer_index]);

	h = layer_data->h;
	w = layer_data->w;
	outputs = layer_data->outputs;
	classes = layer_data->layer_config->classes.value;

    n =   location / (w * h);
    loc = location % (w * h);
    entry_index = batch*outputs + n*w*h*(4+classes+1) + entry*w*h + loc;

	return entry_index;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
UINT32_T AiNetLayerYoloNumDetections(UINT32_T layer_index, FLOAT_AI_T thresh)
{
	AI_NET_LAYER *net_layer;
	AI_NET_LAYER_DATA *layer_data;
	AI_NET_CONFIG_SECTION *option;
	UINT32_T outputs, w, h, num;
	FLOAT_AI_T *output;
    UINT32_T i, n;
    UINT32_T count = 0;
	int obj_index;

	net_layer = AiNetDataGetAiNetLayer();
	layer_data = &(net_layer->layer_data[layer_index]);
	option = layer_data->layer_config;

	h = layer_data->h;
	w = layer_data->w;
	outputs = layer_data->outputs;
	num = layer_data->n;
	output = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->output));

    for (i = 0; i < w*h; i++){
        for(n = 0; n < num; ++n){
            obj_index  = AiNetLayerYoloEntryIndex(layer_index, 0, n*w*h + i, 4);
			if (!(AiBaseFloatCmpLe(output[obj_index],thresh))) {
				++count;
			}
        }
    }
    return count;
}

/*------------------------- End ---------------------------------------------*/
#endif
/*------------------------- End ---------------------------------------------*/
