%{
/* ----------------------------------------------------------------------------
 *   iphone-binutils: development tools for the Apple iPhone       07/18/2007
 *   Copyright (c) 2007 Patrick Walton <pcwalton@uchicago.edu> but freely
 *   redistributable under the terms of the GNU General Public License v2.
 *
 *   army.y - the parser for ARM assembly
 * ------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <mach-o/arm/reloc.h>

#include "arm.h"
#include "messages.h"
#include "struc-symbol.h"

#define YYDEBUG         1
#define YYERROR_VERBOSE 1

unsigned int instruction;
%}

%union {
    unsigned int nval;
    signed int ival; 
    expressionS *eval;
}

%token <nval> OPRD_REG
%token <ival> OPRD_IMM
%token <nval> OP_BRANCH OP_DATA_PROC_1 OP_DATA_PROC_2 OP_DATA_PROC_3 OP_MUL
%token <nval> OP_MLA OP_SMLAL OP_CLZ OP_LDR OP_LDRH OP_LDM OP_SWI OP_BKPT
%token <nval> OPRD_LSL_LIKE OPRD_RRX
%token <eval> OPRD_EXP
%type  <ival> expr
%type  <nval> inst branch_inst data_inst load_inst load_mult_inst maybe_bang 
%type  <nval> reg_list src_reg dest_reg shifter_operand load_am branch_am
%type  <nval> exception_inst multiply_inst maybe_am_lsl_subclause
%type  <nval> load_am_indexed reg_lists reg_list_atom reg_list_contents
%type  <nval> maybe_hat maybe_imm_rotation misc_ls_am imm_with_u_bit
%type  <nval> misc_ls_am_index shifter_imm shifter_operand_lsl_clause
%type  <nval> shifter_operand_lsl_arg load_am_sign fundamental_inst armv6_inst

%%

inst:
      fundamental_inst  { instruction = $1; }
    | armv6_inst        { instruction = $1; }
;

fundamental_inst:
      branch_inst       { instruction = $1; }
    | data_inst         { instruction = $1; }
    | load_inst         { instruction = $1; }
    | load_mult_inst    { instruction = $1; }
    | exception_inst    { instruction = $1; }
    | multiply_inst     { instruction = $1; }
    ;

branch_inst:
      OP_BRANCH branch_am   { $$ = ($1 | $2); }
    ;

data_inst:
      OP_DATA_PROC_1 dest_reg ',' shifter_operand   { $$ = ($1 | $2 | $4); }
    | OP_DATA_PROC_2 src_reg ',' shifter_operand
        { $$ = ($1 | $2 | $4 | (1 << 20)); }
    | OP_DATA_PROC_3 dest_reg ',' src_reg ',' shifter_operand
        { $$ = ($1 | $2 | $4 | $6); }
    ;

load_inst:
      OP_LDR dest_reg ',' load_am
        {
            unsigned int n;
            n = ((1 << 26) | $2 | $4);
            if ($1 && ((n >> 24) & 0xf) == 0x5 && (n & 0x0fff) == 0) {
                /* if T is set and immediate operand is 0, then convert into
                 * a post-indexed instruction */
                n &= ~(1 << 24);    /* clear 24th bit */
                n |= (1 << 23);     /* set U bit */
            }
            $$ = n;
        }
    | OP_LDRH dest_reg ',' misc_ls_am   { $$ = ($1 | $2 | $4); }
    ; 

load_mult_inst:
      OP_LDM src_reg maybe_bang ',' reg_lists maybe_hat
        { $$ = ((1 << 27) | $1 | $2 | $3 | $5 | $6); }
    ;

exception_inst:
      OP_BKPT OPRD_IMM  { $$ = ($1 | (($2 & 0xfff0) << 8) | ($2 & 0x000f)); }
    | OP_SWI OPRD_IMM   { $$ = ($1 | $2); }
    ;

multiply_inst:
      OP_MUL OPRD_REG ',' OPRD_REG ',' OPRD_REG
        { $$ = ($1 | ($2 << 16) | $4 | ($6 << 8)); }
    | OP_MLA OPRD_REG ',' OPRD_REG ',' OPRD_REG ',' OPRD_REG
        { $$ = ($1 | ($2 << 16) | $4 | ($6 << 8) | ($8 << 12)); }
    | OP_SMLAL OPRD_REG ',' OPRD_REG ',' OPRD_REG ',' OPRD_REG
        { $$ = ($1 | ($2 << 12) | ($4 << 16) | $6 | ($8 << 8)); }
    ; 

maybe_hat:
      /* empty */   { $$ = 0;           }
    | '^'           { $$ = (1 << 22);   }
    ;

maybe_bang:
    /* empty */     { $$ = 0; }
    | '!'           { $$ = (0x1 << 21); }
    ;

reg_lists:
      reg_list                                  { $$ = $1;          }
    | reg_lists plus_or_bar reg_list            { $$ = ($1 | $3);   }
    ;

plus_or_bar:
      '+'
    | '|'
    ;

reg_list:
      '{' reg_list_contents '}'     { $$ = $2; }
    | OPRD_IMM                      { $$ = $1; }

reg_list_contents:
      /* empty */                           { $$ = 0;           }
    | reg_list_atom                         { $$ = $1;          }
    | reg_list_contents ',' reg_list_atom   { $$ = ($1 | $3);   }
    ;

reg_list_atom:
      OPRD_REG                      { $$ = (1 << $1);   }
    | OPRD_REG '-' OPRD_REG         {
                                        int i;
                                        unsigned int n = 0;
                                        for (i = $1; i <= $3; i++)
                                            n |= (1 << i);
                                        $$ = n;
                                    }
    ;

src_reg:
      OPRD_REG      { $$ = ($1 << 16); }
    ;

dest_reg:
      OPRD_REG      { $$ = ($1 << 12); }
    ;

shifter_operand:
      '#' shifter_imm   { $$ = $2; }
    | OPRD_REG ',' shifter_operand_lsl_clause   { $$ = ($1 | $3); }
    | OPRD_REG  { $$ = $1; }
    ;

shifter_operand_lsl_clause:
      OPRD_LSL_LIKE shifter_operand_lsl_arg { $$ = ($1 | $2); }
    | OPRD_RRX { $$ = $1; }
    ;

shifter_operand_lsl_arg:
      '#' OPRD_IMM
        {
            unsigned int n = $2;
            if (n == 32)
                n = 0;
            if (n >= (1 << 5))
                as_bad("immediate value (%d) too large", $2);
            $$ = (n << 7); 
        }
    | OPRD_REG  { $$ = ((1 << 4) | ($1 << 8)); }
    ;

shifter_imm:
      OPRD_IMM maybe_imm_rotation
        {
            /* TODO: try to rotate to fit the imm in automatically */
            if ($1 > 0xff || $1 < 0)
                as_bad("immediate constant too large");
            $$ = ((0x1 << 25) | $1 | $2);
        }
    | expr
        {
            register_reloc_type(ARM_RELOC_SHIFT_IMM12, 4, 0);
            $$ = ((0x1 << 25) | $1);
        }
    ;

maybe_imm_rotation:
      /* empty */       { $$ = 0;               }
    | ',' OPRD_IMM      { $$ = (($2 / 2) << 8); }
    ;

misc_ls_am:
      '[' src_reg ',' misc_ls_am_index ']' maybe_bang
        { $$ = ((1 << 24) | (1 << 7) | (1 << 4) | $2 | $4 | $6); }
    | '[' src_reg ']' ',' misc_ls_am_index
        { $$ = ($2 | $5); }
    | '[' src_reg ']'
        { $$ = ((1 << 24) | (1 << 22) | (1 << 23) | $2); }
    ;

misc_ls_am_index:
      '#' imm_with_u_bit
        {
            $$ = ((1 << 22) | ((($2 & 0xf0) >> 4) << 8) | ($2 & 0x0f) |
                ($2 & (1 << 23)));
        }
    | load_am_sign OPRD_REG
        { $$ = ((1 << 7) | (1 << 4) | $1 | $2); }
    ;

load_am_sign:
      /* empty */   { $$ = (1 << 23); }
    | '+'           { $$ = (1 << 23); }
    | '-'           { $$ = (0 << 23); }
    ;

imm_with_u_bit:
      OPRD_IMM      { $$ = ($1 < 0 ? -$1 : ((1 << 23) | $1)); }
    ;

load_am:
      expr
        {
            /* assumes PC-relative addressing */
            int n;
            n = $1 - 8;
            register_reloc_type(ARM_RELOC_PCREL_DATA_IMM12, 4, 1);
            $$ = ((1 << 26) | (1 << 24) | (15 << 16) |
                (n < 0 ? -n : (n | (1 << 23)))); 
        }
    | '[' OPRD_REG ',' load_am_indexed ']' maybe_bang
        {
            $$ = ((1 << 26) | (1 << 24) | ($2 << 16) | $4 | $6);
        }
    | '[' OPRD_REG ']' ',' load_am_indexed
        {
            $$ = ((1 << 26) | ($2 << 16) | $5);
        }
    | '[' OPRD_REG ']'
        {
            $$ = ((1 << 26) | (1 << 24) | (1 << 23) | ($2 << 16));
        }
    ;

load_am_indexed:
      '#' OPRD_IMM { $$ = ($2 < 0 ? -$2 : ($2 | (1 << 23))); }
    | load_am_sign OPRD_REG maybe_am_lsl_subclause
        {
            $$ = ($1 | $2 | $3 | (1 << 25));
        }
    ;

maybe_am_lsl_subclause:
      /* empty */                       {   $$ = 0;                 }
    | ',' OPRD_LSL_LIKE '#' OPRD_IMM
        {
            unsigned int n = $4;
            if (n == 32)
                n = 0;
            if (n >= (1 << 5))
                as_bad("immediate value (%d) too large", $4);
            $$ = ($2 | (n << 7));
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
      OPRD_EXP  { register_expression($1); $$ = $1->X_add_number;  }
    ;

%%

