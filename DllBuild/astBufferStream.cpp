#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "astBufferStream.h"

#define BUFFER_STREAM_CAN_READ(pStream, len) (pStream->size_ - pStream->read_pos_ >= len)

#define BUFFER_STREAM_CAN_WRITE(pStream, len) (pStream->size_ - pStream->write_pos_ >= len)

#define BUFFER_STREAM_PUSH(pStream, len) (pStream->write_pos_ += len)

#define BUFFER_STREAM_POP(pStream, len) (pStream->read_pos_ += len)

astBufferStream astBufferStreamAlloc(int size)
{
    astBufferStream buffer_stream = (astBufferStream)malloc(sizeof(StructastBufferStream));
    buffer_stream->size_ = size;
    buffer_stream->data_ = (char *)malloc(sizeof(char) * size);
    buffer_stream->write_pos_ = 0;
    buffer_stream->read_pos_ = 0;
    return buffer_stream;
}

astBufferStream astBufferStreamAllocFromastBuffer(astBuffer buffer)
{
    astBufferStream buffer_stream = (astBufferStream)malloc(sizeof(StructastBufferStream));
    buffer_stream->size_ = buffer->size_;
    buffer_stream->data_ = buffer->data_;
    buffer_stream->write_pos_ = 0;
    buffer_stream->read_pos_ = 0;
    buffer_stream->write_pos_ = buffer->data_size_;
    buffer->data_ = NULL;
    return buffer_stream;
}

void astBufferStreamFree(astBufferStream buffer_stream)
{
    if (buffer_stream != NULL)
    {
        if (buffer_stream->data_ != NULL)
        {
            free(buffer_stream->data_);
        }
        free(buffer_stream);
        buffer_stream = NULL;
    }
}

char *astBufferStreamData(astBufferStream buffer_stream)
{
    return buffer_stream->data_;
}

int astBufferStreamSize(astBufferStream buffer_stream)
{
    return buffer_stream->size_;
}

bool astBufferStreamRead(astBufferStream buffer_stream, void *buffer, int buffer_size)
{
    if (BUFFER_STREAM_CAN_READ(buffer_stream, buffer_size))
    {
        memcpy(buffer, buffer_stream->data_ + buffer_stream->read_pos_, buffer_size);
        BUFFER_STREAM_POP(buffer_stream, buffer_size);
        return true;
    }
    return false;
}

bool astBufferStreamReadToastBuffer(astBufferStream buffer_stream, astBuffer buffer)
{
    return astBufferStreamRead(buffer_stream, buffer->data_, buffer->size_);
}

bool astBufferStreamWrite(astBufferStream buffer_stream, void *buffer, int buffer_size)
{
    if (BUFFER_STREAM_CAN_WRITE(buffer_stream, buffer_size))
    {
        memcpy(buffer_stream->data_ + buffer_stream->write_pos_, buffer, buffer_size);
        BUFFER_STREAM_PUSH(buffer_stream, buffer_size);
        return true;
    }
    return false;
}

int8_t astBufferStreamReadInt8(astBufferStream buffer_stream)
{
    int8_t ret = 0;
    astBufferStreamRead(buffer_stream, &ret, sizeof(ret));
    return ret;
}

int16_t astBufferStreamReadInt16(astBufferStream buffer_stream)
{
    int16_t ret = 0;
    astBufferStreamRead(buffer_stream, &ret, sizeof(ret));
    return ret;
}

int32_t astBufferStreamReadInt32(astBufferStream buffer_stream)
{
    int32_t ret = 0;
    astBufferStreamRead(buffer_stream, &ret, sizeof(ret));
    return ret;
}

int64_t astBufferStreamReadInt64(astBufferStream buffer_stream)
{
    int64_t ret = 0;
    astBufferStreamRead(buffer_stream, &ret, sizeof(ret));
    return ret;
}

uint8_t astBufferStreamReadUInt8(astBufferStream buffer_stream)
{
    uint8_t ret = 0;
    astBufferStreamRead(buffer_stream, &ret, sizeof(ret));
    return ret;
}

uint16_t astBufferStreamReadUInt16(astBufferStream buffer_stream)
{
    uint16_t ret = 0;
    astBufferStreamRead(buffer_stream, &ret, sizeof(ret));
    return ret;
}

uint32_t astBufferStreamReadUInt32(astBufferStream buffer_stream)
{
    uint32_t ret = 0;
    astBufferStreamRead(buffer_stream, &ret, sizeof(ret));
    return ret;
}

uint64_t astBufferStreamReadUInt64(astBufferStream buffer_stream)
{
    uint64_t ret = 0;
    astBufferStreamRead(buffer_stream, &ret, sizeof(ret));
    return ret;
}

float astBufferStreamReadFloat(astBufferStream buffer_stream)
{
    float ret = 0;
    astBufferStreamRead(buffer_stream, &ret, sizeof(ret));
    return ret;
}

double astBufferStreamReadDouble(astBufferStream buffer_stream)
{
    double ret = 0;
    astBufferStreamRead(buffer_stream, &ret, sizeof(ret));
    return ret;
}

bool astBufferStreamWriteInt8(astBufferStream buffer_stream, int8_t data)
{
    return astBufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool astBufferStreamWriteInt16(astBufferStream buffer_stream, int16_t data)
{
    return astBufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool astBufferStreamWriteInt32(astBufferStream buffer_stream, int32_t data)
{
    return astBufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool astBufferStreamWriteInt64(astBufferStream buffer_stream, int64_t data)
{
    return astBufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool astBufferStreamWriteUInt8(astBufferStream buffer_stream, uint8_t data)
{
    return astBufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool astBufferStreamWriteUInt16(astBufferStream buffer_stream, uint16_t data)
{
    return astBufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool astBufferStreamWriteUInt32(astBufferStream buffer_stream, uint32_t data)
{
    return astBufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool astBufferStreamWriteUInt64(astBufferStream buffer_stream, uint64_t data)
{
    return astBufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool astBufferStreamWriteFloat(astBufferStream buffer_stream, float data)
{
    return astBufferStreamWrite(buffer_stream, &data, sizeof(data));
}

bool astBufferStreamWriteDouble(astBufferStream buffer_stream, double data)
{
    return astBufferStreamWrite(buffer_stream, &data, sizeof(data));
}
