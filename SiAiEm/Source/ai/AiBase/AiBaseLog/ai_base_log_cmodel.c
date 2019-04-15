/**
 ******************************************************************************
 * file    ai_base_log_cmodel.c
 * author  sunlingge
 * version V100
 * brief   the ai base log cmodel driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"
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
ERROR_T AiBaseLogCmodelData(UINT8_T *name, UINT8_T *buffer, UINT32_T length)
{
    UINT8_T file_path[256];
	FILE *f;

    sprintf((char *)file_path, "%s%s", AI_PRODUCT_LOG_IDENT_PATH"/", name);
	f = fopen(file_path, "wb");
	if (!f) {
		AiBaseLogErrorCritial();
		return AI_ERROR_GENERAL;
	}

	fwrite(buffer, 1, length, f);
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
ERROR_T AiBaseLogCmodelInfo(UINT8_T *name, UINT8_T *buffer, UINT32_T length)
{
	UINT8_T file_path[256];
	FILE *f;

	sprintf((char *)file_path, "%s%s", AI_PRODUCT_LOG_IDENT_PATH"/", name);
	f = fopen(file_path, "wb");
	if (!f) {
		AiBaseLogErrorCritial();
		return AI_ERROR_GENERAL;
	}

	fwrite(buffer, 1, length, f);
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
ERROR_T AiBaseLogCmodelInfoAppend(UINT8_T *name, UINT8_T *buffer, UINT32_T length)
{
	UINT8_T file_path[256];
	FILE *f;

	sprintf((char *)file_path, "%s%s", AI_PRODUCT_LOG_IDENT_PATH"/", name);
	f = fopen(file_path, "ab+");
	if (!f) {
		AiBaseLogErrorCritial();
		return AI_ERROR_GENERAL;
	}

	fwrite(buffer, 1, length, f);
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
ERROR_T AiBaseLogCmodelDataFormat(UINT8_T *name, UINT8_T *format, ...) {
	UINT8_T str_buffer[AI_BASE_LOG_CMODEL_STRING_LENGTH_MAX];

	va_list vlArgs;
	va_start(vlArgs, format);
	memset((char *)str_buffer, 0, AI_BASE_LOG_CMODEL_STRING_LENGTH_MAX);
	vsnprintf(str_buffer, AI_BASE_LOG_CMODEL_STRING_LENGTH_MAX - 1, format, vlArgs);
	va_end(vlArgs);

	AiBaseLogCmodelInfo(name, str_buffer, (UINT32_T)strlen(str_buffer));
	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiBaseLogCmodelDataAppendFormat(UINT8_T *name,  UINT8_T *format, ...) {
	UINT8_T str_buffer[AI_BASE_LOG_CMODEL_STRING_LENGTH_MAX];

	va_list vlArgs;
	va_start(vlArgs, format);
	memset((char *)str_buffer, 0, AI_BASE_LOG_CMODEL_STRING_LENGTH_MAX);
	vsnprintf(str_buffer, AI_BASE_LOG_CMODEL_STRING_LENGTH_MAX -1, format, vlArgs);
	va_end(vlArgs);
	
	AiBaseLogCmodelInfoAppend(name, str_buffer, (UINT32_T)strlen(str_buffer));
	return AI_ERROR_SUCCESS;
}


/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiBaseLogCmodelCommandDataFormat(UINT8_T *format, ...) {
	UINT8_T str_buffer[AI_BASE_LOG_CMODEL_STRING_LENGTH_MAX];

	va_list vlArgs;
	va_start(vlArgs, format);
	memset((char *)str_buffer, 0, AI_BASE_LOG_CMODEL_STRING_LENGTH_MAX);
	vsnprintf(str_buffer, AI_BASE_LOG_CMODEL_STRING_LENGTH_MAX - 1, format, vlArgs);
	va_end(vlArgs);

	AiBaseLogCmodelInfo(AI_PRODUCT_CMODEL_COMMAND_FILE, str_buffer, (UINT32_T)strlen(str_buffer));
	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiBaseLogCmodelCommandDataAppendFormat(UINT8_T *format, ...) {
	UINT8_T str_buffer[AI_BASE_LOG_CMODEL_STRING_LENGTH_MAX];

	va_list vlArgs;
	va_start(vlArgs, format);
	memset((char *)str_buffer, 0, AI_BASE_LOG_CMODEL_STRING_LENGTH_MAX);
	vsnprintf(str_buffer, AI_BASE_LOG_CMODEL_STRING_LENGTH_MAX - 1, format, vlArgs);
	va_end(vlArgs);

	AiBaseLogCmodelInfoAppend(AI_PRODUCT_CMODEL_COMMAND_FILE, str_buffer, (UINT32_T)strlen(str_buffer));
	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiBaseLogCmodelClockDataFormat(UINT8_T *format, ...) {
	UINT8_T str_buffer[AI_BASE_LOG_CMODEL_STRING_LENGTH_MAX];

	va_list vlArgs;
	va_start(vlArgs, format);
	memset((char *)str_buffer, 0, AI_BASE_LOG_CMODEL_STRING_LENGTH_MAX);
	vsnprintf(str_buffer, AI_BASE_LOG_CMODEL_STRING_LENGTH_MAX - 1, format, vlArgs);
	va_end(vlArgs);

	AiBaseLogCmodelInfo(AI_PRODUCT_CMODEL_CLOCK_FILE, str_buffer, (UINT32_T)strlen(str_buffer));
	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiBaseLogCmodelClockDataAppendFormat(UINT8_T *format, ...) {
	UINT8_T str_buffer[AI_BASE_LOG_CMODEL_STRING_LENGTH_MAX];

	va_list vlArgs;
	va_start(vlArgs, format);
	memset((char *)str_buffer, 0, AI_BASE_LOG_CMODEL_STRING_LENGTH_MAX);
	vsnprintf(str_buffer, AI_BASE_LOG_CMODEL_STRING_LENGTH_MAX - 1, format, vlArgs);
	va_end(vlArgs);

	AiBaseLogCmodelInfoAppend(AI_PRODUCT_CMODEL_CLOCK_FILE, str_buffer, (UINT32_T)strlen(str_buffer));
	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiBaseLogCmodelForwardDataFormat(UINT8_T *format, ...) {
	UINT8_T str_buffer[AI_BASE_LOG_CMODEL_STRING_LENGTH_MAX];

	va_list vlArgs;
	va_start(vlArgs, format);
	memset((char *)str_buffer, 0, AI_BASE_LOG_CMODEL_STRING_LENGTH_MAX);
	vsnprintf(str_buffer, AI_BASE_LOG_CMODEL_STRING_LENGTH_MAX - 1, format, vlArgs);
	va_end(vlArgs);

	AiBaseLogCmodelInfo(AI_PRODUCT_CMODEL_FORWARD_FILE, str_buffer, (UINT32_T)strlen(str_buffer));
	return AI_ERROR_SUCCESS;
}

/**
* brief    none.
* param    None
* retval   None
* author   Sunlingge
* comment  V100
*/
ERROR_T AiBaseLogCmodelForwardDataAppendFormat(UINT8_T *format, ...) {
	UINT8_T str_buffer[AI_BASE_LOG_CMODEL_STRING_LENGTH_MAX];

	va_list vlArgs;
	va_start(vlArgs, format);
	memset((char *)str_buffer, 0, AI_BASE_LOG_CMODEL_STRING_LENGTH_MAX);
	vsnprintf(str_buffer, AI_BASE_LOG_CMODEL_STRING_LENGTH_MAX - 1, format, vlArgs);
	va_end(vlArgs);

	AiBaseLogCmodelInfoAppend(AI_PRODUCT_CMODEL_FORWARD_FILE, str_buffer, (UINT32_T)strlen(str_buffer));
	return AI_ERROR_SUCCESS;
}


/*------------------------- End ---------------------------------------------*/
