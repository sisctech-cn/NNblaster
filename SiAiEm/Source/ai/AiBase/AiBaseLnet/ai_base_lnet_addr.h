/**
 ******************************************************************************
 * file    ai_base_lnet_addr.h
 * author  sunlingge
 * version V100
 * brief   the ai base lnet addr driver.
 ******************************************************************************
 */
#ifndef AI_BASE_LNET_ADDR_H
#define AI_BASE_LNET_ADDR_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/
typedef struct ai_base_lnet_addr_struct {
    /* the io_addr[0-3] : io addr or distrubution by sw */
    /* the io_addr[4-7] : port: base, manage, server */
    UINT8_T io_addr;
} AI_BASE_LNET_ADDR, *AI_BASE_LNET_ADDR_PTR;

/*------------------------- Function Definition -----------------------------*/

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
