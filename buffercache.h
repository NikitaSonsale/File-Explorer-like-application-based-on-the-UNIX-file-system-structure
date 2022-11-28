#pragma once

typedef struct Buffer buffer_t;
typedef struct BufferHeader bufferheader_t;

struct BufferHeader
{
    int device_number;
    int block_number;
    int status;
    char* p_data;
    buffer_t* prev_buffer_on_hashqueue;
    buffer_t* next_buffer_on_hashqueue;
    buffer_t* prev_buffer_on_freelist;
    buffer_t* next_buffer_on_freelist;
};

struct Buffer
{
    bufferheader_t buffer_header;
    char* data;
};


//hashqueue_t* p_hashqueue = NULL;