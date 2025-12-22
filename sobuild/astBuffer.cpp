#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "astBuffer.h"

astBuffer astBufferAlloc(int size)
{
    astBuffer buffer = (astBuffer)malloc(sizeof(StructastBuffer));
    buffer->size_ = size;
    buffer->data_ = (char *)malloc(sizeof(char) * size);
    buffer->data_size_ = 0;
    return buffer;
}
astBuffer astBufferFromastBuffer(astBuffer buffer)
{
    astBuffer ret = (astBuffer)malloc(sizeof(StructastBuffer));
    ret->size_ = buffer->size_;
    ret->data_ = buffer->data_;
    ret->data_size_ = buffer->data_size_;
    buffer->data_ = NULL;
    return ret;
}

astBuffer astBufferAllocFromastBuffer(astBuffer in_buffer)
{
    astBuffer buffer = (astBuffer)malloc(sizeof(StructastBuffer));
    buffer->size_ = in_buffer->size_;
    buffer->data_ = (char *)malloc(in_buffer->size_);
    buffer->data_size_ = in_buffer->data_size_;
    return buffer;
}

astBuffer astBufferFromStr(const char *data, int data_size)
{
    astBuffer buffer = (astBuffer)malloc(sizeof(StructastBuffer));
    buffer->size_ = data_size;
    buffer->data_ = (char *)data;
    buffer->data_size_ = data_size;
    return buffer;
}

void astBufferFree(astBuffer buffer)
{
    if (buffer != NULL)
    {
        if (buffer->data_ != NULL)
        {
            free(buffer->data_);
        }
        free(buffer);
        buffer = NULL;
    }
}

void astBufferCopy(astBuffer dst, astBuffer src)
{
    dst->data_size_ = src->data_size_;
    dst->size_ = src->size_;
    dst->data_ = (char *)malloc(src->size_);
    memcpy(dst->data_, src->data_, src->data_size_);
}

char *astBufferData(astBuffer buffer)
{
    return buffer->data_;
}

int astBufferSize(astBuffer buffer)
{
    return buffer->size_;
}

void astBufferSetDataSize(astBuffer buffer, int size)
{
    buffer->data_size_ = size;
}

int astBufferDataSize(astBuffer buffer)
{
    return buffer->data_size_;
}

bool astBufferPush(astBuffer buffer, const char *data, int data_size)
{
    if (buffer->data_size_ + data_size > buffer->size_)
    {
        return false;
    }
    memcpy(buffer->data_ + buffer->data_size_, data, data_size);
    buffer->data_size_ += data_size;
    return true;
}

void astBufferPop(astBuffer buffer, int size)
{
    int remain_len = buffer->data_size_ - size;
    if (remain_len >= 0)
    {
        memcpy(buffer->data_, buffer->data_ + size, remain_len);
        buffer->data_size_ = remain_len;
    }
    else
    {
        buffer->data_size_ = 0;
    }
}

void astBufferClear(astBuffer buffer)
{
    buffer->data_size_ = 0;
}

bool astBufferCompare(astBuffer buffer, astBuffer buffer2)
{
    if (buffer->data_size_ != buffer2->data_size_)
    {
        return false;
    }

    return strncmp(buffer->data_, buffer2->data_, buffer->data_size_) == 0;
}