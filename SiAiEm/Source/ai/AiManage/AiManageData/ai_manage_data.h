/**
 ******************************************************************************
 * file    ai_manage_data.h
 * author  sunlingge
 * version V100
 * brief   the ai manage data.
 ******************************************************************************
 */
#ifndef AI_MANAGE_DATA_H
#define AI_MANAGE_DATA_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiManageDataInitPrevious(void);
ERROR_T AiManageDataInitPost(void);
ERROR_T AiManageDataRun(void);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

