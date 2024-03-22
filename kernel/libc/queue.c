#include <stddef.h>
#include "../include/queue.h"

// Queue array implementation (aimed for keyboard.c, non-generic)

void queue_init(queue_t *queue) {
    if (queue)
        queue->front_pos = queue->back_pos = 0;
}


bool queue_is_empty(const queue_t *queue) {
    if (queue)
        return queue->front_pos == queue->back_pos;
    return true;
}


bool queue_is_full(const queue_t *queue) {
    return (queue->back_pos + 1) % QUEUE_MAXSIZE == queue->front_pos;
}


bool queue_get_front(const queue_t *queue, queue_element_t *value) {
    if (!queue_is_empty(queue) && value) {
        *value = queue->data[queue->front_pos];
        return true;
    }
    return false;
}


bool queue_get_back(const queue_t *queue, queue_element_t *value) {
    if (!value || queue_is_empty(queue))
        return false;

    const size_t last_pos = (queue->back_pos) ? (queue->back_pos - 1) : (QUEUE_MAXSIZE - 1);
    *value = queue->data[last_pos];
    return true;
}


bool queue_push(queue_t *queue, queue_element_t value) {
    if (!queue || queue_is_full(queue))
        return false;

    const size_t next_pos = (queue->back_pos + 1) % QUEUE_MAXSIZE;
    queue->data[queue->back_pos] = value;
    queue->back_pos = next_pos;
    return true;
}


bool queue_pop(queue_t *queue) {
    if (!queue_is_empty(queue)) {
        queue->front_pos = (queue->front_pos + 1) % QUEUE_MAXSIZE;
        return true;
    }
    return false;
}


bool queue_pop_back(queue_t *queue) {
    if (!queue_is_empty(queue)) {
        queue->back_pos = (queue->back_pos - 1) % QUEUE_MAXSIZE;
        return true;
    }
    return false;
}


void queue_destroy(queue_t *queue) {
    if (queue)
        queue->front_pos = queue->back_pos = 0;
}