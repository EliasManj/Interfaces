/*
 * buffer.h
 *
 *  Created on: Mar 29, 2019
 *      Author: Miguel Elias
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include "derivative.h" /* include peripheral declarations */

#define BUFLEN 100

struct Buffer {
	volatile uint8_t head;
	volatile uint8_t tail;
	volatile uint8_t size;
	volatile char data[BUFLEN];
};

typedef struct Buffer bufferType;

void buffer_init(bufferType *bf, uint16_t size);
void buffer_push(bufferType *bf, char data);
char buffer_pop(bufferType *bf);
uint8_t buffer_inc(uint8_t pointer, uint8_t size);
uint8_t buffer_isempty(bufferType *bf);
uint8_t buffer_isfull(bufferType *bf);
uint8_t buffer_len(bufferType *bf);

#endif /* BUFFER_H_ */
