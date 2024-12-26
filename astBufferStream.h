#ifndef __C_BUFFER_STREAM_H__
#define __C_BUFFER_STREAM_H__
#include <stdbool.h>
#include <inttypes.h>
#include "astBuffer.h"
typedef struct
{
    int size_;
    char *data_;
    int write_pos_;
    int read_pos_;
} *astBufferStream, StructastBufferStream;

astBufferStream astBufferStreamAlloc(int size);

astBufferStream astBufferStreamAllocFromastBuffer(astBuffer buffer);

void astBufferStreamFree(astBufferStream buffer_stream);

char *astBufferStreamData(astBufferStream buffer_stream);

int astBufferStreamSize(astBufferStream buffer_stream);

bool astBufferStreamRead(astBufferStream buffer_stream, void *buffer, int buffer_size);

bool astBufferStreamPeep(astBufferStream buffer_stream, void *buffer, int buffer_size);

bool astBufferStreamReadToastBuffer(astBufferStream buffer_stream, astBuffer buffer);

bool astBufferStreamWrite(astBufferStream buffer_stream, void *buffer, int buffer_size);

int8_t astBufferStreamReadInt8(astBufferStream buffer_stream);

int16_t astBufferStreamReadInt16(astBufferStream buffer_stream);

int32_t astBufferStreamReadInt32(astBufferStream buffer_stream);

int64_t astBufferStreamReadInt64(astBufferStream buffer_stream);

uint8_t astBufferStreamReadUInt8(astBufferStream buffer_stream);

uint16_t astBufferStreamReadUInt16(astBufferStream buffer_stream);

uint32_t astBufferStreamReadUInt32(astBufferStream buffer_stream);

uint64_t astBufferStreamReadUInt64(astBufferStream buffer_stream);

float astBufferStreamReadFloat(astBufferStream buffer_stream);

double astBufferStreamReadDouble(astBufferStream buffer_stream);

bool astBufferStreamWriteInt8(astBufferStream buffer_stream, int8_t data);

bool astBufferStreamWriteInt16(astBufferStream buffer_stream, int16_t data);

bool astBufferStreamWriteInt32(astBufferStream buffer_stream, int32_t data);

bool astBufferStreamWriteInt64(astBufferStream buffer_stream, int64_t data);

bool astBufferStreamWriteUInt8(astBufferStream buffer_stream, uint8_t data);

bool astBufferStreamWriteUInt16(astBufferStream buffer_stream, uint16_t data);

bool astBufferStreamWriteUInt32(astBufferStream buffer_stream, uint32_t data);

bool astBufferStreamWriteUInt64(astBufferStream buffer_stream, uint64_t data);

bool astBufferStreamWriteFloat(astBufferStream buffer_stream, float data);

bool astBufferStreamWriteDouble(astBufferStream buffer_stream, double data);

#endif
