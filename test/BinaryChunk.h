#ifndef __BINARY_CHUNK_H__
#define __BINARY_CHUNK_H__
#include <inttypes.h>
#include "Buffer.h"
#include <stdbool.h>
#include "vector"
using namespace std;

typedef struct
{
    unsigned char Instack;
    unsigned char Idx;
} Upvalue;

typedef struct
{
    CBuffer VarName;
    uint32_t StartPC;
    uint32_t EndPC;
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

enum ConstantTypeTag
{
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
    union
    {
        uint8_t tag_nil;
        uint8_t tag_boolean;
        uint64_t tag_integer;
        double tag_number;
        CBuffer tag_str;
    } data;
} ConstantType;

typedef struct Prototype
{
    CBuffer Source;
    uint32_t LineDefined;
    uint32_t LastLineDefined;
    unsigned char NumParams;
    unsigned char IsVararg;
    unsigned char MaxStackSize;
    vector<int> Code;        // uint32_t
    ConstantType *constants; // ConstantType
    int ConstantLen = 0;
    Upvalue *Upvalues; // Upvalue
    int UpvaluesLen = 0;
    vector<struct Prototype *> Protos; // struct Prototype
    vector<int> LineInfo;              // uint32_t
    vector<LocVar> LocVars;            // LocVar
    vector<CBuffer> UpvalueNames;      // CBuffer
} Prototype;

typedef struct
{
    Header header;
    unsigned char sizeUpvalues;
    Prototype *mainFunc;
} BinaryChunk;

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

bool BinaryChunkCheckHead();

Prototype *BinaryChunkUnDump(CBuffer buffer);

uint8_t BinaryChunkReadByte();

uint64_t BinaryChunkReadLuaInteger();

double BinaryChunkReadLuaNumber();

#endif
