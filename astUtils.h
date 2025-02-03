#ifndef __UTILS_H__
#define __UTILS_H__
#include "astBuffer.h"
#include "astBinaryChunk.h"
#include "lib/libcomgo.h"
#define OPENANDREAD(fp, buffer, len)      \
    do                                    \
    {                                     \
        if (fp == NULL)                   \
        {                                 \
            printf("file not exists!\n"); \
            return NULL;                  \
        }                                 \
        fseek(fp, 0, SEEK_END);           \
        len = ftell(fp);                  \
        fseek(fp, 0, SEEK_SET);           \
        buffer = (char *)malloc(len);     \
        fread(buffer, len, 1, fp);        \
        fclose(fp);                       \
    } while (0)

char *LoadFileContent(const char *file_name);

astBuffer LoadFileToastBuffer(const char *file_name);
astBuffer LoadViaCodePath(const char *file_name);
void PrintOperand(uint32_t instruction);
void PrintAst(Prototype *proto);
#endif
