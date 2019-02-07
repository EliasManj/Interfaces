/*
 * Buffer.c
 *
 *  Created on: Jan 26, 2019
 *      Author: Miguel Elias
 */
#include "Buffer.h"

void buffer_push(bufferType *bf, uint8_t data){
	bf->data[bf->tail] = data;
	bf->tail = buffer_inc(bf->tail, bf->capacity);
}

uint8_t buffer_pop(bufferType *bf){
	uint8_t item = bf->data[bf->head];
	bf->head = buffer_inc(bf->head, bf->capacity);
	return item;
}

uint32_t buffer_inc(uint32_t pt, uint32_t cap){
	uint32_t  i = pt;
	if(++i >= cap){
		i = 0;
	}
	return i;
}

uint32_t buffer_len(bufferType *bf){
	if(bf->head > bf->tail){
		return bf->capacity - (bf->head - bf->tail);
	} 
	return bf->tail - bf->head;
}

uint8_t buffer_is_full(bufferType *bf){
	return buffer_len(bf) == bf->capacity;
}
