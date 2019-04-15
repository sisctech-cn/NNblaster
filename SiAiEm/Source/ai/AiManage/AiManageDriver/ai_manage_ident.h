/**
 ******************************************************************************
 * file    ai_manage_ident.h
 * author  sunlingge
 * version V100
 * brief   the ai manage ident driver.
 ******************************************************************************
 */
#ifndef AI_MANAGE_IDENT_H
#define AI_MANAGE_IDENT_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/
typedef struct ai_manage_ident_ro_struct {
    UINT8_T data_sets;
	UINT8_T net;
	UINT8_T weights;
	UINT8_T source;
	FLOAT_AI_T confidence_threshold;
} AI_MANAGE_IDENT_RO, *AI_MANAGE_IDENT_RO_PTR;

/*------------------------- Function Definition -----------------------------*/

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
