#include "astUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include "astBinaryChunk.h"
#include "astBuffer.h"
#include <string.h>
#include "astInstruction.h"
#include "unistd.h"
#include "log.h"
#define OPENANDREAD(file_name, buffer, len)             \
    do                                                  \
    {                                                   \
        FILE *fp = fopen(file_name, "rb");              \
        if (fp == NULL)                                 \
        {                                               \
            printf("file %s not exists!\n", file_name); \
            return NULL;                                \
        }                                               \
        fseek(fp, 0, SEEK_END);                         \
        len = ftell(fp);                                \
        fseek(fp, 0, SEEK_SET);                         \
        buffer = (char *)malloc(len);                   \
        fread(buffer, len, 1, fp);                      \
        fclose(fp);                                     \
    } while (0)

char *LoadFileContent(const char *file_name)
{
    char *buffer = NULL;
    int len = 0;
    OPENANDREAD(file_name, buffer, len);
    return buffer;
}

astBuffer LoadFileToastBuffer(const char *file_name)
{
    char *buffer = NULL;
    int len = 0;
    OPENANDREAD(file_name, buffer, len);
    return astBufferFromStr(buffer, len);
}
void PrintHeader(Prototype *proto)
{
    const char *func_type = "main";
    if (proto->LineDefined > 0)
    {
        func_type = "function";
    }
    const char *vararg_flag = "";
    if (proto->IsVararg > 0)
    {
        vararg_flag = "+";
    }

    char buffer[1024] = {0};
    memcpy(buffer, astBufferData(proto->Source), astBufferSize(proto->Source));
    printf("%s <%s:%d, %d> (%d instruction)\n", func_type, buffer, proto->LineDefined, proto->LastLineDefined, proto->Code.len);
    printf("%d%s params, %d slots, %d upvalues, ", proto->NumParams, vararg_flag, proto->MaxStackSize, proto->Upvalues.len);
    printf("%d locals, %d constants, %d functions\n", proto->LocVars.Len, proto->constants.len, proto->ProtosLen);
}

void PrintOperands(Instruction instruction)
{
    switch (InstructionOpMode(instruction))
    {
    case IABC:
    {
        TABC tabc = InstructionTABC(instruction);
        printf("%d", tabc.a);
        if (InstructionBMode(instruction) != OpArgN)
        {
            if (tabc.b > 0xff)
            {
                printf(" %d", -1 - (tabc.b & 0xff));
            }
            else
            {
                printf(" %d", tabc.b);
            }
        }
        if (InstructionCMode(instruction) != OpArgN)
        {
            if (tabc.c > 0xff) // 常量表索引
            {
                printf(" %d", -1 - (tabc.c & 0xff));
            }
            else
            {
                printf(" %d", tabc.c);
            }
        }
        break;
    }
    case IABx:
    {
        TABx tabx = InstructionTABx(instruction);
        printf("%d", tabx.a);
        if (InstructionBMode(instruction) == OpArgK)
        {
            printf(" %d", -1 - tabx.bx);
        }
        else
        {
            printf(" %d", tabx.bx);
        }
        break;
    }
    case IAsBx:
    {
        TAsBx tasbx = InstructionTAsBx(instruction);
        printf("%d %d", tasbx.a, tasbx.sbx);
        break;
    }
    case IAx:
    {
        int ax = InstructionTAX(instruction);
        printf("%d", ax);
        break;
    }
    }
}

void PrintCode(Prototype *proto)
{
    uint32_t *codes = proto->Code.data;
    int len = proto->Code.len;
    bool flag = proto->LineInfo.LineInfoLen > 0;
    for (int i = 0; i < len; ++i)
    {

        uint32_t val = codes[i];
        uint32_t line = proto->LineInfo.LineInfo[i];
        if (flag)
        {
            printf("\t%d\t[%d]\t%s \t", i + 1, line, InstructionOpName((Instruction)val));
        }
        else
        {
            printf("\t%d\t[-]\t%s \t", i + 1);
        }
        PrintOperands((Instruction)val);
        printf("\n");
    }
}

void PrintConstant(Constant constant, int i)
{
    switch (constant.tag)
    {
    case CONSTANT_TAG_NIL:
        printf("\t%d\t%s\n", i + 1, "nil");
        break;
    case CONSTANT_TAG_BOOLEAN:
        printf("\t%d\t%s\n", i + 1, constant.tag_boolean == 0 ? "false" : "true");
        break;
    case CONSTANT_TAG_NUMBER:
        printf("\t%d\t%lf\n", i + 1, constant.tag_number);
        break;
    case CONSTANT_TAG_INTEGER:
        printf("\t%d\t%lld\n", i + 1, constant.tag_integer);
        break;
    case CONSTANT_TAG_STR:
    {
        char buffer[1024] = {0};
        memcpy(buffer, astBufferData(constant.tag_str), astBufferDataSize(constant.tag_str));
        printf("\t%d\t%s\n", i + 1, buffer);
        break;
    }
    }
}

void PrintDetail(Prototype *proto)
{
    ConstantType constants = proto->constants;
    int len = constants.len;
    printf("constants (%d):\n", len);
    for (int i = 0; i < len; ++i)
    {
        Constant constant = constants.data[i];
        PrintConstant(constant, i);
    }

    LocVar locvars = proto->LocVars;
    len = locvars.Len;
    printf("locals (%d):\n", len);
    for (int i = 0; i < len; ++i)
    {
        LocVarData cur = locvars.data[i];
        char buffer[1024] = {0};
        memcpy(buffer, astBufferData(cur.VarName), astBufferDataSize(cur.VarName));
        printf("\t%d\t%s\t%d\t%d\n", i, buffer, cur.StartPC + 1, cur.EndPC + 1);
    }

    Upvalue upval = proto->Upvalues;
    UpvalueNames upvalName = proto->UpvalueNames;
    len = upval.len;
    printf("upvalues (%d):\n", len);
    bool flag = proto->UpvalueNames.upvalusNameslen > 0;
    for (int i = 0; i < len; ++i)
    {
        UpvalueData cur = upval.data[i];
        if (flag)
        {
            astBuffer upname = upvalName.UpvalueNames[i];
            char buffer[1024] = {0};
            memcpy(buffer, astBufferData(upname), astBufferDataSize(upname));
            printf("\t%d\t%s\t%d\t%d\n", i, buffer, cur.Instack, cur.Idx);
        }
        else
        {
            printf("\t%d\t-\t%d\t%d\n", i, cur.Instack, cur.Idx);
        }
    }
}
void PrintAst(Prototype *proto)
{
    PrintHeader(proto);
    PrintCode(proto);
    PrintDetail(proto);
    if (proto->Protos == nullptr)
    {
        return;
    }
    int size = proto->ProtosLen;
    for (int i = 0; i < size; i++)
    {
        PrintAst(proto->Protos[i]);
    }
    return;
}
