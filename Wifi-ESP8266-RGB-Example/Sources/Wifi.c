/*
 * Wifi.c
 *
 *  Created on: Apr 24, 2019
 *      Author: gonmigue
 */

#include "Wifi.h"

void Wifi_Router_Route(Wifi_Obj *Wifi_Obj)
{
	uint8_t led_state;
	Wifi_Obj->request_pending = 0;
	led_state = _parse_uri(Wifi_Obj->uri);
	switch (led_state)
	{
	case (RED):
		RGB(1, 0, 0);
		break;
	case (GREEN):
		RGB(0, 1, 0);
		break;
	case (BLUE):
		RGB(0, 0, 1);
		break;
	}
}

//Wifi Init
void Wifi_InitBuffers(Wifi_Obj *Wifi_Obj, uint32_t size)
{
	Wifi_Obj->wifi_mode = WIFI_CONF_MODE;
	Wifi_Obj->Wifi_Buffer_Tx_pt = &Wifi_Obj->Wifi_Buffer_Tx;
	Wifi_Obj->Wifi_Buffer_Rx_pt = &Wifi_Obj->Wifi_Buffer_Rx;
	Wifi_Obj->Buffer_debug_console_pt = &Wifi_Obj->Buffer_debug_console;
	buffer_init(Wifi_Obj->Buffer_debug_console_pt, size);
	buffer_init(Wifi_Obj->Wifi_Buffer_Tx_pt, size);
	buffer_init(Wifi_Obj->Wifi_Buffer_Rx_pt, size);
}

void Wifi_Init(Wifi_Obj *Wifi_Obj, bufferType *bf, uint8_t uart_channel)
{
	Wifi_Obj->uart_channel = uart_channel;
	Wifi_UART_Init_0();
	Wifi_UART_Init_3();
	Wifi_Router_Reset(Wifi_Obj);
	Wifi_Conf_CipMux(Wifi_Obj, bf);
	Wifi_Conf_Cipserver(Wifi_Obj, bf);
	Wifi_Conf_CIFSR(Wifi_Obj, bf);
}

//Wifi Request router functions
void Wifi_Router_Reset(Wifi_Obj *Wifi_Obj)
{
	Wifi_Obj->router_state = WIFI_ROUTER_IDLE;
	Wifi_Obj->router_field_counter = 0;
	Wifi_Obj->router_counter = 0;
	Wifi_Obj->request_pending = 0;
}

void Wifi_Router_ParseByte(Wifi_Obj *Wifi_Obj, char byte)
{
	Wifi_Obj->router_request[Wifi_Obj->router_counter++] = byte;
	switch (Wifi_Obj->router_state)
	{
	case (WIFI_ROUTER_IDLE):
		if (byte == '+')
		{
			Wifi_Obj->router_state = WIFI_ROUTER_IPD;
			Wifi_Obj->router_field_counter = 0;
		}
		break;
	case (WIFI_ROUTER_IPD):
		if (byte == ',' || Wifi_Obj->router_field_counter == 3)
		{
			Wifi_Obj->router_field_counter = 0;
			if (Wifi_Router_ValidateIPD(Wifi_Obj->router_ipd))
			{
				Wifi_Obj->router_state = WIFI_ROUTER_UNKNOWN_N0;
			}
			else
			{
				Wifi_Obj->router_state = WIFI_ROUTER_IDLE;
			}
		}
		else
		{
			Wifi_Obj->router_ipd[Wifi_Obj->router_field_counter++] = byte;
		}
		break;
	case (WIFI_ROUTER_UNKNOWN_N0):
		if (byte == ',')
		{
			Wifi_Obj->router_state = WIFI_ROUTER_UNKNOWN_N1;
		}
		break;
	case (WIFI_ROUTER_UNKNOWN_N1):
		if (byte == ':')
		{
			Wifi_Obj->router_state = WIFI_ROUTER_HTTPTYPE;
		}
		break;
	case (WIFI_ROUTER_HTTPTYPE):
		if (byte == '/')
		{
			Wifi_Obj->router_state = WIFI_ROUTER_URI;
		}
		break;
	case (WIFI_ROUTER_URI):
		if (byte == ' ')
		{
			Wifi_Obj->router_state = WIFI_ROUTER_IDLE;
			Wifi_Obj->router_field_counter = 0;
			Wifi_Obj->request_pending = 1;
		}
		else
		{
			Wifi_Obj->uri[Wifi_Obj->router_field_counter++] = byte;
		}
		break;
	}
}

//Wifi Configuration functions
void Wifi_Conf_ParseByte(Wifi_Obj *Wifi_Obj, char byte)
{
	Wifi_Obj->conf_debug_parsedbytes[Wifi_Obj->conf_counter++] = byte;
	switch (Wifi_Obj->conf_state)
	{
	case (WIFI_CONF_WAIT_PLUS):
		if (byte == '+')
		{
			Wifi_Obj->conf_state = WIFI_CONF_CHECK_CIFSR_IP;
		}
		break;
	case (WIFI_CONF_CHECK_CIFSR_IP):
		if (byte == ':')
		{
			Wifi_Obj->conf_state = WIFI_CONF_CHECK_IP_TYPE;
			Wifi_Obj->conf_field_counter = 0;
		}
		else
		{
			Wifi_Obj->conf_cifsr[Wifi_Obj->conf_field_counter++] = byte;
		}
		break;
	case (WIFI_CONF_CHECK_IP_TYPE):
		if (byte == ',')
		{
			Wifi_Obj->conf_field_counter = 0;
			if (Wifi_Conf_CheckIPType(Wifi_Obj->conf_ip_type) == WIFI_STAIP)
			{
				Wifi_Obj->conf_state = WIFI_CONF_PARSE_IP;
			}
			else
			{
				Wifi_Obj->conf_state = WIFI_CONF_WAIT_PLUS;
			}
		}
		else
		{
			Wifi_Obj->conf_ip_type[Wifi_Obj->conf_field_counter++] = byte;
		}
		break;
	case (WIFI_CONF_PARSE_IP):
		if (byte == '\r')
		{
			Wifi_Obj->conf_state = WIFI_CONF_IDLE;
			Wifi_Obj->wifi_mode = WIFI_SERVER_MODE;
			Wifi_Obj->conf_field_counter = 0;
		}
		else
		{
			Wifi_Obj->conf_static_ip_addr[Wifi_Obj->conf_field_counter++] = byte;
		}
		break;
	}
}

void Wifi_Conf_CipMux(Wifi_Obj *Wifi_Obj, bufferType *bf)
{
	Wifi_UART_SendString_Enable_Tx(Wifi_Obj, bf, cip_mux_str);
	Wifi_UART_WaitEmptyBuffer();
	delay(WIFI_NOPS);
}

void Wifi_Conf_Cipserver(Wifi_Obj *Wifi_Obj, bufferType *bf)
{
	Wifi_UART_SendString_Enable_Tx(Wifi_Obj, bf, cip_server_str);
	Wifi_UART_WaitEmptyBuffer();
	delay(WIFI_NOPS);
}

void Wifi_Conf_CIFSR(Wifi_Obj *Wifi_Obj, bufferType *bf)
{
	UART3_C2 |= (1 << 2);		//RE reciver enable
	Wifi_UART_SendString_Enable_Tx(Wifi_Obj, bf, cifsr_str);
	Wifi_Obj->conf_state = WIFI_CONF_WAIT_PLUS;
	Wifi_UART_WaitEmptyBuffer();
}

void Wifi_UART_WaitEmptyBuffer()
{
	while (wifi_pt->buffer_is_empty == 0)
		;
}

int Wifi_Conf_CheckIPType(char *str)
{
	if (_strcomp(str, "APIP", 4))
	{
		return WIFI_APIP;
	}
	else if (_strcomp(str, "APMAC", 5))
	{
		return WIFI_APMAC;
	}
	else if (_strcomp(str, "STAIP", 5))
	{
		return WIFI_STAIP;
	}
	else if (_strcomp(str, "STAMAC", 6))
	{
		return WIFI_STAMAC;
	}
	return 0;
}

int Wifi_Router_ValidateIPD(char *str)
{
	if (_strcomp(str, "IPD", 3))
	{
		return 1;
	}
	return 0;
}

void Wifi_Http_Send_Cmd_StartConnection(Wifi_Obj *Wifi_Obj, bufferType *bf, char *ip, char *port)
{
	Wifi_UART_SendString(bf, cip_start);
	Wifi_UART_SendString(bf, "\"");
	Wifi_UART_SendString(bf, ip);
	Wifi_UART_SendString(bf, "\"");
	Wifi_UART_SendString(bf, ",");
	Wifi_UART_SendString(bf, port);
	Wifi_UART_SendString_Enable_Tx(Wifi_Obj, bf, "\r\n");
	Wifi_UART_WaitEmptyBuffer();
	delay(WIFI_NOPS);
}

//Http requests functions
void Wifi_Http_Send_Request_Get(Wifi_Obj *Wifi_Obj, bufferType *bf, char *ip, char *port, char *uri)
{
	Wifi_Http_Send_Cmd_StartConnection(Wifi_Obj, bf, ip, port);
	Wifi_UART_SendString(bf, cip_send);
	Wifi_UART_SendString(bf, port);
	Wifi_UART_SendString_Enable_Tx(Wifi_Obj, bf, "\r\n");
	Wifi_UART_WaitEmptyBuffer();
	delay(WIFI_NOPS);
	Wifi_UART_SendString(bf, "GET ");
	Wifi_UART_SendString(bf, uri);
	Wifi_UART_SendString(bf, " HTTP/1.1\r\nHost: ");
	Wifi_UART_SendString(bf, ip);
	Wifi_UART_SendString_Enable_Tx(Wifi_Obj, bf, "\r\n\r\n");
	Wifi_UART_WaitEmptyBuffer();
	Wifi_Obj->wifi_mode = WIFI_SERVER_MODE;
}

void Wifi_Http_Send_Request_PostJson(Wifi_Obj *Wifi_Obj, bufferType *bf, char *ip, char *port, char *uri, char * keyword, char *content, int content_start_index, int content_end_index)
{
	int content_length, headers_length;
	content_length = Calculate_Content_Length(keyword, content, content_start_index, content_end_index);
	headers_length = Calculate_Headers_Length_Post(ip, uri, content_length);
	Wifi_Http_Send_Cmd_StartConnection(Wifi_Obj, bf, ip, port);
	Wifi_UART_WaitEmptyBuffer();
	Wifi_Http_Send_Request_JSONPostRequestSize(Wifi_Obj, bf, ip, uri, keyword, content, content_length, headers_length);
	Wifi_UART_WaitEmptyBuffer();
	delay(WIFI_NOPS);
	Wifi_UART_SendString(bf, "POST ");
	Wifi_UART_SendString(bf, uri);
	Wifi_UART_SendString(bf, " HTTP/1.1\r\nHost: ");
	Wifi_UART_SendString(bf, ip);
	Wifi_UART_SendString(bf, "\r\n");
	Wifi_UART_SendString(bf, "Content-Type: application/json");
	Wifi_UART_SendString(bf, "\r\n");
	Wifi_UART_SendString_Enable_Tx(Wifi_Obj, bf, "content-length: ");
	Wifi_UART_WaitEmptyBuffer();
	Wifi_Http_Send_Request_ContentLength(bf, keyword, content, content_length);
	Wifi_UART_SendString(bf, "\r\n");
	Wifi_UART_SendString_Enable_Tx(Wifi_Obj, bf, "\r\n");
	Wifi_UART_WaitEmptyBuffer();
	Wifi_UART_SendString(bf, "{\"");
	Wifi_UART_SendString(bf, keyword);
	Wifi_UART_SendString(bf, "\":");
	Wifi_UART_SendString_Enable_Tx(Wifi_Obj, bf, "\"");
	Wifi_UART_WaitEmptyBuffer();
	Wifi_UART_SubString(Wifi_Obj, bf, content, content_start_index, content_end_index);
	Wifi_UART_SendString(bf, "\"}");
	Wifi_UART_WaitEmptyBuffer();
	Wifi_UART_SendString_Enable_Tx(Wifi_Obj, bf, "\r\n");
	Wifi_UART_WaitEmptyBuffer();
}

void Wifi_Http_Prepare_Request_PostJson(Wifi_Obj *Wifi_Obj, bufferType *bf, char *ip, char *port, char *uri, char * keyword, char *image, int content_start_index, int content_end_index)
{
	int content_length, headers_length;
	content_length = Calculate_Content_Length(keyword, image, content_start_index, content_end_index);
	headers_length = Calculate_Headers_Length_Post(ip, uri, content_length);
	Add_To_PostRequest_Message(Wifi_Obj, "POST ");
	Add_To_PostRequest_Message(Wifi_Obj, uri);
	Add_To_PostRequest_Message(Wifi_Obj, " HTTP/1.1\r\nHost: ");
	Add_To_PostRequest_Message(Wifi_Obj, ip);
	Add_To_PostRequest_Message(Wifi_Obj, "\r\n");
	Add_To_PostRequest_Message(Wifi_Obj, "Content-Type: application/json");
	Add_To_PostRequest_Message(Wifi_Obj, "\r\n");
	Add_To_PostRequest_Message(Wifi_Obj, "content-length: ");
	Wifi_Http_Prepare_Request_PostJson_AddContentLength(Wifi_Obj, keyword, image, headers_length);
	Add_To_PostRequest_Message(Wifi_Obj, "\r\n");
	Add_To_PostRequest_Message(Wifi_Obj, "\r\n");
	Add_To_PostRequest_Message(Wifi_Obj, "{\"");
	Add_To_PostRequest_Message(Wifi_Obj, keyword);
	Add_To_PostRequest_Message(Wifi_Obj, "\":");
	Add_To_PostRequest_Message(Wifi_Obj, "\"");
	Add_To_PostRequest_Message_ImageChunk(Wifi_Obj, image, content_start_index, headers_length);
	Add_To_PostRequest_Message(Wifi_Obj, "\"}");
	Add_To_PostRequest_Message(Wifi_Obj, "\r\n");
}

void Wifi_Http_Send_Request_ContentLength(bufferType *bf, char *keyword, char *content, int content_length)
{
	int i;
	char content_length_str[10];
	for (i = 0; i < 10; i++)
	{
		content_length_str[i] = ' ';
	}
	itoa(content_length, content_length_str, 10);
	Wifi_UART_SendString_UntilEmpty(bf, content_length_str);
}

void Add_To_PostRequest_Message_ContentLength_2048(bufferType *bf, char *keyword, char *content, int content_length)
{
	int i;
	char content_length_str[10];
	for (i = 0; i < 10; i++)
	{
		content_length_str[i] = ' ';
	}
	itoa(content_length, content_length_str, 10);
	Wifi_UART_SendString_UntilEmpty(bf, content_length_str);
}

int Calculate_Content_Length(char *keyword, char *content, int content_start_index, int content_end_index)
{
	int content_length;
	content_length = 0;
	content_length += _strlen("{\"");
	content_length += _strlen(keyword);
	content_length += _strlen("\":\"");
	content_length += _strlen("\"}");
	content_length += content_end_index - content_start_index + 1;
	return content_length;
}

int Calculate_Headers_Length_Post(char *ip, char *uri, int content_length)
{
	int request_length = 0;
	int i;
	char content_length_str[10];
	for (i = 0; i < 10; i++)
	{
		content_length_str[i] = ' ';
	}
	itoa(content_length, content_length_str, 10);
	request_length += _strlen("POST ");
	request_length += _strlen(uri);
	request_length += _strlen(" HTTP/1.1\r\n");
	request_length += _strlen("Host: ");
	request_length += _strlen(ip);
	request_length += _strlen("\r\n");
	request_length += _strlen("Content-Type: application/json\r\n");
	request_length += _strlen("content-length: ");
	request_length += _strlen(content_length_str);
	request_length += _strlen("\r\n");
	request_length += _strlen("\r\n");
	request_length += content_length;
	return request_length;
}

void Wifi_Http_Send_Request_JSONPostRequestSize(Wifi_Obj *Wifi_Obj, bufferType *bf, char *ip, char *uri, char *keyword, char *content, int content_length, int headers_length)
{
	char request_length_str[10];
	int i;
	for (i = 0; i < 10; i++)
	{
		request_length_str[i] = ' ';
	}
	itoa(headers_length, request_length_str, 10);
	Wifi_UART_SendString(bf, cip_send);
	Wifi_UART_SendString_UntilEmpty(bf, request_length_str);
	Wifi_UART_SendString_Enable_Tx(Wifi_Obj, bf, "\r\n");
}

void Wifi_Http_Prepare_Request_PostJson_AddContentLength(Wifi_Obj *Wifi_Obj, char *keyword, char *content, int header_length)
{
	char json_length_str[10];
	int json_length, i;
	json_length = 2048 - header_length;
	for (i = 0; i < 10; i++)
	{
		json_length_str[i] = ' ';
	}
	itoa(json_length, json_length_str, 10);
	Add_To_PostRequest_Message_UntilEmpty(Wifi_Obj->post_request, json_length_str);

}

void Add_To_PostRequest_Message(Wifi_Obj *Wifi_Obj, char *str)
{
	int32_t i;
	for (i = 0; i < _strlen(str); i++)
	{
		Wifi_Obj->post_request[Wifi_Obj->post_request_index++] = str[i];
	}
}

void Add_To_PostRequest_Message_UntilEmpty(Wifi_Obj *Wifi_Obj, char *str)
{
	int32_t i;
	i = 0;
	char byte;
	byte = str[i];
	while (byte != ' ' || byte != 0)
	{
		if (byte == 0 || byte == ' ')
		{
			break;
		}
		else
		{
			Wifi_Obj->post_request[Wifi_Obj->post_request_index++] = byte;
		}
		i++;
		byte = str[i];
	}
	wifi_pt->buffer_is_empty = 0;
}

void Add_To_PostRequest_Message_ImageChunk(Wifi_Obj *Wifi_Obj, char *image, int image_start_index, int header_length)
{
	int json_length, i;
	json_length = 2048 - header_length;
	for (i = 0; i < json_length; i++)
	{
		Wifi_Obj->post_request[Wifi_Obj->post_request_index++] =  image[image_start_index+(Wifi_Obj->image_pointer++)];
	}
}

