
#include "astBinaryChunk.h"
#include "astBufferStream.h"
#include "ast.h"
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "log.h"
astBuffer astBinaryChunkReadString(astBufferStream buffer_stream);
void astBinaryChunkReadCode(astBufferStream buffer_stream, Prototype *proto);
Constant astBinaryChunkReadConstant(astBufferStream buffer_stream);
void astBinaryChunkReadConstants(astBufferStream buffer_stream, Prototype *proto);
void astBinaryChunkReadUpvalues(astBufferStream buffer_stream, Prototype *proto);
void astBinaryChunkReadProtos(astBuffer parent, astBufferStream buffer_stream, Prototype *proto);
void astBinaryChunkReadLineInfos(astBufferStream buffer_stream, Prototype *proto);
void astBinaryChunkReadLocVars(astBufferStream buffer_stream, Prototype *proto);
void astBinaryChunkReadUpvalueNames(astBufferStream buffer_stream, Prototype *proto);
Prototype *astBinaryChunkReadProto(astBuffer parent, astBufferStream buffer_stream);

Prototype *astBinaryChunkUnDump(astBuffer buffer)
{
    astBufferStream buffer_stream = (astBufferStream)malloc(sizeof(StructastBufferStream));
    buffer_stream->size_ = buffer->size_;
    buffer_stream->data_ = buffer->data_;
    buffer_stream->read_pos_ = 0;
    buffer_stream->write_pos_ = buffer->data_size_;
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

void astBinaryChunkReadCode(astBufferStream buffer_stream, Prototype *proto)
{
    uint32_t code_element_len = astBufferStreamReadUInt32(buffer_stream);
    proto->Code.data = (uint32_t *)malloc(code_element_len * sizeof(uint32_t));
    proto->Code.len = code_element_len;
    for (int i = 0; i < code_element_len; i++)
    {
        uint32_t j = astBufferStreamReadUInt32(buffer_stream);
        proto->Code.data[i] = j;
    }
}

Constant astBinaryChunkReadConstant(astBufferStream buffer_stream)
{
    uint8_t tag = astBufferStreamReadUInt8(buffer_stream);
    Constant type;
    switch (tag)
    {
    case TAG_NIL:
        type.tag = CONSTANT_TAG_NIL;
        type.tag_nil = 0;
        break;
    case TAG_BOOLEAN:
        type.tag = CONSTANT_TAG_BOOLEAN;
        type.tag_boolean = (astBufferStreamReadUInt8(buffer_stream) != 0);
        break;
    case TAG_INTEGER:
        type.tag = CONSTANT_TAG_INTEGER;
        type.tag_integer = astBufferStreamReadUInt64(buffer_stream);
        break;
    case TAG_NUMBER:
        type.tag = CONSTANT_TAG_NUMBER;
        type.tag_number = astBufferStreamReadDouble(buffer_stream);
        break;
    case TAG_SHORT_STR:
    case TAG_LONG_STR:
        type.tag = CONSTANT_TAG_STR;
        type.tag_str = astBinaryChunkReadString(buffer_stream);
        break;
    default:
        assert(0);
        break;
    }
    return type;
}

void astBinaryChunkReadConstants(astBufferStream buffer_stream, Prototype *proto)
{
    uint32_t constant_element_len = astBufferStreamReadUInt32(buffer_stream);
    ConstantType Constants;
    Constant *data = (Constant *)malloc(sizeof(Constant) * constant_element_len);
    Constants.len = constant_element_len;
    for (int i = 0; i < constant_element_len; i++)
    {
        data[i] = astBinaryChunkReadConstant(buffer_stream);
    }
    Constants.data = data;
    proto->constants = Constants;
}

void astBinaryChunkReadUpvalues(astBufferStream buffer_stream, Prototype *proto)
{
    uint32_t upvalue_element_len = astBufferStreamReadUInt32(buffer_stream);
    Upvalue upvalues;
    UpvalueData *data = (UpvalueData *)calloc(upvalue_element_len, sizeof(UpvalueData));
    upvalues.len = upvalue_element_len;
    for (int i = 0; i < upvalue_element_len; i++)
    {
        data[i].Idx = astBufferStreamReadUInt8(buffer_stream);
        data[i].Instack = astBufferStreamReadUInt8(buffer_stream);
    }
    upvalues.data = data;
    proto->Upvalues = upvalues;
}

void astBinaryChunkReadProtos(astBuffer parent, astBufferStream buffer_stream, Prototype *proto)
{
    uint32_t protos_element_len = astBufferStreamReadUInt32(buffer_stream);
    if (protos_element_len == 0)
    {
        proto->ProtosLen = 0;
        proto->Protos = nullptr;
        return;
    }
    Prototype **protos = (Prototype **)malloc(sizeof(Prototype) * protos_element_len);
    proto->ProtosLen = protos_element_len;
    for (size_t i = 0; i < protos_element_len; ++i)
    {
        protos[i] = astBinaryChunkReadProto(parent, buffer_stream);
    }
    proto->Protos = protos;
}

void astBinaryChunkReadLineInfos(astBufferStream buffer_stream, Prototype *proto)
{
    LineInfo Line;
    uint32_t lineinfo_element_len = astBufferStreamReadUInt32(buffer_stream);
    Line.LineInfoLen = lineinfo_element_len;
    int *LineInfos = (int *)malloc(lineinfo_element_len * sizeof(int));
    int temp;
    for (size_t i = 0; i < lineinfo_element_len; ++i)
    {
        temp = astBufferStreamReadUInt32(buffer_stream);
        LineInfos[i] = temp;
    }
    Line.LineInfo = LineInfos;
    proto->LineInfo = Line;
}

void astBinaryChunkReadLocVars(astBufferStream buffer_stream, Prototype *proto)
{
    uint32_t locvar_element_len = astBufferStreamReadUInt32(buffer_stream);
    LocVarData *data = (LocVarData *)calloc(locvar_element_len, sizeof(LocVarData));
    for (size_t i = 0; i < locvar_element_len; ++i)
    {
        data[i].VarName = astBinaryChunkReadString(buffer_stream);
        data[i].StartPC = astBufferStreamReadUInt32(buffer_stream);
        data[i].EndPC = astBufferStreamReadUInt32(buffer_stream);
    }
    LocVar var;
    var.Len = locvar_element_len;
    var.data = data;
    proto->LocVars = var;
}

void astBinaryChunkReadUpvalueNames(astBufferStream buffer_stream, Prototype *proto)
{
    uint32_t upvaluenames_element_len = astBufferStreamReadUInt32(buffer_stream);
    astBuffer *upvaluenames = (astBuffer *)malloc(upvaluenames_element_len * sizeof(astBuffer));
    UpvalueNames uvm;
    for (size_t i = 0; i < upvaluenames_element_len; ++i)
    {
        astBuffer temp = astBinaryChunkReadString(buffer_stream);
        upvaluenames[i] = temp;
    }
    uvm.UpvalueNames = upvaluenames;
    uvm.upvalusNameslen = upvaluenames_element_len;
    proto->UpvalueNames = uvm;
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
    astBinaryChunkReadCode(buffer_stream, proto);
    astBinaryChunkReadConstants(buffer_stream, proto);
    astBinaryChunkReadUpvalues(buffer_stream, proto);
    astBinaryChunkReadProtos(source, buffer_stream, proto);
    astBinaryChunkReadLineInfos(buffer_stream, proto);
    astBinaryChunkReadLocVars(buffer_stream, proto);
    astBinaryChunkReadUpvalueNames(buffer_stream, proto);
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
