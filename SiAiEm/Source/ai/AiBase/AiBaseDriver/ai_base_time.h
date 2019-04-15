/**
 ******************************************************************************
 * file    ai_base_time.h
 * author  sunlingge
 * version V100
 * brief   the ai base time driver.
 ******************************************************************************
 */
#ifndef AI_BASE_TIME_H
#define AI_BASE_TIME_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/
typedef struct ai_base_tick_rw_struct {
    __IO UINT64_T tick;
} AI_BASE_TICK_RW, *AI_BASE_TICK_RW_PTR;

typedef struct ai_base_tick_ro_struct {
    UINT32_T    per_second;
    UINT32_T    usecond;
    UINT32_T    udelay_times;
} AI_BASE_TICK_RO, *AI_BASE_TICK_RO_PTR;

/*------------------------- Function Definition -----------------------------*/
UINT64_T AiBaseTickGet(void);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
