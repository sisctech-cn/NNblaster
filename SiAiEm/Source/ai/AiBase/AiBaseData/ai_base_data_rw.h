/**
 ******************************************************************************
 * file    ai_base_data_rw.h
 * author  sunlingge
 * version V100
 * brief   the ai base read write data.
 ******************************************************************************
 */
#ifndef AI_BASE_DATA_RW_H
#define AI_BASE_DATA_RW_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/


/*------------------------- Macro Definition --------------------------------*/


/*------------------------- Type Definition----------------------------------*/
typedef struct ai_base_data_export_struct {
	UINT8_T reserved;
} AI_BASE_DATA_EXPORT, *AI_BASE_DATA_EXPORT_PTR;

typedef struct ai_base_data_rw_struct {
	UINT8_T reserved;
} AI_BASE_DATA_RW, *AI_BASE_DATA_RW_PTR;

/*------------------------- Function Definition -----------------------------*/

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

