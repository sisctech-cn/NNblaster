/**
 ******************************************************************************
 * file    ai_board_tna_aux.h
 * author  sunlingge
 * version V100
 * brief   the ai board tna ir bsp aux driver.
 ******************************************************************************
 */
#if (AI_PRODUCT_OPTION_ACCELERATOR_TYPE == AI_PRODUCT_ACCELERATOR_SISCTECH_CMODEL2)
#ifndef AI_BASE_BOARD_CMODEL2_AUX_H
#define AI_BASE_BOARD_CMODEL2_AUX_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AiBaseBoardCmodel2Cpuid(info, x)    __cpuidex(info, x, 0)

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiBaseBoardCmodel2CheckCpuFeatures(void);
INT32_T AiBaseBoardCmodel2IsAvx(void);
INT32_T AiBaseBoardCmodel2IsFmaAvx2(void);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
#endif

