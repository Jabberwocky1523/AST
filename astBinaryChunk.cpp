
#include "astBinaryChunk.h"
#include "astBufferStream.h"
#include "ast.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

astBuffer astBinaryChunkReadString(astBufferStream buffer_stream);
std::vector<int> astBinaryChunkReadCode(astBufferStream buffer_stream);
ConstantType astBinaryChunkReadConstant(astBufferStream buffer_stream);
std::vector<ConstantType> *astBinaryChunkReadConstants(astBufferStream buffer_stream);
std::vector<Upvalue> astBinaryChunkReadUpvalues(astBufferStream buffer_stream);
std::vector<Prototype *> astBinaryChunkReadProtos(astBuffer parent, astBufferStream buffer_stream);
std::vector<int> astBinaryChunkReadLineInfos(astBufferStream buffer_stream);
std::vector<LocVar> astBinaryChunkReadLocVars(astBufferStream buffer_stream);
std::vector<astBuffer> astBinaryChunkReadUpvalueNames(astBufferStream buffer_stream);
Prototype *astBinaryChunkReadProto(astBuffer parent, astBufferStream buffer_stream);

Prototype *astBinaryChunkUnDump(astBuffer buffer)
{
    astBufferStream buffer_stream = (astBufferStream)malloc(sizeof(StructastBufferStream));
    buffer_stream->size_ = buffer->size_;
    buffer_stream->data_ = buffer->data_;
    buffer_stream->read_pos_ = 0;
    buffer_stream->write_pos_ = buffer->data_size_;
    printf(" \b");
    if (!astBinaryChunkCheckHead(buffer_stream))
    {
        PANIC("CheckHeadError");
    }
    astBinaryChunkReadByte(buffer_stream);
    return astBinaryChunkReadProto(NULL, buffer_stream);
}

astBuffer astBinaryChunkReadString(astBufferStream buffer_stream)
{
    assert(buffer_stream != NULL);
    uint64_t len = astBufferStreamReadUInt8(buffer_stream);
    if (len == 0)
    {
        return NULL;
    }
    astBuffer buffer = NULL;
    if (len == 0xff)
    {
        len = astBufferStreamReadUInt64(buffer_stream);
    }

    buffer = astBufferAlloc((int)len - 1);
    astBufferStreamRead(buffer_stream, astBufferData(buffer), (int)len - 1);
    astBufferSetDataSize(buffer, (int)len - 1);
    return buffer;
}

bool astBinaryChunkCheckHead(astBufferStream buffer_stream)
{
    assert(buffer_stream != NULL);
    do
    {
        char buffer[10] = {0};
        int buffer_len = strlen(LUA_SIGNATURE);
        astBufferStreamRead(buffer_stream, buffer, buffer_len);
        if (strncmp(buffer, LUA_SIGNATURE, buffer_len) != 0)
        {
            printf("LUA_SIGNATURE error");
            break;
        }

        if (astBufferStreamReadUInt8(buffer_stream) != LUAC_VERSION)
        {
            printf("LUAC_VERSION error");
            break;
        }

        if (astBufferStreamReadUInt8(buffer_stream) != LUAC_FORMAT)
        {
            printf("LUAC_FORMAT error");
            break;
        }

        buffer_len = strlen(LUAC_DATA);
        astBufferStreamRead(buffer_stream, buffer, buffer_len);
        if (strncmp(buffer, LUAC_DATA, buffer_len) != 0)
        {
            printf("LUAC_DATA error");
            break;
        }

        if (astBufferStreamReadUInt8(buffer_stream) != CINT_SIZE)
        {
            printf("CINT_SIZE error");
            break;
        }

        if ((astBufferStreamReadUInt8(buffer_stream)) != CSIZET_SIZE)
        {
            printf("CSIZET_SIZE error");
            break;
        }

        if (astBufferStreamReadUInt8(buffer_stream) != INSTRUCTION_SIZE)
        {
            printf("INSTRUCTION_SIZE error");
            break;
        }

        if (astBufferStreamReadUInt8(buffer_stream) != LUA_INTEGER_SIZE)
        {
            printf("LUA_INTEGER_SIZE error");
            break;
        }

        if (astBufferStreamReadUInt8(buffer_stream) != LUA_NUMBER_SIZE)
        {
            printf("LUA_NUMBER_SIZE error");
            break;
        }

        if (astBufferStreamReadInt64(buffer_stream) != LUAC_INT)
        {
            printf("LUAC_INT error");
            break;
        }

        if (astBufferStreamReadDouble(buffer_stream) != LUAC_NUM)
        {
            printf("LUAC_NUM error");
            break;
        }
        return true;
    } while (0);
    return false;
}

std::vector<int> astBinaryChunkReadCode(astBufferStream buffer_stream)
{
    uint32_t code_element_len = astBufferStreamReadUInt32(buffer_stream);
    std::vector<int> codes;
    for (int i = 0; i < code_element_len; i++)
    {
        codes.push_back(astBufferStreamReadUInt32(buffer_stream));
    }
    return codes;
}

ConstantType astBinaryChunkReadConstant(astBufferStream buffer_stream)
{
    uint8_t tag = astBufferStreamReadUInt8(buffer_stream);
    ConstantType *type = (ConstantType *)malloc(sizeof(ConstantType));
    switch (tag)
    {
    case TAG_NIL:
        type->tag = CONSTANT_TAG_NIL;
        type->data.tag_nil = 0;
        break;
    case TAG_BOOLEAN:
        type->tag = CONSTANT_TAG_BOOLEAN;
        type->data.tag_boolean = (astBufferStreamReadUInt8(buffer_stream) != 0);
        break;
    case TAG_INTEGER:
        type->tag = CONSTANT_TAG_INTEGER;
        type->data.tag_integer = astBufferStreamReadUInt64(buffer_stream);
        break;
    case TAG_NUMBER:
        type->tag = CONSTANT_TAG_NUMBER;
        type->data.tag_number = astBufferStreamReadDouble(buffer_stream);
        break;
    case TAG_SHORT_STR:
    case TAG_LONG_STR:
        type->tag = CONSTANT_TAG_STR;
        type->data.tag_str = astBinaryChunkReadString(buffer_stream);
        break;
    default:
        assert(0);
        break;
    }
    return *type;
}

std::vector<ConstantType> *astBinaryChunkReadConstants(astBufferStream buffer_stream)
{
    uint32_t constant_element_len = astBufferStreamReadUInt32(buffer_stream);
    std::vector<ConstantType> *Constants = new std::vector<ConstantType>;
    for (int i = 0; i < constant_element_len; i++)
    {
        Constants->push_back(astBinaryChunkReadConstant(buffer_stream));
    }
    return Constants;
}

std::vector<Upvalue> astBinaryChunkReadUpvalues(astBufferStream buffer_stream)
{
    uint32_t upvalue_element_len = astBufferStreamReadUInt32(buffer_stream);
    std::vector<Upvalue> upvalues;
    for (int i = 0; i < upvalue_element_len; i++)
    {
        Upvalue *temp = (Upvalue *)malloc(sizeof(Upvalue));
        temp->Idx = astBufferStreamReadUInt8(buffer_stream);
        temp->Instack = astBufferStreamReadUInt8(buffer_stream);
        upvalues.push_back(*temp);
    }
    return upvalues;
}

std::vector<Prototype *> astBinaryChunkReadProtos(astBuffer parent, astBufferStream buffer_stream)
{
    uint32_t protos_element_len = astBufferStreamReadUInt32(buffer_stream);
    std::vector<Prototype *> protos;
    if (protos_element_len == 0)
    {
        return protos;
    }

    Prototype *temp;
    for (size_t i = 0; i < protos_element_len; ++i)
    {
        temp = astBinaryChunkReadProto(parent, buffer_stream);
        protos.push_back(temp);
    }
    return protos;
}

std::vector<int> astBinaryChunkReadLineInfos(astBufferStream buffer_stream)
{
    uint32_t lineinfo_element_len = astBufferStreamReadUInt32(buffer_stream);
    std::vector<int> LineInfos;
    uint32_t *temp;
    for (size_t i = 0; i < lineinfo_element_len; ++i)
    {
        temp = (uint32_t *)malloc(sizeof(uint32_t));
        *temp = astBufferStreamReadUInt32(buffer_stream);
        LineInfos.push_back(*temp);
    }
    return LineInfos;
}

std::vector<LocVar> astBinaryChunkReadLocVars(astBufferStream buffer_stream)
{
    uint32_t locvar_element_len = astBufferStreamReadUInt32(buffer_stream);
    std::vector<LocVar> locvars;
    LocVar *locvar;
    for (size_t i = 0; i < locvar_element_len; ++i)
    {
        locvar = (LocVar *)malloc(sizeof(LocVar));
        locvar->VarName = astBinaryChunkReadString(buffer_stream);
        locvar->StartPC = astBufferStreamReadUInt32(buffer_stream);
        locvar->EndPC = astBufferStreamReadUInt32(buffer_stream);
        locvars.push_back(*locvar);
    }
    return locvars;
}

std::vector<astBuffer> astBinaryChunkReadUpvalueNames(astBufferStream buffer_stream)
{
    uint32_t upvaluenames_element_len = astBufferStreamReadUInt32(buffer_stream);
    std::vector<astBuffer> upvaluenames;
    astBuffer *temp;
    for (size_t i = 0; i < upvaluenames_element_len; ++i)
    {
        temp = (astBuffer *)malloc(sizeof(astBuffer *));
        *temp = astBinaryChunkReadString(buffer_stream);
        upvaluenames.push_back(*temp);
    }
    return upvaluenames;
}

Prototype *astBinaryChunkReadProto(astBuffer parent, astBufferStream buffer_stream)
{
    astBuffer source = astBinaryChunkReadString(buffer_stream);
    if (source == NULL)
    {
        source = parent;
    }
    Prototype *proto = (Prototype *)malloc(sizeof(Prototype));
    proto->Source = source;
    proto->LineDefined = astBufferStreamReadUInt32(buffer_stream);
    proto->LastLineDefined = astBufferStreamReadUInt32(buffer_stream);
    proto->NumParams = astBufferStreamReadUInt8(buffer_stream);
    proto->IsVararg = astBufferStreamReadUInt8(buffer_stream);
    proto->MaxStackSize = astBufferStreamReadUInt8(buffer_stream);
    proto->Code = astBinaryChunkReadCode(buffer_stream);
    proto->constants = astBinaryChunkReadConstants(buffer_stream);
    proto->Upvalues = astBinaryChunkReadUpvalues(buffer_stream);
    proto->Protos = astBinaryChunkReadProtos(source, buffer_stream);
    proto->LineInfo = astBinaryChunkReadLineInfos(buffer_stream);
    proto->LocVars = astBinaryChunkReadLocVars(buffer_stream);
    proto->UpvalueNames = astBinaryChunkReadUpvalueNames(buffer_stream);
    return proto;
}

uint8_t astBinaryChunkReadByte(astBufferStream buffer_stream)
{
    return astBufferStreamReadUInt8(buffer_stream);
}

uint64_t astBinaryChunkReadLuaInteger(astBufferStream buffer_stream)
{
    return astBufferStreamReadUInt64(buffer_stream);
}

double astBinaryChunkReadLuaNumber(astBufferStream buffer_stream)
{
    return astBufferStreamReadDouble(buffer_stream);
}
