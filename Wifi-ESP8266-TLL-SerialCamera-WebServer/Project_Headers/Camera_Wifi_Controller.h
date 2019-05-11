/*
 * Camera_Wifi_Controller.h
 *
 *  Created on: May 11, 2019
 *      Author: Miguel Elias
 */

#ifndef CAMERA_WIFI_CONTROLLER_H_
#define CAMERA_WIFI_CONTROLLER_H_

void Controller_Send_Snapshot_POST(Wifi_Obj *Wifi_Obj, bufferType *wifi_bf, char *ip, char *port, char *uri, int uart_channel_wifi);

#endif /* CAMERA_WIFI_CONTROLLER_H_ */
