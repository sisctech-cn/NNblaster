/**
 ******************************************************************************
 * file    ai_base_lnet_modbus.c
 * author  sunlingge
 * version V100
 * brief   the ai base lnet modbus driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"

#if (AI_PRODUCT_OPTION_MODBUS == 1)
#if (AI_PRODUCT_OPTION_MODBUS_MASTER == 1)
/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/
extern const UINT8_T g_ai_base_lnet_modbus_crc_hi[256];
extern const UINT8_T g_ai_base_lnet_modbus_crc_low[256];

ERROR_T AiSdkIndexDataTypeGet(UINT16_T index, UINT64_T *sub_index, UINT8_T *sub_data_type, UINT64_T *value_min, UINT64_T *value_max, UINT8_T *category, UINT8_T *operation);
ERROR_T AiSdkIndexDataGet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length);
ERROR_T AiSdkIndexDataSet(UINT8_T source_address, UINT8_T dest_address, UINT16_T index, UINT64_T sub_index, UINT64_T *value, UINT32_T length);
ERROR_T AiServerLogPacket(UINT8_T direction, UINT8_T *packet_buffer, UINT32_T packet_length);

/*------------------------- Variable Definition------------------------------*/
AI_BASE_LNET_PACKET_PUBLIC g_ai_base_lnet_public;

/*------------------------- Function Definition -----------------------------*/
/**
 * brief    Ai base lnet modbus check.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetModbusMasterRxHeadTail(AI_BASE_LNET_LINK_PTR link, AI_BASE_LNET_PACKET_PTR packet)
{
    UINT64_T data;
    UINT32_T start = 0;
    UINT32_T end = 0;
    UINT32_T link_length;
    UINT64_T packet_length;
    UINT32_T i;
    UINT16_T calculate_parity;
    UINT64_T packet_parity;
	UINT64_T source_address;
	UINT64_T function_code;
	UINT64_T function_code_mask;
	UINT64_T error_code;
	UINT64_T reg_start_address;
	UINT64_T reg_value;
	UINT64_T reg_number;
	UINT8_T crc_hi;
    UINT8_T crc_low;
    UINT8_T index;

    /* get the valid buffer range */
    start = packet->start;
    end = packet->start + packet->length;

    /* check the address of packet */
    while (1) {
        if (AiBaseLnetModbusDataGet(packet, start, AI_BASE_LNET_PACKET_SOURCE_ADDR_LENGTH, &source_address) != AI_ERROR_SUCCESS) {
            return AI_ERROR_GENERAL;
        }
		/* mow do not check address code */
		break;
    }

    /* get and check the link length */
    link_length = (end - start);
    if (link_length < AI_BASE_LNET_PACKET_LINK_LENGTH_MIN)
        return AI_ERROR_GENERAL;

	/* check the function code */
	if (AiBaseLnetModbusDataGet(packet, start+AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH, AI_BASE_LNET_PACKET_FCODE_LENGTH, &function_code) != AI_ERROR_SUCCESS) {
		return AI_ERROR_GENERAL;
    }
	function_code_mask = (function_code & AI_BASE_LENT_PACKET_FCODE_MASK);
	if ((function_code_mask != AI_BASE_LNET_PACKET_FCODE_READ_HOLD_REGS) && 
		(function_code_mask != AI_BASE_LNET_PACKET_FCODE_READ_INPUT_REGS) &&
		(function_code_mask != AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REG) &&
		(function_code_mask != AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REGS)) {
		AiBaseLnetRxStartSet(link, AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH);
		return AI_ERROR_GENERAL;
	}

	if (function_code > AI_BASE_LNET_PACKET_FCODE_ERROR_MIN) {
		/* check the error code */
		if (AiBaseLnetModbusDataGet(packet, start+AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH+AI_BASE_LNET_PACKET_FCODE_LENGTH, 
				AI_BASE_LNET_PACKET_ECODE_LENGTH, &error_code) != AI_ERROR_SUCCESS) {
			return AI_ERROR_GENERAL;
		}
		packet_length = AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH+AI_BASE_LNET_PACKET_FCODE_LENGTH+AI_BASE_LNET_PACKET_ECODE_LENGTH;
	} 
	if ((function_code == AI_BASE_LNET_PACKET_FCODE_READ_HOLD_REGS) ||
		(function_code == AI_BASE_LNET_PACKET_FCODE_READ_INPUT_REGS)) {
		/* get the packete length */
		if (AiBaseLnetModbusDataGet(packet, start+AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH+AI_BASE_LNET_PACKET_FCODE_LENGTH, 
				AI_BASE_LNET_PACKET_LENGTH_LENGTH, &packet_length) != AI_ERROR_SUCCESS) {
			return AI_ERROR_GENERAL;
		}
		packet_length = AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH+AI_BASE_LNET_PACKET_FCODE_LENGTH+AI_BASE_LNET_PACKET_LENGTH_LENGTH+packet_length;
	}
	if (function_code == AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REG) {
		/* get the reg_start_address */
		if (AiBaseLnetModbusDataGet(packet, start+AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH+AI_BASE_LNET_PACKET_FCODE_LENGTH, 
				AI_BASE_LNET_PACKET_REG_START_ADDR_LENGTH, &reg_start_address) != AI_ERROR_SUCCESS) {
			return AI_ERROR_GENERAL;
		}
		/* get the reg_value */
		if (AiBaseLnetModbusDataGet(packet, start+AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH+AI_BASE_LNET_PACKET_FCODE_LENGTH+AI_BASE_LNET_PACKET_REG_START_ADDR_LENGTH, 
				AI_BASE_LNET_PACKET_REG_VALUE_LENGTH, &reg_value) != AI_ERROR_SUCCESS) {
			return AI_ERROR_GENERAL;
		}
		packet_length = AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH+AI_BASE_LNET_PACKET_FCODE_LENGTH+AI_BASE_LNET_PACKET_REG_START_ADDR_LENGTH+AI_BASE_LNET_PACKET_REG_VALUE_LENGTH;
	}
	if (function_code == AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REGS) {
		/* get the reg_start_address */
		if (AiBaseLnetModbusDataGet(packet, start+AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH+AI_BASE_LNET_PACKET_FCODE_LENGTH, 
				AI_BASE_LNET_PACKET_REG_START_ADDR_LENGTH, &reg_start_address) != AI_ERROR_SUCCESS) {
			return AI_ERROR_GENERAL;
		}
		/* get the reg_number */
		if (AiBaseLnetModbusDataGet(packet, start+AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH+AI_BASE_LNET_PACKET_FCODE_LENGTH+AI_BASE_LNET_PACKET_REG_START_ADDR_LENGTH, 
				AI_BASE_LNET_PACKET_REG_NUMBER_LENGTH, &reg_number) != AI_ERROR_SUCCESS) {
			return AI_ERROR_GENERAL;
		}
		packet_length = AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH+AI_BASE_LNET_PACKET_FCODE_LENGTH+AI_BASE_LNET_PACKET_REG_START_ADDR_LENGTH+AI_BASE_LNET_PACKET_REG_NUMBER_LENGTH;
	}

	/* get the parity */
	if (AiBaseLnetModbusDataGet(packet, (start + (UINT32_T)packet_length), AI_BASE_LNET_PACKET_CHECKSUM_LENGTH, &packet_parity) != AI_ERROR_SUCCESS) {
		return AI_ERROR_GENERAL;
	}

    /* calculate the parity */
    calculate_parity = 0;
	crc_hi = 0xFF;
	crc_low = 0xFF;
    for (i = 0; i < packet_length; i++) {
        if (AiBaseLnetModbusDataGet(packet, (start + i), 1, &data) != AI_ERROR_SUCCESS)
            return AI_ERROR_GENERAL;
		index = (crc_low ^ ((UINT8_T)data));
        crc_low = (UINT8_T)(crc_hi ^ (g_ai_base_lnet_modbus_crc_hi[index]));
        crc_hi = g_ai_base_lnet_modbus_crc_low[index];
    }
	calculate_parity = (UINT16_T)((crc_hi << 8) | crc_low);

    /* check the parity */ 
    if (calculate_parity != packet_parity) {
        AiBaseLnetRxStartSet(link, AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH);
        return AI_ERROR_GENERAL;
    }

    /* release the link rx_buffer, no etx */
    AiBaseLnetRxStartSet(link, (UINT32_T)(packet_length + AI_BASE_LNET_PACKET_CHECKSUM_LENGTH));

    /* set the packet start end */
    packet->start = (start);
    packet->length = (UINT32_T)packet_length;

#if (AI_PRODUCT_OPTION_LOG_PACKET == 1)
	AiServerLogPacket(0, ((packet->buffer)+(start)), (UINT32_T)(packet_length + AI_BASE_LNET_PACKET_CHECKSUM_LENGTH));
#endif

    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet modbus decode.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetModbusMasterDecode(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode)
{
    UINT32_T start;
    UINT32_T end;
    UINT64_T data;
    UINT32_T field_index;
	UINT64_T error_code;
	UINT64_T reg_start_address;
	UINT64_T reg_value_length;
	UINT64_T reg_number;

    memset((char *) decode, 0, sizeof(AI_BASE_LNET_PACKET_DECODE));

    start = packet->start;
    end = start + packet->length;

    /* get sequence */
    decode->sequence = (UINT8_T)1;

    /* get dest address */
    decode->dest_addr = (UINT8_T)0;

    /* get source address */
    if (AiBaseLnetModbusDataGet(packet, start, AI_BASE_LNET_PACKET_SOURCE_ADDR_LENGTH, &data) != AI_ERROR_SUCCESS) {
        return AI_ERROR_GENERAL;
    }
	decode->source_addr = (UINT8_T)data;
    start = (start + AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH);

    /* get function code */
    if (AiBaseLnetModbusDataGet(packet, start, AI_BASE_LNET_PACKET_FCODE_LENGTH, &data) != AI_ERROR_SUCCESS) {
        return AI_ERROR_GENERAL;
    }
    decode->command = (UINT8_T)data;
    decode->from = AI_BASE_LNET_PACKET_COMMAND_FLAG_RECV;
    start = (start + AI_BASE_LNET_PACKET_FCODE_LENGTH);

    /* check command */
    data = ((decode->command & AI_BASE_LNET_PACKET_COMMAND_MASK) >> AI_BASE_LNET_PACKET_COMMAND_SHIFT);
    if ((data != AI_BASE_LNET_PACKET_FCODE_READ_HOLD_REGS) && (data != AI_BASE_LNET_PACKET_FCODE_READ_INPUT_REGS) &&
        (data != AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REG) && (data != AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REGS)) {
        return AI_ERROR_GENERAL;
    }
	/* error code */
	if (decode->command > AI_BASE_LNET_PACKET_FCODE_ERROR_MIN) {
		/* check the error code */
		if (AiBaseLnetModbusDataGet(packet, start,	AI_BASE_LNET_PACKET_ECODE_LENGTH, &error_code) != AI_ERROR_SUCCESS) {
			return AI_ERROR_GENERAL;
		}
		decode->result = (UINT8_T)error_code;
		return AI_ERROR_SUCCESS;
	}
	/* write hold regs */
	if (decode->command == AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REGS) {
		/* get the reg_start_address */
		if (AiBaseLnetModbusDataGet(packet, start, AI_BASE_LNET_PACKET_REG_START_ADDR_LENGTH, &reg_start_address) != AI_ERROR_SUCCESS) {
			return AI_ERROR_GENERAL;
		}
		if (reg_start_address != g_ai_base_lnet_public.reg_start_address) {
			return AI_ERROR_GENERAL;
		}
		start = (start + AI_BASE_LNET_PACKET_REG_START_ADDR_LENGTH);
		/* get the reg_number */
		if (AiBaseLnetModbusDataGet(packet, start, AI_BASE_LNET_PACKET_REG_NUMBER_LENGTH, &reg_number) != AI_ERROR_SUCCESS) {
			return AI_ERROR_GENERAL;
		}
		if (reg_number != g_ai_base_lnet_public.reg_number) {
			return AI_ERROR_GENERAL;
		}
	}
	/* write hold reg */
	if (decode->command == AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REG) {
		/* get the reg_start_address */
		if (AiBaseLnetModbusDataGet(packet, start, AI_BASE_LNET_PACKET_REG_START_ADDR_LENGTH, &reg_start_address) != AI_ERROR_SUCCESS) {
			return AI_ERROR_GENERAL;
		}
		if (reg_start_address != g_ai_base_lnet_public.reg_start_address) {
			return AI_ERROR_GENERAL;
		}
		start = (start + AI_BASE_LNET_PACKET_REG_START_ADDR_LENGTH);
		reg_value_length = 2;
		reg_number = 1;
	}
	if ((decode->command == AI_BASE_LNET_PACKET_FCODE_READ_HOLD_REGS) ||
		(decode->command == AI_BASE_LNET_PACKET_FCODE_READ_INPUT_REGS)) {
		/* get the packete length */
		if (AiBaseLnetModbusDataGet(packet, start, AI_BASE_LNET_PACKET_LENGTH_LENGTH, &reg_value_length) != AI_ERROR_SUCCESS) {
			return AI_ERROR_GENERAL;
		}
		start = (start + AI_BASE_LNET_PACKET_LENGTH_LENGTH);
		if ((reg_value_length % 2) != 0) {
			return AI_ERROR_GENERAL;
		}
		reg_number = (reg_value_length / 2);
	}
    for (field_index = 0; field_index < AI_BASE_LNET_PACKET_FIELD_MAX; field_index++) {
        if (start == end)
            break;
		if (field_index >= reg_number)
			break;

        /* get reg value */
        if (AiBaseLnetModbusDataGet(packet, start, AI_BASE_LNET_PACKET_REG_VALUE_LENGTH, &data) != AI_ERROR_SUCCESS) {
            return AI_ERROR_PACKET;
        }
        start = (start + AI_BASE_LNET_PACKET_REG_VALUE_LENGTH);

        /* save the field information */
        decode->field[field_index].index = (g_ai_base_lnet_public.reg_start_address + field_index);
        decode->field[field_index].sub_index = 0;
        decode->field[field_index].data_type_size = AI_BASE_LNET_PACKET_DATA_TYPE_SIZE_SHORT;
        decode->field[field_index].data_number = 1;
        decode->field[field_index].value = data;
        decode->field[field_index].field_map = 0;
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
ERROR_T AiBaseLnetModbusMasterCommand(AI_BASE_LNET_PACKET_PTR packet, AI_BASE_LNET_PACKET_DECODE_PTR decode, UINT32_T inteface)
{
    ERROR_T result = AI_ERROR_GENERAL;
    UINT8_T command;
    UINT8_T flag;
    UINT8_T retcode;
    AI_BASE_LNET_INTERFACE_PTR inteface_ptr = (AI_BASE_LNET_INTERFACE_PTR)inteface;

    command = ((decode->command & AI_BASE_LNET_PACKET_COMMAND_MASK) >> AI_BASE_LNET_PACKET_COMMAND_SHIFT);
    flag = decode->from;
	retcode = decode->result;

    switch (command) {
    case AI_BASE_LNET_PACKET_FCODE_READ_HOLD_REGS:
	case AI_BASE_LNET_PACKET_FCODE_READ_INPUT_REGS:
        if (flag == AI_BASE_LNET_PACKET_COMMAND_FLAG_RECV) {
			if (retcode == AI_BASE_LNET_PACKET_COMMAND_RETCODE_SUCCESS) {
				result = AiBaseLnetModbusCommandReadResponse(packet, decode, inteface);
			} else {
				result = AI_ERROR_GENERAL;
			}
			inteface_ptr->sequence_recv_last = inteface_ptr->sequence_send_last;
			inteface_ptr->retcode_recv_last = result;
			result = AI_ERROR_NO_RESPONSE;
        }
        if (flag == AI_BASE_LNET_PACKET_COMMAND_FLAG_SEND) {
            result = AiBaseLnetModbusCommandReadSend(packet, decode, inteface);
        }
        break;
    case AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REG:
	case AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REGS:
        if (flag == AI_BASE_LNET_PACKET_COMMAND_FLAG_RECV) {
            if (retcode == AI_BASE_LNET_PACKET_COMMAND_RETCODE_SUCCESS) {
				result = AiBaseLnetModbusCommandWriteResponse(packet, decode, inteface);
			} else {
				result = AI_ERROR_GENERAL;
			}
			inteface_ptr->sequence_recv_last = inteface_ptr->sequence_send_last;
			inteface_ptr->retcode_recv_last = result;
			result = AI_ERROR_NO_RESPONSE;
        }
        if (flag == AI_BASE_LNET_PACKET_COMMAND_FLAG_SEND) {
            result = AiBaseLnetModbusCommandWriteSend(packet, decode, inteface);
        }
        break;
    default:
        result = AI_ERROR_GENERAL;
        break;
    }

    return result;
}

/**
 * brief    Ai base lnet modbus master encode.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetModbusMasterEncode(AI_BASE_LNET_PACKET_DECODE_PTR decode, ERROR_T retcode, AI_BASE_LNET_PACKET_PTR packet)
{
    UINT32_T start;
    UINT32_T end;
    UINT32_T field_index;
    UINT8_T command;
	UINT32_T index;
	UINT16_T reg_start_address;
	UINT16_T reg_number;

    AI_BASE_LNET_PACKET_FIELD_DECODE_PTR field;

    start = 0 + AI_BASE_LNET_PACKET_HEAD_LENGTH;
    end = 0 + AI_BASE_LNET_PACKET_HEAD_LENGTH;
    packet->start = start;
    packet->length = AI_BASE_LNET_LINK_BUFFER_LENGTH - AI_BASE_LNET_PACKET_HEAD_TAIL_LENGTH;

    /* set dest address */
    if (AiBaseLnetModbusDataSet(packet, end, AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH, decode->source_addr) != AI_ERROR_SUCCESS) {
        return AI_ERROR_GENERAL;
    }
    end = (end + AI_BASE_LNET_PACKET_DEST_ADDR_LENGTH);

    /* set function code */
    command = decode->command;
    if (decode->from == AI_BASE_LNET_PACKET_COMMAND_FLAG_SEND) {
        decode->command = ((command & AI_BASE_LNET_PACKET_COMMAND_MASK) >> AI_BASE_LNET_PACKET_COMMAND_SHIFT);
    } else {
        if (retcode != AI_ERROR_SUCCESS) {
            decode->command = ((((command & AI_BASE_LNET_PACKET_COMMAND_MASK) >> AI_BASE_LNET_PACKET_COMMAND_SHIFT)  + AI_BASE_LNET_PACKET_COMMAND_RESPONSE_INC) | 
							   ((AI_BASE_LNET_PACKET_COMMAND_RETCODE_ERROR << AI_BASE_LNET_PACKET_COMMAND_RETCODE_SHIFT) & AI_BASE_LNET_PACKET_COMMAND_RETCODE_MASK));
        } else {
            decode->command = (((command & AI_BASE_LNET_PACKET_COMMAND_MASK) >> AI_BASE_LNET_PACKET_COMMAND_SHIFT) + AI_BASE_LNET_PACKET_COMMAND_RESPONSE_INC);
        }
    }
    command = ((decode->command & AI_BASE_LNET_PACKET_COMMAND_MASK) >> AI_BASE_LNET_PACKET_COMMAND_SHIFT);
    if (AiBaseLnetModbusDataSet(packet, end, AI_BASE_LNET_PACKET_FCODE_LENGTH, decode->command) != AI_ERROR_SUCCESS) {
        return AI_ERROR_GENERAL;
    }
    end = (end + AI_BASE_LNET_PACKET_FCODE_LENGTH);

	/* set the error code and end error packet */
	if (retcode != AI_ERROR_SUCCESS) {
		decode->result = (UINT8_T)(AI_BASE_LNET_PACKET_ECODE_DATA_ERROR);
		if (AiBaseLnetModbusDataSet(packet, end, AI_BASE_LNET_PACKET_ECODE_LENGTH, decode->result) != AI_ERROR_SUCCESS) {
			return AI_ERROR_GENERAL;
		}
		end = (end + AI_BASE_LNET_PACKET_ECODE_LENGTH);

		packet->start = start;
		packet->length = end-start;
		return AI_ERROR_SUCCESS;
	}

	/* set the reg_start_address */
	reg_number = 0;
	for(index = 0; index < AI_BASE_LNET_PACKET_FIELD_MAX; index++) {
        if (decode->field[index].valid != AI_BASE_LNET_PACKET_FIELD_VALID) {
			break;
		}
        reg_number++;
    }
	if (reg_number == 0) {
		return AI_ERROR_GENERAL;
	}
	reg_start_address = decode->field[0].index;
	g_ai_base_lnet_public.reg_start_address = reg_start_address;
	g_ai_base_lnet_public.reg_number = reg_number;
	if (decode->from == AI_BASE_LNET_PACKET_COMMAND_FLAG_SEND) {
		if ((decode->command == AI_BASE_LNET_PACKET_FCODE_READ_HOLD_REGS) || (decode->command == AI_BASE_LNET_PACKET_FCODE_READ_INPUT_REGS) || 
			(decode->command == AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REG) || (decode->command == AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REGS)) {
			if (AiBaseLnetModbusDataSet(packet, end, AI_BASE_LNET_PACKET_REG_START_ADDR_LENGTH, reg_start_address) != AI_ERROR_SUCCESS) {
				return AI_ERROR_GENERAL;
			}
			end = (end + AI_BASE_LNET_PACKET_REG_START_ADDR_LENGTH);
		}
	}

	/* set the reg_number */
	if (decode->from == AI_BASE_LNET_PACKET_COMMAND_FLAG_SEND) {
		if ((decode->command == AI_BASE_LNET_PACKET_FCODE_READ_HOLD_REGS) || (decode->command == AI_BASE_LNET_PACKET_FCODE_READ_INPUT_REGS) || 
			(decode->command == AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REGS)) {
			if (AiBaseLnetModbusDataSet(packet, end, AI_BASE_LNET_PACKET_REG_NUMBER_LENGTH, reg_number) != AI_ERROR_SUCCESS) {
				return AI_ERROR_GENERAL;
			}
			end = (end + AI_BASE_LNET_PACKET_REG_NUMBER_LENGTH);
		}
	}

	/* end the request read_hold_regs and read_input_regs */
	if (decode->from == AI_BASE_LNET_PACKET_COMMAND_FLAG_SEND) {
		if ((decode->command == AI_BASE_LNET_PACKET_FCODE_READ_HOLD_REGS) || (decode->command == AI_BASE_LNET_PACKET_FCODE_READ_INPUT_REGS)) {
			packet->start = start;
			packet->length = end-start;
			return AI_ERROR_SUCCESS;
		}
	}

	/* set the reg value length */
	if (decode->from == AI_BASE_LNET_PACKET_COMMAND_FLAG_SEND) {
		if (decode->command == AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REGS) {
			if (AiBaseLnetModbusDataSet(packet, end, AI_BASE_LNET_PACKET_LENGTH_LENGTH, (reg_number*2)) != AI_ERROR_SUCCESS) {
				return AI_ERROR_GENERAL;
			}
			end = (end + AI_BASE_LNET_PACKET_LENGTH_LENGTH);
		}
	}

	/* set the reg value */
	if (decode->from == AI_BASE_LNET_PACKET_COMMAND_FLAG_SEND) {
		if ((decode->command == AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REG) || (decode->command == AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REGS)) {
			for (field_index = 0; field_index < AI_BASE_LNET_PACKET_FIELD_MAX; field_index++) {
				field = &(decode->field[field_index]);

				/* check valid */
				if (field->valid != AI_BASE_LNET_PACKET_FIELD_VALID)
					break;

                if (AiBaseLnetModbusDataSet(packet, end, AI_BASE_LNET_PACKET_REG_VALUE_LENGTH, (field->value)) != AI_ERROR_SUCCESS) {
					return AI_ERROR_GENERAL;
                }
                end = (end + AI_BASE_LNET_PACKET_REG_VALUE_LENGTH);
			}
		}
	}        

    packet->start = start;
    packet->length = end-start;
    return AI_ERROR_SUCCESS;
}

#endif
#endif

/*------------------------- End ---------------------------------------------*/
