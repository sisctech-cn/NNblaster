/**
 ******************************************************************************
 * file    ai_net_weights.c
 * author  sunlingge
 * version V100
 * brief   the ai net weights driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_net_global.h"
#pragma warning(disable:4996)

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/
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
ERROR_T AiNetWeightsFp16Generate(UINT8_T *buffer, UINT32_T length)
{
	UINT8_T file_path[256];
	FILE *f;
	UINT32_T index;
	FLOAT32_T f32_value;
	FLOAT16_T f16_value;

	/* save float32 weights */
	sprintf((char *)file_path, "%syolo3.f32.weights", AI_PRODUCT_LOG_IDENT_PATH"/");
	f = fopen(file_path, "wb");
	if (!f) {
		return AI_ERROR_GENERAL;
	}

	fwrite(buffer, 1, length, f);
	fclose(f);

	/* save float16 weights */
	sprintf((char *)file_path, "%syolo3.f16.weights", AI_PRODUCT_LOG_IDENT_PATH"/");
	f = fopen(file_path, "wb");
	if (!f) {
		return AI_ERROR_GENERAL;
	}
	for (index = 0; index < length; index = index + 4) {
		f32_value = *((FLOAT32_T *)(buffer + index));
		f16_value = AiBaseFloat16CvtF32F16(f32_value);
		fwrite((char *)&f16_value, 1, sizeof(FLOAT16_T), f);
	}
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
ERROR_T AiNetWeightsFp16Append(UINT8_T *buffer, UINT32_T length)
{
	UINT8_T file_path[256];
	FILE *f;
	UINT32_T index;
	FLOAT32_T f32_value;
	FLOAT16_T f16_value;

	/* save float 32 */
	sprintf((char *)file_path, "%syolo3.f32.weights", AI_PRODUCT_LOG_IDENT_PATH"/");
	f = fopen(file_path, "ab+");
	if (!f) {
		return AI_ERROR_GENERAL;
	}

	fwrite(buffer, 1, length, f);
	fclose(f);

	/* save float 16 */
	sprintf((char *)file_path, "%syolo3.f16.weights", AI_PRODUCT_LOG_IDENT_PATH"/");
	f = fopen(file_path, "ab+");
	if (!f) {
		return AI_ERROR_GENERAL;
	}
	for (index = 0; index < length; index = index + 4) {
		f32_value = *((FLOAT32_T *)(buffer + index));
		f16_value = AiBaseFloat16CvtF32F16(f32_value);
		fwrite((char *)&f16_value, 1, sizeof(FLOAT16_T), f);
	}
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
ERROR_T AiNetWeightsF16Save(AI_NET_NETWORK *net_network, AI_NET_LAYER *net_layer)
{
	INT32_T layer_index;
	AI_NET_LAYER_DATA *layer_data;
	AI_NET_CONFIG_SECTION *option;
	UINT32_T n;
	UINT32_T c;
	UINT32_T size;
	FLOAT_AI_T *biases;
	FLOAT_AI_T *weights;

	for (layer_index = 0; layer_index < net_network->network_data.layer_index; layer_index++) {
		if (net_layer->layer_data[layer_index].layer_config->dontload.value > 0)
			continue;
		switch (net_layer->layer_data[layer_index].layer_config->type.value) {
		case AI_NET_CONFIG_SECTION_CONVOLUTIONAL_TYPE:
			AiBaseUpdateDlgStatus("AiNetWeightsF16Save layer_index=%d", layer_index);
			layer_data = &(net_layer->layer_data[layer_index]);
			option = layer_data->layer_config;
			biases = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->biases));
			weights = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->weights));
			n = option->filters.value;
			size = option->size.value;
			c = layer_data->c;
			if (layer_index == 0) {
				AiNetWeightsFp16Generate((char *)weights, sizeof(FLOAT_AI_T)*(size*size*c*n));
			}
			else {
				AiNetWeightsFp16Append((char *)weights, sizeof(FLOAT_AI_T)*(size*size*c*n));
			}
			AiNetWeightsFp16Append((char *)biases, sizeof(FLOAT_AI_T)*n);

			break;
		default:
			break;
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
ERROR_T AiNetWeightsLoad(AI_NET_NETWORK *net_network, AI_NET_LAYER *net_layer)
{
	UINT8_T file_path[256];
	FILE *file;
	INT32_T major;
	INT32_T minor;
	INT32_T revision;
	UINT64_T iseen;
	INT32_T transpose;
	INT32_T layer_index;
	ERROR_T result = AI_ERROR_GENERAL;

	sprintf((char *)file_path, AI_PRODUCT_FILE_PATH""AI_PRODUCT_YOLOV3_CONFIG_PATH""AI_PRODUCT_YOLOV3_WEIGHTS_FILE);
	file = fopen((char *)file_path, "rb");
	if (file == 0) {
		return AI_ERROR_GENERAL;
	}

	fread(&major, sizeof(INT32_T), 1, file);
	fread(&minor, sizeof(INT32_T), 1, file);
	fread(&revision, sizeof(INT32_T), 1, file);
	if ((major * 10 + minor) >= 2) {
		iseen = 0;
		fread(&iseen, sizeof(UINT64_T), 1, file);
		net_network->network_data.seen = (UINT32_T)iseen;
	}
	else {
		fread(&(net_network->network_data.seen), sizeof(INT32_T), 1, file);
	}
	transpose = (major > 1000) || (minor > 1000);

	for (layer_index = 0; layer_index < net_network->network_data.layer_index; layer_index++) {
		if (net_layer->layer_data[layer_index].layer_config->dontload.value > 0)
			continue;
		switch (net_layer->layer_data[layer_index].layer_config->type.value) {
		case AI_NET_CONFIG_SECTION_CONVOLUTIONAL_TYPE:
			AiBaseUpdateDlgStatus("AiNetWeightsLoad layer_index=%d", layer_index);
			result = AiNetWeightsLoadConvolutional(net_layer, layer_index, file);
			break;
		default:
			break;
		}

		if (result != AI_ERROR_SUCCESS)
			break;
	}


	fclose(file);
	return result;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetWeightsLoadConvolutional(AI_NET_LAYER *net_layer, INT32_T layer_index, FILE *file)
{
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	return AI_ERROR_GENERAL;
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	AI_NET_LAYER_DATA *layer_data;
	AI_NET_CONFIG_SECTION *option;
	UINT32_T num;
	UINT32_T n;
	UINT32_T c;
	UINT32_T size;
	UINT8_T *content_address;

	layer_data = &(net_layer->layer_data[layer_index]);
	option = layer_data->layer_config;

	if (option->binary.value > 0) {
		/* nothiong to do */
		;
	}
	n = option->filters.value;
	c = layer_data->c;
	size = option->size.value;
	num = (n * c * size * size);

	/* load biases */
	content_address = AiNetLayerContentGetAddress(&(layer_data->biases));
	fread(content_address, sizeof(FLOAT32_T), n, file);

	/* load scale, rolloing mean, rolling variance */
	if ((option->batch_normalize.value > 0) && (option->dontloadscales.value == 0)) {
		/* load scale */
		content_address = AiNetLayerContentGetAddress(&(layer_data->scales));
		fread(content_address, sizeof(FLOAT32_T), n, file);
		/* load rolling mean */
		content_address = AiNetLayerContentGetAddress(&(layer_data->rolling_mean));
		fread(content_address, sizeof(FLOAT32_T), n, file);
		/* load rolling variance */
		content_address = AiNetLayerContentGetAddress(&(layer_data->rolling_variance));
		fread(content_address, sizeof(FLOAT32_T), n, file);
	}

	/* load weights */
	content_address = AiNetLayerContentGetAddress(&(layer_data->weights));
	fread(content_address, sizeof(FLOAT_AI_T), num, file);

	/* load m, v */
	if (layer_data->adam > 0) {
		/* load m */
		content_address = AiNetLayerContentGetAddress(&(layer_data->m));
		fread(content_address, sizeof(FLOAT32_T), num, file);
		/* load v */
		content_address = AiNetLayerContentGetAddress(&(layer_data->v));
		fread(content_address, sizeof(FLOAT32_T), num, file);
	}

	/* transpose matrix */
	if (option->flipped.value > 0) {
		content_address = AiNetLayerContentGetAddress(&(layer_data->weights));
		AiNetWeightsTransposeMatrix((FLOAT_AI_T *)content_address, (c*size*size), n);
	}

	AiNetWeightsFuseConvBatchnorm(net_layer, layer_index);
	AiNetWeightsCalculateBinaryWeights(net_layer, layer_index);

	return AI_ERROR_SUCCESS;
#endif
}

#if 0
/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetWeightsLoadConvolutional(AI_NET_LAYER *net_layer, INT32_T layer_index, FILE *file)
{
	AI_NET_LAYER_DATA *layer_data;
	AI_NET_CONFIG_SECTION *option;
	UINT32_T num;
	UINT32_T n;
	UINT32_T c;
	UINT32_T size;
	UINT8_T *content_address;

	layer_data = &(net_layer->layer_data[layer_index]);
	option = layer_data->layer_config;

	if (option->binary.value > 0) {
		/* nothiong to do */
		;
	}
	n = option->filters.value;
	c = layer_data->c;
	size = option->size.value;
	num = (n * c * size * size);
	content_address = AiNetLayerContentGetAddress(&(layer_data->biases));
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	fread(content_address, sizeof(FLOAT32_T), n, file);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	{
		UINT32_T n_index;
		FLOAT32_T n_value;
		for (n_index = 0; n_index < n; n_index++) {
			fread(&n_value, sizeof(FLOAT32_T), 1, file);
			((FLOAT_AI_T *)content_address)[n_index] = AiBaseFloatCvtF32Fai(n_value);
		}
	}
#endif

	if ((option->batch_normalize.value > 0) && (option->dontloadscales.value == 0)) {
		content_address = AiNetLayerContentGetAddress(&(layer_data->scales));
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
		fread(content_address, sizeof(FLOAT32_T), n, file);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
		{
			UINT32_T n_index;
			FLOAT32_T n_value;
			for (n_index = 0; n_index < n; n_index++) {
				fread(&n_value, sizeof(FLOAT32_T), 1, file);
				((FLOAT_AI_T *)content_address)[n_index] = AiBaseFloatCvtF32Fai(n_value);
			}
		}
#endif
		content_address = AiNetLayerContentGetAddress(&(layer_data->rolling_mean));
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
		fread(content_address, sizeof(FLOAT32_T), n, file);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
		{
			UINT32_T n_index;
			FLOAT32_T n_value;
			for (n_index = 0; n_index < n; n_index++) {
				fread(&n_value, sizeof(FLOAT32_T), 1, file);
				((FLOAT_AI_T *)content_address)[n_index] = AiBaseFloatCvtF32Fai(n_value);
			}
		}
#endif
		content_address = AiNetLayerContentGetAddress(&(layer_data->rolling_variance));
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
		fread(content_address, sizeof(FLOAT32_T), n, file);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
		{
			UINT32_T n_index;
			FLOAT32_T n_value;
			for (n_index = 0; n_index < n; n_index++) {
				fread(&n_value, sizeof(FLOAT32_T), 1, file);
				((FLOAT_AI_T *)content_address)[n_index] = AiBaseFloatCvtF32Fai(n_value);
			}
		}
#endif
	}
	content_address = AiNetLayerContentGetAddress(&(layer_data->weights));
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	fread(content_address, sizeof(FLOAT_AI_T), num, file);
#if (AI_PRODUCT_OPTION_WEIGHTS_ADJUST == 1)
	{
		UINT32_T index;
		FLOAT_AI_T temp_value;
		for (index = 0; index < num; index++) {
			temp_value = ((FLOAT_AI_T *)content_address)[index];
			((FLOAT_AI_T *)content_address)[index] = temp_value * AI_NET_WEIGHTS_MUL_VALUE;
		}
	}
#endif
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	{
		UINT32_T n_index;
		FLOAT32_T n_value;
		for (n_index = 0; n_index < num; n_index++) {
			fread(&n_value, sizeof(FLOAT32_T), 1, file);
			((FLOAT_AI_T *)content_address)[n_index] = AiBaseFloatCvtF32Fai(n_value);
		}
	}
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	fread(content_address, sizeof(FLOAT_AI_T), num, file);
#if (AI_PRODUCT_OPTION_WEIGHTS_ADJUST == 1)
	{
		UINT32_T index;
		FLOAT_AI_T temp_value;
		for (index = 0; index < num; index++) {
			temp_value = ((FLOAT_AI_T *)content_address)[index];
			((FLOAT_AI_T *)content_address)[index] = AiBaseFloatMul(temp_value, AiBaseFloatCvtF32Fai(AI_NET_WEIGHTS_MUL_VALUE));
		}
	}
#endif
#endif
	if (layer_data->adam > 0) {
		content_address = AiNetLayerContentGetAddress(&(layer_data->m));
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
		fread(content_address, sizeof(FLOAT32_T), num, file);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
		{
			UINT32_T n_index;
			FLOAT32_T n_value;
			for (n_index = 0; n_index < num; n_index++) {
				fread(&n_value, sizeof(FLOAT32_T), 1, file);
				((FLOAT_AI_T *)content_address)[n_index] = AiBaseFloatCvtF32Fai(n_value);
			}
		}
#endif
		content_address = AiNetLayerContentGetAddress(&(layer_data->v));
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
		fread(content_address, sizeof(FLOAT32_T), num, file);
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
		{
			UINT32_T n_index;
			FLOAT32_T n_value;
			for (n_index = 0; n_index < num; n_index++) {
				fread(&n_value, sizeof(FLOAT32_T), 1, file);
				((FLOAT_AI_T *)content_address)[n_index] = AiBaseFloatCvtF32Fai(n_value);
			}
		}
#endif
	}
	if (option->flipped.value > 0) {
		content_address = AiNetLayerContentGetAddress(&(layer_data->weights));
		AiNetWeightsTransposeMatrix((FLOAT_AI_T *)content_address, (c*size*size), n);
	}

	AiNetWeightsFuseConvBatchnorm(net_layer, layer_index);
	AiNetWeightsCalculateBinaryWeights(net_layer, layer_index);

	return AI_ERROR_SUCCESS;
}
#endif

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetWeightsTransposeMatrix(FLOAT_AI_T *a, UINT32_T rows, UINT32_T cols)
{
	AI_NET_LAYER *net_layer;
	FLOAT_AI_T *transpose;
	UINT32_T x, y;
	UINT32_T matrix_length;

	matrix_length = rows*cols*sizeof(FLOAT_AI_T);
	net_layer = AiNetDataGetAiNetLayer();

	if (matrix_length > net_layer->layer_rw0.length)
		return AI_ERROR_GENERAL;

	transpose = (FLOAT_AI_T *)(net_layer->layer_rw0.buffer);
	for (x = 0; x < rows; x++) {
		for (y = 0; y < cols; y++) {
			transpose[y*rows + x] = a[x*cols + y];
		}
	}
	memcpy(a, transpose, matrix_length);

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetWeightsFuseConvBatchnorm(AI_NET_LAYER *net_layer, INT32_T layer_index)
{
	AI_NET_LAYER_DATA *layer_data;
	AI_NET_CONFIG_SECTION *option;
	UINT32_T batch_normalize;
	UINT32_T f;
	UINT32_T n;
	UINT32_T filter_size;
	UINT32_T size, c;
	UINT32_T i;
	UINT32_T w_index;
	FLOAT_AI_T *biases;
	FLOAT_AI_T *scales;
	FLOAT_AI_T *rolling_mean;
	FLOAT_AI_T *rolling_variance;
	FLOAT_AI_T *weights;

	layer_data = &(net_layer->layer_data[layer_index]);
	option = layer_data->layer_config;
	batch_normalize = option->batch_normalize.value;

	if (batch_normalize == 0)
		return AI_ERROR_SUCCESS;

	n = option->filters.value;
	size = option->size.value;
	c = layer_data->c;
	filter_size = (size * size * c);
	biases = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->biases));
	scales = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->scales));
	rolling_mean = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->rolling_mean));
	rolling_variance = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->rolling_variance));
	weights = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->weights));
	for (f = 0; f < n; f++) {
		biases[f] = (FLOAT_AI_T)(biases[f] - ((DOUBLE_AI_T)(scales[f]) * rolling_mean[f] / (sqrt((DOUBLE_AI_T)rolling_variance[f]) + .000001f)));
		for (i = 0; i < filter_size; i++) {
			w_index = (f * filter_size + i);
			weights[w_index] = (FLOAT_AI_T)((DOUBLE_AI_T)weights[w_index] * scales[f] / (sqrt((DOUBLE_AI_T)rolling_variance[f]) + .000001f));
		}
	}
	option->batch_normalize.value = 0;

	/* adjust weights */
#if (AI_PRODUCT_OPTION_WEIGHTS_ADJUST == 1)
	{
		UINT32_T index;
		FLOAT_AI_T temp_value;
		for (index = 0; index < num; index++) {
			temp_value = ((FLOAT_AI_T *)content_address)[index];
			((FLOAT_AI_T *)content_address)[index] = temp_value * AI_NET_WEIGHTS_MUL_VALUE;
		}
	}
#endif

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiNetWeightsCalculateBinaryWeights(AI_NET_LAYER *net_layer, INT32_T layer_index)
{
	AI_NET_LAYER_DATA *layer_data;
	AI_NET_CONFIG_SECTION *option;
	
	layer_data = &(net_layer->layer_data[layer_index]);
	option = layer_data->layer_config;

	if (option->xnor.value == 0)
		return AI_ERROR_SUCCESS;

	/* not support the binary weights now */
	return AI_ERROR_SUCCESS;
}

/*------------------------- End ---------------------------------------------*/
