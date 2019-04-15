/**
 ******************************************************************************
 * file    ai_base_lnet_modbus.h
 * author  sunlingge
 * version V100
 * brief   the ai base lnet modbus driver.
 ******************************************************************************
 */
#ifndef AI_BASE_LNET_PACKET_MASTER_H
#define AI_BASE_LNET_PACKET_MASTER_H

#ifdef __cplusplus
extern "C" {
#endif

#if (AI_PRODUCT_OPTION_MODBUS == 1)
#if (AI_PRODUCT_OPTION_MODBUS_MASTER == 1)
/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiBaseLnetModbusMasterRxHeadTail(AI_BASE_LNET_LINK_PTR link, AI_BASE_LNET_PACKET_PTR packet);
ERROR_T AiBaseLnetModbusMasterDecode(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode);
ERROR_T AiBaseLnetModbusMasterCommand(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, UINT32_T inteface);
ERROR_T AiBaseLnetModbusMasterEncode(AI_BASE_LNET_PACKET_DECODE_PTR decode, ERROR_T retcode, AI_BASE_LNET_PACKET_PTR packet);
#endif
#endif

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
