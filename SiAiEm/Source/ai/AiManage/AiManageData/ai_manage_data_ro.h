/**
 ******************************************************************************
 * file    ai_manage_data_ro.h
 * author  sunlingge
 * version V100
 * brief   the ai manage read only data.
 ******************************************************************************
 */
#ifndef AI_MANAGE_DATA_RO_H
#define AI_MANAGE_DATA_RO_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/
typedef struct ai_manage_data_ro_struct {
	AI_MANAGE_ROOT_RO root;
	AI_MANAGE_DETECT_RO detect;
	AI_MANAGE_IDENT_RO ident;
	UINT8_T reserved;
} AI_MANAGE_DATA_RO, *AI_MANAGE_DATA_RO_PTR;

typedef struct ai_manage_data_config_struct {
	UINT8_T reserved;
} AI_MANAGE_DATA_CONFIG, *AI_MANAGE_DATA_CONFIG_PTR;

/*------------------------- Function Definition -----------------------------*/

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

