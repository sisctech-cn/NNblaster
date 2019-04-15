/**
 ******************************************************************************
 * file    ai_base_lnet_modbus_public.c
 * author  sunlingge
 * version V100
 * brief   the ai base lnet modbus public.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"

#if (AI_PRODUCT_OPTION_MODBUS == 1)
/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/
ERROR_T AiServerLogPacket(UINT8_T direction, UINT8_T *packet_buffer, UINT32_T packet_length);

/*------------------------- Variable Definition------------------------------*/
const UINT8_T g_ai_base_lnet_modbus_crc_hi[256] = {
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 
    0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
    0x00, 0xC1, 0x81, 0x40
};

const UINT8_T g_ai_base_lnet_modbus_crc_low[256] = {
    0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
    0x05, 0xC5, 0xC4, 0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E,
    0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9,
    0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D, 0x1C, 0xDC,
    0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
    0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
    0x36, 0xF6, 0xF7, 0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D,
    0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 
    0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E, 0x2F, 0xEF,
    0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
    0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
    0x63, 0xA3, 0xA2, 0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4,
    0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 
    0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B, 0x7A, 0xBA,
    0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
    0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
    0x50, 0x90, 0x91, 0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97,
    0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E,
    0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48, 0x49, 0x89,
    0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
    0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
    0x41, 0x81, 0x80, 0x40
};

/*------------------------- Function Definition -----------------------------*/
/**
 * brief    Ai base lnet modbus master init.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetModbusInit(AI_BASE_LNET_PACKET_PTR packet)
{
    memset((char *) packet, 0, sizeof(AI_BASE_LNET_PACKET));
    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet modbus rx.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetModbusRx(AI_BASE_LNET_LINK_PTR link, AI_BASE_LNET_PACKET_PTR packet)
{
    UINT32_T start = 0;
    UINT32_T end = 0;

    /* rx check */
    if (AiBaseLentRxCheck(link) != AI_ERROR_SUCCESS) {
    	packet->length = 0;
    	packet->start = 0;
        return AI_ERROR_GENERAL;
    }

    /* get the valid buffer range */
    start = (link->rx_start & AI_BASE_LNET_LINK_BUFFER_MASK);
    end = (link->rx_end & AI_BASE_LNET_LINK_BUFFER_MASK);

    /* copy the packet into one copy packet buffer */
    packet->start = AI_BASE_LENT_PACKET_RX_START;
    packet->length = ((end + AI_BASE_LNET_LINK_BUFFER_LENGTH - start) & AI_BASE_LNET_LINK_BUFFER_MASK);
    if (AiBaseLnetRxBufferGet(link, end, start, packet->length, packet->buffer+AI_BASE_LENT_PACKET_RX_START) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet modbus check.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetModbusRxEscape(AI_BASE_LNET_LINK_PTR link, AI_BASE_LNET_PACKET_PTR packet)
{
    UINT32_T index;

    if (link->decode == AI_BASE_LNET_ESCAPE_HLUART) {
        for(index = 0; index < (packet->length / 2); index++) {
            packet->buffer[index] = packet->buffer[index*2+1+packet->start];
        }
        packet->start = 0;
        packet->length = (packet->length / 2);
    } else {
        for(index = 0; index < packet->length; index++) {
            packet->buffer[index] = packet->buffer[index+packet->start];
        }
        packet->start = 0;
    }
    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet modbus buffer data get.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetModbusDataGet(AI_BASE_LNET_PACKET_PTR packet, UINT32_T start, UINT32_T length, UINT64_T *value)
{
    UINT32_T index;
    UINT32_T buffer_index;
    UINT64_T temp = 0;

    buffer_index = start;
    for (index = 0; index < length; index++) {
        if (buffer_index < packet->start)
            return AI_ERROR_GENERAL;
        if (buffer_index >= (packet->start + packet->length))
            return AI_ERROR_GENERAL;
        temp = (temp << 8) + packet->buffer[buffer_index];
        buffer_index = (buffer_index + 1);
    }

    *value = temp;
    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet modbus master command read.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetModbusCommandRead(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, UINT32_T inteface)
{
    UINT32_T field_index;
    ERROR_T result;
    UINT64_T value;
    AI_BASE_LNET_PACKET_FIELD_DECODE_PTR field;

    for (field_index = 0; field_index < AI_BASE_LNET_PACKET_FIELD_MAX; field_index++) {
        field = &decode->field[field_index];

        if (field->valid != AI_BASE_LNET_PACKET_FIELD_VALID)
            break;

		result = AiSdkModbusIndexDataGet(decode->source_addr, decode->dest_addr, field->index, field->sub_index, &value, (UINT32_T)(0));
        field->value_valid = AI_BASE_LNET_PACKET_DATA_VALUE_VALID;
        if (result != AI_ERROR_SUCCESS)
            return result;

        field->value = value;
    }

    return AI_ERROR_SUCCESS;
}

#if (AI_BASE_LENT_PACKET_OPTIMIZE == 1)
/**
 * brief    Ai base lnet modbus command write.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetModbusCommandWrite(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, UINT32_T inteface)
{
    UINT32_T field_index;
    ERROR_T result;
    AI_BASE_LNET_PACKET_FIELD_DECODE_PTR field;
    UINT16_T modbus_s_index;
    UINT16_T modbus_e_index;
    UINT8_T single_flag = 0;
    UINT16_T internal_index;
    UINT16_T internal_sub_index;
    UINT16_T internal_sub_offset;
    UINT16_T internal_index_max;
    UINT16_T internal_sub_index_max;
    UINT16_T internal_sub_offset_max;
    UINT64_T internal_value;
    UINT32_T internal_length;
    UINT64_T sdk_sub_index;
    UINT8_T sdk_sub_data_type;
    UINT64_T sdk_value_min;
    UINT64_T sdk_value_max;
    UINT8_T sdk_category;
    UINT8_T sdk_operation;
    UINT8_T internal_source_address;
    UINT8_T internal_dest_address;
    UINT32_T base_internal_sub_offset;
    UINT8_T internal_value_buffer[AI_DAT_LENGTH_BUFFER_MAX];

    field = &decode->field[0];
    if (field->valid != AI_BASE_LNET_PACKET_FIELD_VALID)
        return AI_ERROR_GENERAL;

    result = AiSdkModbusIndexGet(field->index, &modbus_s_index, &modbus_e_index);
    if (result != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    result = AiSdkModbusIndexToInternalIndex(field->index, &internal_index, &internal_sub_index, &internal_sub_offset);
    if (result != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    if (AiSdkIndexDataTypeGet(internal_index, &sdk_sub_index, &sdk_sub_data_type, &sdk_value_min, &sdk_value_max, &sdk_category, &sdk_operation) != AI_ERROR_SUCCESS) {
        return AI_ERROR_GENERAL;
    }

    internal_length = (UINT32_T)(sdk_sub_index * sdk_sub_data_type);
    base_internal_sub_offset = (internal_sub_index * sdk_sub_data_type)+internal_sub_offset;

    if (internal_length <= AI_DAT_LENGTH_DWORD) {
        single_flag = 1;
    }

    if (single_flag == 0) {
        if (sdk_sub_data_type >=2) {
            single_flag = 1;
        }
    }

    if (single_flag == 0) {
        for (field_index = 0; field_index < AI_BASE_LNET_PACKET_FIELD_MAX; field_index++) {
            field = &decode->field[field_index];
            if (field->valid != AI_BASE_LNET_PACKET_FIELD_VALID)
                break;
            if ((field->index >= modbus_s_index) && (field->index <= modbus_e_index)) {
                if (field_index > 0) {
                    if (decode->field[field_index].index == (decode->field[field_index-1].index + 1)) {
                        continue;
                    }
                }
            }
            single_flag = 1;
        }
    }

    if (single_flag == 0) {
        if ((sdk_operation & AI_DAT_OPERATE_VAR_MASK) == 0) {
            if ((sdk_operation & AI_DAT_OPERATE_STRING_MASK) > 0) {
                single_flag = 0;
            } else {
                single_flag = 1;
            }
        } else {
            single_flag = 1;
        }
    }

    if (single_flag == 1) {
        for (field_index = 0; field_index < AI_BASE_LNET_PACKET_FIELD_MAX; field_index++) {
            field = &decode->field[field_index];

            if (field->valid != AI_BASE_LNET_PACKET_FIELD_VALID)
                break;

            result = AiSdkModbusIndexDataSet(decode->source_addr, decode->dest_addr, field->index, field->sub_index, &(field->value), (UINT32_T)(0));
            if (result != AI_ERROR_SUCCESS)
                return result;

        }
    } else {
        if (AiSdkModbusAddressGet(internal_index, sdk_category, decode->source_addr, decode->dest_addr, &internal_source_address, &internal_dest_address) != AI_ERROR_SUCCESS) {
            return AI_ERROR_GENERAL;
        }

        result = AiSdkIndexDataGet(internal_source_address, internal_dest_address, internal_index, internal_sub_index, &internal_value, internal_length);
        if (result != AI_ERROR_SUCCESS)
            return AI_ERROR_GENERAL;

        memcpy((char *)internal_value_buffer, (char *)((UINT32_T)internal_value), internal_length);
        for (field_index = 0; field_index < AI_BASE_LNET_PACKET_FIELD_MAX; field_index++) {
            field = &decode->field[field_index];
            if (field->valid != AI_BASE_LNET_PACKET_FIELD_VALID) {
                result = AiSdkModbusIndexToInternalIndex(field->index, &internal_index_max, &internal_sub_index_max, &internal_sub_offset_max);
                if (result != AI_ERROR_SUCCESS)
                    return AI_ERROR_GENERAL;
                break;
            }
            internal_value_buffer[base_internal_sub_offset+field_index] = (UINT8_T)((field->value) & (0xFF));
        }
        internal_value = (UINT64_T)((UINT32_T)internal_value_buffer);
        result = AiSdkIndexDataSet(internal_source_address, internal_dest_address, internal_index, internal_sub_index_max, (UINT64_T *)(&(internal_value)), internal_length);
        if (result != AI_ERROR_SUCCESS) {
            return AI_ERROR_GENERAL;
        }
    }

    return AI_ERROR_SUCCESS;
}
#endif

#if (AI_BASE_LENT_PACKET_OPTIMIZE == 0)
/**
 * brief    Ai base lnet modbus command write.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetModbusCommandWrite(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, UINT32_T inteface)
{
    UINT32_T field_index;
    ERROR_T result;
    AI_BASE_LNET_PACKET_FIELD_DECODE_PTR field;

    for (field_index = 0; field_index < AI_BASE_LNET_PACKET_FIELD_MAX; field_index++) {
        field = &decode->field[field_index];

        if (field->valid != AI_BASE_LNET_PACKET_FIELD_VALID)
            break;

		result = AiSdkModbusIndexDataSet(decode->source_addr, decode->dest_addr, field->index, field->sub_index, &(field->value), (UINT32_T)(0));
        if (result != AI_ERROR_SUCCESS)
            return result;

    }

    return AI_ERROR_SUCCESS;
}
#endif

/**
 * brief    Ai base lnet modbus command write.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetModbusCommandReadResponse(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, UINT32_T inteface)
{
    UINT32_T field_index;
    ERROR_T result;
    AI_BASE_LNET_PACKET_FIELD_DECODE_PTR field;

    for (field_index = 0; field_index < AI_BASE_LNET_PACKET_FIELD_MAX; field_index++) {
        field = &decode->field[field_index];

        if (field->valid != AI_BASE_LNET_PACKET_FIELD_VALID)
            break;

		result = AiSdkModbusIndexDataSet(decode->source_addr, decode->dest_addr, field->index, field->sub_index, &(field->value), (UINT32_T)(0));
        if (result != AI_ERROR_SUCCESS)
            return result;

    }

    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet modbus command write.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetModbusCommandWriteResponse(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, UINT32_T inteface)
{
    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet modbus command read.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetModbusCommandReadSend(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, UINT32_T inteface)
{
    UINT32_T field_index;
    AI_BASE_LNET_PACKET_FIELD_DECODE_PTR field;

    for (field_index = 0; field_index < AI_BASE_LNET_PACKET_FIELD_MAX; field_index++) {
        field = &decode->field[field_index];

        if (field->valid != AI_BASE_LNET_PACKET_FIELD_VALID)
            break;

        field->value = 0;
    }

    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet modbus command write.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetModbusCommandWriteSend(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, UINT32_T inteface)
{
    UINT32_T field_index;
    ERROR_T result;
	UINT64_T value = 0;
    AI_BASE_LNET_PACKET_FIELD_DECODE_PTR field;

    for (field_index = 0; field_index < AI_BASE_LNET_PACKET_FIELD_MAX; field_index++) {
        field = &decode->field[field_index];

        if (field->valid != AI_BASE_LNET_PACKET_FIELD_VALID)
            break;

       if (field->value_valid != AI_BASE_LNET_PACKET_DATA_VALUE_VALID) {
			result = AiSdkModbusIndexDataGet(decode->source_addr, decode->dest_addr, field->index, field->sub_index, &value, (UINT32_T)(0));
        } else {
            value = field->value;
            result = AI_ERROR_SUCCESS;
        }
        if (result != AI_ERROR_SUCCESS)
            return result;

        field->value = value;
    }

    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet modbus data get.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetModbusDataSet(AI_BASE_LNET_PACKET_PTR packet, UINT32_T start, UINT32_T length, UINT64_T value)
{
    UINT32_T index;
    UINT32_T buffer_index;
    UINT64_T temp = value;

    buffer_index = start;
    for (index = 0; index < length; index++) {
        if (buffer_index < packet->start)
            return AI_ERROR_GENERAL;
        if (buffer_index >= (packet->start + packet->length))
            return AI_ERROR_GENERAL;
        packet->buffer[buffer_index] = (UINT8_T) ((temp >> ((length - index - 1) * 8)) & 0x00000000000000FF);
        buffer_index = (buffer_index + 1);
    }

    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet modbus buffer data set.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetModbusBufferSet(AI_BASE_LNET_PACKET_PTR packet, UINT32_T start, UINT32_T length, UINT8_T *buffer)
{
    UINT32_T index;
    UINT32_T buffer_index;

    buffer_index = start;
    for (index = 0; index < length; index++) {
        if (buffer_index < packet->start)
            return AI_ERROR_GENERAL;
        if (buffer_index >= (packet->start + packet->length))
            return AI_ERROR_GENERAL;
        packet->buffer[buffer_index] = buffer[index];
        buffer_index = (buffer_index + 1);
    }

    return AI_ERROR_SUCCESS;
}


/**
 * brief    Ai base lnet modbus tx escape.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetModbusTxEscape(AI_BASE_LNET_LINK_PTR link, AI_BASE_LNET_PACKET_PTR packet)
{
    UINT32_T index;

    if (link->decode == AI_BASE_LNET_ESCAPE_HLUART) {
        for(index = 0; index < packet->length; index++) {
            packet->buffer[AI_BASE_LNET_PACKET_TX_START+index*2] = AI_BASE_LNET_ESCAPE_HLUART_FLAG_MASK;
            packet->buffer[AI_BASE_LNET_PACKET_TX_START+index*2+1] = packet->buffer[packet->start+index];
        }
        packet->start = AI_BASE_LNET_PACKET_TX_START;
        packet->length = (packet->length * 2);
    }
    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet modbus tx.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
#if (AI_PRODUCT_SOFT_NAME == AI_PRODUCT_SOFT_NAME_SERVER)
ERROR_T AiServerSleep(UINT32_T timeout);
#endif
ERROR_T AiBaseLnetModbusTx(AI_BASE_LNET_LINK_PTR link, AI_BASE_LNET_PACKET_PTR packet)
{
#if (AI_PRODUCT_SOFT_NAME == AI_PRODUCT_SOFT_NAME_SERVER)
	while(1) {
		if (AiBaseLentRxCheck(link) != AI_ERROR_SUCCESS) {
			break;
		} else {
			AiServerSleep(1);
		}
	}
#endif
    if (AiBaseLnetTxBufferSet(link, packet->buffer+packet->start, packet->length) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet modbus headtail.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetModbusTxHeadTail(AI_BASE_LNET_LINK_PTR link, AI_BASE_LNET_PACKET_PTR packet)
{
    UINT64_T data;
    UINT32_T start = 0;
    UINT32_T end = 0;
    UINT32_T i;
    UINT16_T calculate_parity;
	UINT8_T crc_hi;
    UINT8_T crc_low;
    UINT8_T index;

    /* get the valid buffer range */
    start = packet->start - AI_BASE_LNET_PACKET_HEAD_LENGTH;
    end = packet->start + packet->length;
    packet->start = start;
    packet->length = packet->length + AI_BASE_LNET_PACKET_HEAD_TAIL_LENGTH;

    /* calculate the parity */
	calculate_parity = 0;
	crc_hi = 0xFF;
	crc_low = 0xFF;
    for (i = 0; i < (packet->length-AI_BASE_LNET_PACKET_HEAD_TAIL_LENGTH); i++) {
        if (AiBaseLnetModbusDataGet(packet, (start + AI_BASE_LNET_PACKET_HEAD_LENGTH + i), 1, &data) != AI_ERROR_SUCCESS)
            return AI_ERROR_GENERAL;
		index = (crc_low ^ ((UINT8_T)data));
        crc_low = (UINT8_T)(crc_hi ^ (g_ai_base_lnet_modbus_crc_hi[index]));
        crc_hi = g_ai_base_lnet_modbus_crc_low[index];
    }
	calculate_parity = (UINT16_T)((crc_hi << 8) | crc_low);

    /* set the parity */
    data = calculate_parity;
    if (AiBaseLnetModbusDataSet(packet, end, AI_BASE_LNET_PACKET_CHECKSUM_LENGTH, data) != AI_ERROR_SUCCESS) {
        return AI_ERROR_GENERAL;
    }

#if (AI_PRODUCT_OPTION_LOG_PACKET == 1)
	AiServerLogPacket(1, ((packet->buffer)+(start)), (packet->length));
#endif

    return AI_ERROR_SUCCESS;
}

#endif
/*------------------------- End ---------------------------------------------*/
