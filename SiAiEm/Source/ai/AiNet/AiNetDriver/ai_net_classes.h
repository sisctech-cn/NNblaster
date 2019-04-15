/**
 ******************************************************************************
 * file    ai_net_classes.h
 * author  sunlingge
 * version V100
 * brief   the ai net classes driver.
 ******************************************************************************
 */
#ifndef AI_NET_CLASSES_H
#define AI_NET_CLASSES_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/


/*------------------------- Macro Definition --------------------------------*/
#define AI_NET_CLASSES_LINE_LENGTH_MAX											(1024)
#define AI_NET_CLASSES_NAME_LENGTH_MAX											(64)
#define AI_NET_CLASSES_NUMBER_MAX												(128)

/*------------------------- Type Definition----------------------------------*/
typedef struct ai_net_classes_struct {
	UINT32_T number;
	UINT8_T name[AI_NET_CLASSES_NUMBER_MAX][AI_NET_CLASSES_NAME_LENGTH_MAX];
} AI_NET_CLASSES, *AI_NET_CLASSES_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiNetClassesLoad(AI_NET_CLASSES *net_classes);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/

