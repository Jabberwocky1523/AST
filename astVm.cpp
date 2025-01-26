#include "astVm.h"
#include "astMath.h"
#include "astStack.h"
#include "astOpcode.h"
#include "astMap.h"
#include "log.h"
#include "astTable.h"
ast_Integer ast_GetPc(ast_State *L)
{
    return L->stack->pc;
}
ast_Bool ast_AddPc(ast_State *L, ast_Integer n)
{
    L->stack->pc += n;
    return TRUE;
}
ast_Integer ast_Fetch(ast_State *L)
{
    ast_Integer i = L->stack->closure->pr->Code.data[L->stack->pc];
    L->stack->pc++;
    return i;
}
ast_Bool ast_GetConst(ast_State *L, int idx)
{

    Constant c = L->stack->closure->pr->constants.data[idx];
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
ast_Bool _ast_CloseUpvalues(ast_State *L, ast_Integer n)
{
    for (int i = 0; i < L->stack->openuvs->Mnum; i++)
    {
        if (i >= n - 1)
        {
            astMap_RemoveFromKey(L->stack->openuvs, Int2Ob(i));
        }
    }
    return TRUE;
}
ast_Bool _ast_Jmp(ast_State *L, Instruction i)
{
    TAsBx n = InstructionTAsBx(i);
    ast_AddPc(L, n.sbx);
    if (n.a != 0)
    {
        _ast_CloseUpvalues(L, n.a);
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
        L->stack->pc++;
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
ast_Bool _ast_Add_(ast_State *L, Instruction i)
{
    return _ast_BinaryArith(L, i, AST_OPADD);
}

ast_Bool _ast_Sub_(ast_State *L, Instruction i)
{
    return _ast_BinaryArith(L, i, AST_OPSUB);
}
ast_Bool _ast_Mul_(ast_State *L, Instruction i)
{
    return _ast_BinaryArith(L, i, AST_OPMUL);
}
ast_Bool _ast_Mod_(ast_State *L, Instruction i)
{
    return _ast_BinaryArith(L, i, AST_OPMOD);
}
ast_Bool _ast_Pow_(ast_State *L, Instruction i)
{
    return _ast_BinaryArith(L, i, AST_OPPOW);
}
ast_Bool _ast_Div_(ast_State *L, Instruction i)
{
    return _ast_BinaryArith(L, i, AST_OPDIV);
}
ast_Bool _ast_Idiv_(ast_State *L, Instruction i)
{
    return _ast_BinaryArith(L, i, AST_OPIDIV);
}
ast_Bool _ast_Band_(ast_State *L, Instruction i)
{
    return _ast_BinaryArith(L, i, AST_OPBAND);
}
ast_Bool _ast_Bor_(ast_State *L, Instruction i)
{
    return _ast_BinaryArith(L, i, AST_OPBOR);
}
ast_Bool _ast_Bxor_(ast_State *L, Instruction i)
{
    return _ast_BinaryArith(L, i, AST_OPBXOR);
}
ast_Bool _ast_Shl_(ast_State *L, Instruction i)
{
    return _ast_BinaryArith(L, i, AST_OPSHL);
}
ast_Bool _ast_Shr_(ast_State *L, Instruction i)
{
    return _ast_BinaryArith(L, i, AST_OPSHR);
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
ast_Bool _ast_Bnot_(ast_State *L, Instruction i)
{
    return _ast_UnaryArith(L, i, AST_OPBNOT);
}
ast_Bool _ast_Unm_(ast_State *L, Instruction i)
{
    return _ast_UnaryArith(L, i, AST_OPUNM);
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
    if (ast_Compare(L, -2, -1, op) != (n.a != 0))
    {
        ast_AddPc(L, 1);
    }
    ast_StackPop(PStack(L));
    ast_StackPop(PStack(L));
    return TRUE;
}
ast_Bool _ast_Eq_(ast_State *L, Instruction i)
{
    return _ast_Compare(L, i, AST_OPEQ);
}
ast_Bool _ast_Lt_(ast_State *L, Instruction i)
{
    return _ast_Compare(L, i, AST_OPLT);
}
ast_Bool _ast_Le_(ast_State *L, Instruction i)
{
    return _ast_Compare(L, i, AST_OPLE);
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
    ast_AddPc(L, n.sbx);
    return TRUE;
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
        ast_AddPc(L, n.sbx);
        astack_Copy(PStack(L), n.a, n.a + 3);
    }
    return TRUE;
}
// END

// TABLE
////R(A) = {}(size = B,C)
ast_Bool _ast_NewTable(ast_State *L, Instruction i)
{
    TABC n = InstructionTABC(i);
    ast_CreateTableAndPush(L, FbToInt(n.b), FbToInt(n.c));
    astack_ReplaceToIdx(PStack(L), n.a);
    return TRUE;
}
////R(A) = R(B)[RK(C)]
ast_Bool _ast_GetTable(ast_State *L, Instruction i)
{
    TABC n = InstructionTABC(i);
    ast_GetRk(L, n.c);
    ast_GetTableFromIdx(L, n.b);
    astack_ReplaceToIdx(PStack(L), n.a);
    return TRUE;
}
////R(A)[RK(B)] = RK(C)
ast_Bool _ast_SetTable(ast_State *L, Instruction i)
{
    TABC n = InstructionTABC(i);
    ast_GetRk(L, n.b);
    ast_GetRk(L, n.c);
    ast_SetTableFromIdx(L, n.a);
    TValue tb = L->stack->Value[n.a];
    return TRUE;
}
////R(A)[(C - 1) * FPF + i] = R(A + i) 1 <= i <= B 给数组赋值
ast_Bool _ast_SetList(ast_State *L, Instruction i)
{
    TABC n = InstructionTABC(i);
    if (n.c > 0)
    {
        n.c -= 1;
    }
    else
    {
        int nn = InstructionTAX(ast_Fetch(L));
        n.c = nn;
    }
    int idx = (n.c * FPF);
    TValue tt;
    tt.tt = AST_TINTEGER;
    int flag = 0;
    for (int i = 1; i <= n.b; i++)
    {
        idx++;
        tt.value.i = idx;
        TValue tmp = ast_StackGetTValue(PStack(L), n.a + i);
        ast_StackPush(PStack(L), tmp);
        ast_SetTableFromNum(L, n.a, tt);
    }
    if (n.b == 0)
    {
        for (int i = 1; i <= L->stack->nPrevFuncResults; i++)
        {

            idx++;
            tt.value.i = idx;
            TValue tmp = ast_StackGetTValue(PStack(L), n.a + i);
            ast_StackPush(PStack(L), tmp);
            ast_SetTableFromNum(L, n.a, tt);
        }
        ast_StackSetTop(PStack(L), ast_RegCount(L));
    }
    return TRUE;
}
// END

// FUNCTION
////R(A) = clousure(KPROTO[Bx])
ast_Bool _ast_Clousure(ast_State *L, Instruction i)
{
    TABx n = InstructionTABx(i);
    ast_LoadProto(L, n.bx);
    astack_ReplaceToIdx(PStack(L), n.a);
    return TRUE;
}
ast_Bool _FixStack(ast_State *L, int a)
{
    TValue num = ast_StackPop(PStack(L));
    ast_Integer n = ast_ConvertToInteger(num);
    astack_CheckStack(PStack(L), n - a);
    for (int i = a; i < n; i++)
    {
        TValue tt = ast_StackGetTValue(PStack(L), i);
        ast_StackPush(PStack(L), tt);
    }
    ast_StackRotate(PStack(L), ast_RegCount(L), n - a);
    return TRUE;
}
ast_Integer _PushFuncAndArgs(ast_State *L, int a, int b)
{
    if (b >= 1)
    {
        ast_StackCheck(PStack(L), b);
        for (int i = a; i < a + b; i++)
        {
            TValue tt = ast_StackGetTValue(PStack(L), i);
            ast_StackPush(PStack(L), tt);
        }
        return b - 1;
    }
    else
    {
        // _FixStack(L, a);
        int num = L->stack->PrevIdx - a + L->stack->nPrevFuncResults - 1;
        ast_StackCheck(PStack(L), num);
        for (int i = a; i <= a + num; i++)
        {
            TValue tt = ast_StackGetTValue(PStack(L), i);
            ast_StackPush(PStack(L), tt);
        }
        return num;
    }
}
ast_Bool _PopResults(ast_State *L, int a, int c)
{
    if (c == 1)
    {
        return TRUE;
    }
    else if (c > 1)
    {
        for (int i = a; i <= a + c - 2; i++)
        {

            astack_ReplaceToIdx(PStack(L), i);
        }
    }
    else
    {
        for (int i = a; i <= a + L->stack->nPrevFuncResults - 1; i++)
        {
            astack_ReplaceToIdx(PStack(L), i);
        }
    }
    return TRUE;
}
////R(A),.....R(A+C-2) = R(A)(R(A+1).....R(A+B-1))
ast_Bool _ast_Call(ast_State *L, Instruction i)
{
    TABC n = InstructionTABC(i);
    ast_Integer nArgs = _PushFuncAndArgs(L, n.a, n.b);
    if (n.c == 0)
    {
        L->stack->PrevIdx = n.a;
    }
    ast_Call(L, nArgs, n.c - 1);
    _PopResults(L, n.a, n.c);
    return TRUE;
}
////return R(A) ..... R(A + B - 2)
ast_Bool _ast_Return(ast_State *L, Instruction i)
{
    TABC n = InstructionTABC(i);
    if (n.b == 0)
    {
        n.b = L->stack->nPrevFuncResults + 1;
    }
    if (n.b == 1)
    {
        return TRUE;
    }
    else if (n.b > 1)
    {
        ast_StackCheck(L->stack->prev, n.b - 1);
        for (int i = n.a + n.b - 2; i >= n.a; i--)
        {
            TValue tt = ast_StackGetTValue(PStack(L), i);
            ast_StackPush(L->stack->prev, tt);
        }
        L->stack->prev->nPrevFuncResults = n.b - 1;
    }
    return TRUE;
}
ast_Bool _ast_VarArg(ast_State *L, Instruction i)
{
    TABC n = InstructionTABC(i);
    if (n.b != 1)
    {
        ast_LoadVararg(L, n.b - 1);
        _PopResults(L, n.a, n.b);
    }
    return TRUE;
}
ast_Bool _ast_TailCall(ast_State *L, Instruction i)
{
    return _ast_Call(L, i);
}
//// R(A + 1) = R(B) R(A) = R(B)[RK(C)]
ast_Bool _ast_Self(ast_State *L, Instruction i)
{
    TABC n = InstructionTABC(i);
    astack_Copy(PStack(L), n.b, n.a + 1);
    ast_GetRk(L, n.c);
    ast_GetTableFromIdx(L, n.b);
    astack_ReplaceToIdx(PStack(L), n.a);
    return TRUE;
}
// END
// GetTabUp R[A] = Upvals[B][RK[C]]
ast_Bool _ast_GetTabUp(ast_State *L, Instruction i)
{
    TABC n = InstructionTABC(i);
    ast_GetRk(L, n.c);
    ast_GetTableFromIdx(L, AstUpvalueIndex(n.b));
    astack_ReplaceToIdx(PStack(L), n.a);
    return TRUE;
}
// GetUpval R[A] = Upvals[B]
ast_Bool _ast_GetUpval(ast_State *L, Instruction i)
{
    TABC n = InstructionTABC(i);
    astack_Copy(PStack(L), AstUpvalueIndex(n.b), n.a);
    TValue tb = L->stack->Value[n.a];
    return TRUE;
}
// SetUpval Upvals[B] = R[A]
ast_Bool _ast_SetUpval(ast_State *L, Instruction i)
{
    TABC n = InstructionTABC(i);
    TValue tt = ast_StackGetTValue(PStack(L), n.a);
    L->stack->closure->Upvalues[n.b] = tt;
    return TRUE;
}
// SetTabUp Upvals[A]RK[B] = RK[C]
ast_Bool _ast_SetTabUp(ast_State *L, Instruction i)
{
    TABC n = InstructionTABC(i);
    ast_GetRk(L, n.b);
    ast_GetRk(L, n.c);
    ast_SetTableFromIdx(L, AstUpvalueIndex(n.a));

    return TRUE;
}
//  END
// MetaTable
// END
// next
// TForCall
ast_Bool _ast_TForCall(ast_State *L, Instruction i)
{
    TABC n = InstructionTABC(i);
    _PushFuncAndArgs(L, n.a, 3);
    ast_Call(L, 2, n.c);
    _PopResults(L, n.a + 3, n.c + 1);
    return TRUE;
}
ast_Bool _ast_TForLoop(ast_State *L, Instruction i)
{
    TAsBx n = InstructionTAsBx(i);
    TValue it = ast_StackGetTValue(PStack(L), n.a + 1);
    if (it.tt != AST_TNIL)
    {
        astack_Copy(PStack(L), n.a + 1, n.a);
        ast_AddPc(L, n.sbx);
    }
    return TRUE;
}
// END
ast_OpCode g_ast_opcodes[47] = {
    /*T  A     B       C    mode    name     action  */
    {0, 1, OpArgR, OpArgN, IABC, "MOVE    ", _ast_Move},
    {0, 1, OpArgK, OpArgN, IABx, "LOADK   ", _ast_LoadK},
    {0, 1, OpArgN, OpArgN, IABx, "LOADKX  ", _ast_LoadKx},
    {0, 1, OpArgU, OpArgU, IABC, "LOADBOOL", _ast_LoadBool},
    {0, 1, OpArgU, OpArgN, IABC, "LOADNIL ", _ast_LoadNil},
    {0, 1, OpArgU, OpArgN, IABC, "GETUPVAL", _ast_GetUpval},
    {0, 1, OpArgU, OpArgK, IABC, "GETTABUP", _ast_GetTabUp},
    {0, 1, OpArgR, OpArgK, IABC, "GETTABLE", _ast_GetTable},
    {0, 0, OpArgK, OpArgK, IABC, "SETTABUP", _ast_SetTabUp},
    {0, 0, OpArgU, OpArgN, IABC, "SETUPVAL", _ast_SetUpval},
    {0, 0, OpArgK, OpArgK, IABC, "SETTABLE", _ast_SetTable},
    {0, 1, OpArgU, OpArgU, IABC, "NEWTABLE", _ast_NewTable},
    {0, 1, OpArgR, OpArgK, IABC, "SELF    ", _ast_Self},
    {0, 1, OpArgK, OpArgK, IABC, "ADD     ", _ast_Add_},
    {0, 1, OpArgK, OpArgK, IABC, "SUB     ", _ast_Sub_},
    {0, 1, OpArgK, OpArgK, IABC, "MUL     ", _ast_Mul_},
    {0, 1, OpArgK, OpArgK, IABC, "MOD     ", _ast_Mod_},
    {0, 1, OpArgK, OpArgK, IABC, "POW     ", _ast_Pow_},
    {0, 1, OpArgK, OpArgK, IABC, "DIV     ", _ast_Div_},
    {0, 1, OpArgK, OpArgK, IABC, "IDIV    ", _ast_Idiv_},
    {0, 1, OpArgK, OpArgK, IABC, "BAND    ", _ast_Band_},
    {0, 1, OpArgK, OpArgK, IABC, "BOR     ", _ast_Bor_},
    {0, 1, OpArgK, OpArgK, IABC, "BXOR    ", _ast_Bxor_},
    {0, 1, OpArgK, OpArgK, IABC, "SHL     ", _ast_Shl_},
    {0, 1, OpArgK, OpArgK, IABC, "SHR     ", _ast_Shr_},
    {0, 1, OpArgR, OpArgN, IABC, "UNM     ", _ast_Unm_},
    {0, 1, OpArgR, OpArgN, IABC, "BNOT    ", _ast_Bnot_},
    {0, 1, OpArgR, OpArgN, IABC, "NOT     ", _ast_Not},
    {0, 1, OpArgR, OpArgN, IABC, "LEN     ", _ast_Length},
    {0, 1, OpArgR, OpArgR, IABC, "CONCAT  ", _ast_Concat},
    {0, 0, OpArgR, OpArgN, IAsBx, "JMP     ", _ast_Jmp},
    {1, 0, OpArgK, OpArgK, IABC, "EQ      ", _ast_Eq_},
    {1, 0, OpArgK, OpArgK, IABC, "LT      ", _ast_Lt_},
    {1, 0, OpArgK, OpArgK, IABC, "LE      ", _ast_Le_},
    {1, 0, OpArgN, OpArgU, IABC, "TEST    ", _ast_Test},
    {1, 1, OpArgR, OpArgU, IABC, "TESTSET ", _ast_TestSet},
    {0, 1, OpArgU, OpArgU, IABC, "CALL    ", _ast_Call},
    {0, 1, OpArgU, OpArgU, IABC, "TAILCALL", _ast_Call},
    {0, 0, OpArgU, OpArgN, IABC, "RETURN  ", _ast_Return},
    {0, 1, OpArgR, OpArgN, IAsBx, "FORLOOP ", _ast_ForLoop},
    {0, 1, OpArgR, OpArgN, IAsBx, "FORPREP ", _ast_ForPrep},
    {0, 0, OpArgN, OpArgU, IABC, "TFORCALL", _ast_TForCall},
    {0, 1, OpArgR, OpArgN, IAsBx, "TFORLOOP", _ast_TForLoop},
    {0, 0, OpArgU, OpArgU, IABC, "SETLIST ", _ast_SetList},
    {0, 1, OpArgU, OpArgN, IABx, "CLOSURE ", _ast_Clousure},
    {0, 1, OpArgU, OpArgN, IABC, "VARARG  ", _ast_VarArg},
    {0, 0, OpArgU, OpArgU, IAx, "EXTRAARG"},
};

ast_Bool ast_ExecuteOp(ast_State *L, Instruction i)
{
    ast_OpCode code = g_ast_opcodes[InstructionOpcode(i)];
    if (code.ast_OpAction != nullptr)
    {
        code.ast_OpAction(L, i);
    }
    else
    {
        PANIC("%s", code.name);
    }
    return TRUE;
}