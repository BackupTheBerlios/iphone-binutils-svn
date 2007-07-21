#ifndef ARM_H
#define ARM_H

#include <stdio.h>

#include "expr.h"
#include "struc-symbol.h"
#include "mach-o/arm/reloc.h"

#define AE_INIT 0
#define AE_COND 1
#define AE_CCUP 2
#define AE_LMAM 3
#define AE_OPRD 4
#define AE_SMAM 5
#define AE_BYTM 6
#define AE_PSRU 7
#define AE_LAMS 8
#define AE_TRNS 9

struct fix_info {
    int needed;

    int type;
    int size;
    int pcrel;

    expressionS *expr;
};

extern char *cur_ptr;
extern unsigned int instruction;
extern int yydebug;

void register_reloc_type(int type, int size, int pcrel);
void register_expression(expressionS *expr);

void lexpect(int what);

int yyparse();
int yyerror(char *err);
int yylex();
void yyrestart(FILE *f);

#endif

