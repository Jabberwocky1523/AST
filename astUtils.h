#ifndef __UTILS_H__
#define __UTILS_H__
#include "astBuffer.h"
#include "astBinaryChunk.h"
char *LoadFileContent(const char *file_name);

CBuffer LoadFileToCBuffer(const char *file_name);

void PrintAst(Prototype *proto);
#endif
