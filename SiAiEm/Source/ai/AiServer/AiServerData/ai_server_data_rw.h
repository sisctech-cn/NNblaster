/**
 ******************************************************************************
 * file    ai_server_data_rw.h
 * author  sunlingge
 * version V100
 * brief   the ai server read write data.
 ******************************************************************************
 */
#ifndef AI_SERVER_DATA_RW_H
#define AI_SERVER_DATA_RW_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/


/*------------------------- Macro Definition --------------------------------*/


/*------------------------- Type Definition----------------------------------*/
typedef struct ai_server_data_export_struct {
    UINT8_T reserved;
} AI_SERVER_DATA_EXPORT, *AI_SERVER_DATA_EXPORT_PTR;

typedef struct ai_server_data_rw_struct {
    UINT8_T reserved;
} AI_SERVER_DATA_RW, *AI_SERVER_DATA_RW_PTR;

/*------------------------- Function Definition -----------------------------*/

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

