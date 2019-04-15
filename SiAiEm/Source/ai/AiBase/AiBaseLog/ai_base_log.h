/**
 ******************************************************************************
 * file    ai_base_log.h
 * author  sunlingge
 * version V100
 * brief   the ai base log driver.
 ******************************************************************************
 */
#ifndef AI_BASE_IMAGE_LOG_H
#define AI_BASE_IMAGE_LOG_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/


/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_LOG_LAYER_DATA_STRING_LENGTH_MAX			(512)

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiBaseLogErrorCritial(void);
ERROR_T AiBaseLogLayerData(INT32_T layer_index, UINT8_T *name, UINT8_T *buffer, UINT32_T length);
ERROR_T AiBaseLogLayerInfo(INT32_T layer_index, UINT8_T *name, UINT8_T *buffer, UINT32_T length);
ERROR_T AiBaseLogLayerInfoAppend(INT32_T layer_index, UINT8_T *name, UINT8_T *buffer, UINT32_T length);
ERROR_T AiBaseLogLayerDataFormat(INT32_T layer_index, UINT8_T *name, UINT8_T *format, ...);
ERROR_T AiBaseLogLayerDataAppendFormat(INT32_T layer_index, UINT8_T *name, UINT8_T *format, ...);
ERROR_T AiBaseLogConvertTextF16(UINT8_T *name);
ERROR_T AiBaseLogConvertTextF32(UINT8_T *name);
ERROR_T AiBaseLogAccuracyTextF16(void);
ERROR_T AiBaseLogAccuracyTextF32(void);
ERROR_T AiBaseLogAccuracyWeights(void);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

