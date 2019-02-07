/*
 * Buffer.h
 *
 *  Created on: Jan 30, 2019
 *      Author: Miguel Elias
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include "derivative.h" /* include peripheral declarations */

#define BUFLEN 30

struct UART_Buffer {
	volatile uint32_t head;
	volatile uint32_t tail;
	volatile uint32_t capacity;
	volatile uint8_t data[BUFLEN];
};

typedef struct UART_Buffer bufferType;

void buffer_push(bufferType *bf, uint8_t data);
uint8_t buffer_pop(bufferType *bf);
uint32_t buffer_inc(uint32_t pt, uint32_t cap);
uint8_t buffer_is_full(bufferType *bf);
uint32_t buffer_len(bufferType *bf);


#endif /* BUFFER_H_ */
