#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H
typedef struct {
  int first, last, size, max_size;
  void ** buffer;
} circular_buffer_t;

// Allocate and initialize the circular buffer structure
circular_buffer_t * circular_buffer_init(int size);

// Remove an element from circular buffer. When empty, block until
// an element is appended.
void * circular_buffer_get(circular_buffer_t * b);

// Read (and do not remove) an element from circular buffer. When
// empty, return NULL.
void * circular_buffer_read(circular_buffer_t * b);

// Append an element into circular buffer. When full, block until an
// element is removed.
int circular_buffer_put(circular_buffer_t * b, void * d);

int circular_buffer_size(circular_buffer_t * b);
int circular_buffer_maxsize(circular_buffer_t * b);
#endif
