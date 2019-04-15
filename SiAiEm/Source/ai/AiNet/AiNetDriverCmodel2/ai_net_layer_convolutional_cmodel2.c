/**
 ******************************************************************************
 * file    ai_net_layer_convolutional.c
 * author  sunlingge
 * version V100
 * brief   the ai net layer convolutional driver.
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
FLOAT_AI_T g_ai_cmodel2_map_padding_input[AI_NET_NETWORK_BUFFER_LENGTH_MAX];
FLOAT_AI_T g_ai_cmodel2_map_padding_output[AI_NET_NETWORK_BUFFER_LENGTH_MAX];

/*------------------------- Function Definition -----------------------------*/
/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLayerConvolutionalDefault(AI_NET_CONFIG *net_config, AI_NET_NETWORK *net_network, UINT32_T section_index)
{
	AI_NET_CONFIG_SECTION *option;
	AI_NET_NETWORK_DATA *params;

	option = &(net_config->section[section_index]);
	params = &(net_network->network_data);

	AiNetConfigSetOptionValueUint32(&(option->filters), AI_NET_CONFIG_OPTION_VALUE_FILTERS_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->size), AI_NET_CONFIG_OPTION_VALUE_SIZE_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueInt32(&(option->stride), AI_NET_CONFIG_OPTION_VALUE_STRIDE_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->pad), AI_NET_CONFIG_OPTION_VALUE_PAD_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->padding), AI_NET_CONFIG_OPTION_VALUE_PADDING_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->activation), AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->batch_normalize), AI_NET_CONFIG_OPTION_VALUE_BATCH_NORMALIZE_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->binary), AI_NET_CONFIG_OPTION_VALUE_BINARY_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->xnor), AI_NET_CONFIG_OPTION_VALUE_XNOR_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->bin_output), AI_NET_CONFIG_OPTION_VALUE_BIN_OUTPUT_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->flipped), AI_NET_CONFIG_OPTION_VALUE_FLIPPED_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->dot), AI_NET_CONFIG_OPTION_VALUE_DOT_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);

	if (option->pad.value > 0) {
		option->padding.value = (option->size.value / 2);
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
ERROR_T AiNetLayerConvolutionalSetLayer(AI_NET_LAYER *net_layer, AI_NET_CONFIG *net_config, UINT32_T section_index, AI_NET_NETWORK *net_network)
{
	AI_NET_CONFIG_SECTION *option;
	AI_NET_NETWORK_DATA *params;
	AI_NET_LAYER_DATA *layer_data;
	AI_NET_LAYER_FUNC *layer_func;
	AI_NET_LAYER_BUFFER *layer_ro;
	AI_NET_LAYER_BUFFER *layer_rw;
	INT32_T layer_index;
	UINT32_T batch, h, w, c, n, size, stride, padding, activation, batch_normalize, binary, xnor, adam, use_bin_output;
	UINT32_T weights_length, weights_update_length, biases_length, biases_update_length;
	UINT32_T out_h, out_w, out_c;
	UINT32_T outputs, inputs;
	UINT32_T output_length, delta_length;
	UINT32_T binary_weights_length;
	UINT32_T cweights_length;
	UINT32_T scales_length;
	UINT32_T binary_input_length;
    UINT32_T align;
    UINT32_T src_align;
	UINT32_T mean_arr_length;
	UINT32_T bit_align;
	UINT32_T scale_updates_length;
	UINT32_T mean_length;
	UINT32_T variance_length;
	UINT32_T mean_delta_length;
	UINT32_T variance_delta_length;
    UINT32_T rolling_mean_length;
    UINT32_T rolling_variance_length;
    UINT32_T x_length;
    UINT32_T x_norm_length;
	UINT32_T m_length;
	UINT32_T v_length;

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
	h = params->h;
	w = params->w;
	c = params->c;
	n = option->filters.value;
	size = option->size.value;
	stride = option->stride.value;
	padding = option->padding.value;
	activation = option->activation.value;
	batch_normalize = option->batch_normalize.value;
	binary = option->binary.value;
	xnor = option->xnor.value;
	adam = params->network_config->adam.value;
	use_bin_output = option->bin_output.value;

	layer_data->layer_index = layer_index;
    layer_data->h = h;
    layer_data->w = w;
    layer_data->c = c;
    layer_data->batch = batch;

	weights_length = (c * n * size * size * sizeof(FLOAT_AI_T));
	weights_update_length = (c * n * size * size * sizeof(FLOAT_AI_T));
	biases_length = (n * sizeof(FLOAT_AI_T));
	biases_update_length = (n * sizeof(FLOAT_AI_T));

	out_h = (((h + 2*padding - size) / stride) + 1);
	out_w = (((w + 2*padding - size) / stride) + 1);
	out_c = n;
	layer_data->out_h = out_h;
	layer_data->out_w = out_w;
	layer_data->out_c = out_c;

	outputs = ((out_h) * (out_w) * (out_c));
	inputs = (h * w * c);
	layer_data->outputs = outputs;
	layer_data->inputs = inputs;

	output_length = (batch * outputs * sizeof(FLOAT_AI_T));
	delta_length = (batch * outputs * sizeof(FLOAT_AI_T));

	layer_func->forward = AiNetLayerConvolutionalForward;
	layer_func->backward = AiNetLayerConvolutionalBackward;
	layer_func->update = AiNetLayerConvolutionalUpdate;

	if (binary > 0) {
		binary_weights_length = (c * n * size * size * sizeof(FLOAT_AI_T));
		cweights_length = (c * n * size * size * sizeof(UINT8_T));
		scales_length = (n * sizeof(FLOAT_AI_T));
	}

	if (xnor > 0) {
		binary_weights_length = (c * n * size * size * sizeof(FLOAT_AI_T));
		binary_input_length = (inputs * batch * sizeof(FLOAT_AI_T));
		align = 32;
        src_align = (out_h * out_w);
        bit_align = src_align + (align - src_align % align);
		mean_arr_length = (n * sizeof(FLOAT_AI_T));
		layer_data->bit_align = bit_align;
    }

    if (batch_normalize > 0){
		scales_length = (n * sizeof(FLOAT_AI_T));
		scale_updates_length = (n * sizeof(FLOAT_AI_T));
		mean_length =  (n * sizeof(FLOAT_AI_T));
		variance_length = (n * sizeof(FLOAT_AI_T));
		mean_delta_length = (n * sizeof(FLOAT_AI_T));
		variance_delta_length = (n * sizeof(FLOAT_AI_T));
        rolling_mean_length = (n * sizeof(FLOAT_AI_T));
        rolling_variance_length = (n * sizeof(FLOAT_AI_T));
        x_length = (batch * outputs * sizeof(FLOAT_AI_T));
        x_norm_length = (batch * outputs * sizeof(FLOAT_AI_T));
    }

    if (adam > 0) {
		layer_data->adam = 1;
		layer_data->b1 = params->network_config->b1.value;
		layer_data->b2 = params->network_config->b2.value;
		layer_data->eps = params->network_config->eps.value;
		m_length = (c * n * size * size * sizeof(FLOAT_AI_T));
		v_length = (c * n * size * size * sizeof(FLOAT_AI_T));
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
int tna_layer_main(int layer_index);
ERROR_T AiNetLayerConvolutionalForward(AI_NET_STATE *net_state, UINT32_T layer_index) 
{
	AI_NET_NETWORK *net_network;
	AI_NET_LAYER *net_layer;
	AI_NET_CONFIG_SECTION *option;
	AI_NET_LAYER_DATA *layer_data;
	UINT32_T batch, h, w, c, m, size, stride, padding, activation, k, n;
	UINT32_T out_h, out_w, out_c;
	UINT32_T outputs;
	FLOAT_AI_T *input;
	FLOAT_AI_T *weights;
	FLOAT_AI_T *biases;
    FLOAT_AI_T *workspace;
	FLOAT_AI_T *output;
	FLOAT_AI_T *batch_input;
	FLOAT_AI_T *batch_output;
	UINT32_T i;
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
	m = option->filters.value;
	size = option->size.value;
	stride = option->stride.value;
	padding = option->padding.value;
	activation = option->activation.value;
	outputs = layer_data->outputs;

	out_h = (((h + 2*padding - size) / stride) + 1);
	out_w = (((w + 2*padding - size) / stride) + 1);
	out_c = m;

	/* cmodel 1*/
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

	output = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->output));
    AiBaseBoardX86AioFillCpu(outputs_align*batch, AiBaseFloatCvtF32Fai(0.0f), output, 1);

    k = size*size*c;
    n = out_h*out_w;

    weights = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->weights));
	biases = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->biases));
	workspace = (FLOAT_AI_T *)AiNetNetworkContentGetAddress(&(net_network->network_data.workspace));
	input = net_state->input;

	batch_input = input;
	batch_output = output;
	for (i = 0; i < batch; i++){
#if (AI_PRODUCT_OPTION_CMODEL2_GEMM_TYPE == AI_PRODUCT_CMODEL2_GEMM_TYPE_CPU)
		AiNetLogCmodel2LayerWeights(layer_index, "WeightsCmodel2", (UINT8_T *)(weights), layer_data->weights.length);
		AiNetLogCmodel2LayerInput(layer_index, "InputCmodel2", (UINT8_T *)(net_state->input), (inputs_align * sizeof(FLOAT_AI_T)));
		AiBaseBoardCmodel2AioMapCopyCpu(batch, w_align, h_align, c_align, padding, batch_input, w, h, c, 0, g_ai_cmodel2_map_padding_input);
		AiNetLogCmodel2LayerInput(layer_index, "im2colInputCmodel2", (UINT8_T *)(g_ai_cmodel2_map_padding_input), (c*h*w * sizeof(FLOAT_AI_T)));
		AiBaseBoardX86AioIm2colCpuCustom(g_ai_cmodel2_map_padding_input, c, h, w, size, stride, padding, workspace);
		AiBaseLogLayerData(layer_index, "im2colOutputCmodel2", (char *)(workspace),
			(c*size*size * ((h + 2 * padding - size) / stride + 1) * ((w + 2 * padding - size) / stride + 1) * sizeof(FLOAT_AI_T)));
		memset((char *)g_ai_cmodel2_map_padding_output, 0, AI_NET_NETWORK_BUFFER_LENGTH_MAX*sizeof(FLOAT_AI_T));
		AiBaseBoardX86AioGemm(0, 0, m, n, k, AiBaseFloatCvtF32Fai(1.0f), weights, k, workspace, n, AiBaseFloatCvtF32Fai(1.0f), g_ai_cmodel2_map_padding_output, n);
		AiBaseBoardCmodel2AioMapCopyCpu(batch, out_w, out_h, out_c, 0, g_ai_cmodel2_map_padding_output, out_w_align, out_h_align, out_c_align, padding_next, batch_output);
		AiNetLogCmodel2LayerOutput(layer_index, "GemmOutputCmodel2", (UINT8_T *)(batch_output), (outputs_align * sizeof(FLOAT_AI_T)));
#endif
#if (AI_PRODUCT_OPTION_CMODEL2_GEMM_TYPE == AI_RRODUCT_CMODEL2_GEMM_TYPE_CMODEL1)
		if (layer_index == 0) {
			AiBaseLogCmodelCommandDataFormat("[layer_index]=[%d]\r\n", layer_index);
		}
		else {
			AiBaseLogCmodelCommandDataAppendFormat("\r\n\r\n[layer_index]=[%d]\r\n", layer_index);
		}
		AiNetCmodel2LayerCommandGenerate(layer_index, (UINT64_T)net_state);
		AiNetLogCmodel2LayerWeights(layer_index, "WeightsCmodel2", (UINT8_T *)(weights), layer_data->weights.length);
		AiNetLogCmodel2LayerInput(layer_index, "InputCmodel1", (UINT8_T *)(net_state->input), (inputs_align*sizeof(FLOAT_AI_T)));
		tna_layer_main(layer_index);
		AiNetLogCmodel2LayerOutput(layer_index, "GemmOutputCmodel2", (UINT8_T *)(batch_output), (outputs_align*sizeof(FLOAT_AI_T)));
#endif
		batch_output += outputs_align * sizeof(FLOAT_AI_T);
        batch_input += inputs_align * sizeof(FLOAT_AI_T);
    }

    AiBaseBoardCmodel2AioAddBias(output, biases, batch, m, out_h, out_w, out_h_align, out_w_align, padding_next);
	AiBaseLogLayerData(layer_index, "BiasesOutputCmodel2", (UINT8_T *)(output), (layer_data->output.length));
    AiBaseBoardX86AioActivateArrayCpuCustom(output, m*out_h_align*out_w_align*batch, activation);
	AiBaseLogLayerData(layer_index, "ActivateOutputCmodel2", (UINT8_T *)(output), (layer_data->output.length));

	net_state->output = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->output));

	AiBaseLogLayerData(layer_index, "BiasesCmodel2", (UINT8_T *)biases, layer_data->biases.length);
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
ERROR_T AiNetLayerConvolutionalBackward(AI_NET_STATE *net_state, UINT32_T layer_index) {
	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLayerConvolutionalUpdate(AI_NET_STATE *net_state, UINT32_T layer_index) {
	return AI_ERROR_SUCCESS;
}



/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetLayerConvolutionalSetLayerContent(AI_NET_LAYER *net_layer, AI_NET_CONFIG *net_config, UINT32_T section_index, AI_NET_NETWORK *net_network)
{
	AI_NET_CONFIG_SECTION *option;
	AI_NET_NETWORK_DATA *params;
	AI_NET_LAYER_DATA *layer_data;
	AI_NET_LAYER_FUNC *layer_func;
	AI_NET_LAYER_BUFFER *layer_ro;
	AI_NET_LAYER_BUFFER *layer_rw;
	INT32_T layer_index;
	UINT32_T batch, h, w, c, n, size, stride, padding, activation, batch_normalize, binary, xnor, adam, use_bin_output;
	UINT32_T weights_length, weights_update_length, biases_length, biases_update_length;
	UINT32_T out_h, out_w, out_c;
	UINT32_T outputs, inputs;
	UINT32_T output_length, delta_length;
	UINT32_T binary_weights_length;
	UINT32_T cweights_length;
	UINT32_T scales_length;
	UINT32_T binary_input_length;
	UINT32_T align;
	UINT32_T src_align;
	UINT32_T mean_arr_length;
	UINT32_T bit_align;
	UINT32_T scale_updates_length;
	UINT32_T mean_length;
	UINT32_T variance_length;
	UINT32_T mean_delta_length;
	UINT32_T variance_delta_length;
	UINT32_T rolling_mean_length;
	UINT32_T rolling_variance_length;
	UINT32_T x_length;
	UINT32_T x_norm_length;
	UINT32_T m_length;
	UINT32_T v_length;
	UINT32_T workspace_size;
	FLOAT_AI_T bflops;
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
	h = params->h;
	w = params->w;
	c = params->c;
	n = option->filters.value;
	size = option->size.value;
	stride = option->stride.value;
	padding = option->padding.value;
	activation = option->activation.value;
	batch_normalize = option->batch_normalize.value;
	binary = option->binary.value;
	xnor = option->xnor.value;
	adam = params->network_config->adam.value;
	use_bin_output = option->bin_output.value;

	layer_data->layer_index = layer_index;
	layer_data->h = h;
	layer_data->w = w;
	layer_data->c = c;
	layer_data->batch = batch;

	weights_length = (c * n * size * size * sizeof(FLOAT_AI_T));
	AiNetLayerContentInit(&layer_data->weights, weights_length, layer_ro, layer_index); 
	weights_update_length = (c * n * size * size * sizeof(FLOAT_AI_T));
	AiNetLayerContentInit(&layer_data->weights_update, weights_update_length, layer_rw, layer_index);
	biases_length = (n * sizeof(FLOAT_AI_T));
	AiNetLayerContentInit(&layer_data->biases, biases_length, layer_ro, layer_index);
	biases_update_length = (n * sizeof(FLOAT_AI_T));
	AiNetLayerContentInit(&layer_data->biases_update, biases_update_length, layer_rw, layer_index);

	out_h = (((h + 2 * padding - size) / stride) + 1);
	out_w = (((w + 2 * padding - size) / stride) + 1);
	out_c = n;
	layer_data->out_h = out_h;
	layer_data->out_w = out_w;
	layer_data->out_c = out_c;

	outputs = ((out_h) * (out_w) * (out_c));
	inputs = (h * w * c);
	layer_data->outputs = outputs;
	layer_data->inputs = inputs;

	/* tna_ir */
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

	/* tna ir */
	output_length = (batch * outputs_align * sizeof(FLOAT_AI_T));
	AiNetLayerContentInit(&layer_data->output, output_length, layer_rw, layer_index);
	delta_length = (batch * outputs * sizeof(FLOAT_AI_T));
	AiNetLayerContentInit(&layer_data->delta, delta_length, layer_rw, layer_index);

	layer_func->forward = AiNetLayerConvolutionalForward;
	layer_func->backward = AiNetLayerConvolutionalBackward;
	layer_func->update = AiNetLayerConvolutionalUpdate;

	if (binary > 0) {
		binary_weights_length = (c * n * size * size * sizeof(FLOAT_AI_T));
		cweights_length = (c * n * size * size * sizeof(UINT8_T));
		scales_length = (n * sizeof(FLOAT_AI_T));

		AiNetLayerContentInit(&layer_data->binary_weights, binary_weights_length, layer_rw, layer_index);
		AiNetLayerContentInit(&layer_data->cweights, cweights_length, layer_rw, layer_index);
		AiNetLayerContentInit(&layer_data->scales, scales_length, layer_ro, layer_index);
	}

	if (xnor > 0) {
		binary_weights_length = (c * n * size * size * sizeof(FLOAT_AI_T));
		binary_input_length = (inputs * batch * sizeof(FLOAT_AI_T));
		align = 32;
		src_align = (out_h * out_w);
		bit_align = src_align + (align - src_align % align);
		mean_arr_length = (n * sizeof(FLOAT_AI_T));

		AiNetLayerContentInit(&layer_data->binary_weights, binary_weights_length, layer_rw, layer_index);
		AiNetLayerContentInit(&layer_data->binary_input, binary_input_length, layer_rw, layer_index);
		layer_data->bit_align = bit_align;
		AiNetLayerContentInit(&layer_data->mean_arr, mean_arr_length, layer_rw, layer_index);
	}

	if (batch_normalize > 0) {
		scales_length = (n * sizeof(FLOAT_AI_T));
		scale_updates_length = (n * sizeof(FLOAT_AI_T));
		mean_length = (n * sizeof(FLOAT_AI_T));
		variance_length = (n * sizeof(FLOAT_AI_T));
		mean_delta_length = (n * sizeof(FLOAT_AI_T));
		variance_delta_length = (n * sizeof(FLOAT_AI_T));
		rolling_mean_length = (n * sizeof(FLOAT_AI_T));
		rolling_variance_length = (n * sizeof(FLOAT_AI_T));
		x_length = (batch * outputs * sizeof(FLOAT_AI_T));
		x_norm_length = (batch * outputs * sizeof(FLOAT_AI_T));

		AiNetLayerContentInit(&layer_data->scales, scales_length, layer_ro, layer_index);
		AiNetLayerContentInit(&layer_data->scale_updates, scale_updates_length, layer_rw, layer_index);
		AiNetLayerContentInit(&layer_data->mean, mean_length, layer_rw, layer_index);
		AiNetLayerContentInit(&layer_data->variance, variance_length, layer_rw, layer_index);
		AiNetLayerContentInit(&layer_data->mean_delta, mean_delta_length, layer_rw, layer_index);
		AiNetLayerContentInit(&layer_data->variance_delta, variance_delta_length, layer_rw, layer_index);
		AiNetLayerContentInit(&layer_data->rolling_mean, rolling_mean_length, layer_ro, layer_index);
		AiNetLayerContentInit(&layer_data->rolling_variance, rolling_variance_length, layer_ro, layer_index);
		AiNetLayerContentInit(&layer_data->x, x_length, layer_rw, layer_index);
		AiNetLayerContentInit(&layer_data->x_norm, x_norm_length, layer_rw, layer_index);
	}

	if (adam > 0) {
		layer_data->adam = 1;
		layer_data->b1 = params->network_config->b1.value;
		layer_data->b2 = params->network_config->b2.value;
		layer_data->eps = params->network_config->eps.value;
		m_length = (c * n * size * size * sizeof(FLOAT_AI_T));
		v_length = (c * n * size * size * sizeof(FLOAT_AI_T));
		AiNetLayerContentInit(&layer_data->m, m_length, layer_ro, layer_index);
		AiNetLayerContentInit(&layer_data->v, v_length, layer_ro, layer_index);
	}

	if (xnor > 0) {
		workspace_size = (bit_align * size * size * c * sizeof(FLOAT_AI_T));
	}
	else {
		workspace_size = (out_h * out_w * size * size * c * sizeof(FLOAT_AI_T));
	}
	layer_data->workspace_size = workspace_size;

	bflops = (FLOAT_AI_T)((2.0 * n * size * size * c * out_h * out_w) / 1000000000.);
	layer_data->bflops = bflops;

	return AI_ERROR_SUCCESS;
}

/*------------------------- End ---------------------------------------------*/
#endif
/*------------------------- End ---------------------------------------------*/
