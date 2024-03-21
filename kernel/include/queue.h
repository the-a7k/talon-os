#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <stdbool.h>
#include <stdint.h>

typedef uint8_t queue_element_t;

#define QUEUE_MAXSIZE 32

typedef struct {
	size_t front_pos;
	size_t back_pos;			
	queue_element_t data[QUEUE_MAXSIZE];
} queue_t;


void queue_init(queue_t *queue);
bool queue_is_empty(const queue_t *queue);
bool queue_is_full(const queue_t *queue);
bool queue_get_front(const queue_t *queue, queue_element_t *value);
bool queue_get_back(const queue_t *queue, queue_element_t *value);
bool queue_push(queue_t *queue, queue_element_t value);
bool queue_pop(queue_t *queue);
void queue_destroy(queue_t *queue);

#endif
