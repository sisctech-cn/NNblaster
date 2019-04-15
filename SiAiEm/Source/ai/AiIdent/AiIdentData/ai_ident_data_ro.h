/**
 ******************************************************************************
 * file    ai_ident_data_ro.h
 * author  sunlingge
 * version V100
 * brief   the ai ident read only data.
 ******************************************************************************
 */
#ifndef AI_IDENT_DATA_RO_H
#define AI_IDENT_DATA_RO_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/
typedef struct ai_ident_data_ro_struct {
	UINT8_T reserved;
} AI_IDENT_DATA_RO, *AI_IDENT_DATA_RO_PTR;

typedef struct ai_ident_data_config_struct {
	UINT8_T reserved;
} AI_IDENT_DATA_CONFIG, *AI_IDENT_DATA_CONFIG_PTR;

/*------------------------- Function Definition -----------------------------*/

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

