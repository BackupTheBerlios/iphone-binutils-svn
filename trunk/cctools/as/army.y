%{
#include <stdio.h>
#include <stdlib.h>
#include <mach-o/arm/reloc.h>

#include "arm.h"
#include "struc-symbol.h"

#define YYDEBUG 1

unsigned int instruction;
%}

%union {
    unsigned int nval;
    signed int ival; 
    symbolS *sval;
}

%token <nval> OPRD_REG
%token <ival> OPRD_IMM
%token <nval> INST_BL INST_BX INST_ADD_LIKE INST_LDM INST_LDR_LIKE INST_MOV_LIKE
%token <nval> INST_STM
%token <sval> OPRD_SYM
%token <nval> COND CCUP LMAM SMAM
%type  <nval> inst branch_inst data_inst load_inst load_mult_inst maybe_bang 
%type  <nval> reg_list src_reg dest_reg shifter_operand load_am branch_am
%type  <ival> expr

%%

inst:
      branch_inst       { instruction = $1; }
    | data_inst         { instruction = $1; }
    | load_inst         { instruction = $1; }
    | load_mult_inst    { instruction = $1; }
    ;

branch_inst:
      INST_BL { lexpect(AE_COND); } COND { lexpect(AE_OPRD); } branch_am
        {   $$ = ($1 | $3 | $5); }
    | INST_BX { lexpect(AE_COND); } COND { lexpect(AE_OPRD); } OPRD_REG
        {   $$ = ($1 | $3 | $5); }
    ;

data_inst:
      INST_MOV_LIKE { lexpect(AE_COND); } COND { lexpect(AE_CCUP); } CCUP
        { lexpect(AE_OPRD); } dest_reg ',' shifter_operand
        {   $$ = ($1 | $3 | $5 | $7 | $9); }
    | INST_ADD_LIKE { lexpect(AE_COND); } COND { lexpect(AE_CCUP); } CCUP { lexpect(AE_OPRD); } dest_reg ',' src_reg ',' shifter_operand
        { $$ = ($1 | $3 | $5 | $7 | $9 | $11); }
    ;

load_inst:
      INST_LDR_LIKE { lexpect(AE_COND); } COND { lexpect(AE_OPRD); } dest_reg
        ',' load_am
        {   $$ = ($1 | $3 | $5 | $7);   }
    ; 

load_mult_inst:
      INST_LDM { lexpect(AE_COND); } COND { lexpect(AE_LMAM); } LMAM
        { lexpect(AE_OPRD); } src_reg maybe_bang ',' '{' reg_list '}'
        {   $$ = ((1 << 27) | $1 | $3 | $5 | $7 | $8 | $11); } 
    | INST_STM { lexpect(AE_COND); } COND { lexpect(AE_SMAM); } SMAM
        { lexpect(AE_OPRD); } src_reg maybe_bang ',' '{' reg_list '}'
        {   $$ = ((1 << 27) | $1 | $3 | $5 | $7 | $8 | $11); } 
    ;

maybe_bang:
    /* empty */     { $$ = 0; }
    | '!'           { $$ = (0x1 << 21); }
    ;

reg_list:
    /* empty */             { $$ = 0;                   }
    | OPRD_REG              { $$ = (1 << $1);           }
    | reg_list ',' OPRD_REG { $$ = ($1 | (1 << $3));    }
    ;

src_reg:
      OPRD_REG      { $$ = ($1 << 16); }
    ;

dest_reg:
      OPRD_REG      { $$ = ($1 << 12); }
    ;

shifter_operand:
      '#' OPRD_IMM
        {
            /* TODO: support expressions here */

            if ($2 > 0xff)
                abort();    /* FIXME: try to rotate to fit it in */
            $$ = ((0x1 << 25) | $2);
        }
    | OPRD_REG      { $$ = $1; }
    ;

load_am:
      expr
        {
            /* assumes PC-relative addressing */
            register_reloc_type(ARM_RELOC_PCREL_DATA_IMM12, 4, 1);
            $$ = ((1 << 26) | (1 << 24) | (15 << 16) |
                ($1 < 0 ? -$1 : ($1 | (1 << 23)))); 
        }
    | '[' OPRD_REG ',' '#' expr ']'
        {
            /* TODO: allow symbols here */
            $$ = ((1 << 26) | (1 << 24) | ($2 << 16) |
                ($5 < 0 ? -$5 : ($5 | (1 << 23))));
        }
    | '[' OPRD_REG ']'
        {
            $$ = ((1 << 26) | (1 << 24) | ($2 << 16));
        }
    ;

branch_am:
      expr
        {
            register_reloc_type(ARM_RELOC_PCREL_IMM24, 4, 1);
            $$ = $1;
        }
    ;

expr:
      OPRD_IMM              { $$ = $1;                              }
    | '+' OPRD_IMM          { $$ = $2;                              }
    | '-' OPRD_IMM          { $$ = -$2;                             }
    | OPRD_SYM              { register_add_symbol($1, 0);  $$ = 0;  }
    ;

%%

