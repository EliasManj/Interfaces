/*
 * buffer.c
 *
 *  Created on: Mar 29, 2019
 *      Author: Miguel Elias
 */

#include "buffer.h"

void buffer_init(bufferType *bf, uint16_t size){
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

uint8_t buffer_inc(uint8_t pointer, uint8_t size) {
	uint8_t i = pointer;
	if (++i >= size) {
		i = 0;
	}
	return i;
}

uint8_t buffer_isempty(bufferType *bf) {
	return bf->head == bf->tail;
}

uint8_t buffer_isfull(bufferType *bf) {
	return buffer_len(bf) == (bf->size - 1);
}

uint8_t buffer_len(bufferType *bf) {
	uint8_t len = bf->tail - bf->head;
	if (len < 0) {
		len += bf->size;
	}
	return len;
}

