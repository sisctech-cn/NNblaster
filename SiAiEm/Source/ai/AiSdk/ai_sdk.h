/**
 ******************************************************************************
 * file    ai_sdk.h
 * author  sunlingge
 * version V100
 * brief   the ai manage software development kit.
 ******************************************************************************
 */
#ifndef AI_SDK_H
#define AI_SDK_H

#ifdef __cplusplus
extern "C" {
#endif

/*------------------------- Include File ------------------------------------*/
#if (AI_PRODUCT_OPTION_WIN32 == 1)
#else
#include <stdint.h>
#include <string.h>
#endif

/*------------------------- Type Definition --------------------------------*/
typedef int8_t INT8_T;
typedef uint8_t UINT8_T;
typedef int16_t INT16_T;
typedef uint16_t UINT16_T;
typedef int32_t INT32_T;
typedef uint32_t UINT32_T;
typedef int64_t INT64_T;
typedef uint64_t UINT64_T;
typedef int32_t ERROR_T;

/*------------------------- Macro Definition --------------------------------*/
#define AI_DAT_OPERATE_READ_MASK							(0x01)
#define AI_DAT_OPERATE_WRITE_MASK							(0x02)
#define AI_DAT_OPERATE_STRING_MASK							(0x04)
#define AI_DAT_OPERATE_PRODUCT_MASK							(0x08)
#define AI_DAT_OPERATE_CALIBRATION_MASK						(0x10)
#define AI_DAT_OPERATE_ROOT_MASK							(0x20)
#define AI_DAT_OPERATE_VAR_MASK								(0x40)
#define AI_DAT_OPERATE_READ									(AI_DAT_OPERATE_READ_MASK)
#define AI_DAT_OPERATE_WRITE								(AI_DAT_OPERATE_WRITE_MASK)
#define AI_DAT_OPERATE_READ_WRITE							(AI_DAT_OPERATE_WRITE_MASK | AI_DAT_OPERATE_READ_MASK)
#define AI_DAT_OPERATE_READ_STRING							(AI_DAT_OPERATE_STRING_MASK | AI_DAT_OPERATE_READ_MASK)
#define AI_DAT_OPERATE_WRITE_STRING							(AI_DAT_OPERATE_STRING_MASK | AI_DAT_OPERATE_WRITE_MASK)
#define AI_DAT_OPERATE_READ_WRITE_STRING					(AI_DAT_OPERATE_STRING_MASK | AI_DAT_OPERATE_WRITE_MASK | AI_DAT_OPERATE_READ_MASK)
#define AI_DAT_OPERATE_READ_PRODUCT							(AI_DAT_OPERATE_PRODUCT_MASK | AI_DAT_OPERATE_READ_MASK)
#define AI_DAT_OPERATE_WRITE_PRODUCT						(AI_DAT_OPERATE_PRODUCT_MASK | AI_DAT_OPERATE_WRITE_MASK)
#define AI_DAT_OPERATE_READ_WRITE_PRODUCT					(AI_DAT_OPERATE_PRODUCT_MASK | AI_DAT_OPERATE_WRITE_MASK | AI_DAT_OPERATE_READ_MASK)
#define AI_DAT_OPERATE_READ_STRING_PRODUCT					(AI_DAT_OPERATE_PRODUCT_MASK | AI_DAT_OPERATE_STRING_MASK | AI_DAT_OPERATE_READ_MASK)
#define AI_DAT_OPERATE_WRITE_STRING_PRODUCT					(AI_DAT_OPERATE_PRODUCT_MASK | AI_DAT_OPERATE_STRING_MASK | AI_DAT_OPERATE_WRITE_MASK)
#define AI_DAT_OPERATE_READ_WRITE_STRING_PRODUCT			(AI_DAT_OPERATE_PRODUCT_MASK | AI_DAT_OPERATE_STRING_MASK | AI_DAT_OPERATE_WRITE_MASK | AI_DAT_OPERATE_READ_MASK)
#define AI_DAT_OPERATE_READ_CALIBRATION						(AI_DAT_OPERATE_CALIBRATION_MASK | AI_DAT_OPERATE_READ_MASK)
#define AI_DAT_OPERATE_WRITE_CALIBRATION					(AI_DAT_OPERATE_CALIBRATION_MASK | AI_DAT_OPERATE_WRITE_MASK)
#define AI_DAT_OPERATE_READ_WRITE_CALIBRATION				(AI_DAT_OPERATE_CALIBRATION_MASK | AI_DAT_OPERATE_WRITE_MASK | AI_DAT_OPERATE_READ_MASK)
#define AI_DAT_OPERATE_READ_STRING_CALIBRATION				(AI_DAT_OPERATE_CALIBRATION_MASK | AI_DAT_OPERATE_STRING_MASK | AI_DAT_OPERATE_READ_MASK)
#define AI_DAT_OPERATE_WRITE_STRING_CALIBRATION				(AI_DAT_OPERATE_CALIBRATION_MASK | AI_DAT_OPERATE_STRING_MASK | AI_DAT_OPERATE_WRITE_MASK)
#define AI_DAT_OPERATE_READ_WRITE_STRING_CALIBRATION		(AI_DAT_OPERATE_CALIBRATION_MASK | AI_DAT_OPERATE_STRING_MASK | AI_DAT_OPERATE_WRITE_MASK | AI_DAT_OPERATE_READ_MASK)
#define AI_DAT_OPERATE_READ_ROOT							(AI_DAT_OPERATE_ROOT_MASK | AI_DAT_OPERATE_READ_MASK)
#define AI_DAT_OPERATE_WRITE_ROOT							(AI_DAT_OPERATE_ROOT_MASK | AI_DAT_OPERATE_WRITE_MASK)
#define AI_DAT_OPERATE_READ_WRITE_ROOT						(AI_DAT_OPERATE_ROOT_MASK | AI_DAT_OPERATE_WRITE_MASK | AI_DAT_OPERATE_READ_MASK)
#define AI_DAT_OPERATE_READ_STRING_ROOT						(AI_DAT_OPERATE_ROOT_MASK | AI_DAT_OPERATE_STRING_MASK | AI_DAT_OPERATE_READ_MASK)
#define AI_DAT_OPERATE_WRITE_STRING_ROOT					(AI_DAT_OPERATE_ROOT_MASK | AI_DAT_OPERATE_STRING_MASK | AI_DAT_OPERATE_WRITE_MASK)
#define AI_DAT_OPERATE_READ_WRITE_STRING_ROOT				(AI_DAT_OPERATE_ROOT_MASK | AI_DAT_OPERATE_STRING_MASK | AI_DAT_OPERATE_WRITE_MASK | AI_DAT_OPERATE_READ_MASK)
#define AI_DAT_OPERATE_READ_VAR								(AI_DAT_OPERATE_VAR_MASK | AI_DAT_OPERATE_READ_MASK)
#define AI_DAT_OPERATE_WRITE_VAR							(AI_DAT_OPERATE_VAR_MASK | AI_DAT_OPERATE_WRITE_MASK)
#define AI_DAT_OPERATE_READ_WRITE_VAR						(AI_DAT_OPERATE_VAR_MASK | AI_DAT_OPERATE_WRITE_MASK | AI_DAT_OPERATE_READ_MASK)
#define AI_DAT_OPERATE_READ_STRING_VAR						(AI_DAT_OPERATE_VAR_MASK | AI_DAT_OPERATE_STRING_MASK | AI_DAT_OPERATE_READ_MASK)
#define AI_DAT_OPERATE_WRITE_STRING_VAR						(AI_DAT_OPERATE_VAR_MASK | AI_DAT_OPERATE_STRING_MASK | AI_DAT_OPERATE_WRITE_MASK)
#define AI_DAT_OPERATE_READ_WRITE_STRING_VAR				(AI_DAT_OPERATE_VAR_MASK | AI_DAT_OPERATE_STRING_MASK | AI_DAT_OPERATE_WRITE_MASK | AI_DAT_OPERATE_READ_MASK)

#define AI_DAT_LENGTH_ZERO									(0)
#define AI_DAT_LENGTH_BYTE									(1)
#define AI_DAT_LENGTH_SHORT									(2)
#define AI_DAT_LENGTH_WORD									(4)
#define AI_DAT_LENGTH_DWORD									(8)
#define AI_DAT_LENGTH_BUFFER_MAX							(256)

#define AI_DAT_CLASS_BASE									(0x20)
#define AI_DAT_CLASS_MANAGE									(0x40)
#define AI_DAT_CLASS_SERVER									(0x60)

/*------------------------- Macro Definition --------------------------------*/
/* the data tag define */
/*...........................................................................*/
/*......................... BASE ............................................*/
/*...........................................................................*/
/*...........................................................................*/
/* the unit number */
#define AI_DAT_UNIT_N_INDEX									(0)
#define AI_DAT_UNIT_N_SUB_INDEX								(1)
#define AI_DAT_UNIT_N_LENGTH								(AI_DAT_LENGTH_BYTE)
#define AI_DAT_UNIT_N_MIN									(0)
#define AI_DAT_UNIT_N_MAX									(255)
#define AI_DAT_UNIT_N_NAME									("ai unit number")
#define AI_DAT_UNIT_N_UNIT									("number")
#define AI_DAT_UNIT_N_DESCRIPTION							("")
#define AI_DAT_UNIT_N_CLASS									(AI_DAT_CLASS_BASE)
#define AI_DAT_UNIT_N_OPERATION								(AI_DAT_OPERATE_READ)
#define AI_DAT_UNIT_N_COMMENT								(                   \
    "\tOnly center unit response.\r\n"                                          \
    "\tThe request should use broadcase destination address.\r\n"               \
    "\tThe sub index must be 0.\r\n"                                            \
    "\tThe sub index maybe option in packete command.\r\n"                      \
)
#if (AI_SDK_PRE_CALCULATE == 0)
#define AI_DAT_UNIT_N_MB_S_INDEX							(0)
#define AI_DAT_UNIT_N_MB_LENGTH								((AI_DAT_UNIT_N_SUB_INDEX*((AI_DAT_UNIT_N_LENGTH+1)/2)))
#define AI_DAT_UNIT_N_MB_E_INDEX							(AI_DAT_UNIT_N_MB_S_INDEX+AI_DAT_UNIT_N_MB_LENGTH-1)
#endif
#if (AI_SDK_PRE_CALCULATE == 1)
#define AI_DAT_UNIT_N_MB_S_INDEX							(0)
#define AI_DAT_UNIT_N_MB_LENGTH								(1)
#define AI_DAT_UNIT_N_MB_E_INDEX							(0)
#endif
/*...........................................................................*/

/*...........................................................................*/
/* the unit valid */
#define AI_DAT_UNIT_VALID_INDEX								(AI_DAT_UNIT_N_INDEX +1)
#define AI_DAT_UNIT_VALID_SUB_INDEX							(1)
#define AI_DAT_UNIT_VALID_LENGTH							(AI_DAT_LENGTH_BYTE)
#define AI_DAT_UNIT_VALID_MIN								(0)
#define AI_DAT_UNIT_VALID_MAX								(1)
#define AI_DAT_UNIT_VALID_NAME								("ai unit valid")
#define AI_DAT_UNIT_VALID_UNIT								("bool")
#define AI_DAT_UNIT_VALID_DESCRIPTION						("0: invalid, 1: valid")
#define AI_DAT_UNIT_VALID_CLASS								(AI_DAT_CLASS_BASE)
#define AI_DAT_UNIT_VALID_OPERATION							(AI_DAT_OPERATE_READ)
#define AI_DAT_UNIT_INVALID									(0)
#define AI_DAT_UNIT_VALID									(1)
#define AI_DAT_UNIT_VALID_COMMENT                           (                   \
        "\tOnly center unit response.\r\n"                                      \
        "\tThe request should use broadcase destination address.\r\n"           \
)
#if (AI_SDK_PRE_CALCULATE == 0)
#define AI_DAT_UNIT_VALID_MB_S_INDEX						(AI_DAT_UNIT_N_MB_E_INDEX+1)
#define AI_DAT_UNIT_VALID_MB_LENGTH							((AI_DAT_UNIT_VALID_SUB_INDEX*((AI_DAT_UNIT_VALID_LENGTH+1)/2)))
#define AI_DAT_UNIT_VALID_MB_E_INDEX						(AI_DAT_UNIT_VALID_MB_S_INDEX+AI_DAT_UNIT_VALID_MB_LENGTH-1)
#endif
#if (AI_SDK_PRE_CALCULATE == 1)
#define AI_DAT_UNIT_VALID_MB_S_INDEX						(1)
#define AI_DAT_UNIT_VALID_MB_LENGTH							(1)
#define AI_DAT_UNIT_VALID_MB_E_INDEX						(1)
#endif
/*...........................................................................*/

/*...........................................................................*/
/* the unit addr */
#define AI_DAT_UNIT_ADDR_INDEX								(AI_DAT_UNIT_VALID_INDEX + 1)
#define AI_DAT_UNIT_ADDR_SUB_INDEX							(1)
#define AI_DAT_UNIT_ADDR_LENGTH								(AI_DAT_LENGTH_BYTE)
#define AI_DAT_UNIT_ADDR_MIN								(0x0)
#define AI_DAT_UNIT_ADDR_MAX								(0x1F)
#define AI_DAT_UNIT_ADDR_NAME								("ai unit addr")
#define AI_DAT_UNIT_ADDR_UNIT								("value")
#define AI_DAT_UNIT_ADDR_DESCRIPTION						("00-0F: io addr, 10-1F: cascade addr, FF: broadcast, FE: unknown, FD: invalid")
#define AI_DAT_UNIT_ADDR_CLASS								(AI_DAT_CLASS_BASE)
#define AI_DAT_UNIT_ADDR_OPERATION							(AI_DAT_OPERATE_READ)
#define AI_DAT_UNIT_ADDR_SUB_ALL							(AI_DAT_UNIT_ADDR_SUB_N + 1)
#define AI_DAT_UNIT_ADDR_INVALID							(0xFD)
#define AI_DAT_UNIT_ADDR_UNKNOWN							(0xFE)
#define AI_DAT_UNIT_ADDR_BROADCAST							(0xFF)
#define AI_DAT_UNIT_ADDR_COMMENT							(                   \
        "\tOnly center unit response.\r\n"                                      \
        "\tThe request should use broadcase destination address.\r\n"           \
)
#if (AI_SDK_PRE_CALCULATE == 0)
#define AI_DAT_UNIT_ADDR_MB_S_INDEX							(AI_DAT_UNIT_VALID_MB_E_INDEX+1)
#define AI_DAT_UNIT_ADDR_MB_LENGTH							((AI_DAT_UNIT_ADDR_SUB_INDEX*((AI_DAT_UNIT_ADDR_LENGTH+1)/2)))
#define AI_DAT_UNIT_ADDR_MB_E_INDEX							(AI_DAT_UNIT_ADDR_MB_S_INDEX+AI_DAT_UNIT_ADDR_MB_LENGTH-1)
#endif
#if (AI_SDK_PRE_CALCULATE == 1)
#define AI_DAT_UNIT_ADDR_MB_S_INDEX							(2)
#define AI_DAT_UNIT_ADDR_MB_LENGTH							(1)
#define AI_DAT_UNIT_ADDR_MB_E_INDEX							(2)
#endif
/*...........................................................................*/

/*...........................................................................*/
/* the unit type */
#define AI_DAT_UNIT_TYPE_INDEX								(AI_DAT_UNIT_ADDR_INDEX + 1)
#define AI_DAT_UNIT_TYPE_SUB_INDEX							(1)
#define AI_DAT_UNIT_TYPE_LENGTH								(AI_DAT_LENGTH_BYTE)
#define AI_DAT_UNIT_TYPE_MIN								(0x00)
#define AI_DAT_UNIT_TYPE_MAX								(0xFF)
#define AI_DAT_UNIT_TYPE_NAME								("ai unit type")
#define AI_DAT_UNIT_TYPE_UNIT								("bit value")
#define AI_DAT_UNIT_TYPE_DESCRIPTION						("b0: base, b1: manage, b2: server")
#define AI_DAT_UNIT_TYPE_CLASS								(AI_DAT_CLASS_BASE)
#define AI_DAT_UNIT_TYPE_OPERATION							(AI_DAT_OPERATE_READ)
#define AI_DAT_UNIT_TYPE_BASE								(0x01)
#define AI_DAT_UNIT_TYPE_NETWORK							(0x02)
#define AI_DAT_UNIT_TYPE_CONTROL							(0x04)
#define AI_DAT_UNIT_TYPE_CENTER								(0x08)
#define AI_DAT_UNIT_TYPE_MANAGE								(0x10)
#define AI_DAT_UNIT_TYPE_SERVER								(0x20)
#define AI_DAT_UNIT_TYPE_SLAVE								(0x40)
#define AI_DAT_UNIT_TYPE_BACKUP								(0x80)
#define AI_DAT_UNIT_TYPE_COMMENT							(                   \
        "\r\n"																	\
)
#if (AI_SDK_PRE_CALCULATE == 0)
#define AI_DAT_UNIT_TYPE_MB_S_INDEX							(AI_DAT_UNIT_ADDR_MB_E_INDEX+1)
#define AI_DAT_UNIT_TYPE_MB_LENGTH							((AI_DAT_UNIT_TYPE_SUB_INDEX*((AI_DAT_UNIT_TYPE_LENGTH+1)/2)))
#define AI_DAT_UNIT_TYPE_MB_E_INDEX							(AI_DAT_UNIT_TYPE_MB_S_INDEX+AI_DAT_UNIT_TYPE_MB_LENGTH-1)
#endif
#if (AI_SDK_PRE_CALCULATE == 1)
#define AI_DAT_UNIT_TYPE_MB_S_INDEX							(3)
#define AI_DAT_UNIT_TYPE_MB_LENGTH							(1)
#define AI_DAT_UNIT_TYPE_MB_E_INDEX							(3)
#endif
/*...........................................................................*/

/*...........................................................................*/
/* the unit pn */
#define AI_DAT_PN_INDEX                                    (AI_DAT_UNIT_TYPE_INDEX + 1)
#define AI_DAT_PN_SUB_INDEX                                (16)
#define AI_DAT_PN_LENGTH                                   (AI_DAT_LENGTH_BYTE)
#define AI_DAT_PN_MIN                                      (0x20)
#define AI_DAT_PN_MAX                                      (0x7E)
#define AI_DAT_PN_NAME                                     ("ai unit pn")
#define AI_DAT_PN_UNIT                                     ("string")
#define AI_DAT_PN_DESCRIPTION                              ("the unit model")
#define AI_DAT_PN_CLASS                                    (AI_DAT_CLASS_BASE)
#define AI_DAT_PN_OPERATION                                (AI_DAT_OPERATE_READ_WRITE_STRING_PRODUCT)
#define AI_DAT_PN_COMMENT                                  (                    \
        "\r\n"                                                                  \
)
#if (AI_SDK_PRE_CALCULATE == 0)
#define AI_DAT_PN_MB_S_INDEX                               (AI_DAT_UNIT_TYPE_MB_E_INDEX+1)
#define AI_DAT_PN_MB_LENGTH                                ((AI_DAT_PN_SUB_INDEX*((AI_DAT_PN_LENGTH+1)/2)))
#define AI_DAT_PN_MB_E_INDEX                               (AI_DAT_PN_MB_S_INDEX+AI_DAT_PN_MB_LENGTH-1)
#endif
#if (AI_SDK_PRE_CALCULATE == 1)
#define AI_DAT_PN_MB_S_INDEX                               (4)
#define AI_DAT_PN_MB_LENGTH                                (16)
#define AI_DAT_PN_MB_E_INDEX                               (19)
#endif
/*...........................................................................*/

/*...........................................................................*/
/* the unit sn */
#define AI_DAT_SN_INDEX                                    (AI_DAT_PN_INDEX + 1)
#define AI_DAT_SN_SUB_INDEX                                (16)
#define AI_DAT_SN_LENGTH                                   (AI_DAT_LENGTH_BYTE)
#define AI_DAT_SN_MIN                                      (0x20)
#define AI_DAT_SN_MAX                                      (0x7E)
#define AI_DAT_SN_NAME                                     ("ai unit sn")
#define AI_DAT_SN_UNIT                                     ("string")
#define AI_DAT_SN_DESCRIPTION                              ("the unit serial")
#define AI_DAT_SN_CLASS                                    (AI_DAT_CLASS_BASE)
#define AI_DAT_SN_OPERATION                                (AI_DAT_OPERATE_READ_WRITE_STRING_PRODUCT)
#define AI_DAT_SN_COMMENT                                  (                   \
        "\r\n"                                                                  \
)
#if (AI_SDK_PRE_CALCULATE == 0)
#define AI_DAT_SN_MB_S_INDEX                               (AI_DAT_PN_MB_E_INDEX+1)
#define AI_DAT_SN_MB_LENGTH                                ((AI_DAT_SN_SUB_INDEX*((AI_DAT_SN_LENGTH+1)/2)))
#define AI_DAT_SN_MB_E_INDEX                               (AI_DAT_SN_MB_S_INDEX+AI_DAT_SN_MB_LENGTH-1)
#endif
#if (AI_SDK_PRE_CALCULATE == 1)
#define AI_DAT_SN_MB_S_INDEX                               (20)
#define AI_DAT_SN_MB_LENGTH                                (16)
#define AI_DAT_SN_MB_E_INDEX                               (35)
#endif
/*...........................................................................*/

/*...........................................................................*/
/* the unit hard version */
#define AI_DAT_HARD_VER_INDEX                              (AI_DAT_SN_INDEX + 1)
#define AI_DAT_HARD_VER_SUB_INDEX                          (16)
#define AI_DAT_HARD_VER_LENGTH                             (AI_DAT_LENGTH_BYTE)
#define AI_DAT_HARD_VER_MIN                                (0x20)
#define AI_DAT_HARD_VER_MAX                                (0x7E)
#define AI_DAT_HARD_VER_NAME                               ("ai unit hardware vesion")
#define AI_DAT_HARD_VER_UNIT                               ("string")
#define AI_DAT_HARD_VER_DESCRIPTION                        ("")
#define AI_DAT_HARD_VER_CLASS                              (AI_DAT_CLASS_BASE)
#define AI_DAT_HARD_VER_OPERATION                          (AI_DAT_OPERATE_READ_WRITE_STRING_PRODUCT)
#define AI_DAT_HARD_VER_COMMENT                            (                   \
        "\r\n"                                                                  \
)
#if (AI_SDK_PRE_CALCULATE == 0)
#define AI_DAT_HARD_VER_MB_S_INDEX                         (AI_DAT_SN_MB_E_INDEX+1)
#define AI_DAT_HARD_VER_MB_LENGTH                          ((AI_DAT_HARD_VER_SUB_INDEX*((AI_DAT_HARD_VER_LENGTH+1)/2)))
#define AI_DAT_HARD_VER_MB_E_INDEX                         (AI_DAT_HARD_VER_MB_S_INDEX+AI_DAT_HARD_VER_MB_LENGTH-1)
#endif
#if (AI_SDK_PRE_CALCULATE == 1)
#define AI_DAT_HARD_VER_MB_S_INDEX                         (36)
#define AI_DAT_HARD_VER_MB_LENGTH                          (16)
#define AI_DAT_HARD_VER_MB_E_INDEX                         (51)
#endif
/*...........................................................................*/

/*...........................................................................*/
/* the unit soft version */
#define AI_DAT_SOFT_VER_INDEX                              (AI_DAT_HARD_VER_INDEX + 1)
#define AI_DAT_SOFT_VER_SUB_INDEX                          (16)
#define AI_DAT_SOFT_VER_LENGTH                             (AI_DAT_LENGTH_BYTE)
#define AI_DAT_SOFT_VER_MIN                                (0x20)
#define AI_DAT_SOFT_VER_MAX                                (0x7E)
#define AI_DAT_SOFT_VER_NAME                               ("ai unit software vesion")
#define AI_DAT_SOFT_VER_UNIT                               ("string")
#define AI_DAT_SOFT_VER_DESCRIPTION                        ("")
#define AI_DAT_SOFT_VER_CLASS                              (AI_DAT_CLASS_BASE)
#define AI_DAT_SOFT_VER_OPERATION                          (AI_DAT_OPERATE_READ_WRITE_STRING_PRODUCT)
#define AI_DAT_SOFT_VER_COMMENT                            (                   \
        "\r\n"                                                                  \
)
#if (AI_SDK_PRE_CALCULATE == 0)
#define AI_DAT_SOFT_VER_MB_S_INDEX                         (AI_DAT_HARD_VER_MB_E_INDEX+1)
#define AI_DAT_SOFT_VER_MB_LENGTH                          ((AI_DAT_SOFT_VER_SUB_INDEX*((AI_DAT_SOFT_VER_LENGTH+1)/2)))
#define AI_DAT_SOFT_VER_MB_E_INDEX                         (AI_DAT_SOFT_VER_MB_S_INDEX+AI_DAT_SOFT_VER_MB_LENGTH-1)
#endif
#if (AI_SDK_PRE_CALCULATE == 1)
#define AI_DAT_SOFT_VER_MB_S_INDEX                         (71)
#define AI_DAT_SOFT_VER_MB_LENGTH                          (16)
#define AI_DAT_SOFT_VER_MB_E_INDEX                         (86)
#endif
/*...........................................................................*/

/*------------------------- Global Macro Definition---------------------------*/
#if (AI_PRODUCT_SOFT_NAME == AI_PRODUCT_SOFT_NAME_BOOT)
#define AI_DAT_INDEX_N                                     (AI_DAT_SOFT_VER_INDEX + 1)
#endif
#if ((AI_PRODUCT_SOFT_NAME == AI_PRODUCT_SOFT_NAME_MONITOR) || (AI_PRODUCT_SOFT_NAME == AI_PRODUCT_SOFT_NAME_SERVER))
#define AI_DAT_INDEX_N                                     (AI_DAT_SOFT_VER_INDEX + 1)
#endif
#if (AI_PRODUCT_OPTION_MODBUS == 0)
#define AI_DAT_DEFINE(DAT_NAME)                            \
{                                                          \
    AI_DAT_##DAT_NAME##_INDEX,                             \
    AI_DAT_##DAT_NAME##_SUB_INDEX,                         \
    AI_DAT_##DAT_NAME##_LENGTH,                            \
    AI_DAT_##DAT_NAME##_MIN,                               \
    AI_DAT_##DAT_NAME##_MAX,                               \
    AI_DAT_##DAT_NAME##_CLASS,                             \
    AI_DAT_##DAT_NAME##_OPERATION,                         \
}
#if (AI_PRODUCT_OPTION_SDK_NAME == 1)
#undef AI_DAT_DEFINE
#define AI_DAT_DEFINE(DAT_NAME)                            \
{                                                          \
    AI_DAT_##DAT_NAME##_INDEX,                             \
    AI_DAT_##DAT_NAME##_SUB_INDEX,                         \
    AI_DAT_##DAT_NAME##_LENGTH,                            \
    AI_DAT_##DAT_NAME##_MIN,                               \
    AI_DAT_##DAT_NAME##_MAX,                               \
    AI_DAT_##DAT_NAME##_CLASS,                             \
    AI_DAT_##DAT_NAME##_OPERATION,                         \
    (UINT8_T *)(AI_DAT_##DAT_NAME##_NAME),                 \
}
#endif
#endif
#if (AI_PRODUCT_OPTION_MODBUS == 1)
#define AI_DAT_DEFINE(DAT_NAME)                            \
{                                                           \
    AI_DAT_##DAT_NAME##_INDEX,                             \
    AI_DAT_##DAT_NAME##_SUB_INDEX,                         \
    AI_DAT_##DAT_NAME##_LENGTH,                            \
    AI_DAT_##DAT_NAME##_MIN,                               \
    AI_DAT_##DAT_NAME##_MAX,                               \
    AI_DAT_##DAT_NAME##_CLASS,                             \
    AI_DAT_##DAT_NAME##_OPERATION,                         \
    AI_DAT_##DAT_NAME##_MB_S_INDEX,                        \
    AI_DAT_##DAT_NAME##_MB_E_INDEX,                        \
    AI_DAT_##DAT_NAME##_MB_LENGTH,                         \
}
#if (AI_PRODUCT_OPTION_SDK_NAME == 1)
#undef AI_DAT_DEFINE
#define AI_DAT_DEFINE(DAT_NAME)                            \
{                                                           \
    AI_DAT_##DAT_NAME##_INDEX,                             \
    AI_DAT_##DAT_NAME##_SUB_INDEX,                         \
    AI_DAT_##DAT_NAME##_LENGTH,                            \
    AI_DAT_##DAT_NAME##_MIN,                               \
    AI_DAT_##DAT_NAME##_MAX,                               \
    AI_DAT_##DAT_NAME##_CLASS,                             \
    AI_DAT_##DAT_NAME##_OPERATION,                         \
    AI_DAT_##DAT_NAME##_MB_S_INDEX,                        \
    AI_DAT_##DAT_NAME##_MB_E_INDEX,                        \
    AI_DAT_##DAT_NAME##_MB_LENGTH,                         \
    (UINT8_T *)(AI_DAT_##DAT_NAME##_NAME),                 \
}
#endif
#endif


/*------------------------- Type Definition----------------------------------*/
typedef struct ai_dat_struct {
    UINT16_T index;
    UINT32_T sub_n;
    UINT8_T sub_data_type;
    UINT64_T min;
    UINT64_T max;
    UINT8_T category;
    UINT8_T operation;
#if (AI_PRODUCT_OPTION_MODBUS == 1)
    UINT16_T modbus_s_index;
    UINT16_T modbus_e_index;
    UINT16_T modbus_length;
#endif
#if (AI_PRODUCT_OPTION_SDK_NAME == 0)
#endif
#if (AI_PRODUCT_OPTION_SDK_NAME == 1)
    UINT8_T *name;
    UINT8_T *description;
    UINT8_T *unit;
    UINT8_T *comment;
#endif
} AI_DAT, *AI_DAT_PTR;

#if (AI_PRODUCT_OPTION_MODBUS == 1)
typedef struct ai_dat_modbus_index_struct {
    UINT16_T base_internal_sub_offset;
} AI_DAT_MODBUS_INDEX, AI_DAT_MODBUS_INDEX_PTR;
#endif

/*------------------------- Function Definition -----------------------------*/
#if ((AI_PRODUCT_OPTION_MENU_ENABLE > 0) || (AI_PRODUCT_OPTION_SERVER == 1))
ERROR_T AiSdkIndexDataTypeMenuGet(UINT16_T index, UINT8_T **name, UINT8_T **description, UINT8_T **unit, UINT8_T **comment);
#endif
ERROR_T AiSdkIndexDataTypeGet(UINT16_T index, UINT64_T *sub_index, UINT8_T *sub_data_type, UINT64_T *value_min, UINT64_T *value_max, UINT8_T *category, UINT8_T *operation);
ERROR_T AiSdkIndexDataGet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length);
ERROR_T AiSdkIndexDataSet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length);
#if (AI_PRODUCT_OPTION_MODBUS == 1)
ERROR_T AiSdkInternalIndexToModbusIndex(UINT16_T internal_index, UINT64_T internal_sub_index, UINT16_T *modbus_index, UINT16_T *modbus_number);
ERROR_T AiSdkModbusIndexToInternalIndex(UINT16_T modbus_index, UINT16_T *internal_index, UINT16_T *internal_sub_index, UINT16_T *internal_sub_offset);
ERROR_T AiSdkModbusIndexGet(UINT16_T modbus_index, UINT16_T *modbus_s_index, UINT16_T *modbus_e_index);
ERROR_T AiSdkModbusAddressGet(UINT16_T internal_index, UINT8_T sdk_category, UINT8_T source_address, UINT8_T dest_address, UINT8_T *internal_source_address, UINT8_T *internal_dest_address);
ERROR_T AiSdkModbusIndexDataTypeGet(UINT16_T index, UINT64_T *sub_index, UINT8_T *sub_data_type, UINT64_T *value_min, UINT64_T *value_max, UINT8_T *category, UINT8_T *operation);
ERROR_T AiSdkModbusIndexDataGet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length);
ERROR_T AiSdkModbusIndexDataSet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length);
#endif
AI_DAT_PTR BucSdkGetAiDat(void);

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
