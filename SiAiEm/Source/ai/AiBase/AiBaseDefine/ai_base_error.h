 /**
 ******************************************************************************
 * file    ai_base_error.h
 * author  sunlingge
 * version V100
 * brief   the ai base error definition.
 ******************************************************************************
 */
#ifndef AI_BASE_DATA_ERROR_H
#define AI_BASE_DATA_ERROR_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AI_ERROR_SUCCESS					(0)
#define AI_ERROR_GENERAL                   	(-1)
#define AI_ERROR_PACKET                    	(-2)
#define AI_ERROR_MENU						(-3)
#define AI_ERROR_NO_RESPONSE				(-4)
#define AI_ERROR_CRC						(-5)
#define AI_ERROR_FILE_EOF					(-6)
#define AI_ERROR_MODEL_READY				(-7)
#define AI_ERROR_MODEL_BUSY					(-8)

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
