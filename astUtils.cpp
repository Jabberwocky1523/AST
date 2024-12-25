#include "astUtils.h"
#include <stdio.h>
#include <stdlib.h>
#include "astBinaryChunk.h"
#include "astBuffer.h"
#include <string.h>
#include "astInstruction.h"
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

CBuffer LoadFileToCBuffer(const char *file_name)
{
    char *buffer = NULL;
    int len = 0;
    OPENANDREAD(file_name, buffer, len);
    return CBufferFromStr(buffer, len);
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
    memcpy(buffer, CBufferData(proto->Source), CBufferSize(proto->Source));
    printf("%s <%s:%d, %d> (%d instruction)\n", func_type, buffer, proto->LineDefined, proto->LastLineDefined, proto->Code.size());
    printf("%d%s params, %d slots, %d upvalues, ", proto->NumParams, vararg_flag, proto->MaxStackSize, proto->Upvalues.size());
    printf("%d locals, %d constants, %d functions\n", proto->LocVars.size(), proto->constants.size(), proto->Protos.size());
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
    vector<int> codes = proto->Code;
    int len = codes.size();
    bool flag = proto->LineInfo.size() > 0;
    for (int i = 0; i < len; ++i)
    {
        uint32_t val = codes[i];
        uint32_t line = proto->LineInfo[i];
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

void PrintConstant(ConstantType *constant, int i)
{
    switch (constant->tag)
    {
    case CONSTANT_TAG_NIL:
        printf("\t%d\t%s\n", i + 1, "nil");
        break;
    case CONSTANT_TAG_BOOLEAN:
        printf("\t%d\t%s\n", i + 1, constant->data.tag_boolean == 0 ? "false" : "true");
        break;
    case CONSTANT_TAG_NUMBER:
        printf("\t%d\t%lf\n", i + 1, constant->data.tag_number);
        break;
    case CONSTANT_TAG_INTEGER:
        printf("\t%d\t%lld\n", i + 1, constant->data.tag_integer);
        break;
    case CONSTANT_TAG_STR:
    {
        char buffer[1024] = {0};
        memcpy(buffer, CBufferData(constant->data.tag_str), CBufferDataSize(constant->data.tag_str));
        printf("\t%d\t%s\n", i + 1, buffer);
        break;
    }
    }
}

void PrintDetail(Prototype *proto)
{
    vector<ConstantType> constants = proto->constants;
    printf("constants (%d):\n", constants.size());
    int len = constants.size();
    for (int i = 0; i < len; ++i)
    {
        PrintConstant(&(constants[i]), i);
    }

    vector<LocVar> locvars = proto->LocVars;
    len = locvars.size();
    printf("locals (%d):\n", len);
    for (int i = 0; i < len; ++i)
    {
        LocVar *cur = &(locvars[i]);
        char buffer[1024] = {0};
        memcpy(buffer, CBufferData(cur->VarName), CBufferDataSize(cur->VarName));
        printf("\t%d\t%s\t%d\t%d\n", i, buffer, cur->StartPC + 1, cur->EndPC + 1);
    }

    vector<Upvalue> upval = proto->Upvalues;
    vector<CBuffer> upvalName = proto->UpvalueNames;
    len = upval.size();
    printf("upvalues (%d):\n", len);

    bool flag = upvalName.size() > 0;
    for (int i = 0; i < len; ++i)
    {
        Upvalue *cur = &(upval[i]);
        if (flag)
        {
            CBuffer *upname = &(upvalName[i]);
            char buffer[1024] = {0};
            memcpy(buffer, CBufferData(*upname), CBufferDataSize(*upname));
            printf("\t%d\t%s\t%d\t%d\n", i, buffer, cur->Instack, cur->Idx);
        }
        else
        {
            printf("\t%d\t-\t%d\t%d\n", i, cur->Instack, cur->Idx);
        }
    }
}
void PrintAst(Prototype *proto)
{
    if (proto == nullptr)
    {
        return;
    }
    PrintHeader(proto);
    PrintCode(proto);
    PrintDetail(proto);
    for (int i = 0; i < proto->Protos.size(); i++)
    {
        PrintAst(proto->Protos[i]);
    }
    return;
}