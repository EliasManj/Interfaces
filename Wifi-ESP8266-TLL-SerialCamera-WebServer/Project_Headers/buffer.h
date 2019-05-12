/*
 * buffer.h
 *
 *  Created on: Mar 29, 2019
 *      Author: Miguel Elias
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include "derivative.h" /* include peripheral declarations */

#define BUFLEN 20100

struct Buffer {
	volatile int head;
	volatile int tail;
	volatile int size;
	volatile char data[BUFLEN];
};

typedef struct Buffer bufferType;

void buffer_init(bufferType *bf, int size);
void buffer_push(bufferType *bf, char data);
char buffer_pop(bufferType *bf);
int buffer_inc(int pointer, int size);
int buffer_isempty(bufferType *bf);
int buffer_isfull(bufferType *bf);
int buffer_len(bufferType *bf);

#endif /* BUFFER_H_ */
