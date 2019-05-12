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
 * UART1_TX -> PTC4
 * UART1_RX -> PTC3
 * ALT 3 -> 011
 * */
#define CAMERA__NVIC_PRIORITY_SHIFT   4

#define CAMERA_IDLE					0
#define CAMERA_GET_VERSION			1
#define CAMERA_STOP_CFBUF			2
#define CAMERA_RECIVE_IMAGE			3
#define CAMERA_RESUME				4
#define CAMERA_GET_FBUF_LEN			5

//UART
#define CAMERA_UART0			0
#define CAMERA_UART1			1
#define CAMERA_UART3			3

//BAUD RATES
#define CAMERA_BAUD_9600		0
#define CAMERA_BAUD_14400		1
#define CAMERA_BAUD_38400		2

#define CAMERA_BAUD_9600_BDH 	0
#define CAMERA_BAUD_9600_BDL 	137

#define CAMERA_BAUD_38400_BDH 	0
#define CAMERA_BAUD_38400_BDL 	34

#define CAMERA_BAUD_14400_BDH 	0
#define CAMERA_BAUD_14400_BDL 	7

#define CAMERA_VER_SIZE 		12
#define CAMERA_IMAGE_SIZE		50000

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
	char cam_version[CAMERA_VER_SIZE];
	uint8_t uart_channel;
	//Image
	char image_buffer[CAMERA_IMAGE_SIZE];
	int image_end_pointer;
	int image_start_pointer;
	uint8_t image_buffering;
	uint8_t incoming_jpg_marker;
	//FBufer
	uint8_t parsing_fbuf_len;
	char buffer_len[4];
	//Snapshot
	uint8_t take_snapshot;
};

typedef struct Camera_Struct Camera;

//Camera functions
void Camera_Init(Camera *camera);
void Camera_ResetProperties(Camera *camera);
void Camera_WaitForState(Camera *camera, int state);

//Camera commands
void Camera_Cmd_GetVersion(Camera *camera, bufferType *bf);
void Camera_Cmd_Snapshot(Camera *camera, bufferType *bf);
void Camera_Cmd_ResumeFrame(Camera *camera, bufferType *bf);
void Camera_Cmd_GetFBufLen(Camera *camera, bufferType *bf);
void Camera_Cmd_GetReadFBuf(Camera *camera, bufferType *bf);

//UART Init functios
void Camera_UART_Init(Camera *camera, int uart_channel, int baud_rate);
void Camera_UART_Init_0(int baud_rate);
void Camera_UART_Init_1(int baud_rate);
void Camera_UART_Init_3(int baud_rate);
void Camera_UART_NVIC_SetPriority(int iInterruptID, unsigned char ucPriority);
//UART Send functions
void Camera_UART_SendByte(bufferType *bf, char byte);
void Camera_UART_SendString(Camera *camera, bufferType *bf, char *str);
void Camera_UART_SendString_Enable_Tx(Camera *camera, bufferType *bf, char *str);
//UART Control functions
void Camera_UART_WaitEmptyBuffer(Camera *camera);
void Camera_UART_Enable_Tx(Camera *camera);
void Camera_UART_Enable_Rx(Camera *camera);

//Parser functions
void Camera_Parse_Byte(Camera *camera, char byte);
void Camera_Parse_GetVersion(Camera *camera, char byte);
void Camera_Parse_GetSnapshot(Camera *camera, char byte);
void Camera_Parse_StopCFBuf(Camera *camera, char byte);
void Camera_Parse_Resume(Camera *camera, char byte);
void Camera_Parse_GetFBufLen(Camera *camera, char byte);



#endif /* CAMERA_H_ */
