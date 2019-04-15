/**
 ******************************************************************************
 * file    ai_base_lnet_modbus_public.h
 * author  sunlingge
 * version V100
 * brief   the ai base lnet modbus public.
 ******************************************************************************
 */
#ifndef AI_BASE_LNET_PACKET_PUBLIC_H
#define AI_BASE_LNET_PACKET_PUBLIC_H

#ifdef __cplusplus
extern "C" {
#endif

#if (AI_PRODUCT_OPTION_MODBUS == 1)
/*------------------------- Include File ------------------------------------*/

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/
typedef struct ai_base_lnet_modbus_public_struct {
    UINT16_T reg_start_address;
	UINT16_T reg_number;
	UINT16_T reg_value;
} AI_BASE_LNET_PACKET_PUBLIC, *AI_BASE_LNET_PACKET_PUBLIC_PTR;

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
    AI_BASE_LNET_PACKET_PUBLIC packet_public;
    AI_BASE_LNET_PACKET_FIELD_DECODE field[AI_BASE_LNET_PACKET_FIELD_MAX];
} AI_BASE_LNET_PACKET_DECODE, *AI_BASE_LNET_PACKET_DECODE_PTR;

typedef struct ai_base_lnet_packet_struct {
    UINT32_T start;
    UINT32_T length;
    UINT8_T buffer[AI_BASE_LNET_PACKET_BUFFER_LENGTH];
} AI_BASE_LNET_PACKET, *AI_BASE_LNET_PACKET_PTR;

/*------------------------- Function Definition -----------------------------*/
ERROR_T AiBaseLnetModbusInit(AI_BASE_LNET_PACKET_PTR packet);
ERROR_T AiBaseLnetModbusRx(AI_BASE_LNET_LINK_PTR link, AI_BASE_LNET_PACKET_PTR packet);
ERROR_T AiBaseLnetModbusRxEscape(AI_BASE_LNET_LINK_PTR link, AI_BASE_LNET_PACKET_PTR packet);
ERROR_T AiBaseLnetModbusDataGet(AI_BASE_LNET_PACKET_PTR packet, UINT32_T start, UINT32_T length, UINT64_T *value);
ERROR_T AiBaseLnetModbusCommandRead(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, UINT32_T inteface);
ERROR_T AiBaseLnetModbusCommandWrite(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, UINT32_T inteface);
ERROR_T AiBaseLnetModbusCommandReadResponse(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, UINT32_T inteface);
ERROR_T AiBaseLnetModbusCommandWriteResponse(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, UINT32_T inteface);
ERROR_T AiBaseLnetModbusCommandReadSend(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, UINT32_T inteface);
ERROR_T AiBaseLnetModbusCommandWriteSend(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, UINT32_T inteface);
ERROR_T AiBaseLnetModbusDataSet(AI_BASE_LNET_PACKET_PTR packet, UINT32_T start, UINT32_T length, UINT64_T value);
ERROR_T AiBaseLnetModbusBufferSet(AI_BASE_LNET_PACKET_PTR packet, UINT32_T start, UINT32_T length, UINT8_T *buffer);
ERROR_T AiBaseLnetModbusTxEscape(AI_BASE_LNET_LINK_PTR link, AI_BASE_LNET_PACKET_PTR packet);
ERROR_T AiBaseLnetModbusTx(AI_BASE_LNET_LINK_PTR link, AI_BASE_LNET_PACKET_PTR packet);
ERROR_T AiBaseLnetModbusTxHeadTail(AI_BASE_LNET_LINK_PTR link, AI_BASE_LNET_PACKET_PTR packet);

#endif

#ifdef __cplusplus
}
#endif

#endif
/*------------------------- End ---------------------------------------------*/
