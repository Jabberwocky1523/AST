
#include "BinaryChunk.h"
#include "BufferStream.h"
#include "Type.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

static CBufferStream buffer_stream;

static void BinaryChunkInit(CBuffer buffer);
static CBuffer BinaryChunkReadString();
static vector<int> BinaryChunkReadCode();
static ConstantType BinaryChunkReadConstant();
ConstantType *BinaryChunkReadConstants(int &len);
Upvalue *BinaryChunkReadUpvalues(int &len);
static vector<Prototype *> BinaryChunkReadProtos(CBuffer parent);
static vector<int> BinaryChunkReadLineInfos();
static vector<LocVar> BinaryChunkReadLocVars();
static vector<CBuffer> BinaryChunkReadUpvalueNames();
static Prototype *BinaryChunkReadProto(CBuffer parent);

Prototype *BinaryChunkUnDump(CBuffer buffer)
{
    BinaryChunkInit(buffer);
    // printf("%d\n", buffer_stream == NULL);
    if (!BinaryChunkCheckHead())
    {
        PANIC("CheckHeadError");
    }
    BinaryChunkReadByte();
    return BinaryChunkReadProto(NULL);
}

static void BinaryChunkInit(CBuffer buffer)
{
    assert(buffer_stream == NULL);
    buffer_stream = CBufferStreamAllocFromCBuffer(buffer);
}

static CBuffer BinaryChunkReadString()
{
    assert(buffer_stream != NULL);
    uint64_t len = CBufferStreamReadUInt8(buffer_stream);
    if (len == 0)
    {
        return NULL;
    }
    CBuffer buffer = NULL;
    if (len == 0xff)
    {
        len = CBufferStreamReadUInt64(buffer_stream);
    }

    buffer = CBufferAlloc((int)len - 1);
    CBufferStreamRead(buffer_stream, CBufferData(buffer), (int)len - 1);
    CBufferSetDataSize(buffer, (int)len - 1);
    return buffer;
}

bool BinaryChunkCheckHead()
{
    assert(buffer_stream != NULL);
    do
    {
        char buffer[10] = {0};
        int buffer_len = strlen(LUA_SIGNATURE);
        CBufferStreamRead(buffer_stream, buffer, buffer_len);
        if (strncmp(buffer, LUA_SIGNATURE, buffer_len) != 0)
        {
            printf("LUA_SIGNATURE error");
            break;
        }

        if (CBufferStreamReadUInt8(buffer_stream) != LUAC_VERSION)
        {
            printf("LUAC_VERSION error");
            break;
        }

        if (CBufferStreamReadUInt8(buffer_stream) != LUAC_FORMAT)
        {
            printf("LUAC_FORMAT error");
            break;
        }

        buffer_len = strlen(LUAC_DATA);
        CBufferStreamRead(buffer_stream, buffer, buffer_len);
        if (strncmp(buffer, LUAC_DATA, buffer_len) != 0)
        {
            printf("LUAC_DATA error");
            break;
        }

        if (CBufferStreamReadUInt8(buffer_stream) != CINT_SIZE)
        {
            printf("CINT_SIZE error");
            break;
        }

        if ((CBufferStreamReadUInt8(buffer_stream)) != CSIZET_SIZE)
        {
            printf("CSIZET_SIZE error");
            break;
        }

        if (CBufferStreamReadUInt8(buffer_stream) != INSTRUCTION_SIZE)
        {
            printf("INSTRUCTION_SIZE error");
            break;
        }

        if (CBufferStreamReadUInt8(buffer_stream) != LUA_INTEGER_SIZE)
        {
            printf("LUA_INTEGER_SIZE error");
            break;
        }

        if (CBufferStreamReadUInt8(buffer_stream) != LUA_NUMBER_SIZE)
        {
            printf("LUA_NUMBER_SIZE error");
            break;
        }

        if (CBufferStreamReadInt64(buffer_stream) != LUAC_INT)
        {
            printf("LUAC_INT error");
            break;
        }

        if (CBufferStreamReadDouble(buffer_stream) != LUAC_NUM)
        {
            printf("LUAC_NUM error");
            break;
        }
        return true;
    } while (0);
    return false;
}

static vector<int> BinaryChunkReadCode()
{
    uint32_t code_element_len = CBufferStreamReadUInt32(buffer_stream);
    vector<int> codes;
    for (int i = 0; i < code_element_len; i++)
    {
        codes.push_back(CBufferStreamReadUInt32(buffer_stream));
    }
    return codes;
}

static ConstantType BinaryChunkReadConstant()
{
    uint8_t tag = CBufferStreamReadUInt8(buffer_stream);
    ConstantType *type = (ConstantType *)malloc(sizeof(ConstantType));
    switch (tag)
    {
    case TAG_NIL:
        type->tag = CONSTANT_TAG_NIL;
        type->data.tag_nil = 0;
        break;
    case TAG_BOOLEAN:
        type->tag = CONSTANT_TAG_BOOLEAN;
        type->data.tag_boolean = (CBufferStreamReadUInt8(buffer_stream) != 0);
        break;
    case TAG_INTEGER:
        type->tag = CONSTANT_TAG_INTEGER;
        type->data.tag_integer = CBufferStreamReadUInt64(buffer_stream);
        break;
    case TAG_NUMBER:
        type->tag = CONSTANT_TAG_NUMBER;
        type->data.tag_number = CBufferStreamReadDouble(buffer_stream);
        break;
    case TAG_SHORT_STR:
    case TAG_LONG_STR:
        type->tag = CONSTANT_TAG_STR;
        type->data.tag_str = BinaryChunkReadString();
        break;
    default:
        assert(0);
        break;
    }
    return *type;
}

ConstantType *BinaryChunkReadConstants(int &len)
{
    uint32_t constant_element_len = CBufferStreamReadUInt32(buffer_stream);
    len = constant_element_len;
    ConstantType *constants = (ConstantType *)malloc(sizeof(ConstantType) * constant_element_len);
    for (int i = 0; i < constant_element_len; i++)
    {
        constants[i] = BinaryChunkReadConstant();
    }
    return constants;
}

Upvalue *BinaryChunkReadUpvalues(int &len)
{
    uint32_t upvalue_element_len = CBufferStreamReadUInt32(buffer_stream);
    Upvalue *temp = (Upvalue *)malloc(sizeof(Upvalue) * upvalue_element_len);
    for (int i = 0; i < upvalue_element_len; i++)
    {

        temp[i].Idx = CBufferStreamReadUInt8(buffer_stream);
        temp[i].Instack = CBufferStreamReadUInt8(buffer_stream);
    }
    return temp;
}

static vector<Prototype *> BinaryChunkReadProtos(CBuffer parent)
{
    uint32_t protos_element_len = CBufferStreamReadUInt32(buffer_stream);
    vector<Prototype *> protos;
    if (protos_element_len == 0)
    {
        return protos;
    }

    Prototype *temp;
    for (size_t i = 0; i < protos_element_len; ++i)
    {
        temp = BinaryChunkReadProto(parent);
        protos.push_back(temp);
    }
    return protos;
}

static vector<int> BinaryChunkReadLineInfos()
{
    uint32_t lineinfo_element_len = CBufferStreamReadUInt32(buffer_stream);
    vector<int> LineInfos;
    uint32_t *temp;
    for (size_t i = 0; i < lineinfo_element_len; ++i)
    {
        temp = (uint32_t *)malloc(sizeof(uint32_t));
        *temp = CBufferStreamReadUInt32(buffer_stream);
        LineInfos.push_back(*temp);
    }
    return LineInfos;
}

static vector<LocVar> BinaryChunkReadLocVars()
{
    uint32_t locvar_element_len = CBufferStreamReadUInt32(buffer_stream);
    vector<LocVar> locvars;
    LocVar *locvar;
    for (size_t i = 0; i < locvar_element_len; ++i)
    {
        locvar = (LocVar *)malloc(sizeof(LocVar));
        locvar->VarName = BinaryChunkReadString();
        locvar->StartPC = CBufferStreamReadUInt32(buffer_stream);
        locvar->EndPC = CBufferStreamReadUInt32(buffer_stream);
        locvars.push_back(*locvar);
    }
    return locvars;
}

static vector<CBuffer> BinaryChunkReadUpvalueNames()
{
    uint32_t upvaluenames_element_len = CBufferStreamReadUInt32(buffer_stream);
    vector<CBuffer> upvaluenames;
    CBuffer *temp;
    for (size_t i = 0; i < upvaluenames_element_len; ++i)
    {
        temp = (CBuffer *)malloc(sizeof(CBuffer *));
        *temp = BinaryChunkReadString();
        upvaluenames.push_back(*temp);
    }
    return upvaluenames;
}

static Prototype *BinaryChunkReadProto(CBuffer parent)
{
    CBuffer source = BinaryChunkReadString();
    if (source == NULL)
    {
        source = parent;
    }
    Prototype *proto = (Prototype *)malloc(sizeof(Prototype));
    proto->Source = source;
    proto->LineDefined = CBufferStreamReadUInt32(buffer_stream);
    proto->LastLineDefined = CBufferStreamReadUInt32(buffer_stream);
    proto->NumParams = CBufferStreamReadUInt8(buffer_stream);
    proto->IsVararg = CBufferStreamReadUInt8(buffer_stream);
    proto->MaxStackSize = CBufferStreamReadUInt8(buffer_stream);
    proto->Code = BinaryChunkReadCode();
    int len = 0;
    proto->constants = BinaryChunkReadConstants(len);
    proto->ConstantLen = len;
    proto->Upvalues = BinaryChunkReadUpvalues(len);
    proto->UpvaluesLen = len;
    proto->Protos = BinaryChunkReadProtos(parent);
    proto->LineInfo = BinaryChunkReadLineInfos();
    proto->LocVars = BinaryChunkReadLocVars();
    proto->UpvalueNames = BinaryChunkReadUpvalueNames();
    return proto;
}

uint8_t BinaryChunkReadByte()
{
    return CBufferStreamReadUInt8(buffer_stream);
}

uint64_t BinaryChunkReadLuaInteger()
{
    return CBufferStreamReadUInt64(buffer_stream);
}

double BinaryChunkReadLuaNumber()
{
    return CBufferStreamReadDouble(buffer_stream);
}
