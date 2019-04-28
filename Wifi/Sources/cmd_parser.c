#include "cmd_parser.h"
#include "Checks.h"

uint8_t parse_configuration_byte(Request *request, uint8_t byte) {
	request->conf_string[request->counter++] = byte;
	switch (request->current_state_conf) {
	case (IDLE_CONF):
		break;
	case (CIPMUX):
		break;
	case (OK0):
		break;
	case (CIPSERVER):
		break;
	case (OK1):
		break;
	case (CIFSR):
		break;
	case (WAITPLUS):
		break;
	case (PARSINGCIFSR):
		break;
	case (STATIC_IP):
		break;

	}
	return 0;
}

uint8_t parse_request_byte(Request *request, uint8_t byte) {
	request->request_string[request->counter++] = byte;
	switch (request->current_state) {
	case (IDLE):
		if (byte == '+') {
			request->current_state = WAITFORCOLON;
			request->field_counter = 0;
		}
		break;
	case (WAITFORCOLON):
		if (byte == ':') {
			request->current_state = WAITFORSLASH_OR_COMMA;
			request->max_wait = 0;
		} else {
			if (request->max_wait++ == 20) {
				request->max_wait = 0;
				request->current_state = IDLE;
			}
		}
		break;
	case (WAITFORSLASH_OR_COMMA):
		request->CIFSR_Type[request->field_counter++];
		if (byte == '/') {
			request->current_state = PARSING_URI;
			request->max_wait = 0;
		} else if (byte == ',') {
			request->current_state = PARSING_IP;
			request->max_wait = 0;
			uint8_t addr = Check_Address_Type(request->CIFSR_Type);
			if (addr == APIP || addr == APMAC || addr == STAMAC) {
				request->current_state = IDLE;
			}
			if (addr == STAIP) {
				request->current_state = PARSING_IP;
			}
		} else {
			if (request->max_wait++ == 20) {
				request->max_wait = 0;
				request->current_state = IDLE;
			}
		}
		break;
	case (PARSING_IP):
		if (byte == '\n') {
			request->current_state = IDLE;
		} else if (request->max_wait++ == 20) {
			request->max_wait = 0;
			request->current_state = IDLE;
		} else {
			request->static_ip_addr[request->field_counter++] = byte;
			return byte;
		}
		break;
	case (PARSING_URI):
		if (byte == ' ') {
			request->current_state = IDLE;
			request->led_state = parse_request(request);
			request->field_counter = 0;
			return 1;
		} else {
			request->request_string[request->field_counter++] = byte;
		}
		break;
	}
	return 0;
}

uint8_t parse_request(Request *request) {
	if (request->request_string[0] == 'L' && request->request_string[1] == 'E'
			&& request->request_string[2] == 'D'
			&& request->request_string[3] == '+') {
		if (request->request_string[4] == 'R') {
			return RED;
		} else if (request->request_string[4] == 'G') {
			return GREEN;
		} else if (request->request_string[4] == 'B') {
			return BLUE;
		}
	}
	return NONE;
}

uint8_t validate_ipd(uint8_t byte[]) {
	if (string_compare(byte, (uint8_t *) "IPD", 0XFF) == 1) {
		return 1;
	}
	return 0;
}

uint8_t is_digit_or_equals(uint8_t byte) {
	return byte == 0x30 || byte == 0x31 || byte == 0x32 || byte == 0x33
			|| byte == 0x34 || byte == 0x35 || byte == 0x36 || byte == 0x37
			|| byte == 0x38 || byte == 0x39 || byte == '=';
}

uint8_t string_compare(volatile uint8_t *array1, volatile uint8_t *array2,
		uint16_t until) {
	uint32_t i;
	uint8_t response = 0;
	i = 0;
	while (array1[i] == array2[i] && response == 0) {
		if (array1[i] == '\0' || array2[i] == '\0' || until == i) {
			response = 1;
		}
		i++;
	}
	return response;
}

void ConfigureCIP_MUX(bufferType *bf, Request *request) {
	request->current_state_conf = CIPMUX;
	buffer_push(bf, 'A');
	buffer_push(bf, 'T');
	buffer_push(bf, '+');
	buffer_push(bf, 'C');
	buffer_push(bf, 'I');
	buffer_push(bf, 'P');
	buffer_push(bf, 'M');
	buffer_push(bf, 'U');
	buffer_push(bf, 'X');
	buffer_push(bf, '=');
	buffer_push(bf, '1');
	buffer_push(bf, '\r');
	buffer_push(bf, '\n');
	WaitOK(request);
}

void WaitOK(Request *request) {
	request->waiting_for_okay = 1;
	UART3_C2 |= 0x80; //Turn on TX interrupt
	while (request->ok_signal != 1)
		;
	request->ok_signal = 0;
	UART3_C2 &= ~(0x80);
}

void request_parser_init(Request *request) {
	request->configurating = 1;
}

void CheckOkay(Request *request) {

}
