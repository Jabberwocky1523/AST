#include "astVm.h"
#include "astMath.h"
#include "astStack.h"
ast_Integer ast_GetPc(ast_State *L)
{
    return L->pc;
}
ast_Bool ast_AddPc(ast_State *L, ast_Integer n)
{
    L->pc += n;
    return TRUE;
}
ast_Integer ast_Fetch(ast_State *L)
{
    ast_Integer i = L->proto->Code[L->pc];
    L->pc++;
    return i;
}
ast_Bool ast_GetConst(ast_State *L, int idx)
{
    ConstantType c = L->proto->constants[idx];
    ast_StackPushConstant(L, c);
    return TRUE;
}
ast_Bool ast_GetRk(ast_State *L, int rk)
{
    if (rk > 0xff)
    {
        ast_GetConst(L, rk & 0xff);
        return TRUE;
    }
    else
    {
        TValue tmp = ast_StackGetTValue(PStack(L), rk);
        ast_StackPush(PStack(L), tmp);
        return TRUE;
    }
    return FALSE;
}
// R(A) = R(B)
ast_Bool _ast_Move(ast_State *L, Instruction i)
{
    TABC n = InstructionTABC(i);
    astack_Copy(PStack(L), n.b, n.a);
    return TRUE;
}
ast_Bool _ast_Jmp(ast_State *L, Instruction i)
{
    TAsBx n = InstructionTAsBx(i);
    ast_AddPc(L, n.sbx);
    if (n.a != 0)
    {
        PANIC("Jmp错误");
    }
    return TRUE;
}
// LOAD R(A).....R(A + B) = NIL
ast_Bool _ast_LoadNil(ast_State *L, Instruction i)
{
    TABC n = InstructionTABC(i);
    ast_StackPush(PStack(L), nullptr, AST_TNIL);
    for (int p = n.a; p <= n.a + n.b; p++)
    {
        astack_Copy(PStack(L), -1, p);
    }
    ast_StackPop(PStack(L));
    return TRUE;
}
// R(A) = (R(B) ?= 0)
ast_Bool _ast_LoadBool(ast_State *L, Instruction i)
{
    TABC n = InstructionTABC(i);
    ast_StackPush(PStack(L), (ast_Bool)(n.b != 0), AST_TBOOLEAN);
    astack_ReplaceToIdx(PStack(L), n.a);
    if (n.c != 0)
    {
        L->pc++;
    }
    return TRUE;
}
// 加载常量表中的值 R(A) = Kst(Bx)
ast_Bool _ast_LoadK(ast_State *L, Instruction i)
{
    TABx n = InstructionTABx(i);
    ast_GetConst(L, n.bx);
    astack_ReplaceToIdx(PStack(L), n.a);
    return TRUE;
}
// 配合EXTRAAGR扩展_ast_LoadK
ast_Bool _ast_LoadKx(ast_State *L, Instruction i)
{
    TABx n = InstructionTABx(i);
    int n2 = InstructionTAX(ast_Fetch(L));
    ast_GetConst(L, n2);
    astack_ReplaceToIdx(PStack(L), n.a);
    return TRUE;
}
// END

// 运算符
////二元运算符 R(A) = RK(B) op RK(C) 寄存器或常量表b c位置op给栈a位置
ast_Bool _ast_BinaryArith(ast_State *L, Instruction i, int op)
{
    TABC n = InstructionTABC(i);
    ast_GetRk(L, n.b);
    ast_GetRk(L, n.c);
    ast_Arith(L, op);
    astack_ReplaceToIdx(PStack(L), n.a);
    return TRUE;
}




////一元运算 R(A) = op R(B); 将栈b位置覆盖a位置
ast_Bool _ast_UnaryArith(ast_State *L, Instruction i, int op)
{
    TABC n = InstructionTABC(i);
    TValue tmp = ast_StackGetTValue(PStack(L), n.b);
    ast_StackPush(PStack(L), tmp);
    ast_Arith(L, op);
    astack_ReplaceToIdx(PStack(L), n.a);
    return TRUE;
}
// END

// 长度和拼接指令
////LEN R(A) = LengthOf R(B)
ast_Bool _ast_Length(ast_State *L, Instruction i)
{
    TABC n = InstructionTABC(i);
    ast_Len(L, n.b);
    astack_ReplaceToIdx(PStack(L), n.a);
    return TRUE;
}
/////CONCAT R(A) = R(B)......R(C)
ast_Bool _ast_Concat(ast_State *L, Instruction i)
{
    TABC n = InstructionTABC(i);
    int need = n.c - n.b + 1;
    ast_StackCheck(PStack(L), need);
    for (int p = n.b; p <= n.c; p++)
    {
        TValue tmp = ast_StackGetTValue(PStack(L), p);
        ast_StackPush(PStack(L), tmp);
    }
    ast_Concat(L, need);
    astack_ReplaceToIdx(PStack(L), n.a);
    return TRUE;
}
// END

// 比较指令 if((RK(B) op RK(C) != A)) then pc++
ast_Bool _ast_Compare(ast_State *L, Instruction i, int op)
{
    TABC n = InstructionTABC(i);
    ast_GetRk(L, n.b);
    ast_GetRk(L, n.c);
    if (ast_Compare(L, -1, -2, op) != (n.a != 0))
    {
        ast_AddPc(L, 1);
    }
    ast_StackPop(PStack(L));
    ast_StackPop(PStack(L));
    return TRUE;
}
// END

// 逻辑运算符
ast_Bool _ast_Not(ast_State *L, Instruction i)
{
    TABC n = InstructionTABC(i);
    ast_StackPush(PStack(L), ast_ConvertToBoolean(ast_StackGetTValue(PStack(L), n.b)), AST_TBOOLEAN);
    astack_ReplaceToIdx(PStack(L), n.a);
    return TRUE;
}
ast_Bool _ast_TestSet(ast_State *L, Instruction i)
{
    TABC n = InstructionTABC(i);
    if (ast_ConvertToBoolean(ast_StackGetTValue(PStack(L), n.b)) == (n.c != 0))
    {
        astack_Copy(PStack(L), n.b, n.a);
    }
    else
    {
        ast_AddPc(L, 1);
    }
    return TRUE;
}
ast_Bool _ast_Test(ast_State *L, Instruction i)
{
    TABC n = InstructionTABC(i);
    if (ast_ConvertToBoolean(ast_StackGetTValue(PStack(L), n.a)) != (n.c != 0))
    {
        ast_AddPc(L, 1);
    }
    return TRUE;
}
// END

// for语句
ast_Bool _ast_ForPrep(ast_State *L, Instruction i)
{
    TAsBx n = InstructionTAsBx(i);
    TValue tmp = ast_StackGetTValue(PStack(L), n.a);
    ast_StackPush(PStack(L), tmp);
    tmp = ast_StackGetTValue(PStack(L), n.a + 2);
    ast_StackPush(PStack(L), tmp);
    ast_Arith(L, AST_OPSUB);
    astack_ReplaceToIdx(PStack(L), n.a);
    ast_AddPc(L, 1);
}
ast_Bool _ast_ForLoop(ast_State *L, Instruction i)
{
    TAsBx n = InstructionTAsBx(i);
    TValue tmp = ast_StackGetTValue(PStack(L), n.a + 2);
    ast_StackPush(PStack(L), tmp);
    tmp = ast_StackGetTValue(PStack(L), n.a);
    ast_StackPush(PStack(L), tmp);
    ast_Arith(L, AST_OPADD);
    astack_ReplaceToIdx(PStack(L), n.a);
    ast_Bool isPositiveStep = (ast_Bool)(ast_ConvertToNumber(ast_StackGetTValue(PStack(L), n.a + 2)) >= 0);
    if ((isPositiveStep && ast_Compare(L, n.a, n.a + 1, AST_OPLE)) || (!isPositiveStep && ast_Compare(L, n.a + 1, n.a, AST_OPLE)))
    {
        ast_AddPc(L, 1);
        astack_Copy(PStack(L), n.a, n.a + 3);
    }
    return TRUE;
}
// END

ast_OpCode g_ast_opcodes[47] = {
    /*T  A     B       C    mode    name */
    {0, 1, OpArgR, OpArgN, IABC, "MOVE    "},
    {0, 1, OpArgK, OpArgN, IABx, "LOADK   "},
    {0, 1, OpArgN, OpArgN, IABx, "LOADKX  "},
    {0, 1, OpArgU, OpArgU, IABC, "LOADBOOL"},
    {0, 1, OpArgU, OpArgN, IABC, "LOADNIL "},
    {0, 1, OpArgU, OpArgN, IABC, "GETUPVAL"},
    {0, 1, OpArgU, OpArgK, IABC, "GETTABUP"},
    {0, 1, OpArgR, OpArgK, IABC, "GETTABLE"},
    {0, 0, OpArgK, OpArgK, IABC, "SETTABUP"},
    {0, 0, OpArgU, OpArgN, IABC, "SETUPVAL"},
    {0, 0, OpArgK, OpArgK, IABC, "SETTABLE"},
    {0, 1, OpArgU, OpArgU, IABC, "NEWTABLE"},
    {0, 1, OpArgR, OpArgK, IABC, "SELF    "},
    {0, 1, OpArgK, OpArgK, IABC, "ADD     "},
    {0, 1, OpArgK, OpArgK, IABC, "SUB     "},
    {0, 1, OpArgK, OpArgK, IABC, "MUL     "},
    {0, 1, OpArgK, OpArgK, IABC, "MOD     "},
    {0, 1, OpArgK, OpArgK, IABC, "POW     "},
    {0, 1, OpArgK, OpArgK, IABC, "DIV     "},
    {0, 1, OpArgK, OpArgK, IABC, "IDIV    "},
    {0, 1, OpArgK, OpArgK, IABC, "BAND    "},
    {0, 1, OpArgK, OpArgK, IABC, "BOR     "},
    {0, 1, OpArgK, OpArgK, IABC, "BXOR    "},
    {0, 1, OpArgK, OpArgK, IABC, "SHL     "},
    {0, 1, OpArgK, OpArgK, IABC, "SHR     "},
    {0, 1, OpArgR, OpArgN, IABC, "UNM     "},
    {0, 1, OpArgR, OpArgN, IABC, "BNOT    "},
    {0, 1, OpArgR, OpArgN, IABC, "NOT     "},
    {0, 1, OpArgR, OpArgN, IABC, "LEN     "},
    {0, 1, OpArgR, OpArgR, IABC, "CONCAT  "},
    {0, 0, OpArgR, OpArgN, IAsBx, "JMP     "},
    {1, 0, OpArgK, OpArgK, IABC, "EQ      "},
    {1, 0, OpArgK, OpArgK, IABC, "LT      "},
    {1, 0, OpArgK, OpArgK, IABC, "LE      "},
    {1, 0, OpArgN, OpArgU, IABC, "TEST    "},
    {1, 1, OpArgR, OpArgU, IABC, "TESTSET "},
    {0, 1, OpArgU, OpArgU, IABC, "CALL    "},
    {0, 1, OpArgU, OpArgU, IABC, "TAILCALL"},
    {0, 0, OpArgU, OpArgN, IABC, "RETURN  "},
    {0, 1, OpArgR, OpArgN, IAsBx, "FORLOOP "},
    {0, 1, OpArgR, OpArgN, IAsBx, "FORPREP "},
    {0, 0, OpArgN, OpArgU, IABC, "TFORCALL"},
    {0, 1, OpArgR, OpArgN, IAsBx, "TFORLOOP"},
    {0, 0, OpArgU, OpArgU, IABC, "SETLIST "},
    {0, 1, OpArgU, OpArgN, IABx, "CLOSURE "},
    {0, 1, OpArgU, OpArgN, IABC, "VARARG  "},
    {0, 0, OpArgU, OpArgU, IAx, "EXTRAARG"},
};