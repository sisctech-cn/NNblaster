/**
 ******************************************************************************
 * file    ai_base_os.h
 * author  sunlingge
 * version V100
 * brief   the ai base os driver.
 ******************************************************************************
 */
#ifndef AI_BASE_OS_H
#define AI_BASE_OS_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/
#ifdef WIN32
#define AI_SERVER_WIN32					(1)
#define inline
#define bool							char
#ifndef DEBUG
#define DEBUG							(1)
#endif
#define CONFIG_LOG_LEVEL				(LOG_VERBOSE)
#define CONFIG_LOG_ENABLE				
#define CONFIG_LOG_COLORS				(1) 
#define LOG_KERNEL
typedef long signed int					ssize_t;
#define STDIN_FILENO					0       /* standard input file descriptor */
#define STDOUT_FILENO					1       /* standard output file descriptor */
#define STDERR_FILENO					2       /* standard error file descriptor */
#endif
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#ifndef AI_UI_INCLUDE
#include <math.h>
#endif
#include <stdarg.h>

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
