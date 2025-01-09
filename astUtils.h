#ifndef __UTILS_H__
#define __UTILS_H__
#include "astBuffer.h"
#include "astBinaryChunk.h"
char *LoadFileContent(const char *file_name);

astBuffer LoadFileToastBuffer(const char *file_name);
void PrintOperand(uint32_t instruction);
void PrintAst(Prototype *proto);
#endif
