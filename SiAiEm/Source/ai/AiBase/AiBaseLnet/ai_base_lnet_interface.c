/**
 ******************************************************************************
 * file    ai_base_tnet_interface.c
 * author  sunlingge
 * version V100
 * brief   the ai base lnet interface driver.
 ******************************************************************************
 */
/*------------------------- Include File ------------------------------------*/
#include "ai_base_global.h"

/*------------------------- Macro Definition --------------------------------*/

/*------------------------- Type Definition----------------------------------*/

/*------------------------- External Variable Definition --------------------*/

/*------------------------- Variable Definition------------------------------*/

/*------------------------- Function Definition -----------------------------*/
/**
 * brief    ai base lnet inteface init.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetInterfaceInit(AI_BASE_LNET_INTERFACE_PTR inteface,
        UINT32_T phy,
        AI_BASE_LNET_ADDR_PTR addr,
        UINT32_T decode,
        AI_BASE_LNET_LINK_RX link_rx_func,
        AI_BASE_LNET_LINK_TX link_tx_func,
        AI_BASE_LNET_PHY_TX_FUNC phy_tx_func,
        AI_BASE_LNET_PHY_TX_COMPLETED_FUNC phy_tx_completed_func) {
	memset((char *)inteface, 0, sizeof(AI_BASE_LNET_INTERFACE));
    AiBaseLnetLinkInit(&(inteface->link), phy, addr, decode, link_rx_func, link_tx_func, phy_tx_func, phy_tx_completed_func);
    inteface->net.brigde = (AI_BASE_LNET_INTERFACE_ADDR)(UINT64_T)NULL;

    return AI_ERROR_SUCCESS;
}

#if (AI_PRODUCT_OPTION_LNET_PACKET == 1)
/**
 * brief    Ai base lnet inteface Run.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetInterfaceRun(AI_BASE_LNET_INTERFACE_PTR inteface) {
    AI_BASE_LNET_PACKET_DECODE packet_decode;
    AI_BASE_LNET_PACKET packet;
    ERROR_T retcode;

    /* receive packet */
    if (AiBaseLnetPacketRx(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* receive escape */
    if (AiBaseLnetPacketRxEscape(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* receive head tail */
    if (AiBaseLnetPacketRxHeadTail(&(inteface->link), &packet) != AI_ERROR_SUCCESS) {
        return AI_ERROR_GENERAL;
    }

    /* receive decode */
    retcode = AiBaseLnetPacketDecode(&packet, &packet_decode);
    if (retcode == AI_ERROR_GENERAL)
        return AI_ERROR_GENERAL;

    /* deal */
    if (retcode == AI_ERROR_SUCCESS) {
        retcode = AiBaseLnetPacketCommand(&packet, &packet_decode, (VOID_T)inteface);
    }
	if (retcode == AI_ERROR_NO_RESPONSE) {
		return AI_ERROR_SUCCESS;
	}

    /* send encode */
    if (AiBaseLnetPacketEncode(&packet_decode, retcode, &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* send heat tail */
    if (AiBaseLnetPacketTxHeadTail(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* send escape */
    if (AiBaseLnetPacketTxEscape(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* send packet */
    if (AiBaseLnetPacketTx(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;


    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet inteface read request.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetInterfaceRequestRead(AI_BASE_LNET_INTERFACE_PTR inteface, UINT8_T source_addr, UINT8_T dest_addr, UINT32_T field_num, AI_BASE_LNET_PACKET_FIELD_READ_PTR field_read) {
    AI_BASE_LNET_PACKET_DECODE packet_decode;
    AI_BASE_LNET_PACKET packet;
    ERROR_T retcode;
    UINT32_T index;

    memset((char *)&packet_decode, 0, sizeof(AI_BASE_LNET_PACKET_DECODE));
    inteface->sequence_send++;
    packet_decode.sequence = inteface->sequence_send;
	inteface->sequence_send_last = inteface->sequence_send;
    packet_decode.command = AI_BASE_LNET_PACKET_COMMAND_READ_REQUEST;
	packet_decode.from = AI_BASE_LNET_PACKET_COMMAND_FLAG_SEND;
    packet_decode.dest_addr = source_addr;
    packet_decode.source_addr = dest_addr;
    packet_decode.result = AI_ERROR_SUCCESS;
	if (field_num == 0) {
		return AI_ERROR_GENERAL;
	}
	if (field_num > AI_BASE_LNET_PACKET_FIELD_MAX) {
		return AI_ERROR_GENERAL;
	}
    for(index = 0; index < field_num; index++) {
        packet_decode.field[index].valid = AI_BASE_LNET_PACKET_FIELD_VALID;
        packet_decode.field[index].index = field_read[index].index;
        packet_decode.field[index].sub_index = field_read[index].sub_index;
        packet_decode.field[index].data_number = field_read[index].data_number;
		packet_decode.field[index].value = 0;
		packet_decode.field[index].value_valid = AI_BASE_LNET_PACKET_DATA_VALUE_INVALID;
    }

    /* deal */
    retcode = AiBaseLnetPacketCommand(&packet, &packet_decode, (UINT32_T)(UINT64_T)inteface);

    /* send encode */
    if (AiBaseLnetPacketEncode(&packet_decode, retcode, &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* send heat tail */
    if (AiBaseLnetPacketTxHeadTail(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* send escape */
    if (AiBaseLnetPacketTxEscape(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* send packet */
    if (AiBaseLnetPacketTx(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;


    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet inteface write request.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetInterfaceRequestWrite(AI_BASE_LNET_INTERFACE_PTR inteface, UINT8_T source_addr, UINT8_T dest_addr, UINT32_T field_num, AI_BASE_LNET_PACKET_FIELD_WRITE_PTR field_write) {
    AI_BASE_LNET_PACKET_DECODE packet_decode;
    AI_BASE_LNET_PACKET packet;
    ERROR_T retcode;
    UINT32_T index;

    memset((char *)&packet_decode, 0, sizeof(AI_BASE_LNET_PACKET_DECODE));
    inteface->sequence_send++;
    packet_decode.sequence = inteface->sequence_send;
	inteface->sequence_send_last = inteface->sequence_send;
    packet_decode.command = AI_BASE_LNET_PACKET_COMMAND_WRITE_REQUEST;
	packet_decode.from = AI_BASE_LNET_PACKET_COMMAND_FLAG_SEND;
    packet_decode.dest_addr = source_addr;
    packet_decode.source_addr = dest_addr;
    packet_decode.result = AI_ERROR_SUCCESS;
	if (field_num == 0) {
		return AI_ERROR_GENERAL;
	}
	if (field_num > AI_BASE_LNET_PACKET_FIELD_MAX) {
		return AI_ERROR_GENERAL;
	}
    for(index = 0; index < field_num; index++) {
        packet_decode.field[index].valid = AI_BASE_LNET_PACKET_FIELD_VALID;
        packet_decode.field[index].index = field_write[index].index;
        packet_decode.field[index].sub_index = field_write[index].sub_index;
        packet_decode.field[index].data_number = field_write[index].data_number;
        packet_decode.field[index].value = field_write[index].value;
		packet_decode.field[index].value_valid = field_write[index].value_valid;
    }

    /* deal */
    retcode = AiBaseLnetPacketCommand(&packet, &packet_decode, (UINT32_T)(UINT64_T)inteface);

    /* send encode */
    if (AiBaseLnetPacketEncode(&packet_decode, retcode, &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* send heat tail */
    if (AiBaseLnetPacketTxHeadTail(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* send escape */
    if (AiBaseLnetPacketTxEscape(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* send packet */
    if (AiBaseLnetPacketTx(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;


    return AI_ERROR_SUCCESS;
}
#endif

#if (AI_PRODUCT_OPTION_MODBUS == 1)
#if (AI_PRODUCT_OPTION_MODBUS_MASTER == 1)
/**
 * brief    Ai base lnet inteface Run.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetInterfaceRun(AI_BASE_LNET_INTERFACE_PTR inteface) {
    AI_BASE_LNET_PACKET_DECODE packet_decode;
    AI_BASE_LNET_PACKET packet;
    ERROR_T retcode;
	UINT64_T tick;

    /* receive packet */
    if (AiBaseLnetModbusRx(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* receive escape */
    if (AiBaseLnetModbusRxEscape(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* receive head tail */
    if (AiBaseLnetModbusMasterRxHeadTail(&(inteface->link), &packet) != AI_ERROR_SUCCESS) {
		tick = AiBaseTickGet();
		if (tick > (inteface->link.rx_tick + AI_BASE_LNET_LINK_RX_TIMEOUT)) {
			inteface->link.rx_start = inteface->link.rx_end;
		}
        return AI_ERROR_GENERAL;
    }

    /* receive decode */
    retcode = AiBaseLnetModbusMasterDecode(&packet, &packet_decode);
    if (retcode == AI_ERROR_GENERAL)
        return AI_ERROR_GENERAL;

    /* deal */
    if (retcode == AI_ERROR_SUCCESS) {
        retcode = AiBaseLnetModbusMasterCommand(&packet, &packet_decode, (UINT32_T)inteface);
    }
	if (retcode == AI_ERROR_NO_RESPONSE) {
		return AI_ERROR_SUCCESS;
	}

    /* send encode */
    if (AiBaseLnetModbusMasterEncode(&packet_decode, retcode, &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* send heat tail */
    if (AiBaseLnetModbusTxHeadTail(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* send escape */
    if (AiBaseLnetModbusTxEscape(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* send packet */
    if (AiBaseLnetModbusTx(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;


    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet inteface read request.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetInterfaceRequestRead(AI_BASE_LNET_INTERFACE_PTR inteface, UINT8_T source_addr, UINT8_T dest_addr, UINT32_T field_num, AI_BASE_LNET_PACKET_FIELD_READ_PTR field_read) {
    AI_BASE_LNET_PACKET_DECODE packet_decode;
    AI_BASE_LNET_PACKET packet;
    ERROR_T retcode;
    UINT32_T index;
	UINT16_T modbus_index;
	UINT16_T modbus_number;
	UINT16_T modbus_number_total;
	UINT64_T sdk_sub_index;
	UINT8_T sdk_sub_data_type;
	UINT64_T sdk_value_min;
	UINT64_T sdk_value_max;
	UINT8_T sdk_category;
	UINT8_T sdk_operation;

    memset((char *)&packet_decode, 0, sizeof(AI_BASE_LNET_PACKET_DECODE));
    inteface->sequence_send++;
    packet_decode.sequence = inteface->sequence_send;
	inteface->sequence_send_last = inteface->sequence_send;
    packet_decode.command = AI_BASE_LNET_PACKET_COMMAND_READ_REQUEST;
	packet_decode.from = AI_BASE_LNET_PACKET_COMMAND_FLAG_SEND;
    packet_decode.dest_addr = source_addr;
    packet_decode.source_addr = dest_addr;
    packet_decode.result = AI_ERROR_SUCCESS;
	if (field_num == 0) {
		return AI_ERROR_GENERAL;
	}
	if (field_num > AI_BASE_LNET_PACKET_FIELD_MAX) {
		return AI_ERROR_GENERAL;
	}
	if (field_num != 1) {
		return AI_ERROR_GENERAL;
	}

	if (AiSdkIndexDataTypeGet(field_read->index, &sdk_sub_index, &sdk_sub_data_type, &sdk_value_min, &sdk_value_max, &sdk_category, &sdk_operation) != AI_ERROR_SUCCESS) {
		return AI_ERROR_GENERAL;
	}

	retcode = AiSdkInternalIndexToModbusIndex(field_read->index, field_read->sub_index, &modbus_index, &modbus_number);
	if (retcode != AI_ERROR_SUCCESS) {
		return AI_ERROR_GENERAL;
	}

	if ((sdk_operation & AI_DAT_OPERATE_VAR_MASK) == 0) {
		if ((sdk_operation & AI_DAT_OPERATE_STRING_MASK) > 0) {
			modbus_number_total = (UINT16_T)(modbus_number * sdk_sub_index);
		} else {
			modbus_number_total = modbus_number;
		}
	} else {
		if ((sdk_operation & AI_DAT_OPERATE_STRING_MASK) > 0) {
			modbus_number_total = (UINT16_T)(modbus_number * field_read->data_number);
		} else {
			modbus_number_total = (UINT16_T)(modbus_number * sdk_sub_index);
		}
	}

	if ((modbus_number_total < 1) || (modbus_number_total > AI_BASE_LNET_PACKET_FIELD_MAX)) {
		return AI_ERROR_GENERAL;
	}
	packet_decode.command = AI_BASE_LNET_PACKET_FCODE_READ_HOLD_REGS;
    for(index = 0; index < modbus_number_total; index++) {
        packet_decode.field[index].valid = AI_BASE_LNET_PACKET_FIELD_VALID;
        packet_decode.field[index].index = (modbus_index + index);
        packet_decode.field[index].sub_index = 0;
        packet_decode.field[index].data_number = 1;
		packet_decode.field[index].value = 0;
		packet_decode.field[index].value_valid = AI_BASE_LNET_PACKET_DATA_VALUE_INVALID;
    }

    /* deal */
    retcode = AiBaseLnetModbusMasterCommand(&packet, &packet_decode, (UINT32_T)inteface);

    /* send encode */
    if (AiBaseLnetModbusMasterEncode(&packet_decode, retcode, &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* send heat tail */
    if (AiBaseLnetModbusTxHeadTail(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* send escape */
    if (AiBaseLnetModbusTxEscape(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* send packet */
    if (AiBaseLnetModbusTx(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;


    return AI_ERROR_SUCCESS;
}

/**
 * brief    Ai base lnet inteface write request.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetInterfaceRequestWrite(AI_BASE_LNET_INTERFACE_PTR inteface, UINT8_T source_addr, UINT8_T dest_addr, UINT32_T field_num, AI_BASE_LNET_PACKET_FIELD_WRITE_PTR field_write) {
    AI_BASE_LNET_PACKET_DECODE packet_decode;
    AI_BASE_LNET_PACKET packet;
    ERROR_T retcode;
    UINT32_T index;
	UINT16_T modbus_index;
	UINT16_T modbus_number;
	UINT16_T modbus_number_total;
	UINT64_T sdk_sub_index;
	UINT8_T sdk_sub_data_type;
	UINT64_T sdk_value_min;
	UINT64_T sdk_value_max;
	UINT8_T sdk_category;
	UINT8_T sdk_operation;

    memset((char *)&packet_decode, 0, sizeof(AI_BASE_LNET_PACKET_DECODE));
    inteface->sequence_send++;
    packet_decode.sequence = inteface->sequence_send;
	inteface->sequence_send_last = inteface->sequence_send;
    packet_decode.command = AI_BASE_LNET_PACKET_COMMAND_WRITE_REQUEST;
	packet_decode.from = AI_BASE_LNET_PACKET_COMMAND_FLAG_SEND;
    packet_decode.dest_addr = source_addr;
    packet_decode.source_addr = dest_addr;
    packet_decode.result = AI_ERROR_SUCCESS;
	if (field_num == 0) {
		return AI_ERROR_GENERAL;
	}
	if (field_num > AI_BASE_LNET_PACKET_FIELD_MAX) {
		return AI_ERROR_GENERAL;
	}
	if (field_num != 1) {
		return AI_ERROR_GENERAL;
	}

	if (AiSdkIndexDataTypeGet(field_write->index, &sdk_sub_index, &sdk_sub_data_type, &sdk_value_min, &sdk_value_max, &sdk_category, &sdk_operation) != AI_ERROR_SUCCESS) {
		return AI_ERROR_GENERAL;
	}

	retcode = AiSdkInternalIndexToModbusIndex(field_write->index, field_write->sub_index, &modbus_index, &modbus_number);
	if (retcode != AI_ERROR_SUCCESS) {
		return AI_ERROR_GENERAL;
	}

	if ((sdk_operation & AI_DAT_OPERATE_VAR_MASK) == 0) {
		if ((sdk_operation & AI_DAT_OPERATE_STRING_MASK) > 0) {
			modbus_number_total = (UINT16_T)(modbus_number * sdk_sub_index);
		} else {
			modbus_number_total = modbus_number;
		}
	} else {
		if ((sdk_operation & AI_DAT_OPERATE_STRING_MASK) > 0) {
			modbus_number_total = (UINT16_T)(modbus_number * field_write->data_number);
		} else {
			modbus_number_total = (UINT16_T)(modbus_number * sdk_sub_index);
		}
	}

	if ((modbus_number_total < 1) || (modbus_number_total > AI_BASE_LNET_PACKET_FIELD_MAX)) {
		return AI_ERROR_GENERAL;
	}

	if (modbus_number_total == 1) {
		packet_decode.command = AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REG;
	} else {
		packet_decode.command = AI_BASE_LNET_PACKET_FCODE_WRITE_HOLD_REGS;
	}
    for(index = 0; index < modbus_number_total; index++) {
        packet_decode.field[index].valid = AI_BASE_LNET_PACKET_FIELD_VALID;
        packet_decode.field[index].index = (modbus_index + index);
        packet_decode.field[index].sub_index = 0;
        packet_decode.field[index].data_number = 1;

		if ((sdk_operation & AI_DAT_OPERATE_VAR_MASK) == 0) {
			if ((sdk_operation & AI_DAT_OPERATE_STRING_MASK) > 0) {
				packet_decode.field[index].value = ((UINT8_T *)((UINT32_T)field_write->value))[index];
			} else {
				packet_decode.field[index].value = ((field_write->value >> (((modbus_number_total-1)-index)*2*8)) & (0xFFFF));
			}
		} else {
			if ((sdk_operation & AI_DAT_OPERATE_STRING_MASK) > 0) {
				packet_decode.field[index].value = ((UINT8_T *)((UINT32_T)field_write->value))[index];
			} else {
				packet_decode.field[index].value = ((field_write->value >> (((modbus_number_total-1)-index)*2*8)) & (0xFFFF));
			}
		}		
		
		packet_decode.field[index].value_valid = AI_BASE_LNET_PACKET_DATA_VALUE_VALID;
    }

    /* deal */
    retcode = AiBaseLnetModbusMasterCommand(&packet, &packet_decode, (UINT32_T)inteface);

    /* send encode */
    if (AiBaseLnetModbusMasterEncode(&packet_decode, retcode, &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* send heat tail */
    if (AiBaseLnetModbusTxHeadTail(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* send escape */
    if (AiBaseLnetModbusTxEscape(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* send packet */
    if (AiBaseLnetModbusTx(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;


    return AI_ERROR_SUCCESS;
}
#endif
#endif

#if (AI_PRODUCT_OPTION_MODBUS == 1)
#if (AI_PRODUCT_OPTION_MODBUS_SLAVE == 1)
/**
 * brief    Ai base lnet inteface Run.
 * param    None
 * retval   None
 * author   Sunlingge
 * comment  V100
 */
ERROR_T AiBaseLnetInterfaceRun(AI_BASE_LNET_INTERFACE_PTR inteface) {
    AI_BASE_LNET_PACKET_DECODE packet_decode;
    AI_BASE_LNET_PACKET packet;
    ERROR_T retcode;
    UINT64_T tick;

    /* receive packet */
    if (AiBaseLnetModbusRx(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* receive escape */
    if (AiBaseLnetModbusRxEscape(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* receive head tail */
    if (AiBaseLnetModbusSlaveRxHeadTail(&(inteface->link), &packet) != AI_ERROR_SUCCESS) {
		tick = AiBaseTickGet();
		if (tick > (inteface->link.rx_tick + AI_BASE_LNET_LINK_RX_TIMEOUT)) {
			inteface->link.rx_start = inteface->link.rx_end;
		}
        return AI_ERROR_GENERAL;
    }

    /* receive decode */
    retcode = AiBaseLnetModbusSlaveDecode(&packet, &packet_decode);
    if (retcode == AI_ERROR_GENERAL)
        return AI_ERROR_GENERAL;

    /* deal */
    if (retcode == AI_ERROR_SUCCESS) {
        retcode = AiBaseLnetModbusSlaveCommand(&packet, &packet_decode, (UINT32_T)inteface);
    }
	if (retcode == AI_ERROR_NO_RESPONSE) {
		return AI_ERROR_SUCCESS;
	}

    /* send encode */
    if (AiBaseLnetModbusSlaveEncode(&packet_decode, retcode, &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* send heat tail */
    if (AiBaseLnetModbusTxHeadTail(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* send escape */
    if (AiBaseLnetModbusTxEscape(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;

    /* send packet */
    if (AiBaseLnetModbusTx(&(inteface->link), &packet) != AI_ERROR_SUCCESS)
        return AI_ERROR_GENERAL;


    return AI_ERROR_SUCCESS;
}
#endif
#endif

/*------------------------- End ---------------------------------------------*/
