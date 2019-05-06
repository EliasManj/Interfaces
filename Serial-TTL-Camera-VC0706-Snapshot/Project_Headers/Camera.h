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

/*
 * UART_TX -> PTC4
 * UART_RX -> PTC5
 * */
#define __NVIC_PRIORITY_SHIFT   4

#define IDLE			0
#define GET_VERSION		1
#define STOP_CFBUF		2
#define RECIVE_IMAGE		3

#define UART0			0
#define UART1			1

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
	char cam_version[12];
	uint8_t uart_channel;
};

typedef struct Camera_Struct Camera;

//Camera functions
void Camera_SetUART(Camera *camera, bufferType *bf, int uart_chanel);
void Camera_Init(Camera *camera);
void Camera_GetVersion(Camera *camera);
void Camera_StopCFBuf(Camera *camera);
void Camera_Snapshot(Camera *camera);


//UART functions
void Camera_UART_SendString(Camera *camera, char *str);
void Camera_UART_SendString_Enable_Tx(Camera *camera, char *str);
void Camera_NVIC_SetPriority(int iInterruptID, unsigned char ucPriority);
void Camera_WaitEmptyBuffer(Camera *camera);

//Parser functions
void Camera_Parse_Byte(Camera *camera, char byte);
void Camera_Parse_GetVersion(Camera *camera, char byte);
void Camera_UART0_Init_USB(void);

#endif /* CAMERA_H_ */