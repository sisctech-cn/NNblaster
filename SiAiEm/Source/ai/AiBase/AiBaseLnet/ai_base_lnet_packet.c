/**
 ******************************************************************************
 * file    ai_base_tnet_packet.c
 * author  sunlingge
 * version V100
 * brief   the ai base lnet packet driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"

#if (AI_PRODUCT_OPTION_LNET_PACKET == 1)
/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/
ERROR_T AiSdkIndexDataTypeGet(UINT16_T index, UINT64_T *sub_index, UINT8_T *sub_data_type, UINT64_T *value_min, UINT64_T *value_max, UINT8_T *category, UINT8_T *operation);
ERROR_T AiSdkIndexDataGet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length);
ERROR_T AiSdkIndexDataSet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length);
ERROR_T AiServerLogPacket(UINT8_T direction, UINT8_T *packet_buffer, UINT32_T packet_length);

/*------------------------- Variable Definition------------------------------*/

/*------------------------- Function Definition -----------------------------*/
/**
 * brief    Ai base lnet packet init.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetPacketInit(AI_BASE_LNET_PACKET_PTR packet)
{
    memset((char *) packet, 0, sizeof(AI_BASE_LNET_PACKET));
    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet packet rx.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetPacketRx(AI_BASE_LNET_LINK_PTR link, AI_BASE_LNET_PACKET_PTR packet)
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
 * brief    Ai base lnet packet check.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetPacketRxEscape(AI_BASE_LNET_LINK_PTR link, AI_BASE_LNET_PACKET_PTR packet)
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
 * brief    Ai base lnet packet check.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetPacketRxHeadTail(AI_BASE_LNET_LINK_PTR link, AI_BASE_LNET_PACKET_PTR packet)
{
    UINT64_T data;
    UINT32_T start = 0;
    UINT32_T end = 0;
    UINT32_T link_length;
    UINT64_T packet_length;
    UINT32_T i;
    UINT16_T calculate_parity;
    UINT64_T packet_parity;

    /* get the valid buffer range */
    start = packet->start;
    end = packet->start + packet->length;

    /* check the stx of packet */
    while (1) {
        if (AiBaseLnetPacketDataGet(packet, start, AI_BASE_LNET_PACKET_STX_LENGTH, &data) != AI_ERROR_SUCCESS) {
            return AI_ERROR_GENERAL;
        }
        if (data == AI_BASE_LNET_PACKET_STX)
            break;
        AiBaseLnetRxStartSet(link, AI_BASE_LNET_PACKET_STX_LENGTH);
        start = (start + AI_BASE_LNET_PACKET_STX_LENGTH);
    }

    /* get and check the link length */
    link_length = (end - start);
    if (link_length < AI_BASE_LNET_PACKET_LINK_LENGTH_MIN)
        return AI_ERROR_GENERAL;

    /* get the packete length */
    if (AiBaseLnetPacketDataGet(packet, (start + AI_BASE_LNET_PACKET_STX_LENGTH), AI_BASE_LNET_PACKET_LENGTH_LENGTH, &packet_length) != AI_ERROR_SUCCESS) {
        return AI_ERROR_GENERAL;
    }

    /* check the packet length value */
    if ((packet_length < AI_BASE_LNET_PACKET_LENGTH_MIN) || (packet_length > AI_BASE_LNET_PACKET_LENGTH_MAX)) {
        AiBaseLnetRxStartSet(link, AI_BASE_LNET_PACKET_STX_LENGTH);
        return AI_ERROR_GENERAL;
    }
    if (link_length < (packet_length + AI_BASE_LNET_PACKET_HEAD_TAIL_LENGTH)) {
        return AI_ERROR_GENERAL;
    }

    /* get the parity */
    if (AiBaseLnetPacketDataGet(packet, (start + AI_BASE_LNET_PACKET_HEAD_LENGTH + (UINT32_T)packet_length), AI_BASE_LNET_PACKET_CHECKSUM_LENGTH, &packet_parity) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* calculate the parity */
    calculate_parity = 0;
    for (i = 0; i < packet_length; i++) {
        if (AiBaseLnetPacketDataGet(packet, (start + AI_BASE_LNET_PACKET_HEAD_LENGTH + i), 1, &data) != AI_ERROR_SUCCESS)
            return AI_ERROR_GENERAL;
        calculate_parity = calculate_parity + (UINT16_T)data;
    }

    /* check the parity */
    if (calculate_parity != packet_parity) {
        AiBaseLnetRxStartSet(link, AI_BASE_LNET_PACKET_STX_LENGTH);
        return AI_ERROR_GENERAL;
    }

    /* release the link rx_buffer, no etx */
    AiBaseLnetRxStartSet(link, (UINT32_T)(packet_length + AI_BASE_LNET_PACKET_HEAD_LENGTH + AI_BASE_LNET_PACKET_CHECKSUM_LENGTH));

    /* set the packet start end */
    packet->start = (start + AI_BASE_LNET_PACKET_HEAD_LENGTH);
    packet->length = (UINT32_T)packet_length;

#if (AI_PRODUCT_OPTION_LOG_PACKET == 1)
	AiServerLogPacket(0, ((packet->buffer)+(start)), (UINT32_T)(packet_length + AI_BASE_LNET_PACKET_HEAD_LENGTH + AI_BASE_LNET_PACKET_CHECKSUM_LENGTH));
#endif

    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet packet buffer data get.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetPacketDataGet(AI_BASE_LNET_PACKET_PTR packet, UINT32_T start, UINT32_T length, UINT64_T *value)
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
 * brief    Ai base lnet packet decode.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetPacketDecode(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode)
{
    UINT32_T start;
    UINT32_T end;
    UINT64_T data;
    UINT32_T field_index;
    UINT8_T field_map;
    UINT8_T index_length;
    UINT16_T index;
    UINT8_T sub_index_valid;
    UINT8_T sub_index_length;
    UINT64_T sub_index;
    UINT8_T data_type_size_valid;
    UINT8_T data_type_size;
    UINT8_T data_number_valid;
    UINT8_T data_number_length;
    UINT32_T data_number;
    UINT8_T data_value_valid;
    UINT32_T data_value_length;
    UINT64_T data_value;

    memset((char *) decode, 0, sizeof(AI_BASE_LNET_PACKET_DECODE));

    start = packet->start;
    end = start + packet->length;

    /* get sequence */
    if (AiBaseLnetPacketDataGet(packet, start, AI_BASE_LNET_PACKET_SEQUENCE_LENGTH, &data) != AI_ERROR_SUCCESS) {
        return AI_ERROR_GENERAL;
    }
    decode->sequence = (UINT8_T)data;
    start = (start + AI_BASE_LNET_PACKET_SEQUENCE_LENGTH);

    /* get source address */
    if (AiBaseLnetPacketDataGet(packet, start, AI_BASE_LNET_PACKET_SOURCE_ADDR_LENGTH, &data) != AI_ERROR_SUCCESS) {
        return AI_ERROR_GENERAL;
    }
    decode->source_addr = (UINT8_T)data;
    start = (start + AI_BASE_LNET_PACKET_SOURCE_ADDR_LENGTH);

    /* get dest address */
    if (AiBaseLnetPacketDataGet(packet, start, AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH, &data) != AI_ERROR_SUCCESS) {
        return AI_ERROR_GENERAL;
    }
    decode->dest_addr = (UINT8_T)data;
    start = (start + AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH);

    /* get command */
    if (AiBaseLnetPacketDataGet(packet, start, AI_BASE_LNET_PACKET_COMMAND_LENGTH, &data) != AI_ERROR_SUCCESS) {
        return AI_ERROR_GENERAL;
    }
    decode->command = (UINT8_T)data;
    decode->from = AI_BASE_LNET_PACKET_COMMAND_FLAG_RECV;
    start = (start + AI_BASE_LNET_PACKET_COMMAND_LENGTH);

    /* check command */
    data = ((decode->command & AI_BASE_LNET_PACKET_COMMAND_MASK) >> AI_BASE_LNET_PACKET_COMMAND_SHIFT);
    if ((data != AI_BASE_LNET_PACKET_COMMAND_READ_REQUEST) && (data != AI_BASE_LNET_PACKET_COMMAND_WRITE_REQUEST) &&
            (data != AI_BASE_LNET_PACKET_COMMAND_READ_RESPONSE) && (data != AI_BASE_LNET_PACKET_COMMAND_WRITE_RESPONSE)) {
        return AI_ERROR_GENERAL;
    }

    for (field_index = 0; field_index < AI_BASE_LNET_PACKET_FIELD_MAX; field_index++) {
        if (start == end)
            break;

        /* get field map */
        if (AiBaseLnetPacketDataGet(packet, start, AI_BASE_LNET_PACKET_FIELD_MAP_LENGTH, &data) != AI_ERROR_SUCCESS) {
            return AI_ERROR_PACKET;
        }
        field_map = (UINT8_T)data;
        start = (start + AI_BASE_LNET_PACKET_FIELD_MAP_LENGTH);

        /* get index_length */
#if (AI_BASE_LNET_PACKET_INDEX_LENGTH_MAP == 1)
        index_length = (((field_map & AI_BASE_LNET_PACKET_INDEX_LENGTH_MASK) >> AI_BASE_LNET_PACKET_INDEX_LENGTH_SHIFT) + AI_BASE_LNET_PACKET_INDEX_LENGTH_ADD);
#else
        index_length = AI_BASE_LNET_PACKET_INDEX_LENGTH_BYTE;
#endif

        /* get index */
        if (AiBaseLnetPacketDataGet(packet, start, index_length, &data) != AI_ERROR_SUCCESS) {
            return AI_ERROR_PACKET;
        }
        index = (UINT16_T)data;
        start = (start + index_length);

        /* get sub_index_valid */
        sub_index_valid = (((field_map & AI_BASE_LNET_PACKET_SUB_INDEX_VALID_MASK) >> AI_BASE_LNET_PACKET_SUB_INDEX_VALID_SHIFT) + AI_BASE_LNET_PACKET_SUB_INDEX_VALID_ADD);
        /* get sub_index_length */
        if (sub_index_valid == AI_BASE_LNET_PACKET_SUB_INDEX_VALID) {
            sub_index_length = (((field_map & AI_BASE_LNET_PACKET_SUB_INDEX_LENGTH_MASK) >> AI_BASE_LNET_PACKET_SUB_INDEX_LENGTH_SHIFT));
            sub_index_length = ((sub_index_length == AI_BASE_LNET_PACKET_SUB_INDEX_MAP_BYTE)?AI_BASE_LNET_PACKET_SUB_INDEX_LENGTH_BYTE:
                                ((sub_index_length == AI_BASE_LNET_PACKET_SUB_INDEX_MAP_SHORT)?AI_BASE_LNET_PACKET_SUB_INDEX_LENGTH_SHORT:
                                 ((sub_index_length == AI_BASE_LNET_PACKET_SUB_INDEX_MAP_THREE)?AI_BASE_LNET_PACKET_SUB_INDEX_LENGTH_WORD:
                                  ((sub_index_length <= AI_BASE_LNET_PACKET_SUB_INDEX_MAP_WORD)?AI_BASE_LNET_PACKET_SUB_INDEX_LENGTH_WORD:
                                   AI_BASE_LNET_PACKET_SUB_INDEX_LENGTH_DWORD))));
        }
        /* get sub_index */
        if (sub_index_valid == AI_BASE_LNET_PACKET_SUB_INDEX_INVALID) {
            sub_index = AI_BASE_LNET_PACKET_SUB_INDEX_DEFAULT;
        } else {
            /* get sub_index */
            if (AiBaseLnetPacketDataGet(packet, start, sub_index_length, &data) != AI_ERROR_SUCCESS) {
                return AI_ERROR_PACKET;
            }
            sub_index = data;
            start = (start + sub_index_length);
        }

        /* get data_type_size_valid */
        data_type_size_valid = (((field_map & AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_VALID_MASK) >> AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_VALID_SHIFT) + AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_VALID_ADD);
        /* get dat_type_size */
        if (data_type_size_valid == AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_INVALID) {
            data_type_size = AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_DEFAULT;
        } else {
            /* get data_type_size */
            if (AiBaseLnetPacketDataGet(packet, start, AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_LENGTH, &data) != AI_ERROR_SUCCESS) {
                return AI_ERROR_PACKET;
            }
            data_type_size = (UINT8_T)data;
            start = (start + AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_LENGTH);
        }

        /* get data_number_valid */
        data_number_valid = (((field_map & AI_BASE_LNET_PACKET_DATA_NUMBER_VALID_MASK) >> AI_BASE_LNET_PACKET_DATA_NUMBER_VALID_SHIFT) + AI_BASE_LNET_PACKET_DATA_NUMBER_VALID_ADD);
        /* get data_number_length */
        data_number_length = (((field_map & AI_BASE_LNET_PACKET_DATA_NUMBER_LENGTH_MASK) >> AI_BASE_LNET_PACKET_DATA_NUMBER_LENGTH_SHIFT) + AI_BASE_LNET_PACKET_DATA_NUMBER_LENGTH_ADD);
        /* get data_number */
        if (data_number_valid == AI_BASE_LNET_PACKET_DATA_NUMBER_INVALID) {
            data_number = AI_BASE_LNET_PACKET_DATA_NUMBER_DEFAULT;
        } else {
            /* get data_number */
            if (AiBaseLnetPacketDataGet(packet, start, data_number_length, &data) != AI_ERROR_SUCCESS) {
                return AI_ERROR_PACKET;
            }
            data_number = (UINT32_T)data;
            start = (start + data_number_length);
        }

        /* get data_number_valid */
        data_value_valid = (((field_map & AI_BASE_LNET_PACKET_DATA_VALUE_VALID_MASK) >> AI_BASE_LNET_PACKET_DATA_VALUE_VALID_SHIFT) + AI_BASE_LNET_PACKET_DATA_VALUE_VALID_ADD);
        /* get value */
        if (data_value_valid == AI_BASE_LNET_PACKET_DATA_VALUE_INVALID) {
            data_value_length = (data_type_size * data_number);
            data_value = 0;
        } else {
            /* get value */
            data_value_length = (data_type_size * data_number);
            if (data_value_length > AI_BASE_LNET_PACKET_DATA_LENGTH_AS_ADDR) {
                data_value = (UINT64_T)(packet->buffer + start);
                start = (start + data_value_length);
            } else {
                /* get data_number */
                if (AiBaseLnetPacketDataGet(packet, start, data_value_length, &data) != AI_ERROR_SUCCESS) {
                    return AI_ERROR_PACKET;
                }
                data_value = data;
                start = (start + data_value_length);
            }
        }

        /* save the field information */
        decode->field[field_index].index = index;
        decode->field[field_index].sub_index = sub_index;
        decode->field[field_index].data_type_size = data_type_size;
        decode->field[field_index].data_number = data_number;
        decode->field[field_index].value = data_value;
        decode->field[field_index].field_map = field_map;
        decode->field[field_index].valid = AI_BASE_LNET_PACKET_FIELD_VALID;
    }

    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet packet command.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetPacketCommand(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, VOID_T inteface)
{
    ERROR_T result = AI_ERROR_GENERAL;
    UINT8_T command;
    UINT8_T flag;
    UINT8_T retcode;
    AI_BASE_LNET_INTERFACE_PTR inteface_ptr = (AI_BASE_LNET_INTERFACE_PTR)inteface;


    command = ((decode->command & AI_BASE_LNET_PACKET_COMMAND_MASK) >> AI_BASE_LNET_PACKET_COMMAND_SHIFT);
    flag = decode->from;
    retcode = ((decode->command & AI_BASE_LNET_PACKET_COMMAND_RETCODE_MASK) >> AI_BASE_LNET_PACKET_COMMAND_RETCODE_SHIFT);
    switch (command) {
    case AI_BASE_LNET_PACKET_COMMAND_READ_REQUEST:
        if (flag == AI_BASE_LNET_PACKET_COMMAND_FLAG_RECV) {
            result = AiBaseLnetPacketCommandRead(packet, decode, inteface);
        }
        if (flag == AI_BASE_LNET_PACKET_COMMAND_FLAG_SEND) {
            result = AiBaseLnetPacketCommandReadSend(packet, decode, inteface);
        }
        break;
    case AI_BASE_LNET_PACKET_COMMAND_WRITE_REQUEST:
        if (flag == AI_BASE_LNET_PACKET_COMMAND_FLAG_RECV) {
            result = AiBaseLnetPacketCommandWrite(packet, decode, inteface);
        }
        if (flag == AI_BASE_LNET_PACKET_COMMAND_FLAG_SEND) {
            result = AiBaseLnetPacketCommandWriteSend(packet, decode, inteface);
        }
        break;
    case AI_BASE_LNET_PACKET_COMMAND_READ_RESPONSE:
        if (retcode == AI_BASE_LNET_PACKET_COMMAND_RETCODE_SUCCESS) {
            result = AiBaseLnetPacketCommandReadResponse(packet, decode, inteface);
        } else {
            result = AI_ERROR_GENERAL;
        }
        inteface_ptr->sequence_recv_last = decode->sequence;
        inteface_ptr->retcode_recv_last = result;
        result = AI_ERROR_NO_RESPONSE;
        break;
    case AI_BASE_LNET_PACKET_COMMAND_WRITE_RESPONSE:
        if (retcode == AI_BASE_LNET_PACKET_COMMAND_RETCODE_SUCCESS) {
            result = AiBaseLnetPacketCommandWriteResponse(packet, decode, inteface);
        } else {
            result = AI_ERROR_GENERAL;
        }
        inteface_ptr->sequence_recv_last = decode->sequence;
        inteface_ptr->retcode_recv_last = result;
        result = AI_ERROR_NO_RESPONSE;
        break;
    default:
        result = AI_ERROR_GENERAL;
        break;
    }

    return result;
}

/**
 * brief    Ai base lnet packet command read.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetPacketCommandRead(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, VOID_T inteface)
{
    UINT32_T field_index;
    ERROR_T result;
    UINT64_T value;
    UINT64_T sub_index;
    UINT8_T sub_data_type;
    UINT64_T value_min;
    UINT64_T value_max;
    UINT8_T catagory;
	UINT8_T operation;
    AI_BASE_LNET_PACKET_FIELD_DECODE_PTR field;

    for (field_index = 0; field_index < AI_BASE_LNET_PACKET_FIELD_MAX; field_index++) {
        field = &decode->field[field_index];

        if (field->valid != AI_BASE_LNET_PACKET_FIELD_VALID)
            break;
        result = AiSdkIndexDataTypeGet(field->index, &sub_index, &sub_data_type, &value_min, &value_max, &catagory, &operation);
        if (result != AI_ERROR_SUCCESS)
            return result;

        if ((field->data_number > AI_BASE_LNET_PACKET_SUB_INDEX_MULTI_SUPPORT) && (sub_data_type == AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_BYTE)) {
            field->data_number = ((field->data_type_size * field->data_number) / (sub_data_type));
            field->data_type_size = sub_data_type;
            if ((field->sub_index + field->data_number) > sub_index) {
                if (field->sub_index >= sub_index)
                    return AI_ERROR_GENERAL;
                field->data_number = (UINT32_T)(sub_index - field->sub_index);
            }
        } else {
            field->data_type_size = sub_data_type;
            if ((field->sub_index + field->data_number) > sub_index)
                return AI_ERROR_GENERAL;
        }

		result = AiSdkIndexDataGet(decode->source_addr, decode->dest_addr, field->index, field->sub_index, &value, (UINT32_T)(field->data_number*field->data_type_size));
        field->value_valid = AI_BASE_LNET_PACKET_DATA_VALUE_VALID;
        if (result != AI_ERROR_SUCCESS)
            return result;

        field->value = value;
    }

    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet packet command write.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetPacketCommandWrite(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, VOID_T inteface)
{
    UINT32_T field_index;
    ERROR_T result;
    UINT64_T sub_index;
    UINT8_T sub_data_type;
    UINT64_T value_min;
    UINT64_T value_max;
    UINT8_T catagory;
	UINT8_T operation;
    AI_BASE_LNET_PACKET_FIELD_DECODE_PTR field;

    for (field_index = 0; field_index < AI_BASE_LNET_PACKET_FIELD_MAX; field_index++) {
        field = &decode->field[field_index];

        if (field->valid != AI_BASE_LNET_PACKET_FIELD_VALID)
            break;
        result = AiSdkIndexDataTypeGet(field->index, &sub_index, &sub_data_type, &value_min, &value_max, &catagory, &operation);
        if (result != AI_ERROR_SUCCESS)
            return result;

        if ((field->data_number > AI_BASE_LNET_PACKET_SUB_INDEX_MULTI_SUPPORT) && (sub_data_type == AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_BYTE)) {
            field->data_number = ((field->data_type_size * field->data_number) / (sub_data_type));
            field->data_type_size = sub_data_type;
            if ((field->sub_index + field->data_number) > sub_index)
                return AI_ERROR_GENERAL;
        } else {
            if (field->data_number != 1)
                return AI_ERROR_GENERAL;
            field->data_type_size = sub_data_type;
            if ((field->sub_index + field->data_number) > sub_index)
                return AI_ERROR_GENERAL;
        }

		result = AiSdkIndexDataSet(decode->source_addr, decode->dest_addr, field->index, field->sub_index, (UINT64_T *)&(field->value), (UINT32_T)(field->data_number * field->data_type_size));
        if (result != AI_ERROR_SUCCESS)
            return result;

    }

    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet packet command write.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetPacketCommandReadResponse(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, VOID_T inteface)
{
    UINT32_T field_index;
    ERROR_T result;
    UINT64_T sub_index;
    UINT8_T sub_data_type;
    UINT64_T value_min;
    UINT64_T value_max;
    UINT8_T catagory;
	UINT8_T operation;
    AI_BASE_LNET_PACKET_FIELD_DECODE_PTR field;

    for (field_index = 0; field_index < AI_BASE_LNET_PACKET_FIELD_MAX; field_index++) {
        field = &decode->field[field_index];

        if (field->valid != AI_BASE_LNET_PACKET_FIELD_VALID)
            break;
        result = AiSdkIndexDataTypeGet(field->index, &sub_index, &sub_data_type, &value_min, &value_max, &catagory, &operation);
        if (result != AI_ERROR_SUCCESS)
            return result;

        if ((field->data_number > AI_BASE_LNET_PACKET_SUB_INDEX_MULTI_SUPPORT) && (sub_data_type == AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_BYTE)) {
            field->data_number = ((field->data_type_size * field->data_number) / (sub_data_type));
            field->data_type_size = sub_data_type;
            if ((field->sub_index + field->data_number) > sub_index)
                return AI_ERROR_GENERAL;
        } else {
            field->data_type_size = sub_data_type;
            if ((field->sub_index + field->data_number) > sub_index)
                return AI_ERROR_GENERAL;
        }

		result = AiSdkIndexDataSet(decode->source_addr, decode->dest_addr, field->index, field->sub_index, (UINT64_T *)&(field->value), (UINT32_T)(field->data_number * field->data_type_size));
        if (result != AI_ERROR_SUCCESS)
            return result;

    }

    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet packet command write.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetPacketCommandWriteResponse(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, VOID_T inteface)
{
    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet packet command read.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetPacketCommandReadSend(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, VOID_T inteface)
{
    UINT32_T field_index;
    ERROR_T result;
    UINT64_T sub_index;
    UINT8_T sub_data_type;
    UINT64_T value_min;
    UINT64_T value_max;
    UINT8_T catagory;
	UINT8_T operation;
    AI_BASE_LNET_PACKET_FIELD_DECODE_PTR field;

    for (field_index = 0; field_index < AI_BASE_LNET_PACKET_FIELD_MAX; field_index++) {
        field = &decode->field[field_index];

        if (field->valid != AI_BASE_LNET_PACKET_FIELD_VALID)
            break;
        result = AiSdkIndexDataTypeGet(field->index, &sub_index, &sub_data_type, &value_min, &value_max, &catagory, &operation);
        if (result != AI_ERROR_SUCCESS)
            return result;

        if ((field->data_number > AI_BASE_LNET_PACKET_SUB_INDEX_MULTI_SUPPORT) && (sub_data_type == AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_BYTE)) {
            field->data_type_size = sub_data_type;
            field->data_number = ((field->data_type_size * field->data_number) / (sub_data_type));
            if ((field->sub_index + field->data_number) > sub_index) {
                if (field->sub_index >= sub_index)
                    return AI_ERROR_GENERAL;
                field->data_number = (UINT32_T)(sub_index - field->sub_index);
            }
        } else {
            field->data_type_size = sub_data_type;
            if ((field->sub_index + field->data_number) > sub_index)
                return AI_ERROR_GENERAL;
        }

        field->value = 0;
    }

    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet packet command write.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetPacketCommandWriteSend(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, VOID_T inteface)
{
    UINT32_T field_index;
    ERROR_T result;
    UINT64_T value;
    UINT64_T sub_index;
    UINT8_T sub_data_type;
    UINT64_T value_min;
    UINT64_T value_max;
    UINT8_T catagory;
	UINT8_T operation;
    AI_BASE_LNET_PACKET_FIELD_DECODE_PTR field;

    for (field_index = 0; field_index < AI_BASE_LNET_PACKET_FIELD_MAX; field_index++) {
        field = &decode->field[field_index];

        if (field->valid != AI_BASE_LNET_PACKET_FIELD_VALID)
            break;
        result = AiSdkIndexDataTypeGet(field->index, &sub_index, &sub_data_type, &value_min, &value_max, &catagory, &operation);
        if (result != AI_ERROR_SUCCESS)
            return result;

        if ((field->data_number > AI_BASE_LNET_PACKET_SUB_INDEX_MULTI_SUPPORT) && (sub_data_type == AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_BYTE)) {
            field->data_type_size = sub_data_type;
            field->data_number = ((field->data_type_size * field->data_number) / (sub_data_type));
            if ((field->sub_index + field->data_number) > sub_index) {
                if (field->sub_index >= sub_index)
                    return AI_ERROR_GENERAL;
                field->data_number = (UINT32_T)(sub_index - field->sub_index);
            }
        } else {
            field->data_type_size = sub_data_type;
            field->data_number = 1;
            if ((field->sub_index + field->data_number) > sub_index)
                return AI_ERROR_GENERAL;
        }

        if (field->value_valid != AI_BASE_LNET_PACKET_DATA_VALUE_VALID) {
			result = AiSdkIndexDataGet(decode->source_addr, decode->dest_addr, field->index, field->sub_index, &value, (UINT32_T)(field->data_number*field->data_type_size));
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
 * brief    Ai base lnet packet data get.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetPacketDataSet(AI_BASE_LNET_PACKET_PTR packet, UINT32_T start, UINT32_T length, UINT64_T value)
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
 * brief    Ai base lnet packet buffer data set.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetPacketBufferSet(AI_BASE_LNET_PACKET_PTR packet, UINT32_T start, UINT32_T length, UINT8_T *buffer)
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
 * brief    Ai base lnet packet encode.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetPacketEncode(AI_BASE_LNET_PACKET_DECODE_PTR decode, ERROR_T retcode, AI_BASE_LNET_PACKET_PTR packet)
{
    UINT32_T start;
    UINT32_T end;
    UINT32_T field_index;
    UINT8_T field_map;
    UINT8_T index_length;
    UINT8_T sub_index_valid;
    UINT8_T sub_index_length;
    UINT8_T sub_index_map;
    UINT8_T data_type_size_valid;
    UINT8_T data_number_valid;
    UINT8_T data_number_length;
    UINT8_T data_value_valid;
    UINT32_T data_value_length;
    UINT8_T command;
    AI_BASE_LNET_PACKET_FIELD_DECODE_PTR field;

    start = 0 + AI_BASE_LNET_PACKET_HEAD_LENGTH;
    end = 0 + AI_BASE_LNET_PACKET_HEAD_LENGTH;
    packet->start = start;
    packet->length = AI_BASE_LNET_LINK_BUFFER_LENGTH - AI_BASE_LNET_PACKET_HEAD_TAIL_LENGTH;

    /* set sequence */
    if (AiBaseLnetPacketDataSet(packet, end, AI_BASE_LNET_PACKET_SEQUENCE_LENGTH, decode->sequence) != AI_ERROR_SUCCESS) {
        return AI_ERROR_GENERAL;
    }
    end = (end + AI_BASE_LNET_PACKET_SEQUENCE_LENGTH);

    /* get source address */
    if (AiBaseLnetPacketDataSet(packet, end, AI_BASE_LNET_PACKET_SOURCE_ADDR_LENGTH, decode->dest_addr) != AI_ERROR_SUCCESS) {
        return AI_ERROR_GENERAL;
    }
    end = (end + AI_BASE_LNET_PACKET_SOURCE_ADDR_LENGTH);

    /* get dest address */
    if (AiBaseLnetPacketDataSet(packet, end, AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH, decode->source_addr) != AI_ERROR_SUCCESS) {
        return AI_ERROR_GENERAL;
    }
    end = (end + AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH);

    /* set command */
    command = decode->command;
    if (decode->from == AI_BASE_LNET_PACKET_COMMAND_FLAG_SEND) {
        decode->command = ((command & AI_BASE_LNET_PACKET_COMMAND_MASK) >> AI_BASE_LNET_PACKET_COMMAND_SHIFT);
    } else {
        if (retcode != AI_ERROR_SUCCESS) {
            decode->command = ((((command & AI_BASE_LNET_PACKET_COMMAND_MASK) >> AI_BASE_LNET_PACKET_COMMAND_SHIFT)  + AI_BASE_LNET_PACKET_COMMAND_RESPONSE_INC) | ((AI_BASE_LNET_PACKET_COMMAND_RETCODE_ERROR << AI_BASE_LNET_PACKET_COMMAND_RETCODE_SHIFT) & AI_BASE_LNET_PACKET_COMMAND_RETCODE_MASK));
        } else {
            decode->command = (((command & AI_BASE_LNET_PACKET_COMMAND_MASK) >> AI_BASE_LNET_PACKET_COMMAND_SHIFT) + AI_BASE_LNET_PACKET_COMMAND_RESPONSE_INC);
        }
    }
    command = ((decode->command & AI_BASE_LNET_PACKET_COMMAND_MASK) >> AI_BASE_LNET_PACKET_COMMAND_SHIFT);
    if (AiBaseLnetPacketDataSet(packet, end, AI_BASE_LNET_PACKET_COMMAND_LENGTH, decode->command) != AI_ERROR_SUCCESS) {
        return AI_ERROR_GENERAL;
    }
    end = (end + AI_BASE_LNET_PACKET_COMMAND_LENGTH);

    // AI_BASE_LNET_PACKET_COMMAND_READ_REQUEST need pack field
    // AI_BASE_LNET_PACKET_COMMAND_WRITE_REQUEST) need pack field
    // AI_BASE_LNET_PACKET_COMMAND_READ_RESPONSE need pack field
    // AI_BASE_LNET_PACKET_COMMAND_WRITE_RESPONSE not need pack field
    if (((decode->command & AI_BASE_LNET_PACKET_COMMAND_MASK) >> AI_BASE_LNET_PACKET_COMMAND_SHIFT) == AI_BASE_LNET_PACKET_COMMAND_WRITE_RESPONSE) {
        packet->start = start;
        packet->length = end-start;

        return AI_ERROR_SUCCESS;
    }

    if (retcode == AI_ERROR_SUCCESS) {
        for (field_index = 0; field_index < AI_BASE_LNET_PACKET_FIELD_MAX; field_index++) {
            field = &(decode->field[field_index]);

            /* check valid */
            if (field->valid != AI_BASE_LNET_PACKET_FIELD_VALID)
                break;

            /* calculate field map */
            field_map = 0;
            /* set index length map */
#if (AI_BASE_LNET_PACKET_INDEX_LENGTH_MAP == 1)
            index_length = (field->index <= AI_BASE_LNET_PACKET_BYTE_ONE)?AI_BASE_LNET_PACKET_INDEX_LENGTH_BYTE:AI_BASE_LNET_PACKET_INDEX_LENGTH_SHORT;
            field_map = field_map | ((((index_length - AI_BASE_LNET_PACKET_INDEX_LENGTH_ADD) << AI_BASE_LNET_PACKET_INDEX_LENGTH_SHIFT) & AI_BASE_LNET_PACKET_INDEX_LENGTH_MASK));
#else
            index_length = AI_BASE_LNET_PACKET_INDEX_LENGTH_BYTE;
#endif
            /* set sub index valid map */
            sub_index_valid = (field->sub_index == AI_BASE_LNET_PACKET_SUB_INDEX_DEFAULT)?AI_BASE_LNET_PACKET_SUB_INDEX_INVALID:AI_BASE_LNET_PACKET_SUB_INDEX_VALID;
            field_map = field_map | ((((sub_index_valid) << AI_BASE_LNET_PACKET_SUB_INDEX_VALID_SHIFT)  & AI_BASE_LNET_PACKET_SUB_INDEX_VALID_MASK));
            /* set sub index length */
            sub_index_length = (((field->sub_index <= AI_BASE_LNET_PACKET_BYTE_ONE)?AI_BASE_LNET_PACKET_SUB_INDEX_LENGTH_BYTE:
                                 ((field->sub_index <= AI_BASE_LNET_PACKET_BYTE_TWO)?AI_BASE_LNET_PACKET_SUB_INDEX_LENGTH_SHORT:
                                  ((field->sub_index <= AI_BASE_LNET_PACKET_BYTE_THREE)?AI_BASE_LNET_PACKET_SUB_INDEX_LENGTH_WORD:
                                   ((field->sub_index <= AI_BASE_LNET_PACKET_BYTE_FOUR)?AI_BASE_LNET_PACKET_SUB_INDEX_LENGTH_WORD:
                                    AI_BASE_LNET_PACKET_SUB_INDEX_LENGTH_DWORD)))));
            sub_index_map = (((field->sub_index <= AI_BASE_LNET_PACKET_BYTE_ONE)?AI_BASE_LNET_PACKET_SUB_INDEX_MAP_BYTE:
                              ((field->sub_index <= AI_BASE_LNET_PACKET_BYTE_TWO)?AI_BASE_LNET_PACKET_SUB_INDEX_MAP_SHORT:
                               ((field->sub_index <= AI_BASE_LNET_PACKET_BYTE_FOUR)?AI_BASE_LNET_PACKET_SUB_INDEX_MAP_WORD:
                                AI_BASE_LNET_PACKET_SUB_INDEX_MAP_DWORD))));
            field_map = field_map | ((sub_index_map << AI_BASE_LNET_PACKET_SUB_INDEX_LENGTH_SHIFT)  & AI_BASE_LNET_PACKET_SUB_INDEX_LENGTH_MASK);
            /* set data type size map */
            data_type_size_valid = (field->data_type_size == AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_DEFAULT)?AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_INVALID:AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_VALID;
            field_map = field_map | ((((data_type_size_valid) << AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_VALID_SHIFT)  & AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_VALID_MASK));
            /* set data number valid map */
            data_number_valid = (field->data_number == AI_BASE_LNET_PACKET_DATA_NUMBER_DEFAULT)?AI_BASE_LNET_PACKET_DATA_NUMBER_INVALID:AI_BASE_LNET_PACKET_DATA_NUMBER_VALID;
            field_map = field_map | ((((data_number_valid) << AI_BASE_LNET_PACKET_DATA_NUMBER_VALID_SHIFT)  & AI_BASE_LNET_PACKET_DATA_NUMBER_VALID_MASK));
            /* set data number map */
            data_number_length = (field->data_number <= AI_BASE_LNET_PACKET_BYTE_ONE)?AI_BASE_LNET_PACKET_INDEX_LENGTH_BYTE:AI_BASE_LNET_PACKET_INDEX_LENGTH_SHORT;
            field_map = field_map | ((((data_number_length - AI_BASE_LNET_PACKET_DATA_NUMBER_LENGTH_ADD) << AI_BASE_LNET_PACKET_DATA_NUMBER_LENGTH_SHIFT) & AI_BASE_LNET_PACKET_DATA_NUMBER_LENGTH_MASK));
            /* set data value valid map */
            if (decode->from == AI_BASE_LNET_PACKET_COMMAND_FLAG_SEND) {
                data_value_valid = (((command & AI_BASE_LNET_PACKET_COMMAND_MASK)>> AI_BASE_LNET_PACKET_COMMAND_SHIFT) != AI_BASE_LNET_PACKET_COMMAND_WRITE_REQUEST)?AI_BASE_LNET_PACKET_DATA_VALUE_INVALID:AI_BASE_LNET_PACKET_DATA_VALUE_VALID;
            } else {
                data_value_valid = (((command & AI_BASE_LNET_PACKET_COMMAND_MASK)>> AI_BASE_LNET_PACKET_COMMAND_SHIFT) != AI_BASE_LNET_PACKET_COMMAND_READ_RESPONSE)?AI_BASE_LNET_PACKET_DATA_VALUE_INVALID:AI_BASE_LNET_PACKET_DATA_VALUE_VALID;
            }
            field_map = field_map | ((((data_value_valid) << AI_BASE_LNET_PACKET_DATA_VALUE_VALID_SHIFT)  & AI_BASE_LNET_PACKET_DATA_VALUE_VALID_MASK));

            /* set the field map */
            if (AiBaseLnetPacketDataSet(packet, end, AI_BASE_LNET_PACKET_FIELD_MAP_LENGTH, field_map) != AI_ERROR_SUCCESS) {
                return AI_ERROR_GENERAL;
            }
            end = (end + AI_BASE_LNET_PACKET_FIELD_MAP_LENGTH);

            /* set index */
            if (AiBaseLnetPacketDataSet(packet, end, index_length, field->index) != AI_ERROR_SUCCESS) {
                return AI_ERROR_GENERAL;
            }
            end = (end + index_length);

            /* set sub_index */
            if (sub_index_valid == AI_BASE_LNET_PACKET_SUB_INDEX_VALID) {
                /* set sub_index */
                if (AiBaseLnetPacketDataSet(packet, end, sub_index_length, field->sub_index) != AI_ERROR_SUCCESS) {
                    return AI_ERROR_GENERAL;
                }
                end = (end + sub_index_length);
            }

            /* set dat_type_size */
            if (data_type_size_valid == AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_VALID) {
                /* set data_type_size */
                if (AiBaseLnetPacketDataSet(packet, end, AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_LENGTH, field->data_type_size) != AI_ERROR_SUCCESS) {
                    return AI_ERROR_GENERAL;
                }
                end = (end + AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_LENGTH);
            }

            /* set data_number */
            if (data_number_valid == AI_BASE_LNET_PACKET_DATA_NUMBER_VALID) {
                /* set data_number */
                if (AiBaseLnetPacketDataSet(packet, end, data_number_length, field->data_number) != AI_ERROR_SUCCESS) {
                    return AI_ERROR_GENERAL;
                }
                end = (end + data_number_length);
            }

            /* set value */
            if (data_value_valid == AI_BASE_LNET_PACKET_DATA_VALUE_VALID) {
                /* set value */
                data_value_length = (UINT32_T)(field->data_type_size * field->data_number);
                if (data_value_length > AI_BASE_LNET_PACKET_DATA_LENGTH_AS_ADDR) {
                    if (AiBaseLnetPacketBufferSet(packet, end, data_value_length, (UINT8_T *)(field->value)) != AI_ERROR_SUCCESS) {
                        return AI_ERROR_GENERAL;
                    }
                    end = (end + data_value_length);
                } else {
                    /* set data_number */
                    if (AiBaseLnetPacketDataSet(packet, end, data_value_length, field->value) != AI_ERROR_SUCCESS) {
                        return AI_ERROR_GENERAL;
                    }
                    end = (end + data_value_length);
                }
            }
        }
    }

    packet->start = start;
    packet->length = end-start;

    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet packet check.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetPacketTxHeadTail(AI_BASE_LNET_LINK_PTR link, AI_BASE_LNET_PACKET_PTR packet)
{
    UINT64_T data;
    UINT32_T start = 0;
    UINT32_T end = 0;
    UINT32_T i;
    UINT16_T calculate_parity;

    /* get the valid buffer range */
    start = packet->start - AI_BASE_LNET_PACKET_HEAD_LENGTH;
    end = packet->start + packet->length;
    packet->start = start;
    packet->length = packet->length + AI_BASE_LNET_PACKET_HEAD_TAIL_LENGTH;

    /* set the stx */
    data = AI_BASE_LNET_PACKET_STX;
    if (AiBaseLnetPacketDataSet(packet, start, AI_BASE_LNET_PACKET_STX_LENGTH, data) != AI_ERROR_SUCCESS) {
        return AI_ERROR_GENERAL;
    }

    /* set the packet length */
    data = packet->length - AI_BASE_LNET_PACKET_HEAD_TAIL_LENGTH;
    if (AiBaseLnetPacketDataSet(packet, start+AI_BASE_LNET_PACKET_STX_LENGTH, AI_BASE_LNET_PACKET_LENGTH_LENGTH, data) != AI_ERROR_SUCCESS) {
        return AI_ERROR_GENERAL;
    }

    /* calculate the parity */
    calculate_parity = 0;
    for (i = 0; i < (packet->length-AI_BASE_LNET_PACKET_HEAD_TAIL_LENGTH); i++) {
        if (AiBaseLnetPacketDataGet(packet, (start + AI_BASE_LNET_PACKET_HEAD_LENGTH + i), 1, &data) != AI_ERROR_SUCCESS)
            return AI_ERROR_GENERAL;
        calculate_parity = calculate_parity + (UINT16_T)data;
    }

    /* set the parity */
    data = calculate_parity;
    if (AiBaseLnetPacketDataSet(packet, end, AI_BASE_LNET_PACKET_CHECKSUM_LENGTH, data) != AI_ERROR_SUCCESS) {
        return AI_ERROR_GENERAL;
    }

    /* set the etx */
    data = AI_BASE_LNET_PACKET_ETX;
    if (AiBaseLnetPacketDataSet(packet, end+AI_BASE_LNET_PACKET_CHECKSUM_LENGTH, AI_BASE_LNET_PACKET_ETX_LENGTH, data) != AI_ERROR_SUCCESS) {
        return AI_ERROR_GENERAL;
    }

#if (AI_PRODUCT_OPTION_LOG_PACKET == 1)
	AiServerLogPacket(1, ((packet->buffer)+(start)), (packet->length));
#endif

    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet packet tx escape.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetPacketTxEscape(AI_BASE_LNET_LINK_PTR link, AI_BASE_LNET_PACKET_PTR packet)
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
 * brief    Ai base lnet packet tx.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetPacketTx(AI_BASE_LNET_LINK_PTR link, AI_BASE_LNET_PACKET_PTR packet)
{
    if (AiBaseLnetTxBufferSet(link, packet->buffer+packet->start, packet->length) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    return AI_ERROR_SUCCESS;
}
#endif

/*------------------------- End ---------------------------------------------*/
