#ifndef __C_BUFFER_H__
#define __C_BUFFER_H__
#include <stdbool.h>

typedef struct
{
    int size_;
    int data_size_;
    char *data_;
} *astBuffer, StructastBuffer;

astBuffer astBufferAlloc(int size);

astBuffer astBufferFromastBuffer(astBuffer buffer);

astBuffer astBufferAllocFromastBuffer(astBuffer buffer);

astBuffer astBufferFromStr(const char *data, int data_size);

void astBufferFree(astBuffer buffer);

void astBufferCopy(astBuffer dst, astBuffer src);

char *astBufferData(astBuffer buffer);

int astBufferSize(astBuffer buffer);

void astBufferSetDataSize(astBuffer buffer, int size);

int astBufferDataSize(astBuffer buffer);

bool astBufferPush(astBuffer buffer, const char *data, int data_size);

void astBufferPop(astBuffer buffer, int size);

void astBufferClear(astBuffer buffer);

bool astBufferCompare(astBuffer buffer, astBuffer buffer2);

#endif
