/**
 ******************************************************************************
 * file    ai_ident_data.h
 * author  sunlingge
 * version V100
 * brief   the ai ident data.
 ******************************************************************************
 */
#ifndef AI_IDENT_DATA_H
#define AI_IDENT_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiIdentDataInitPrevious(void);
ERROR_T AiIdentDataInitPost(void);
ERROR_T AiIdentDataMemoryAddressCheck(UINT64_T address);
ERROR_T AiIdentDataRun(void);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

