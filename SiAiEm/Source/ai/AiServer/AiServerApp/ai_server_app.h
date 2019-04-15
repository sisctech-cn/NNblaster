/**
 ******************************************************************************
 * file    ai_server_app.h
 * author  sunlingge
 * version V100
 * brief   the ai center app.
 ******************************************************************************
 */
#ifndef AI_SERVER_APP_H
#define AI_SERVER_APP_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/
#define AI_SERVER_APP_INIT_NONE		(0)
#define AI_SERVER_APP_INIT_DONE		(1)

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/
#if (AI_PRODUCT_OPTION_WIN32 == 0)
int main(void);
#else
int AiMain(void);
int AiWeightsConvert(void);
#endif
ERROR_T AiServerAppInitPrevious(void);
ERROR_T AiServerAppInitPost(void);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
