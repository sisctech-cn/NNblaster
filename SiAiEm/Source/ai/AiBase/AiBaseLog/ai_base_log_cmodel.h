/**
 ******************************************************************************
 * file    ai_base_log_cmodel.h
 * author  sunlingge
 * version V100
 * brief   the ai base log cmodel driver.
 ******************************************************************************
 */
#ifndef AI_BASE_LOG_CMODEL_H
#define AI_BASE_LOG_CMODEL_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/


/*------------------------- Macro Definition --------------------------------*/
#define AI_BASE_LOG_CMODEL_STRING_LENGTH_MAX		(512)

/*------------------------- Type Definition----------------------------------*/
	
/*------------------------- Function Definition -----------------------------*/
ERROR_T AiBaseLogCmodelData(UINT8_T *name, UINT8_T *buffer, UINT32_T length);
ERROR_T AiBaseLogCmodelInfo(UINT8_T *name, UINT8_T *buffer, UINT32_T length);
ERROR_T AiBaseLogCmodelInfoAppend(UINT8_T *name, UINT8_T *buffer, UINT32_T length);
ERROR_T AiBaseLogCmodelDataFormat(UINT8_T *name, UINT8_T *format, ...);
ERROR_T AiBaseLogCmodelDataAppendFormat(UINT8_T *name, UINT8_T *format, ...);
ERROR_T AiBaseLogCmodelCommandDataFormat(UINT8_T *format, ...);
ERROR_T AiBaseLogCmodelCommandDataAppendFormat(UINT8_T *format, ...);
ERROR_T AiBaseLogCmodelClockDataFormat(UINT8_T *format, ...);
ERROR_T AiBaseLogCmodelClockDataAppendFormat(UINT8_T *format, ...);
ERROR_T AiBaseLogCmodelForwardDataFormat(UINT8_T *format, ...);
ERROR_T AiBaseLogCmodelForwardDataAppendFormat(UINT8_T *format, ...);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

