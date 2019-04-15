/**
 ******************************************************************************
 * file    ai_net_config.c
 * author  sunlingge
 * version V100
 * brief   the ai net config driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_net_global.h"
#pragma warning(disable:4996)

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
ERROR_T AiNetNetworkInit(AI_NET_NETWORK *net_network, AI_NET_CONFIG *net_config)
{
	UINT32_T section_index;
	ERROR_T result = AI_ERROR_SUCCESS;

	for (section_index = 0; section_index < net_config->section_number; section_index++) {
		switch (net_config->section[section_index].type.value) {
			case AI_NET_CONFIG_SECTION_NET_TYPE:
			case AI_NET_CONFIG_SECTION_NETWORK_TYPE:
				result = AiNetNetworkConfigDefault(net_config, section_index);
				if (result != AI_ERROR_SUCCESS) {
					return result;
				}
				result = AiNetNetworkSetNetwork(net_network, net_config, section_index);
				return result;
			default:
				break;
		}
	}

	return AI_ERROR_GENERAL;
}
/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiNetNetworkConfigDefault(AI_NET_CONFIG *net_config, UINT32_T section_index)
{
	AI_NET_CONFIG_SECTION *option;

	option = &(net_config->section[section_index]);
	AiNetConfigSetOptionValueUint32(&(option->batch), AI_NET_CONFIG_OPTION_VALUE_BATCH_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->subdivisions), AI_NET_CONFIG_OPTION_VALUE_SUBDIVISIONS_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->width), AI_NET_CONFIG_OPTION_VALUE_WIDTH_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->height), AI_NET_CONFIG_OPTION_VALUE_HEIGHT_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->channels), AI_NET_CONFIG_OPTION_VALUE_CHANNELS_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueFloat(&(option->momentum), AI_NET_CONFIG_OPTION_VALUE_MOMENTUM_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueFloat(&(option->decay), AI_NET_CONFIG_OPTION_VALUE_DECAY_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->angle), AI_NET_CONFIG_OPTION_VALUE_ANGLE_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueFloat(&(option->saturation), AI_NET_CONFIG_OPTION_VALUE_SATURATION_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueFloat(&(option->exposure), AI_NET_CONFIG_OPTION_VALUE_EXPOSURE_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueFloat(&(option->hue), AI_NET_CONFIG_OPTION_VALUE_HUE_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueFloat(&(option->learning_rate), AI_NET_CONFIG_OPTION_VALUE_LEARNING_RATE_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->burn_in), AI_NET_CONFIG_OPTION_VALUE_BURN_IN_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->max_batches), AI_NET_CONFIG_OPTION_VALUE_MAX_BATCHS_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->policy), AI_NET_CONFIG_OPTION_VALUE_POLICY_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->step), AI_NET_CONFIG_OPTION_VALUE_STEP_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueFloat(&(option->scale), AI_NET_CONFIG_OPTION_VALUE_SCALE_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueFloat(&(option->gamma), AI_NET_CONFIG_OPTION_VALUE_GAMMA_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->adam), AI_NET_CONFIG_OPTION_VALUE_ADAM_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueFloat(&(option->b1), AI_NET_CONFIG_OPTION_VALUE_B1_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueFloat(&(option->b2), AI_NET_CONFIG_OPTION_VALUE_B2_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueFloat(&(option->eps), AI_NET_CONFIG_OPTION_VALUE_EPS_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->time_steps), AI_NET_CONFIG_OPTION_VALUE_TIME_STEPS_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->inputs), AI_NET_CONFIG_OPTION_VALUE_INPUTS_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->max_crop), AI_NET_CONFIG_OPTION_VALUE_MAX_CROP_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->min_crop), AI_NET_CONFIG_OPTION_VALUE_MIN_CROP_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->flip), AI_NET_CONFIG_OPTION_VALUE_FLIP_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->small_object), AI_NET_CONFIG_OPTION_VALUE_SMALL_OBJECT_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->power), AI_NET_CONFIG_OPTION_VALUE_POWER_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);
	AiNetConfigSetOptionValueUint32(&(option->aspect), AI_NET_CONFIG_OPTION_VALUE_ASPECT_DEFAULT, AI_NET_CONFIG_SET_BY_DEFAULT);

	option->batch.value /= (option->subdivisions.value);
	option->batch.value *= (option->time_steps.value);
	if (option->inputs.set_type != AI_NET_CONFIG_SET_BY_CUSTOM) {
		option->inputs.value = (option->height.value * option->width.value * option->channels.value);
	}
	if (option->max_crop.set_type != AI_NET_CONFIG_SET_BY_CUSTOM) {
		option->max_crop.value = (option->width.value*2);
	}
	if (option->min_crop.set_type != AI_NET_CONFIG_SET_BY_CUSTOM) {
		option->min_crop.value = (option->width.value);
	}
	return AI_ERROR_SUCCESS;
}

ERROR_T AiNetNetworkSetNetwork(AI_NET_NETWORK *net_network, AI_NET_CONFIG *net_config, UINT32_T section_index)
{
	AI_NET_CONFIG_SECTION *option;
	AI_NET_NETWORK_DATA *network_data;

	option = &(net_config->section[section_index]);
	network_data = &(net_network->network_data);

	network_data->batch = option->batch.value;
	network_data->inputs = option->inputs.value;
	network_data->h = option->height.value;
	network_data->w = option->width.value;
	network_data->c = option->channels.value;
	network_data->layer_index = 0;
	network_data->time_steps = option->time_steps.value;
	network_data->network_config = option;

	return AI_ERROR_SUCCESS;
}

/*------------------------- End ---------------------------------------------*/
