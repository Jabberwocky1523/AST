
#include "astBinaryChunk.h"
#include "astBufferStream.h"
#include "ast.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

CBuffer BinaryChunkReadString(CBufferStream buffer_stream);
vector<int> BinaryChunkReadCode(CBufferStream buffer_stream);
ConstantType BinaryChunkReadConstant(CBufferStream buffer_stream);
vector<ConstantType> BinaryChunkReadConstants(CBufferStream buffer_stream);
vector<Upvalue> BinaryChunkReadUpvalues(CBufferStream buffer_stream);
vector<Prototype *> BinaryChunkReadProtos(CBuffer parent, CBufferStream buffer_stream);
vector<int> BinaryChunkReadLineInfos(CBufferStream buffer_stream);
vector<LocVar> BinaryChunkReadLocVars(CBufferStream buffer_stream);
vector<CBuffer> BinaryChunkReadUpvalueNames(CBufferStream buffer_stream);
Prototype *BinaryChunkReadProto(CBuffer parent, CBufferStream buffer_stream);

Prototype *BinaryChunkUnDump(CBuffer buffer)
{
    CBufferStream buffer_stream = (CBufferStream)malloc(sizeof(StructCBufferStream));
    buffer_stream->size_ = buffer->size_;
    buffer_stream->data_ = buffer->data_;
    buffer_stream->read_pos_ = 0;
    buffer_stream->write_pos_ = buffer->data_size_;
    printf(" \b");
    if (!BinaryChunkCheckHead(buffer_stream))
    {
        PANIC("CheckHeadError");
    }
    BinaryChunkReadByte(buffer_stream);
    return BinaryChunkReadProto(NULL, buffer_stream);
}

CBuffer BinaryChunkReadString(CBufferStream buffer_stream)
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

bool BinaryChunkCheckHead(CBufferStream buffer_stream)
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

vector<int> BinaryChunkReadCode(CBufferStream buffer_stream)
{
    uint32_t code_element_len = CBufferStreamReadUInt32(buffer_stream);
    vector<int> codes;
    for (int i = 0; i < code_element_len; i++)
    {
        codes.push_back(CBufferStreamReadUInt32(buffer_stream));
    }
    return codes;
}

ConstantType BinaryChunkReadConstant(CBufferStream buffer_stream)
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
        type->data.tag_str = BinaryChunkReadString(buffer_stream);
        break;
    default:
        assert(0);
        break;
    }
    return *type;
}

vector<ConstantType> BinaryChunkReadConstants(CBufferStream buffer_stream)
{
    uint32_t constant_element_len = CBufferStreamReadUInt32(buffer_stream);
    vector<ConstantType> Constants;
    for (int i = 0; i < constant_element_len; i++)
    {
        Constants.push_back(BinaryChunkReadConstant(buffer_stream));
    }
    return Constants;
}

vector<Upvalue> BinaryChunkReadUpvalues(CBufferStream buffer_stream)
{
    uint32_t upvalue_element_len = CBufferStreamReadUInt32(buffer_stream);
    vector<Upvalue> upvalues;
    for (int i = 0; i < upvalue_element_len; i++)
    {
        Upvalue *temp = (Upvalue *)malloc(sizeof(Upvalue));
        temp->Idx = CBufferStreamReadUInt8(buffer_stream);
        temp->Instack = CBufferStreamReadUInt8(buffer_stream);
        upvalues.push_back(*temp);
    }
    return upvalues;
}

vector<Prototype *> BinaryChunkReadProtos(CBuffer parent, CBufferStream buffer_stream)
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
        temp = BinaryChunkReadProto(parent, buffer_stream);
        protos.push_back(temp);
    }
    return protos;
}

vector<int> BinaryChunkReadLineInfos(CBufferStream buffer_stream)
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

vector<LocVar> BinaryChunkReadLocVars(CBufferStream buffer_stream)
{
    uint32_t locvar_element_len = CBufferStreamReadUInt32(buffer_stream);
    vector<LocVar> locvars;
    LocVar *locvar;
    for (size_t i = 0; i < locvar_element_len; ++i)
    {
        locvar = (LocVar *)malloc(sizeof(LocVar));
        locvar->VarName = BinaryChunkReadString(buffer_stream);
        locvar->StartPC = CBufferStreamReadUInt32(buffer_stream);
        locvar->EndPC = CBufferStreamReadUInt32(buffer_stream);
        locvars.push_back(*locvar);
    }
    return locvars;
}

vector<CBuffer> BinaryChunkReadUpvalueNames(CBufferStream buffer_stream)
{
    uint32_t upvaluenames_element_len = CBufferStreamReadUInt32(buffer_stream);
    vector<CBuffer> upvaluenames;
    CBuffer *temp;
    for (size_t i = 0; i < upvaluenames_element_len; ++i)
    {
        temp = (CBuffer *)malloc(sizeof(CBuffer *));
        *temp = BinaryChunkReadString(buffer_stream);
        upvaluenames.push_back(*temp);
    }
    return upvaluenames;
}

Prototype *BinaryChunkReadProto(CBuffer parent, CBufferStream buffer_stream)
{
    CBuffer source = BinaryChunkReadString(buffer_stream);
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
    proto->Code = BinaryChunkReadCode(buffer_stream);
    proto->constants = BinaryChunkReadConstants(buffer_stream);
    proto->Upvalues = BinaryChunkReadUpvalues(buffer_stream);
    proto->Protos = BinaryChunkReadProtos(source, buffer_stream);
    proto->LineInfo = BinaryChunkReadLineInfos(buffer_stream);
    proto->LocVars = BinaryChunkReadLocVars(buffer_stream);
    proto->UpvalueNames = BinaryChunkReadUpvalueNames(buffer_stream);
    return proto;
}

uint8_t BinaryChunkReadByte(CBufferStream buffer_stream)
{
    return CBufferStreamReadUInt8(buffer_stream);
}

uint64_t BinaryChunkReadLuaInteger(CBufferStream buffer_stream)
{
    return CBufferStreamReadUInt64(buffer_stream);
}

double BinaryChunkReadLuaNumber(CBufferStream buffer_stream)
{
    return CBufferStreamReadDouble(buffer_stream);
}
