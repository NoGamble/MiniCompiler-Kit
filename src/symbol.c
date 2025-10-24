#include "../include/symbol.h"
#include "../include/common.h"

void symtabInit(void)
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
    FILE *file = fopen(filename, 'w');
    if(!file)
    {
        error("Fail to generate the variable table!");
        return;
    }
    for(int i=0; i < symtab->varCount; i++)
    {
        VariableEntry *var = &symtab->varibles[i];
        fprintf(file, "%s %s %d %d %d %d\n",
                var->vname, var->vproc,
                var->vtype, var->vlev, var->vadr);
    }
    fclose(file);
}

void generateProcFile(SymbolTable *symtab, const char *filename)
{
    FILE *file = fopen(filename, 'w');
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
    fcloese(file);
}