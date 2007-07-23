#ifndef ARM_H
#define ARM_H

#include <stdio.h>

#include "expr.h"
#include "struc-symbol.h"
#include "mach-o/arm/reloc.h"

#define ROTL(n, k)  (((n) << (k)) | ((n) >> (32 - (k))))

/* We INITialize in this phase. */
#define AE_INIT 0
/* Expect a CONDition. */
#define AE_COND 1
/* Expect an 's' or nothing to specify if the Condition Codes are UPdated. */
#define AE_CCUP 2
/* Expect a Load Multiple Addressing Mode. */
#define AE_LMAM 3
/* Expect a piece of an operand (reserved words only). */
#define AE_OPRD 4
/* Expect a Store Multiple Addressing Mode. */
#define AE_SMAM 5
/* Expect a 'b' or nothing for byte mode */
#define AE_BYTM 6
#define AE_PSRU 7
/* Expect a Load Addressing Mode Sign: +, -, or assume +. */
#define AE_LAMS 8
/* Expect a 't' or nothing to specify if TRaNSlation is to occur. */
#define AE_TRNS 9
/* Expect a LSL-like keyword, including RRX. This is an ugly hack in here
 * because GAS eliminates any whitespace between the keyword and a register
 * name, if one follows it (resulting in junk like "mov r5,r6,asrr7") */
#define AE_LSLK 10
/* used to separate instructions from operands */
#define AE_SP 11

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

