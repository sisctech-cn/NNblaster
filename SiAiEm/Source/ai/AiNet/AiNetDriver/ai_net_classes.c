/**
 ******************************************************************************
 * file    ai_net_classes.c
 * author  sunlingge
 * version V100
 * brief   the ai net classes driver.
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
ERROR_T AiNetClassesGetFileLine(FILE *fp, UINT8_T *line)
{
	UINT32_T curr;

	if (feof(fp)) {
		return AI_ERROR_FILE_EOF;
	}

	memset(line, 0, AI_NET_CLASSES_LINE_LENGTH_MAX);
    if (!fgets((char *)line, AI_NET_CLASSES_LINE_LENGTH_MAX, fp)){
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
ERROR_T AiNetClassesStrip(UINT8_T *s)
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
ERROR_T AiNetClassesReadOption(UINT8_T *s, UINT8_T *key, UINT8_T *value)
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
ERROR_T AiNetClassesGetFilePath(UINT8_T *classes_file_path)
{
	UINT8_T file_path[256];
	UINT8_T line[AI_NET_CONFIG_LINE_LENGTH_MAX];
	UINT8_T key_str[AI_NET_CONFIG_LINE_LENGTH_MAX];
	UINT8_T value_str[AI_NET_CONFIG_LINE_LENGTH_MAX];
	FILE *file;
	ERROR_T result = AI_ERROR_GENERAL;

    sprintf((char *)file_path, AI_PRODUCT_FILE_PATH""AI_PRODUCT_YOLOV3_CONFIG_PATH ""AI_PRODUCT_YOLOV3_CLASSES_FILE);
	file = fopen((char *)file_path, "rt");
    if(file == 0)  {
		return AI_ERROR_GENERAL;
	}

	while(1) {
		result = AiNetClassesGetFileLine(file, line);
		if (result == AI_ERROR_FILE_EOF) {
			break;
		}
		if (result != AI_ERROR_SUCCESS) {
			break;
		}

		AiNetClassesStrip(line);
		switch (line[0]){
            /* empty line */
            case '\0':
			/* comment line */
            case '#':
			/* comment line */
            case ';':
                break;
            /* option line */
            default:
                result = AiNetClassesReadOption(line, key_str, value_str);
				if (result != AI_ERROR_SUCCESS){
					break;		
                }
				if (strcmp((char *)key_str, "names") == 0) {
					strcpy((char *)classes_file_path, (char *)AI_PRODUCT_FILE_PATH);
					strcpy((char *)classes_file_path + strlen((char *)classes_file_path), (char *)AI_PRODUCT_YOLOV3_CONFIG_PATH);
					strcpy((char *)classes_file_path+strlen((char *)classes_file_path), (char *)value_str);
					fclose(file);
					return AI_ERROR_SUCCESS;
				}

                break;
        }
		if (result != AI_ERROR_SUCCESS) {
			break;
		}
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
ERROR_T AiNetClassesLoad(AI_NET_CLASSES *net_classes)
{
	UINT8_T file_path[256];
	UINT8_T line[AI_NET_CONFIG_LINE_LENGTH_MAX];
	INT32_T classes_index;
	FILE *file;
	ERROR_T result = AI_ERROR_SUCCESS;


	result = AiNetClassesGetFilePath(file_path);
	if (result != AI_ERROR_SUCCESS) {
		return AI_ERROR_GENERAL;
	}

	file = fopen((char *)file_path, "rt");
    if(file == 0)  {
		return AI_ERROR_GENERAL;
	}

	memset((char *)net_classes, 0, sizeof(AI_NET_CLASSES));
	classes_index = 0;
	while(1) {
		result = AiNetClassesGetFileLine(file, line);
		if (result == AI_ERROR_FILE_EOF) {
			result = AI_ERROR_SUCCESS;
			break;
		}
		if (result != AI_ERROR_SUCCESS) {
			result = AI_ERROR_SUCCESS;
			break;
		}

		AiNetClassesStrip(line);
		switch (line[0]){
            /* empty line */
            case '\0':
			/* comment line */
            case '#':
			/* comment line */
            case ';':
                break;
            /* option line */
            default:
				if (strlen((char *)line) >= (AI_NET_CLASSES_NAME_LENGTH_MAX-1)) {
					line[AI_NET_CLASSES_NAME_LENGTH_MAX-1] = '\0';
				}
				strcpy((char *)(net_classes->name[classes_index]), (char *)line);
				classes_index++;
				net_classes->number = classes_index;
                break;
        }
		if (result != AI_ERROR_SUCCESS) {
			break;
		}
	}

	fclose(file);
	return AI_ERROR_SUCCESS;
}

/*------------------------- End ---------------------------------------------*/
