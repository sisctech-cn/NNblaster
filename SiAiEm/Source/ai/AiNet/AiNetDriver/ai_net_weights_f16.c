/**
 ******************************************************************************
 * file    ai_net_weights_f16.c
 * author  sunlingge
 * version V100
 * brief   the ai net weights f16 driver.
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
ERROR_T AiNetWeightsF16Load(AI_NET_NETWORK *net_network, AI_NET_LAYER *net_layer)
{
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	return AI_ERROR_GENERAL;
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	UINT8_T file_path[256];
	FILE *f;
	INT32_T layer_index;
	AI_NET_LAYER_DATA *layer_data;
	AI_NET_CONFIG_SECTION *option;
	UINT32_T n;
	UINT32_T c;
	UINT32_T size;
	FLOAT_AI_T *biases;
	FLOAT_AI_T *weights;
	UINT32_T length;

	sprintf((char *)file_path, "%syolo3.f16.weights", AI_PRODUCT_FILE_PATH""AI_PRODUCT_YOLOV3_CONFIG_PATH"/");
	f = fopen(file_path, "rb");
	if (!f) {
		return AI_ERROR_GENERAL;
	}

	for (layer_index = 0; layer_index < net_network->network_data.layer_index; layer_index++) {
		if (net_layer->layer_data[layer_index].layer_config->dontload.value > 0)
			continue;
		switch (net_layer->layer_data[layer_index].layer_config->type.value) {
		case AI_NET_CONFIG_SECTION_CONVOLUTIONAL_TYPE:
			AiBaseUpdateDlgStatus("AiNetWeightsF16Load layer_index=%d", layer_index);
			layer_data = &(net_layer->layer_data[layer_index]);
			option = layer_data->layer_config;
			biases = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->biases));
			weights = (FLOAT_AI_T *)AiNetLayerContentGetAddress(&(layer_data->weights));
			n = option->filters.value;
			size = option->size.value;
			c = layer_data->c;
			length = (UINT32_T)fread((char *)weights, 1, sizeof(FLOAT_AI_T)*(size*size*c*n), f);
#if (AI_PRODUCT_OPTION_WEIGHTS_ADJUST == 1)
			{
				UINT32_T index;
				FLOAT_AI_T temp_value;
				for (index = 0; index < (size*size*c*n); index++) {
					temp_value = ((FLOAT_AI_T *)weights)[index];
					((FLOAT_AI_T *)weights)[index] = AiBaseFloatMul(temp_value, AiBaseFloatCvtF32Fai(AI_NET_WEIGHTS_MUL_VALUE));
				}
			}
#endif
			if (length != sizeof(FLOAT_AI_T)*(size*size*c*n)) {
				fclose(f);
				return AI_ERROR_GENERAL;
			}
			length = (UINT32_T)fread((char *)biases, 1, sizeof(FLOAT_AI_T)*n, f);
			if (length != sizeof(FLOAT_AI_T)*n) {
				fclose(f);
				return AI_ERROR_GENERAL;
			}
			break;
		default:
			break;
		}
	}
	fclose(f);

	return AI_ERROR_SUCCESS;
#endif
}

/*------------------------- End ---------------------------------------------*/
