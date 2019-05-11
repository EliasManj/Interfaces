/*
 * Camera.h
 *
 *  Created on: May 5, 2019
 *      Author: Miguel Elias
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include "derivative.h" /* include peripheral declarations */
#include "buffer.h"
#include "helper_functions.h"
#include "RGB.h"

/*
 * UART_TX -> PTC4
 * UART_RX -> PTC5
 * */
#define __NVIC_PRIORITY_SHIFT   4

#define IDLE			0
#define GET_VERSION		1
#define STOP_CFBUF		2
#define RECIVE_IMAGE	3
#define RESUME			4
#define GET_FBUF_LEN	5

//UART
#define UART0			0
#define UART1			1
#define UART3			3

//BAUD RATES
#define BAUD_9600		0
#define BAUD_14400		1
#define BAUD_38400		2

#define BAUD_9600_BDH 	0
#define BAUD_9600_BDL 	137

#define BAUD_38400_BDH 	0
#define BAUD_38400_BDL 	34

#define BAUD_14400_BDH 	8
#define BAUD_14400_BDL 	7

#define VER_SIZE 		12
#define IMAGE_SIZE		50000

struct Camera_Struct
{
	bufferType *Buffer_debug_console_pt;
	bufferType Buffer_debug_console;
	bufferType *Wifi_Buffer_Tx_pt;
	bufferType Wifi_Buffer_Tx;
	bufferType *Wifi_Buffer_Rx_pt;
	bufferType Wifi_Buffer_Rx;
	bufferType **buffer;
	uint8_t buffer_is_empty;
	//Camera state
	uint8_t camera_state;
	int camera_field_counter;
	int camera_delta_counter;
	char cam_version[VER_SIZE];
	uint8_t uart_channel;
	//Image
	char image_buffer[IMAGE_SIZE];
	int image_end_pointer;
	uint8_t image_buffering;
	uint8_t incoming_marker;
	//FBufer
	uint8_t parsing_fbuf_len;
	char buffer_len[4];
};

typedef struct Camera_Struct Camera;

//Camera functions
void Camera_Init(Camera *camera);
void Camera_GetVersion(Camera *camera, bufferType *bf);
void Camera_Snapshot(Camera *camera, bufferType *bf);
void Camera_Resume(Camera *camera, bufferType *bf);
void Camera_TestCmd(Camera *camera, bufferType *bf);
void Camera_GetFBufLen(Camera *camera, bufferType *bf);
void Camera_GetReadFBuf(Camera *camera, bufferType *bf);
void Camera_WaitForState(Camera *camera, int state);
void Camera_ResetProperties(Camera *camera);

//UART functions
void Camera_SetUART(Camera *camera, bufferType *bf, int uart_channel, int baud_rate);
void Camera_UART_SendByte(bufferType *bf, char byte);
void Camera_UART_SendString(Camera *camera, bufferType *bf, char *str);
void Camera_UART_SendString_Enable_Tx(Camera *camera, bufferType *bf, char *str);
void Camera_NVIC_SetPriority(int iInterruptID, unsigned char ucPriority);
void Camera_WaitEmptyBuffer(Camera *camera);
void Camera_UART_Enable_Rx(Camera *camera);
void Camera_UART0_Init_USB(int baud_rate);
void Camera_UART3_Init(int baud_rate);
void Camera_Uart_Set_Baud(int uart_channel, int baud_rate);
void Camera_UART_Enable_Tx(Camera *camera);

//Parser functions
void Camera_Parse_Byte(Camera *camera, char byte);
void Camera_Parse_GetVersion(Camera *camera, char byte);
void Camera_Parse_GetSnapshot(Camera *camera, char byte);
void Camera_Parse_StopCFBuf(Camera *camera, char byte);
void Camera_Parse_Resume(Camera *camera, char byte);
void Camera_Parse_GetFBufLen(Camera *camera, char byte);



#endif /* CAMERA_H_ */
