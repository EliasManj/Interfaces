/*
 * buffer.c
 *
 *  Created on: Mar 29, 2019
 *      Author: Miguel Elias
 */

#include "buffer.h"

void buffer_init(bufferType *bf, int size){
	bf->size = 0;
	bf->head = 0;
	bf->size = size;
}

void buffer_push(bufferType *bf, char data) {
	bf->data[bf->tail] = data;
	bf->tail = buffer_inc(bf->tail, bf->size);
}

char buffer_pop(bufferType *bf) {
	uint8_t item = bf->data[bf->head];
	bf->head = buffer_inc(bf->head, bf->size);
	return item;
}

int buffer_inc(int pointer, int size) {
	int i = pointer;
	if (++i >= size) {
		i = 0;
	}
	return i;
}

int buffer_isempty(bufferType *bf) {
	return bf->head == bf->tail;
}

int buffer_isfull(bufferType *bf) {
	return buffer_len(bf) == (bf->size - 1);
}

int buffer_len(bufferType *bf) {
	int len = bf->tail - bf->head;
	if (len < 0) {
		len += bf->size;
	}
	return len;
}

