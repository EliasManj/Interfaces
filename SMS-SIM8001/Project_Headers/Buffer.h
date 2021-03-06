/*
 * Buffer.h
 *
 *  Created on: Jan 30, 2019
 *      Author: Miguel Elias
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include "derivative.h" /* include peripheral declarations */

#define BUFLEN 1000

struct UART_Buffer {
	volatile uint32_t head;
	volatile uint32_t tail;
	volatile uint32_t cnt;
	volatile uint32_t size;
	volatile uint8_t data[BUFLEN];
};

typedef struct UART_Buffer bufferType;

void buffer_push(bufferType *bf, uint8_t data);
void buffer_init(bufferType *bf, uint32_t size);
uint8_t buffer_pop(bufferType *bf);
uint32_t buffer_inc(uint32_t pt, uint32_t cap);
uint8_t buffer_is_full(bufferType *bf);
uint32_t buffer_len(bufferType *bf);
uint8_t buffer_is_empty(bufferType *bf);
uint8_t buffer_has_one(bufferType *bf);
void buffer_clear(bufferType *bf);

#endif /* BUFFER_H_ */
