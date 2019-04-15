/**
 ******************************************************************************
 * file    ai_base_log.c
 * author  sunlingge
 * version V100
 * brief   the ai base log driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"
#pragma warning(disable:4996)

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/
UINT8_T g_ai_base_log_layer_data_string[AI_BASE_LOG_LAYER_DATA_STRING_LENGTH_MAX];
UINT32_T g_ai_base_log_accuracy_times = 0;

/*------------------------- Function Definition -----------------------------*/
/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
UINT32_T g_ai_base_log_error_loop = 0;
UINT32_T g_ai_base_log_error_break = 0;
ERROR_T AiBaseLogErrorCritial(void)
{
	while (1) {
		if (g_ai_base_log_error_break == 1)
			break;
		g_ai_base_log_error_loop++;
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
ERROR_T AiBaseLogLayerDataFloat16(INT32_T layer_index, UINT8_T *name, UINT8_T *buffer, UINT32_T length)
{
#if (AI_RPODUCT_OPTION_LOG_FP16 == 1)
	UINT8_T file_path[256];
	FILE *f;
	UINT32_T index;
	FLOAT16_T fp16;
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	FLOAT32_T fp32;
#endif

	sprintf((char *)file_path, "%sLayer%d%s.f16.dat", AI_PRODUCT_LOG_IDENT_PATH"/", layer_index, name);
	f = fopen(file_path, "wb");
	if (!f) {
		AiBaseLogErrorCritial();
		return AI_ERROR_GENERAL;
	}
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
	for (index = 0; index < length; index = index + 4) {
		fp32 = (FLOAT32_T)(*((FLOAT32_T *)(buffer + index)));
		fp16 = AiBaseFloat16CvtF32F16(fp32);
		fwrite((char *)&fp16, 1, sizeof(FLOAT16_T), f);
	}
#endif
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_SOFTWARE_FP16)
	for (index = 0; index < length; index = index + 2) {
		fp16 = (FLOAT16_T)(*((FLOAT16_T *)(buffer + index)));
		fwrite((char *)&fp16, 1, sizeof(FLOAT16_T), f);
	}
#endif
	fclose(f);
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
ERROR_T AiBaseLogLayerData(INT32_T layer_index, UINT8_T *name, UINT8_T *buffer, UINT32_T length)
{
#if (AI_PRODUCT_OPTION_LOG == 1)
#if (FLOAT_AI_T_TYPE_SIZE == AI_PRODUCT_FLOAT_HARDWARE_FP32)
    UINT8_T file_path[256];
	FILE *f;

    sprintf((char *)file_path, "%sLayer%d%s.dat", AI_PRODUCT_LOG_IDENT_PATH"/", layer_index, name);
	f = fopen(file_path, "wb");
	if (!f) {
		AiBaseLogErrorCritial();
		return AI_ERROR_GENERAL;
	}

	fwrite(buffer, 1, length, f);
	fclose(f);
#endif
#endif
	AiBaseLogLayerDataFloat16(layer_index, name, buffer, length);

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiBaseLogLayerInfo(INT32_T layer_index, UINT8_T *name, UINT8_T *buffer, UINT32_T length)
{
#if (AI_PRODUCT_OPTION_LOG == 1)
	UINT8_T file_path[256];
	FILE *f;

	sprintf((char *)file_path, "%sLayer%d%s.dat", AI_PRODUCT_LOG_IDENT_PATH"/", layer_index, name);
	f = fopen(file_path, "wb");
	if (!f) {
		AiBaseLogErrorCritial();
		return AI_ERROR_GENERAL;
	}

	fwrite(buffer, 1, length, f);
	fclose(f);
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
ERROR_T AiBaseLogLayerInfoAppend(INT32_T layer_index, UINT8_T *name, UINT8_T *buffer, UINT32_T length)
{
#if (AI_PRODUCT_OPTION_LOG == 1)
	UINT8_T file_path[256];
	FILE *f;

	sprintf((char *)file_path, "%sLayer%d%s.dat", AI_PRODUCT_LOG_IDENT_PATH"/", layer_index, name);
	f = fopen(file_path, "wb+");
	if (!f) {
		AiBaseLogErrorCritial();
		return AI_ERROR_GENERAL;
	}

	fwrite(buffer, 1, length, f);
	fclose(f);
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
ERROR_T AiBaseLogLayerDataFormat(INT32_T layer_index, UINT8_T *name, UINT8_T *format, ...) {
#if (AI_PRODUCT_OPTION_LOG == 1)
	UINT8_T *str_buffer = g_ai_base_log_layer_data_string;

	va_list vlArgs;
	va_start(vlArgs, format);
	//_vsnprintf_s(str_buffer, sizeof(str_buffer) - 1, format, vlArgs); 
	//vsprintf(str_buffer, format, vlArgs);
	memset((char *)str_buffer, 0, AI_BASE_LOG_LAYER_DATA_STRING_LENGTH_MAX);
	vsnprintf(str_buffer, AI_BASE_LOG_LAYER_DATA_STRING_LENGTH_MAX - 1, format, vlArgs);
	va_end(vlArgs);

	AiBaseLogLayerInfo(layer_index, name, str_buffer, (UINT32_T)strlen(str_buffer));
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
ERROR_T AiBaseLogLayerDataAppendFormat(INT32_T layer_index, UINT8_T *name,  UINT8_T *format, ...) {
#if (AI_PRODUCT_OPTION_LOG == 1)
	UINT8_T *str_buffer = g_ai_base_log_layer_data_string;

	va_list vlArgs;
	va_start(vlArgs, format);
	//_vsnprintf_s(str_buffer, sizeof(str_buffer) - 1, format, vlArgs); 
	//vsprintf(str_buffer, format, vlArgs);
	memset((char *)str_buffer, 0, AI_BASE_LOG_LAYER_DATA_STRING_LENGTH_MAX);
	vsnprintf(str_buffer, AI_BASE_LOG_LAYER_DATA_STRING_LENGTH_MAX-1, format, vlArgs);
	va_end(vlArgs);
	
	AiBaseLogLayerInfoAppend(layer_index, name, str_buffer, (UINT32_T)strlen(str_buffer));
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
ERROR_T AiBaseLogConvertTextF16(UINT8_T *name)
{
	UINT8_T file_path[256];
	FILE *f;
	FILE *f_out;
	UINT32_T index;
	FLOAT16_T fp16;
	FLOAT32_T fp32;

	f = fopen(name, "rb");
	if (!f) {
		return AI_ERROR_GENERAL;
	}

	sprintf((char *)file_path, "%sFileContentF16.txt", AI_PRODUCT_LOG_IDENT_PATH"/");
	f_out = fopen(file_path, "wt");
	if (!f_out) {
		return AI_ERROR_GENERAL;
	}
	index = 0;
	while(1) {
		if (fread((char *)&fp16, 1, sizeof(FLOAT16_T), f) != sizeof(FLOAT16_T))
			break;
		fp32 = AiBaseFloat32CvtF16F32(fp16);
		if ((index % 8) == 0) {
			sprintf((char *)file_path, "%08x:\t", index);
			fwrite((char *)file_path, 1, strlen(file_path), f_out);
		}
		sprintf((char *)file_path, "%02x%02x\t%12f\t", 
			((unsigned char *)&fp16)[0], ((unsigned char *)&fp16)[1], 
			fp32);
		fwrite((char *)file_path, 1, strlen(file_path), f_out);
		if ((index % 8) == 7) {
			sprintf((char *)file_path, "%s", "\r\n");
			fwrite((char *)file_path, 1, strlen(file_path), f_out);
		}
		index++;
	}
	fclose(f);
	fclose(f_out);

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiBaseLogConvertTextF32(UINT8_T *name)
{
	UINT8_T file_path[256];
	FILE *f;
	FILE *f_out;
	UINT32_T index;
	FLOAT32_T fp32;

	f = fopen(name, "rb");
	if (!f) {
		return AI_ERROR_GENERAL;
	}

	sprintf((char *)file_path, "%sFileContentF32.txt", AI_PRODUCT_LOG_IDENT_PATH"/");
	f_out = fopen(file_path, "wt");
	if (!f_out) {
		return AI_ERROR_GENERAL;
	}
	index = 0;
	while (1) {
		if (fread((char *)&fp32, 1, sizeof(FLOAT32_T), f) != sizeof(FLOAT32_T))
			break;
		if ((index % 8) == 0) {
			sprintf((char *)file_path, "%08x:\t", index);
			fwrite((char *)file_path, 1, strlen(file_path), f_out);
		}
		sprintf((char *)file_path, "%02x%02x%02x%02x\t%12f\t",
			((unsigned char *)&fp32)[0], ((unsigned char *)&fp32)[1], 
			((unsigned char *)&fp32)[2], ((unsigned char *)&fp32)[3],
			fp32);
		fwrite((char *)file_path, 1, strlen(file_path), f_out);
		if ((index % 8) == 7) {
			sprintf((char *)file_path, "%s", "\r\n");
			fwrite((char *)file_path, 1, strlen(file_path), f_out);
		}
		index++;
	}
	fclose(f);
	fclose(f_out);

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiBaseLogAccuracyTextF16(void)
{
	UINT8_T file_path[256];
	FILE *f_16;
	FILE *f_32;
	FILE *f_16_out;
	FILE *f_32_out;
	FILE *f_out;
	UINT32_T index;
	FLOAT16_T fp16;
	FLOAT32_T fp16_32;
	FLOAT32_T fp32;
	FLOAT32_T accuracy_max = (FLOAT32_T)(0.0f);
	FLOAT32_T accuracy_min = (FLOAT32_T)(0.0f);
	FLOAT32_T accuracy_average = (FLOAT32_T)(0.0f);
	FLOAT32_T accuracy_sum = (FLOAT32_T)(0.0f);
	FLOAT32_T accuracy = (FLOAT32_T)(0.0f);
	FLOAT32_T accuracy_abs = (FLOAT32_T)(0.0f);
	FLOAT32_T accuracy_abs_max = (FLOAT32_T)(0.0f);
	FLOAT32_T sub_value_max = (FLOAT32_T)(0.0f);
	FLOAT32_T sub_value_min = (FLOAT32_T)(0.0f);
	FLOAT32_T sub_value_average = (FLOAT32_T)(0.0f);
	FLOAT32_T sub_value_sum = (FLOAT32_T)(0.0f);
	FLOAT32_T sub_value = (FLOAT32_T)(0.0f);
	FLOAT32_T sub_value_abs = (FLOAT32_T)(0.0f);
	FLOAT32_T sub_value_abs_max = (FLOAT32_T)(0.0f);
	INT32_T layer_index;

	/* open the file */
	sprintf((char *)file_path, "%sFileContentF16.txt", AI_PRODUCT_LOG_IDENT_PATH"/");
	f_out = fopen(file_path, "wt");
	if (!f_out) {
		return AI_ERROR_GENERAL;
	}


	for (layer_index = 0; layer_index < 128; layer_index++) {
		/* reset accuracy */
		accuracy_max = (FLOAT32_T)(0.0f);
		accuracy_min = (FLOAT32_T)(0.0f);
		accuracy_average = (FLOAT32_T)(0.0f);
		accuracy = (FLOAT32_T)(0.0f);
		accuracy_abs = (FLOAT32_T)(0.0f);
		accuracy_abs_max = (FLOAT32_T)(0.0f);
		sub_value_max = (FLOAT32_T)(0.0f);
		sub_value_min = (FLOAT32_T)(0.0f);
		sub_value_average = (FLOAT32_T)(0.0f);
		sub_value_sum = (FLOAT32_T)(0.0f);
		sub_value = (FLOAT32_T)(0.0f);
		sub_value_abs = (FLOAT32_T)(0.0f);
		sub_value_abs_max = (FLOAT32_T)(0.0f);

		/* open the file */
		sprintf((char *)file_path, "%sLayer%d%s.f16.dat", AI_PRODUCT_LOG_IDENT_PATH"/", layer_index, "Output");
		f_16 = fopen(file_path, "rb");
		if (!f_16) {
			continue;
		}

		sprintf((char *)file_path, "%sLayer%d%s.dat", AI_PRODUCT_LOG_IDENT_PATH"/", layer_index, "Output");
		f_32 = fopen(file_path, "rb");
		if (!f_32) {
			continue;
		}

		sprintf((char *)file_path, "%sLayer%d%s.f16.txt", AI_PRODUCT_LOG_IDENT_PATH"/", layer_index, "Output");
		f_16_out = fopen(file_path, "wb");
		if (!f_16_out) {
			continue;
		}

		sprintf((char *)file_path, "%sLayer%d%s.txt", AI_PRODUCT_LOG_IDENT_PATH"/", layer_index, "Output");
		f_32_out = fopen(file_path, "wb");
		if (!f_32_out) {
			continue;
		}

		index = 0;
		while (1) {
			/* read the value */
			if (fread((char *)&fp16, 1, sizeof(FLOAT16_T), f_16) != sizeof(FLOAT16_T))
				break;
			fp16_32 = AiBaseFloat32CvtF16F32(fp16);
			if (fread((char *)&fp32, 1, sizeof(FLOAT32_T), f_32) != sizeof(FLOAT32_T))
				break;

			/* calc accuracy */
			accuracy = (fp16_32 - fp32)*100.0f / fp32;
			if (fp16_32 != 0) {
				if (accuracy < accuracy_min) {
					accuracy_min = accuracy;
				}
				if (accuracy > accuracy_max) {
					accuracy_max = accuracy;
				}
			}
			if (accuracy < 0.0f) {
				accuracy_abs = 0.0f - accuracy;
			}
			else {
				accuracy_abs = accuracy;
			}
			if (fp16_32 != 0) {
				if (accuracy_abs > accuracy_abs_max) {
					accuracy_abs_max = accuracy_abs;
				}
			}
			if (accuracy_abs > 100.0f) {
				g_ai_base_log_accuracy_times++;
			}
			accuracy_sum = accuracy_sum + accuracy_abs;

			/* calc accuracy */
			sub_value = (fp16_32 - fp32);
			if (fp16_32 != 0) {
				if (sub_value < sub_value_min) {
					sub_value_min = sub_value;
				}
				if (sub_value > sub_value_max) {
					sub_value_max = sub_value;
				}
			}
			if (sub_value < 0.0f) {
				sub_value_abs = 0.0f - sub_value;
			}
			else {
				sub_value_abs = sub_value;
			}
			if (fp16_32 != 0) {
				if (sub_value_abs > sub_value_abs_max) {
					sub_value_abs_max = sub_value_abs;
				}
			}
			if (sub_value_abs > 1.0f) {
				g_ai_base_log_accuracy_times++;
			}
			sub_value_sum = sub_value_sum + sub_value_abs;

			/* save the max value */
			if ((sub_value == sub_value_min) ||
				(sub_value == sub_value_max) ||
				(accuracy == accuracy_min) ||
				(accuracy == accuracy_max)) {
				/* save index */
				sprintf((char *)file_path, "%08x:\t", index);
				fwrite((char *)file_path, 1, strlen(file_path), f_out);
				/* save data */
				sprintf((char *)file_path, "%02x%02x\t[fp32=%24f][fp16=%12f][accuracy=%12f%%][sub=%12f][fp32=%e][fp16=%e]\t",
					((unsigned char *)&fp16)[0], ((unsigned char *)&fp16)[1],
					fp32, fp16_32, accuracy, sub_value, fp32, fp16_32);
				fwrite((char *)file_path, 1, strlen(file_path), f_out);
				sprintf((char *)file_path, "%s", "\r\n");
				fwrite((char *)file_path, 1, strlen(file_path), f_out);
			}
#if 0
			/* write f_16_out */
			if ((index % 4) == 0) {
				sprintf((char *)file_path, "%08x:\t", index);
				fwrite((char *)file_path, 1, strlen(file_path), f_16_out);
			}
			sprintf((char *)file_path, "%02x%02x\t%12f[%12f%%][%12e]\t",
				((unsigned char *)&fp16)[0], ((unsigned char *)&fp16)[1],
				fp16_32, accuracy, sub_value);
			fwrite((char *)file_path, 1, strlen(file_path), f_16_out);
			if ((index % 4) == 3) {
				sprintf((char *)file_path, "%s", "\r\n");
				fwrite((char *)file_path, 1, strlen(file_path), f_16_out);
			}

			/* write f_32_out */
			if ((index % 4) == 0) {
				sprintf((char *)file_path, "%08x:\t", index);
				fwrite((char *)file_path, 1, strlen(file_path), f_32_out);
			}
			sprintf((char *)file_path, "%02x%02x%02x%02x\t%12f[%12f%%][%12e]\t",
				((unsigned char *)&fp32)[0], ((unsigned char *)&fp32)[1],
				((unsigned char *)&fp32)[2], ((unsigned char *)&fp32)[3],
				fp32, accuracy, sub_value);
			fwrite((char *)file_path, 1, strlen(file_path), f_32_out);
			if ((index % 4) == 3) {
				sprintf((char *)file_path, "%s", "\r\n");
				fwrite((char *)file_path, 1, strlen(file_path), f_32_out);
			}
#endif
			index++;
		}

		/* no data */
		if (index == 0)
			continue;

	/* average */
	accuracy_average = accuracy_sum / index;
	sub_value_average = sub_value_sum / index;
	/* write out */
	sprintf((char *)file_path, "layer%04d accuracy:\tsum:[%12f%%]\tavg:[%12f%%]\tmax:[%12f%%]\tmin:[%12f%%]\r\n",
		    layer_index,
			accuracy_sum,
			accuracy_average,
			accuracy_max, 
			accuracy_min);
	fwrite((char *)file_path, 1, strlen(file_path), f_out);
	sprintf((char *)file_path, "layer%04d   value:\tsum:[%13f]\tavg:[%13f]\tmax:[%13f]\tmin:[%13f]\r\n",
	    layer_index,
		sub_value_sum,
		sub_value_average,
		sub_value_max,
		sub_value_min);
	fwrite((char *)file_path, 1, strlen(file_path), f_out);

		fclose(f_16);
		fclose(f_32);
		fclose(f_16_out);
		fclose(f_32_out);
	}

	fclose(f_out);

	return AI_ERROR_SUCCESS;

}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiBaseLogAccuracyTextF32(void)
{
	UINT8_T file_path[256];
	FILE *f_16;
	FILE *f_32;
	FILE *f_16_out;
	FILE *f_32_out;
	FILE *f_out;
	UINT32_T index;
	FLOAT16_T fp16;
	FLOAT32_T fp16_32;
	FLOAT32_T fp32;
	FLOAT32_T accuracy_max = (FLOAT32_T)(0.0f);
	FLOAT32_T accuracy_min = (FLOAT32_T)(0.0f);
	FLOAT32_T accuracy_average = (FLOAT32_T)(0.0f);
	FLOAT32_T accuracy_sum = (FLOAT32_T)(0.0f);
	FLOAT32_T accuracy = (FLOAT32_T)(0.0f);
	FLOAT32_T accuracy_abs = (FLOAT32_T)(0.0f);
	FLOAT32_T accuracy_abs_max = (FLOAT32_T)(0.0f);
	FLOAT32_T sub_value_max = (FLOAT32_T)(0.0f);
	FLOAT32_T sub_value_min = (FLOAT32_T)(0.0f);
	FLOAT32_T sub_value_average = (FLOAT32_T)(0.0f);
	FLOAT32_T sub_value_sum = (FLOAT32_T)(0.0f);
	FLOAT32_T sub_value = (FLOAT32_T)(0.0f);
	FLOAT32_T sub_value_abs = (FLOAT32_T)(0.0f);
	FLOAT32_T sub_value_abs_max = (FLOAT32_T)(0.0f);
	INT32_T layer_index;

	/* open the file */
	sprintf((char *)file_path, "%sFileContentF32.txt", AI_PRODUCT_LOG_IDENT_PATH"/");
	f_out = fopen(file_path, "wt");
	if (!f_out) {
		return AI_ERROR_GENERAL;
	}


	for (layer_index = 0; layer_index < 128; layer_index++) {
		/* reset accuracy */
		accuracy_max = (FLOAT32_T)(0.0f);
		accuracy_min = (FLOAT32_T)(0.0f);
		accuracy_average = (FLOAT32_T)(0.0f);
		accuracy = (FLOAT32_T)(0.0f);
		accuracy_abs = (FLOAT32_T)(0.0f);
		accuracy_abs_max = (FLOAT32_T)(0.0f);
		sub_value_max = (FLOAT32_T)(0.0f);
		sub_value_min = (FLOAT32_T)(0.0f);
		sub_value_average = (FLOAT32_T)(0.0f);
		sub_value_sum = (FLOAT32_T)(0.0f);
		sub_value = (FLOAT32_T)(0.0f);
		sub_value_abs = (FLOAT32_T)(0.0f);
		sub_value_abs_max = (FLOAT32_T)(0.0f);

		/* open the file */
		sprintf((char *)file_path, "%sLayer%d%s.f16.dat", AI_PRODUCT_LOG_IDENT_PATH"/", layer_index, "Output");
		f_16 = fopen(file_path, "rb");
		if (!f_16) {
			continue;
		}

		sprintf((char *)file_path, "%sLayer%d%s.dat", AI_PRODUCT_LOG_IDENT_PATH"/", layer_index, "Output");
		f_32 = fopen(file_path, "rb");
		if (!f_32) {
			continue;
		}

		sprintf((char *)file_path, "%sLayer%d%s.f16.txt", AI_PRODUCT_LOG_IDENT_PATH"/", layer_index, "Output");
		f_16_out = fopen(file_path, "wb");
		if (!f_16_out) {
			continue;
		}

		sprintf((char *)file_path, "%sLayer%d%s.txt", AI_PRODUCT_LOG_IDENT_PATH"/", layer_index, "Output");
		f_32_out = fopen(file_path, "wb");
		if (!f_32_out) {
			continue;
		}

		index = 0;
		while (1) {
			/* read the value */
			if (fread((char *)&fp16, 1, sizeof(FLOAT16_T), f_16) != sizeof(FLOAT16_T))
				break;
			fp16_32 = AiBaseFloat32CvtF16F32(fp16);
			if (fread((char *)&fp32, 1, sizeof(FLOAT32_T), f_32) != sizeof(FLOAT32_T))
				break;

			/* calc accuracy */
			accuracy = (fp16_32 - fp32)*100.0f / fp32;
			if (fp16_32 != 0) {
				if (accuracy < accuracy_min) {
					accuracy_min = accuracy;
				}
				if (accuracy > accuracy_max) {
					accuracy_max = accuracy;
				}
			}
			if (accuracy < 0.0f) {
				accuracy_abs = 0.0f - accuracy;
			}
			else {
				accuracy_abs = accuracy;
			}
			if (fp16_32 != 0) {
				if (accuracy_abs > accuracy_abs_max) {
					accuracy_abs_max = accuracy_abs;
				}
			}
			if (accuracy_abs > 100.0f) {
				g_ai_base_log_accuracy_times++;
			}
			accuracy_sum = accuracy_sum + accuracy_abs;

			/* calc accuracy */
			sub_value = (fp16_32 - fp32);
			if (fp16_32 != 0) {
				if (sub_value < sub_value_min) {
					sub_value_min = sub_value;
				}
				if (sub_value > sub_value_max) {
					sub_value_max = sub_value;
				}
			}
			if (sub_value < 0.0f) {
				sub_value_abs = 0.0f - sub_value;
			}
			else {
				sub_value_abs = sub_value;
			}
			if (fp16_32 != 0) {
				if (sub_value_abs > sub_value_abs_max) {
					sub_value_abs_max = sub_value_abs;
				}
			}
			if (sub_value_abs > 1.0f) {
				g_ai_base_log_accuracy_times++;
			}
			sub_value_sum = sub_value_sum + sub_value_abs;

			/* save the max value */
			if ((sub_value == sub_value_min) ||
				(sub_value == sub_value_max) ||
				(accuracy == accuracy_min) ||
				(accuracy == accuracy_max)) {
				/* save index */
				sprintf((char *)file_path, "%08x:\t", index);
				fwrite((char *)file_path, 1, strlen(file_path), f_out);
				/* save data */
				sprintf((char *)file_path, "%02x%02x%02x%02x\t[fp32=%24f][fp16=%12f][accuracy=%12f%%][sub=%12f][fp32=%e][fp16=%e]\t",
					((unsigned char *)&fp32)[0], ((unsigned char *)&fp32)[1],
					((unsigned char *)&fp32)[2], ((unsigned char *)&fp32)[3],
					fp32, fp16_32, accuracy, sub_value, fp32, fp16_32);
				fwrite((char *)file_path, 1, strlen(file_path), f_out);
				sprintf((char *)file_path, "%s", "\r\n");
				fwrite((char *)file_path, 1, strlen(file_path), f_out);
			}
#if 0
			/* write f_16_out */
			if ((index % 4) == 0) {
				sprintf((char *)file_path, "%08x:\t", index);
				fwrite((char *)file_path, 1, strlen(file_path), f_16_out);
			}
			sprintf((char *)file_path, "%02x%02x\t%12f[%12f%%][%12e]\t",
				((unsigned char *)&fp16)[0], ((unsigned char *)&fp16)[1],
				fp16_32, accuracy, sub_value);
			fwrite((char *)file_path, 1, strlen(file_path), f_16_out);
			if ((index % 4) == 3) {
				sprintf((char *)file_path, "%s", "\r\n");
				fwrite((char *)file_path, 1, strlen(file_path), f_16_out);
			}

			/* write f_32_out */
			if ((index % 4) == 0) {
				sprintf((char *)file_path, "%08x:\t", index);
				fwrite((char *)file_path, 1, strlen(file_path), f_32_out);
			}
			sprintf((char *)file_path, "%02x%02x%02x%02x\t%12f[%12f%%][%12e]\t",
				((unsigned char *)&fp32)[0], ((unsigned char *)&fp32)[1],
				((unsigned char *)&fp32)[2], ((unsigned char *)&fp32)[3],
				fp32, accuracy, sub_value);
			fwrite((char *)file_path, 1, strlen(file_path), f_32_out);
			if ((index % 4) == 3) {
				sprintf((char *)file_path, "%s", "\r\n");
				fwrite((char *)file_path, 1, strlen(file_path), f_32_out);
			}
#endif

			index++;
		}

		/* no data */
		if (index == 0)
			continue;

	/* average */
	accuracy_average = accuracy_sum / index;
	sub_value_average = sub_value_sum / index;
	/* write out */
	sprintf((char *)file_path, "layer%04d accuracy:\tsum:[%12f%%]\tavg:[%12f%%]\tmax:[%12f%%]\tmin:[%12f%%]\r\n",
		    layer_index,
			accuracy_sum,
			accuracy_average,
			accuracy_max, 
			accuracy_min);
	fwrite((char *)file_path, 1, strlen(file_path), f_out);
	sprintf((char *)file_path, "layer%04d   value:\tsum:[%13f]\tavg:[%13f]\tmax:[%13f]\tmin:[%13f]\r\n",
	    layer_index,
		sub_value_sum,
		sub_value_average,
		sub_value_max,
		sub_value_min);
	fwrite((char *)file_path, 1, strlen(file_path), f_out);

		fclose(f_16);
		fclose(f_32);
		fclose(f_16_out);
		fclose(f_32_out);
	}

	fclose(f_out);

	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiBaseLogAccuracyWeights(void)
{
	UINT8_T file_path[256];
	FILE *f_16;
	FILE *f_32;
	FILE *f_16_out;
	FILE *f_32_out;
	FILE *f_out;
	UINT32_T index;
	FLOAT16_T fp16;
	FLOAT32_T fp16_32;
	FLOAT32_T fp32;
	FLOAT32_T accuracy_max = (FLOAT32_T)(0.0f);
	FLOAT32_T accuracy_min = (FLOAT32_T)(0.0f);
	FLOAT32_T accuracy_average = (FLOAT32_T)(0.0f);
	FLOAT32_T accuracy_sum = (FLOAT32_T)(0.0f);
	FLOAT32_T accuracy = (FLOAT32_T)(0.0f);
	FLOAT32_T accuracy_abs = (FLOAT32_T)(0.0f);
	FLOAT32_T accuracy_abs_max = (FLOAT32_T)(0.0f);
	FLOAT32_T sub_value_max = (FLOAT32_T)(0.0f);
	FLOAT32_T sub_value_min = (FLOAT32_T)(0.0f);
	FLOAT32_T sub_value_average = (FLOAT32_T)(0.0f);
	FLOAT32_T sub_value_sum = (FLOAT32_T)(0.0f);
	FLOAT32_T sub_value = (FLOAT32_T)(0.0f);
	FLOAT32_T sub_value_abs = (FLOAT32_T)(0.0f);
	FLOAT32_T sub_value_abs_max = (FLOAT32_T)(0.0f);

	/* open the file */
	sprintf((char *)file_path, "%sWeightsAccuracy.txt", AI_PRODUCT_LOG_IDENT_PATH"/");
	f_out = fopen(file_path, "wt");
	if (!f_out) {
		return AI_ERROR_GENERAL;
	}

	/* reset accuracy */
	accuracy_max = (FLOAT32_T)(0.0f);
	accuracy_min = (FLOAT32_T)(0.0f);
	accuracy_average = (FLOAT32_T)(0.0f);
	accuracy = (FLOAT32_T)(0.0f);
	accuracy_abs = (FLOAT32_T)(0.0f);
	accuracy_abs_max = (FLOAT32_T)(0.0f);
	sub_value_max = (FLOAT32_T)(0.0f);
	sub_value_min = (FLOAT32_T)(0.0f);
	sub_value_average = (FLOAT32_T)(0.0f);
	sub_value_sum = (FLOAT32_T)(0.0f);
	sub_value = (FLOAT32_T)(0.0f);
	sub_value_abs = (FLOAT32_T)(0.0f);
	sub_value_abs_max = (FLOAT32_T)(0.0f);

	/* open the file */
	sprintf((char *)file_path, "%syolo3.f16.weights", AI_PRODUCT_LOG_IDENT_PATH"/");
	f_16 = fopen(file_path, "rb");
	if (!f_16) {
		return AI_ERROR_GENERAL;
	}

	sprintf((char *)file_path, "%syolo3.f32.weights", AI_PRODUCT_LOG_IDENT_PATH"/");
	f_32 = fopen(file_path, "rb");
	if (!f_32) {
		return AI_ERROR_GENERAL;
	}

	sprintf((char *)file_path, "%syolo3.f16.txt", AI_PRODUCT_LOG_IDENT_PATH"/");
	f_16_out = fopen(file_path, "wb");
	if (!f_16_out) {
		return AI_ERROR_GENERAL;
	}

	sprintf((char *)file_path, "%syolo3.f32.txt", AI_PRODUCT_LOG_IDENT_PATH"/");
	f_32_out = fopen(file_path, "wb");
	if (!f_32_out) {
		return AI_ERROR_GENERAL;
	}

	index = 0;
	while (1) {
		/* read the value */
		if (fread((char *)&fp16, 1, sizeof(FLOAT16_T), f_16) != sizeof(FLOAT16_T))
			break;
		fp16_32 = AiBaseFloat32CvtF16F32(fp16);
		if (fread((char *)&fp32, 1, sizeof(FLOAT32_T), f_32) != sizeof(FLOAT32_T))
			break;

		/* calc accuracy */
		accuracy = (fp16_32 - fp32)*100.0f/fp32;
		if (fp16_32 != 0) {
			if (accuracy < accuracy_min) {
				accuracy_min = accuracy;
			}
			if (accuracy > accuracy_max) {
				accuracy_max = accuracy;
			}
		}
		if (accuracy < 0.0f) {
			accuracy_abs = 0.0f - accuracy;
		}
		else {
			accuracy_abs = accuracy;
		}
		if (fp16_32 != 0) {
			if (accuracy_abs > accuracy_abs_max) {
				accuracy_abs_max = accuracy_abs;
			}
		}
		if (accuracy_abs > 100.0f) {
			g_ai_base_log_accuracy_times++;
		}
		accuracy_sum = accuracy_sum + accuracy_abs;

		/* calc accuracy */
		sub_value = (fp16_32 - fp32);
		if (fp16_32 != 0) {
			if (sub_value < sub_value_min) {
				sub_value_min = sub_value;
			}
			if (sub_value > sub_value_max) {
				sub_value_max = sub_value;
			}
		}
		if (sub_value < 0.0f) {
			sub_value_abs = 0.0f - sub_value;
		}
		else {
			sub_value_abs = sub_value;
		}
		if (fp16_32 != 0) {
			if (sub_value_abs > sub_value_abs_max) {
				sub_value_abs_max = sub_value_abs;
			}
		}
		if (sub_value_abs > 1.0f) {
			g_ai_base_log_accuracy_times++;
		}
		sub_value_sum = sub_value_sum + sub_value_abs;

		/* save the max value */
		if ((sub_value == sub_value_min) ||
			(sub_value == sub_value_max) || 
			(accuracy == accuracy_min) ||
			(accuracy == accuracy_max)) {
			/* save index */
			sprintf((char *)file_path, "%08x:\t", index);
			fwrite((char *)file_path, 1, strlen(file_path), f_out);
			/* save data */
			sprintf((char *)file_path, "%02x%02x%02x%02x\t[fp32=%24f][fp16=%12f][accuracy=%12f%%][sub=%12f][fp32=%e][fp16=%e]\t",
				((unsigned char *)&fp32)[0], ((unsigned char *)&fp32)[1],
				((unsigned char *)&fp32)[2], ((unsigned char *)&fp32)[3],
				fp32, fp16_32, accuracy, sub_value, fp32, fp16_32);
			fwrite((char *)file_path, 1, strlen(file_path), f_out);
			sprintf((char *)file_path, "%s", "\r\n");
			fwrite((char *)file_path, 1, strlen(file_path), f_out);
		}
#if 0
		/* write f_16_out */
		if ((index % 4) == 0) {
			sprintf((char *)file_path, "%08x:\t", index);
			fwrite((char *)file_path, 1, strlen(file_path), f_16_out);
		}
		sprintf((char *)file_path, "%02x%02x\t%12f[%12f%%][%12e]\t",
			((unsigned char *)&fp16)[0], ((unsigned char *)&fp16)[1],
			fp16_32, accuracy, sub_value);
		fwrite((char *)file_path, 1, strlen(file_path), f_16_out);
		if ((index % 4) == 3) {
			sprintf((char *)file_path, "%s", "\r\n");
			fwrite((char *)file_path, 1, strlen(file_path), f_16_out);
		}

		/* write f_32_out */
		if ((index % 4) == 0) {
			sprintf((char *)file_path, "%08x:\t", index);
			fwrite((char *)file_path, 1, strlen(file_path), f_32_out);
		}
		sprintf((char *)file_path, "%02x%02x%02x%02x\t%12f[%12f%%][%12e]\t",
			((unsigned char *)&fp32)[0], ((unsigned char *)&fp32)[1],
			((unsigned char *)&fp32)[2], ((unsigned char *)&fp32)[3],
			fp32, accuracy, sub_value);
		fwrite((char *)file_path, 1, strlen(file_path), f_32_out);
		if ((index % 4) == 3) {
			sprintf((char *)file_path, "%s", "\r\n");
			fwrite((char *)file_path, 1, strlen(file_path), f_32_out);
		}
#endif

		index++;
	}

	/* no data */
	if (index == 0) {

	}

	/* average */
	accuracy_average = accuracy_sum / index;
	sub_value_average = sub_value_sum / index;
	/* write out */
	sprintf((char *)file_path, "weights accuracy:\tsum:[%12f%%]\tavg:[%12f%%]\tmax:[%12f%%]\tmin:[%12f%%]\r\n",
			accuracy_sum,
			accuracy_average,
			accuracy_max, 
			accuracy_min);
	fwrite((char *)file_path, 1, strlen(file_path), f_out);
	sprintf((char *)file_path, "weights    value:\tsum:[%13f]\tavg:[%13f]\tmax:[%13f]\tmin:[%13f]\r\n",
		sub_value_sum,
		sub_value_average,
		sub_value_max,
		sub_value_min);
	fwrite((char *)file_path, 1, strlen(file_path), f_out);

	fclose(f_16);
	fclose(f_32);
	fclose(f_16_out);
	fclose(f_32_out);

	fclose(f_out);

	return AI_ERROR_SUCCESS;
}

/*------------------------- End ---------------------------------------------*/
