#ifndef ARM_H
#define ARM_H

#include "struc-symbol.h"

#define AE_INIT 0
#define AE_COND 1
#define AE_CCUP 2
#define AE_LMAM 3
#define AE_OPRD 4
#define AE_SMAM 5

struct fix_info {
    int needed;

    symbolS *add_symbol;
    symbolS *sub_symbol;
    int offset;

    int type;
    int size;
    int pcrel; 
};

void register_reloc_type(int type, int size, int pcrel);
void register_add_symbol(symbolS *symbol, int offset);
void register_sub_symbol(symbolS *symbol, int offset);

void lexpect(int what);

int yyerror(char *err);
int yylex();

#endif

