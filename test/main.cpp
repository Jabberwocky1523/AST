#include "BinaryChunk.h"
#include "Buffer.h"
#include "utils.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
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
    printf("\n%s <%s:%d, %d> (%d instruction)\n", func_type, buffer, proto->LineDefined, proto->LastLineDefined, proto->Code.size());
    printf("%d%s params, %d slots, %d upvalues, ", proto->NumParams, vararg_flag, proto->MaxStackSize, proto->Upvalues.size());
    printf("%d locals, %d constants, %d functions\n", proto->LocVars.size(), proto->constants.size(), proto->Protos.size());
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
            printf("\t%d\t[%d]\t0x%08x \t", i + 1, line, val);
        }
        else
        {
            printf("\t%d\t[-]\t%s \t", i + 1);
        }
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

// void Main(Prototype *proto)
// {
//     int nRegs = proto->MaxStackSize;
//     LuaState lua_state = LuaStateAlloc(nRegs + 8, proto);
//     LuaStateSetTop(lua_state, nRegs);
//     for (;;)
//     {
//         int pc = LuaStatePC(lua_state);
//         Instruction inst = LuaStateFetch(lua_state);
//         if (InstructionOpcode(inst) != OP_RETURN)
//         {
//             InstructionExecute(inst, lua_state);
//             printf("[%02d] %s", pc + 1, InstructionOpName(inst));

//             PrintStack(lua_state);
//         }
//         else
//         {
//             break;
//         }
//     }
// }

int main(int argc, const char *const *argv)
{
    const char *file_name = "luac.out";
    if (argc >= 2)
    {
        file_name = argv[1];
    }
    CBuffer file_contont = LoadFileToCBuffer(file_name);
    Prototype *proto = BinaryChunkUnDump(file_contont);
    PrintHeader(proto);
    PrintCode(proto);
    PrintDetail(proto);
    // LuaMain(proto);
    return 0;
}
