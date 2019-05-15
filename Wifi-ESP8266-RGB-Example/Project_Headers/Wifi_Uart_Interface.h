/*
 * Wifi_Uart_Interface.h
 *
 *  Created on: May 14, 2019
 *      Author: Miguel Elias
 */

#ifndef WIFI_UART_INTERFACE_H_
#define WIFI_UART_INTERFACE_H_

#include "Wifi.h"

//UART init functions
void Wifi_UART_Init(Wifi_Obj *Wifi_Obj, int uart_channel);
void Wifi_UART_Init_3(void);
void Wifi_UART_Init_0(void);

//UART functions
void Wifi_UART_SendString_Enable_Tx(Wifi_Obj *Wifi_Obj, bufferType *bf, char *str);
void Wifi_UART_SendString(bufferType *bf, char *str);
void Wifi_UART_SendString_UntilEmpty(bufferType *bf, char *str);
void Wifi_UART_SubString(Wifi_Obj *Wifi_Obj, bufferType *bf, char *str, int start, int end);
void Wifi_UART_WaitEmptyBuffer();
void Wifi_Enable_Tx(Wifi_Obj *Wifi_Obj);

void Wifi_NVIC_SetPriority(int iInterruptID, unsigned char ucPriority);

#endif /* WIFI_UART_INTERFACE_H_ */
