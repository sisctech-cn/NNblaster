/**
 ******************************************************************************
 * file    ai_base_tnet_packet.h
 * author  sunlingge
 * version V100
 * brief   the ai base lnet packet driver.
 ******************************************************************************
 */
#ifndef AI_BASE_LNET_PACKET_H
#define AI_BASE_LNET_PACKET_H

#ifdef __cplusplus
extern "C" {
#endif

#if (AI_PRODUCT_OPTION_LNET_PACKET == 1)
/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/
typedef struct ai_base_lnet_packet_field_read_struct {
    UINT16_T index;
    UINT64_T sub_index;
    UINT32_T data_number;
} AI_BASE_LNET_PACKET_FIELD_READ, *AI_BASE_LNET_PACKET_FIELD_READ_PTR;

typedef struct ai_base_lnet_packet_field_write_struct {
    UINT16_T index;
    UINT64_T sub_index;
    UINT32_T data_number;
    UINT64_T value;
	UINT8_T value_valid;
} AI_BASE_LNET_PACKET_FIELD_WRITE, *AI_BASE_LNET_PACKET_FIELD_WRITE_PTR;

typedef struct ai_base_lnet_packet_field_decode_struct {
    UINT8_T valid;
    UINT8_T field_map;
    UINT16_T index;
    UINT64_T sub_index;
    UINT8_T data_type_size;
    UINT32_T data_number;
    UINT64_T value;
	UINT8_T value_valid;
} AI_BASE_LNET_PACKET_FIELD_DECODE, *AI_BASE_LNET_PACKET_FIELD_DECODE_PTR;

typedef struct ai_base_lnet_packet_decode_struct {
    UINT8_T sequence;
    UINT8_T source_addr;
    UINT8_T dest_addr;
    UINT8_T command;
    UINT8_T result;
	UINT8_T from;
    AI_BASE_LNET_PACKET_FIELD_DECODE field[AI_BASE_LNET_PACKET_FIELD_MAX];
} AI_BASE_LNET_PACKET_DECODE, *AI_BASE_LNET_PACKET_DECODE_PTR;

typedef struct ai_base_lnet_packet_struct {
    UINT32_T start;
    UINT32_T length;
    UINT8_T buffer[AI_BASE_LNET_PACKET_BUFFER_LENGTH];
} AI_BASE_LNET_PACKET, *AI_BASE_LNET_PACKET_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiBaseLnetPacketInit(AI_BASE_LNET_PACKET_PTR packet);
ERROR_T AiBaseLnetPacketRx(AI_BASE_LNET_LINK_PTR link, AI_BASE_LNET_PACKET_PTR packet);
ERROR_T AiBaseLnetPacketRxEscape(AI_BASE_LNET_LINK_PTR link, AI_BASE_LNET_PACKET_PTR packet);
ERROR_T AiBaseLnetPacketRxHeadTail(AI_BASE_LNET_LINK_PTR link, AI_BASE_LNET_PACKET_PTR packet);
ERROR_T AiBaseLnetPacketDataGet(AI_BASE_LNET_PACKET_PTR packet, UINT32_T start, UINT32_T length, UINT64_T *value);
ERROR_T AiBaseLnetPacketDecode(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode);
ERROR_T AiBaseLnetPacketCommand(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, VOID_T inteface);
ERROR_T AiBaseLnetPacketCommandRead(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, VOID_T inteface);
ERROR_T AiBaseLnetPacketCommandWrite(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, VOID_T inteface);
ERROR_T AiBaseLnetPacketCommandReadResponse(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, VOID_T inteface);
ERROR_T AiBaseLnetPacketCommandWriteResponse(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, VOID_T inteface);
ERROR_T AiBaseLnetPacketCommandReadSend(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, VOID_T inteface);
ERROR_T AiBaseLnetPacketCommandWriteSend(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, VOID_T inteface);
ERROR_T AiBaseLnetPacketDataSet(AI_BASE_LNET_PACKET_PTR packet, UINT32_T start, UINT32_T length, UINT64_T value);
ERROR_T AiBaseLnetPacketBufferSet(AI_BASE_LNET_PACKET_PTR packet, UINT32_T start, UINT32_T length, UINT8_T *buffer);
ERROR_T AiBaseLnetPacketEncode(AI_BASE_LNET_PACKET_DECODE_PTR decode, ERROR_T retcode, AI_BASE_LNET_PACKET_PTR packet);
ERROR_T AiBaseLnetPacketTxHeadTail(AI_BASE_LNET_LINK_PTR link, AI_BASE_LNET_PACKET_PTR packet);
ERROR_T AiBaseLnetPacketTxEscape(AI_BASE_LNET_LINK_PTR link, AI_BASE_LNET_PACKET_PTR packet);
ERROR_T AiBaseLnetPacketTx(AI_BASE_LNET_LINK_PTR link, AI_BASE_LNET_PACKET_PTR packet);
#endif

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
