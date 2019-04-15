/**
 ******************************************************************************
 * file    ai_board_x86_aux.h
 * author  sunlingge
 * version V100
 * brief   the ai board x86 bsp aux driver.
 ******************************************************************************
 */
#ifndef AI_BASE_BOARD_X86_AUX_H
#define AI_BASE_BOARD_X86_AUX_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AiBaseBoardX86Cpuid(info, x)    __cpuidex(info, x, 0)

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiBaseBoardX86CheckCpuFeatures(void);
INT32_T AiBaseBoardX86IsAvx(void);
INT32_T AiBaseBoardX86IsFmaAvx2(void);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

