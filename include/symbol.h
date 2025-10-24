#ifndef SYMBOL_H
#define SYMBOL_H

#include "common.h"
#define MAX_SYMBOLS 100

typedef enum {
    VK_VARIABLE = 0,
    VK_PARAMETER = 1
} VarKind;

typedef enum {
    VT_INT,
    VT_FLOAT,
    VT_CHAR,
    VT_BOOL
} VarType;

typedef enum{
    PT_PROGRAM,
    PT_FUNCTION,
    PT_PROCEDURE
} ProcType;

typedef struct{
    char vname[MAX_IDENTIFERLEN_LEN];
    char vproc[MAX_IDENTIFERLEN_LEN];
    VarKind vkind;
    VarType vtype;
    int vlev;
    int vadr;
} VariableEntry;

typedef struct{
    char pname[MAX_IDENTIFERLEN_LEN];
    ProcType ptype;
    int plev;
    int fadr;
    int ladr;
} ProcedureEntry;

typedef struct{
    VariableEntry varibles[MAX_SYMBOLS];
    ProcedureEntry procedures[MAX_SYMBOLS];
    int varCount;
    int procCount;
    int currentLevel;
    char currentProc[MAX_IDENTIFERLEN_LEN];
} SymbolTable;

SymbolTable* symtabInit(void);
void symtabCleanup(SymbolTable *symtab);
void symtabEnterProc(SymbolTable *symtab, const char *procname, ProcType ptype);
void symtabExitProc(SymbolTable *symtab);
int symtabAddVariable(SymbolTable *symtab, const char *varname, VarType vtype, VarKind vkind);

void generateVarFile(SymbolTable *symtab, const char *filename);
void generateProcFile(SymbolTable *symtab, const char *filename);

#endif