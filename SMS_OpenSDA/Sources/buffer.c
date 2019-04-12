#include "Buffer.h"

void buffer_init(bufferType *bf, uint32_t size)
{
	bf->size = size;
}

void buffer_push(bufferType *bf, uint8_t data)
{
	if (bf->cnt < bf->size)
	{
		bf->data[bf->tail] = data;
		bf->tail = buffer_inc(bf->tail, bf->size);
		bf->cnt++;
	}
}

uint8_t buffer_pop(bufferType *bf)
{
	if (bf->cnt != 0)
	{
		uint8_t item = bf->data[bf->head];
		bf->head = buffer_inc(bf->head, bf->size);
		bf->cnt--;
		return item;
	}
	return 1;
}

uint32_t buffer_inc(uint32_t pointer, uint32_t size)
{
	uint8_t i = pointer;
	if (++i >= size)
	{
		i = 0;
	}
	return i;
}

uint8_t buffer_is_empty(bufferType *bf)
{
	return (bf->cnt == 0) || (bf->head == bf->tail);
}

uint8_t buffer_is_full(bufferType *bf)
{
	return bf->cnt >= bf->size;
}

uint32_t buffer_len(bufferType *bf)
{
	return bf->cnt;
}

void buffer_clear(bufferType *bf)
{
	bf->head = 0;
	bf->tail = 0;
	bf->cnt = 0;
}
