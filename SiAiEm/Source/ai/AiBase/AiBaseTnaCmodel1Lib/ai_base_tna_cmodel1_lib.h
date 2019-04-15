/**
 ******************************************************************************
 * file    ai_base_cmodel_lib.h
 * author  sunlingge
 * version V100
 * brief   the ai base cmodel lib driver.
 ******************************************************************************
 */
#ifndef AI_BASE_CMODEL_LIB_H
#define AI_BASE_CMODEL_LIB_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/
typedef   int   BIT16;
typedef   int   BIT32;
typedef unsigned short                          UBIT16;

int AiBaseLogCmodelForwardDataAppendFormat(char *format, ...);
#define AI_PRODUCT_FILE_PATH			"./"
#define AI_BASE_TNA_CMODEL1_COMMAND_PATH			AI_PRODUCT_FILE_PATH"/Log/Cmodel1/Command/"
#define AI_BASE_TNA_CMODEL1_FORWARD_PATH			AI_PRODUCT_FILE_PATH"/Log/Cmodel1/Forward/"
#define AI_PRODUCT_LOG_IDENT_PATH					AI_PRODUCT_FILE_PATH"/Log/AiIdent/"

/*------------------------- Function Definition -----------------------------*/
int tna_singles2halfp(void *target, void *source, int numel);
int tna_halfp2singles(void *target, void *source, int numel);
UBIT16 tna_func_RND_eval(UBIT16 RND, UBIT16 Sign, UBIT16 l, UBIT16 r, UBIT16 STK);
UBIT16 tna_SISC_fp_add(UBIT16 a, UBIT16 b, UBIT16 rnd);
UBIT16 tna_func_RND_eval_u(UBIT16 RND, UBIT16 Sign, UBIT16 l, UBIT16 r, UBIT16 STK);
UBIT16 tna_SISC_fp_mult(UBIT16 a, UBIT16 b, UBIT16 rnd);
UBIT16 tna_SISC_fp_div(UBIT16 a, UBIT16 b, UBIT16 rnd);
UBIT16 tna_SISC_fp_exp(UBIT16 s_data);
UBIT16 tna_SISC_fp_sigmoid(UBIT16 s_data);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

