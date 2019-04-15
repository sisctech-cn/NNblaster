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
ERROR_T AiNetConfigGetFileLine(FILE *fp, UINT8_T *line)
{
	UINT32_T curr;

	if (feof(fp)) {
		return AI_ERROR_FILE_EOF;
	}

	memset(line, 0, AI_NET_CONFIG_LINE_LENGTH_MAX);
    if (!fgets((char *)line, AI_NET_CONFIG_LINE_LENGTH_MAX, fp)){
        return AI_ERROR_GENERAL;
    }
    curr = (UINT32_T)strlen((char *)line);
    if ((line[curr-1] != 0x0a) && !feof(fp)){
        return AI_ERROR_GENERAL;
    }

    if(curr >= 2) {
        if (line[curr-2] == 0x0d) {
			line[curr-2] = 0x00;
		}
	}
    if(curr >= 1) {
        if (line[curr-1] == 0x0a) {
			line[curr-1] = 0x00;
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
ERROR_T AiNetConfigStrip(UINT8_T *s)
{
    UINT32_T i;
    UINT32_T len = (UINT32_T)strlen((char *)s);
    UINT32_T offset = 0;
	UINT8_T c;

    for(i = 0; i < len; ++i){
        c = s[i];
        if ((c==' ') || (c=='\t') || (c=='\n') || (c =='\r') || (c==0x0d) || (c==0x0a)) {
			++offset;
		} else {
			s[i-offset] = c;
		}
    }
    s[len-offset] = '\0';

	return AI_ERROR_SUCCESS;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiNetConfigParseSectionType(UINT8_T *name, UINT32_T *section_type) 
{
	if (strcmp(name, AI_NET_CONFIG_SECTION_NET_NAME) == 0) {
		*section_type = AI_NET_CONFIG_SECTION_NET_TYPE;
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(name, AI_NET_CONFIG_SECTION_CONVOLUTIONAL_NAME) == 0) {
		*section_type = AI_NET_CONFIG_SECTION_CONVOLUTIONAL_TYPE;
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(name, AI_NET_CONFIG_SECTION_SHORTCUT_NAME) == 0) {
		*section_type = AI_NET_CONFIG_SECTION_SHORTCUT_TYPE;
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(name, AI_NET_CONFIG_SECTION_YOLO_NAME) == 0) {
		*section_type = AI_NET_CONFIG_SECTION_YOLO_TYPE;
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(name, AI_NET_CONFIG_SECTION_ROUTE_NAME) == 0) {
		*section_type = AI_NET_CONFIG_SECTION_ROUTE_TYPE;
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(name, AI_NET_CONFIG_SECTION_UPSAMPLE_NAME) == 0) {
		*section_type = AI_NET_CONFIG_SECTION_UPSAMPLE_TYPE;
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(name, AI_NET_CONFIG_SECTION_NETWORK_NAME) == 0) {
		*section_type = AI_NET_CONFIG_SECTION_NETWORK_TYPE;
		return AI_ERROR_SUCCESS;
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
ERROR_T AiNetConfigReadOption(UINT8_T *s, UINT8_T *key, UINT8_T *value)
{
    UINT32_T i;
    UINT32_T len = (UINT32_T)strlen(s);
    UINT8_T *val = 0;

    for(i = 0; i < len; i++){
          if(s[i] == '='){
            s[i] = '\0';
            val = s+i+1;
            break;
        }
    }

    if(i == len) {
		return AI_ERROR_GENERAL;
	}

	strcpy((char *)key, (char *)s);
	strcpy((char *)value, (char *)val);
    return AI_ERROR_SUCCESS;
}

/**
 * brief    none.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiNetConfigReadOptionValue(UINT8_T *value_str, UINT8_T_PTR *option_array_ptr)
{
    UINT32_T i;
    UINT32_T len = (UINT32_T)strlen((char *)value_str);
	UINT8_T index;

	if (len == 0) {
		return AI_ERROR_GENERAL;
	}

	index = 0;
	option_array_ptr[index] = value_str;
	index++;
    for(i = 0; i < len; ++i){
          if(value_str[i] == ','){
            value_str[i] = '\0';
			if (index < AI_NET_CONFIG_OPTION_VALUE_ARRAY_NUMBER_MAX) {
				option_array_ptr[index] = value_str+i+1;
				index++;
			}
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
ERROR_T AiNetConfigSetOptionValueUint32(AI_NET_CONFIG_OPTION_UINT32_T *option, UINT8_T *value_str, UINT8_T set_type)
{
	if (option->set_type == AI_NET_CONFIG_SET_BY_NONE) {
		option->value = atoi((char *)value_str);
		option->set_type = set_type;
		option->valid = AI_NET_CONFIG_OPTION_VALUE_VALID;
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
ERROR_T AiNetConfigSetOptionValueInt32(AI_NET_CONFIG_OPTION_INT32_T *option, UINT8_T *value_str, UINT8_T set_type)
{
	if (option->set_type == AI_NET_CONFIG_SET_BY_NONE) {
		option->value = atoi((char *)value_str);
		option->set_type = set_type;
		option->valid = AI_NET_CONFIG_OPTION_VALUE_VALID;
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
ERROR_T AiNetConfigSetOptionValueFloat(AI_NET_CONFIG_OPTION_FLOAT_AI_T *option, UINT8_T *value_str, UINT8_T set_type)
{
	if (option->set_type == AI_NET_CONFIG_SET_BY_NONE) {
		option->value = (FLOAT_AI_T)AiBaseFloatCvtF32Fai((FLOAT32_T)atof((char *)value_str));
		option->set_type = set_type;
		option->valid = AI_NET_CONFIG_OPTION_VALUE_VALID;
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
ERROR_T AiNetConfigParseOptionKeyValue(UINT8_T *key_str, UINT8_T *value_str, AI_NET_CONFIG_SECTION *option) 
{
	UINT8_T_PTR option_array_ptr[AI_NET_CONFIG_OPTION_VALUE_ARRAY_NUMBER_MAX];
	UINT32_T index;

	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_BATCH_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->batch), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_SUBDIVISIONS_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->subdivisions), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_WIDTH_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->width), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_HEIGHT_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->height), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_CHANNELS_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->channels), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_MOMENTUM_NAME) == 0) {
		AiNetConfigSetOptionValueFloat(&(option->momentum), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_DECAY_NAME) == 0) {
		AiNetConfigSetOptionValueFloat(&(option->decay), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_ANGLE_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->angle), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_SATURATION_NAME) == 0) {
		AiNetConfigSetOptionValueFloat(&(option->saturation), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_EXPOSURE_NAME) == 0) {
		AiNetConfigSetOptionValueFloat(&(option->exposure), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_HUE_NAME) == 0) {
		AiNetConfigSetOptionValueFloat(&(option->hue), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_LEARNING_RATE_NAME) == 0) {
		AiNetConfigSetOptionValueFloat(&(option->learning_rate), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_BURN_IN_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->burn_in), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_MAX_BATCHS_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->max_batches), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_POLICY_NAME) == 0) {
		if (strcmp(value_str, AI_NET_CONFIG_OPTION_VALUE_POLICY_STEPS_NAME) == 0) {
			AiNetConfigSetOptionValueUint32(&(option->policy), AI_NET_CONFIG_OPTION_VALUE_POLICY_STEPS, AI_NET_CONFIG_SET_BY_CUSTOM);
		}
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_STEP_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->step), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_SCALE_NAME) == 0) {
		AiNetConfigSetOptionValueFloat(&(option->scale), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_STEPS_NAME) == 0) {
		memset((char *)option_array_ptr, 0, sizeof(UINT8_T *)*AI_NET_CONFIG_OPTION_VALUE_ARRAY_NUMBER_MAX);
		AiNetConfigReadOptionValue(value_str, (UINT8_T_PTR *)option_array_ptr);
		for(index = 0; index < AI_NET_CONFIG_OPTION_VALUE_STEPS_ARRAY_LENGTH_MAX; index++) {
			if (option_array_ptr[index] == NULL)
				break;
			AiNetConfigSetOptionValueUint32(&(option->steps[index]), option_array_ptr[index], AI_NET_CONFIG_SET_BY_CUSTOM);
		}
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_SCALES_NAME) == 0) {
		memset((char *)option_array_ptr, 0, sizeof(UINT8_T *)*AI_NET_CONFIG_OPTION_VALUE_ARRAY_NUMBER_MAX);
		AiNetConfigReadOptionValue(value_str, (UINT8_T_PTR *)option_array_ptr);
		for(index = 0; index < AI_NET_CONFIG_OPTION_VALUE_SCALES_ARRAY_LENGTH_MAX; index++) {
			if (option_array_ptr[index] == NULL)
				break;
			AiNetConfigSetOptionValueFloat(&(option->scales[index]), option_array_ptr[index], AI_NET_CONFIG_SET_BY_CUSTOM);
		}
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_GAMMA_NAME) == 0) {
		AiNetConfigSetOptionValueFloat(&(option->gamma), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_ADAM_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->adam), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_B1_NAME) == 0) {
		AiNetConfigSetOptionValueFloat(&(option->b1), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_B2_NAME) == 0) {
		AiNetConfigSetOptionValueFloat(&(option->b2), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_EPS_NAME) == 0) {
		AiNetConfigSetOptionValueFloat(&(option->eps), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_TIME_STEPS_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->time_steps), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_INPUTS_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->inputs), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_MAX_CROP_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->max_crop), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_MIN_CROP_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->min_crop), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_FLIP_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->flip), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_SMALL_OBJECT_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->small_object), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_POWER_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->power), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_ASPECT_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->aspect), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_BATCH_NORMALIZE_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->batch_normalize), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_BINARY_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->binary), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_XNOR_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->xnor), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_BIN_OUTPUT_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->bin_output), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_FILTERS_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->filters), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_SIZE_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->size), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_STRIDE_NAME) == 0) {
		AiNetConfigSetOptionValueInt32(&(option->stride), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_PAD_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->pad), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_PADDING_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->padding), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_ACTIVATION_NAME) == 0) {
		if (strcmp(value_str, AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LOGISTIC_NAME) == 0) {
			AiNetConfigSetOptionValueUint32(&(option->activation), AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LOGISTIC, AI_NET_CONFIG_SET_BY_CUSTOM);
		}
		if (strcmp(value_str, AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_RELU_NAME) == 0) {
			AiNetConfigSetOptionValueUint32(&(option->activation), AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_RELU, AI_NET_CONFIG_SET_BY_CUSTOM);
		}
		if (strcmp(value_str, AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_RELIE_NAME) == 0) {
			AiNetConfigSetOptionValueUint32(&(option->activation), AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_RELIE, AI_NET_CONFIG_SET_BY_CUSTOM);
		}
		if (strcmp(value_str, AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LINEAR_NAME) == 0) {
			AiNetConfigSetOptionValueUint32(&(option->activation), AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LINEAR, AI_NET_CONFIG_SET_BY_CUSTOM);
		}
		if (strcmp(value_str, AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_RAMP_NAME) == 0) {
			AiNetConfigSetOptionValueUint32(&(option->activation), AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_RAMP, AI_NET_CONFIG_SET_BY_CUSTOM);
		}
		if (strcmp(value_str, AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_TANH_NAME) == 0) {
			AiNetConfigSetOptionValueUint32(&(option->activation), AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_TANH, AI_NET_CONFIG_SET_BY_CUSTOM);
		}
		if (strcmp(value_str, AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_PLSE_NAME) == 0) {
			AiNetConfigSetOptionValueUint32(&(option->activation), AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_PLSE, AI_NET_CONFIG_SET_BY_CUSTOM);
		}
		if (strcmp(value_str, AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LEAKY_NAME) == 0) {
			AiNetConfigSetOptionValueUint32(&(option->activation), AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LEAKY, AI_NET_CONFIG_SET_BY_CUSTOM);
		}
		if (strcmp(value_str, AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_ELU_NAME) == 0) {
			AiNetConfigSetOptionValueUint32(&(option->activation), AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_ELU, AI_NET_CONFIG_SET_BY_CUSTOM);
		}
		if (strcmp(value_str, AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LOGGY_NAME) == 0) {
			AiNetConfigSetOptionValueUint32(&(option->activation), AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LOGGY, AI_NET_CONFIG_SET_BY_CUSTOM);
		}
		if (strcmp(value_str, AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_STAIR_NAME) == 0) {
			AiNetConfigSetOptionValueUint32(&(option->activation), AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_STAIR, AI_NET_CONFIG_SET_BY_CUSTOM);
		}
		if (strcmp(value_str, AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_HARDTAN_NAME) == 0) {
			AiNetConfigSetOptionValueUint32(&(option->activation), AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_HARDTAN, AI_NET_CONFIG_SET_BY_CUSTOM);
		}
		if (strcmp(value_str, AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LHTAN_NAME) == 0) {
			AiNetConfigSetOptionValueUint32(&(option->activation), AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_LHTAN, AI_NET_CONFIG_SET_BY_CUSTOM);
		}
		if (strcmp(value_str, AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_SELU_NAME) == 0) {
			AiNetConfigSetOptionValueUint32(&(option->activation), AI_NET_CONFIG_OPTION_VALUE_ACTIVATION_SELU, AI_NET_CONFIG_SET_BY_CUSTOM);
		}
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_FLIPPED_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->flipped), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_DOT_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->dot), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_FROM_NAME) == 0) {
		AiNetConfigSetOptionValueInt32(&(option->from), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_MASK_NAME) == 0) {
		memset((char *)option_array_ptr, 0, sizeof(UINT8_T *)*AI_NET_CONFIG_OPTION_VALUE_ARRAY_NUMBER_MAX);
		AiNetConfigReadOptionValue(value_str, (UINT8_T_PTR *)option_array_ptr);
		for(index = 0; index < AI_NET_CONFIG_OPTION_VALUE_MASK_ARRAY_LENGTH_MAX; index++) {
			if (option_array_ptr[index] == NULL)
				break;
			AiNetConfigSetOptionValueUint32(&(option->mask[index]), option_array_ptr[index], AI_NET_CONFIG_SET_BY_CUSTOM);
		}
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_ANCHORS_NAME) == 0) {
		memset((char *)option_array_ptr, 0, sizeof(UINT8_T *)*AI_NET_CONFIG_OPTION_VALUE_ARRAY_NUMBER_MAX);
		AiNetConfigReadOptionValue(value_str, (UINT8_T_PTR *)option_array_ptr);
		for(index = 0; index < AI_NET_CONFIG_OPTION_VALUE_ANCHORS_ARRAY_LENGTH_MAX; index++) {
			if (option_array_ptr[index] == NULL)
				break;
			AiNetConfigSetOptionValueFloat(&(option->anchors[index]), option_array_ptr[index], AI_NET_CONFIG_SET_BY_CUSTOM);
		}
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_CLASSES_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->classes), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_NUM_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->num), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_JITTER_NAME) == 0) {
		AiNetConfigSetOptionValueFloat(&(option->jitter), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_IGNORE_THRESH_NAME) == 0) {
		AiNetConfigSetOptionValueFloat(&(option->ignore_thresh), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_TRUTH_THRESH_NAME) == 0) {
		AiNetConfigSetOptionValueFloat(&(option->truth_thresh), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_RANDOM_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->random), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_MAX_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->max), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_FOCAL_LOSS_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->focal_loss), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_LAYERS_NAME) == 0) {
		memset((char *)option_array_ptr, 0, sizeof(UINT8_T *)*AI_NET_CONFIG_OPTION_VALUE_ARRAY_NUMBER_MAX);
		AiNetConfigReadOptionValue(value_str, (UINT8_T_PTR *)option_array_ptr);
		for(index = 0; index < AI_NET_CONFIG_OPTION_VALUE_LAYERS_ARRAY_LENGTH_MAX; index++) {
			if (option_array_ptr[index] == NULL)
				break;
			AiNetConfigSetOptionValueInt32(&(option->layers[index]), option_array_ptr[index], AI_NET_CONFIG_SET_BY_CUSTOM);
		}
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_ONLYFORWARD_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->onlyforward), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_STOPBACKWARD_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->stopbackward), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_DONTLOAD_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->dontload), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
	}
	if (strcmp(key_str, AI_NET_CONFIG_OPTION_KEY_DONTLOADSCALES_NAME) == 0) {
		AiNetConfigSetOptionValueUint32(&(option->dontloadscales), value_str, AI_NET_CONFIG_SET_BY_CUSTOM);
		return AI_ERROR_SUCCESS;
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
ERROR_T AiNetConfigLoadFile(AI_NET_CONFIG *net_config)
{
	UINT8_T file_path[256];
	UINT8_T line[AI_NET_CONFIG_LINE_LENGTH_MAX];
	UINT8_T key_str[AI_NET_CONFIG_LINE_LENGTH_MAX];
	UINT8_T value_str[AI_NET_CONFIG_LINE_LENGTH_MAX];
	UINT32_T section_index;
	UINT32_T section_type;
	FILE *file;
	ERROR_T result = AI_ERROR_SUCCESS;

	sprintf((char *)file_path, AI_PRODUCT_FILE_PATH""AI_PRODUCT_YOLOV3_CONFIG_PATH""AI_PRODUCT_YOLOV3_CONFIG_FILE);
	file = fopen((char *)file_path, "rt");
	if (file == 0) {
		return AI_ERROR_GENERAL;
	}

	memset((char *)net_config, 0, sizeof(AI_NET_CONFIG));
	section_index = 0;
	while (1) {
		result = AiNetConfigGetFileLine(file, line);
		if (result == AI_ERROR_FILE_EOF) {
			result = AI_ERROR_SUCCESS;
			break;
		}
		if (result != AI_ERROR_SUCCESS) {
			fclose(file);
			return AI_ERROR_GENERAL;
		}

		AiNetConfigStrip(line);
		switch (line[0]) {
			/* empty line */
		case '\0':
			/* comment line */
		case '#':
			/* comment line */
		case ';':
			break;
			/* section line */
		case '[':
			result = AiNetConfigParseSectionType(line, &section_type);
			if (result != AI_ERROR_SUCCESS) {
				fclose(file);
				return AI_ERROR_GENERAL;
			}
			section_index++;
			if (section_index >= AI_NET_CONFIG_SECTION_MAX) {
				fclose(file);
				return AI_ERROR_GENERAL;
			}
			net_config->section_number = section_index;
			net_config->section[section_index - 1].type.value = section_type;
			break;

			/* option line */
		default:
			result = AiNetConfigReadOption(line, key_str, value_str);
			if (result != AI_ERROR_SUCCESS) {
				fclose(file);
				return AI_ERROR_GENERAL;
			}
			result = AiNetConfigParseOptionKeyValue(key_str, value_str, &(net_config->section[section_index - 1]));
			if (result != AI_ERROR_SUCCESS) {
				fclose(file);
				return AI_ERROR_GENERAL;
			}

			break;
		}
	}

	if (net_config->section_number < 1) {
		fclose(file);
		return AI_ERROR_GENERAL;
	}

	if ((net_config->section[0].type.value != AI_NET_CONFIG_SECTION_NET_TYPE) && (net_config->section[0].type.value != AI_NET_CONFIG_SECTION_NETWORK_TYPE)) {
		fclose(file);
		return AI_ERROR_GENERAL;
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
ERROR_T AiNetConfigLoad(AI_NET_CONFIG *net_config)
{
	ERROR_T result = AI_ERROR_SUCCESS;

	AiNetConfigLoadFile(net_config);

	return result;
}



/*------------------------- End ---------------------------------------------*/
