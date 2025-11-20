#include "../include/symbol.h"
#include "../include/common.h"

static const char* varTypeName(VarType t)
{
    switch(t)
    {
        case VT_INT: return "integer";
        case VT_FLOAT: return "float";
        case VT_CHAR: return "char";
        case VT_BOOL: return "bool";
        default: return "unknown";
    }
}

SymbolTable* symtabInit(void)
{
    SymbolTable *symtab = (SymbolTable *)malloc(sizeof(SymbolTable));
    symtab->currentLevel = 0;
    symtab->procCount= 0;
    symtab->varCount = 0;
    strcpy(symtab->currentProc, "global");
    return symtab;
}

void generateVarFile(SymbolTable *symtab, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if(!file)
    {
        error("Fail to generate the variable table!");
        return;
    }
    for(int i=0; i < symtab->varCount; i++)
    {
        VariableEntry *var = &symtab->varibles[i];
        fprintf(file, "%s %s %d %s %d %d\n",
                var->vname, var->vproc,
                var->vkind, varTypeName(var->vtype), var->vlev, var->vadr);
    }
    fclose(file);
}

void generateProcFile(SymbolTable *symtab, const char *filename)
{
    FILE *file = fopen(filename, "w");
    if(!file)
    {
        error("Fail to generate the process table!");
        return;
    }
    for(int i=0; i < symtab->procCount; i++)
    {
        ProcedureEntry *proc = &symtab->procedures[i];
        fprintf(file, "%s %d %d %d %d\n",
                proc->pname, proc->ptype, proc->plev,
                proc->fadr, proc->ladr);
    }
    fclose(file);
}

void symtabCleanup(SymbolTable *symtab)
{
    free(symtab);
}

void symtabEnterProc(SymbolTable *symtab, const char *procname, ProcType ptype)
{
    ProcedureEntry *proc = &symtab->procedures[symtab->procCount++];
    strncpy(proc->pname, procname, MAX_IDENTIFERLEN_LEN - 1);
    proc->pname[MAX_IDENTIFERLEN_LEN - 1] = '\0';
    proc->ptype = ptype;
    proc->plev = symtab->currentLevel + 1;
    proc->fadr = symtab->varCount;
    proc->ladr = symtab->varCount - 1;
    symtab->currentLevel++;
    strncpy(symtab->currentProc, procname, MAX_IDENTIFERLEN_LEN - 1);
    symtab->currentProc[MAX_IDENTIFERLEN_LEN - 1] = '\0';
}

void symtabExitProc(SymbolTable *symtab)
{
    if(symtab->procCount > 0)
    {
        ProcedureEntry *proc = &symtab->procedures[symtab->procCount - 1];
        proc->ladr = symtab->varCount - 1;
    }
    if(symtab->currentLevel > 0) symtab->currentLevel--;
    strcpy(symtab->currentProc, "global");
}

int symtabAddVariable(SymbolTable *symtab, const char *varname, VarType vtype, VarKind vkind)
{
    VariableEntry *var = &symtab->varibles[symtab->varCount];
    strncpy(var->vname, varname, MAX_IDENTIFERLEN_LEN - 1);
    var->vname[MAX_IDENTIFERLEN_LEN - 1] = '\0';
    strncpy(var->vproc, symtab->currentProc, MAX_IDENTIFERLEN_LEN - 1);
    var->vproc[MAX_IDENTIFERLEN_LEN - 1] = '\0';
    var->vkind = vkind;
    var->vtype = vtype;
    var->vlev = symtab->currentLevel;
    var->vadr = symtab->varCount;
    symtab->varCount++;
    return var->vadr;
}