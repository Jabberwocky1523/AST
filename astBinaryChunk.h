#ifndef __BINARY_CHUNK_H__
#define __BINARY_CHUNK_H__
#include <inttypes.h>
#include "astBuffer.h"
#include "astBufferStream.h"
#include <stdbool.h>
#include "vector"
typedef struct
{
    unsigned char Instack;
    unsigned char Idx;
} UpvalueData;
typedef struct Upvalue {
    int len;
    UpvalueData *data;
} Upvalue;
typedef struct
{
    astBuffer VarName;
    uint32_t StartPC;
    uint32_t EndPC;
} LocVarData;
typedef struct LocVar {
    int Len;
    LocVarData *data;
} LocVar;

typedef struct
{
    unsigned char signature[4];
    unsigned char version;
    unsigned char format;
    unsigned char luaCData[6];
    unsigned char cintSize;
    unsigned char sizetSize;
    unsigned char instructionSize;
    unsigned char luaIntegerSize;
    unsigned char luaNumberSize;
    int64_t luacInt;
    double luacNum;
} Header;

enum ConstantTypeTag {
    CONSTANT_TAG_NONE = -1,
    CONSTANT_TAG_NIL = 0,
    CONSTANT_TAG_BOOLEAN = 1,
    CONSTANT_TAG_INTEGER = 2,
    CONSTANT_TAG_NUMBER = 3,
    CONSTANT_TAG_STR = 4,
};
typedef struct
{
    enum ConstantTypeTag tag;
    union {
        uint8_t tag_nil;
        uint8_t tag_boolean;
        uint64_t tag_integer;
        double tag_number;
        astBuffer tag_str;
    };
} Constant;
typedef struct ConstantType {
    int len;
    Constant *data;
} ConstantType;
typedef struct Code {
    int len;
    uint32_t *data;
} Code;
typedef struct LineInfo {
    int *LineInfo; // uint32_t
    int LineInfoLen;
} LineInfo;
typedef struct UpvalueNames {
    astBuffer *UpvalueNames; // astBuffer
    int upvalusNameslen;
} UpvalueNames;
typedef struct Prototype {
    astBuffer Source;
    uint32_t LineDefined;
    uint32_t LastLineDefined;
    unsigned char NumParams;
    unsigned char IsVararg;
    unsigned char MaxStackSize;
    struct Code Code;              // uint32_t
    struct ConstantType constants; // ConstantType
    struct Upvalue Upvalues;       // Upvalue
    struct Prototype **Protos;     // struct Prototype
    int ProtosLen;
    struct LineInfo LineInfo;
    struct LocVar LocVars; // LocVar
    struct UpvalueNames UpvalueNames;
} Prototype;

typedef struct
{
    Header header;
    unsigned char sizeUpvalues;
    Prototype *mainFunc;
} astBinaryChunk;

#define LUA_SIGNATURE "\x1bLua"
#define LUAC_VERSION 0x53
#define LUAC_FORMAT 0
#define LUAC_DATA "\x19\x93\r\n\x1a\n"
#define CINT_SIZE 4
#define CSIZET_SIZE 8
#define INSTRUCTION_SIZE 4
#define LUA_INTEGER_SIZE 8
#define LUA_NUMBER_SIZE 8
#define LUAC_INT 0x5678
#define LUAC_NUM 370.5f

#define TAG_NIL 0x00
#define TAG_BOOLEAN 0x01
#define TAG_NUMBER 0x03
#define TAG_INTEGER 0x13
#define TAG_SHORT_STR 0x04
#define TAG_LONG_STR 0x14

bool astBinaryChunkCheckHead(astBufferStream buffer_stream);

Prototype *astBinaryChunkUnDump(astBuffer buffer);

uint8_t astBinaryChunkReadByte(astBufferStream buffer_stream);

uint64_t astBinaryChunkReadLuaInteger(astBufferStream buffer_stream);

double astBinaryChunkReadLuaNumber(astBufferStream buffer_stream);

#endif
