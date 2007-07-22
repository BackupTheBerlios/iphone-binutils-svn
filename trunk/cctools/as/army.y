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

#define YYDEBUG 1

unsigned int instruction;
%}

%union {
    unsigned int nval;
    signed int ival; 
    expressionS *eval;
}

%token <nval> OPRD_REG
%token <ival> OPRD_IMM
%token <nval> INST_BL INST_BX INST_ADD_LIKE INST_LDM INST_LDR_LIKE INST_STM
%token <nval> INST_MOV_LIKE INST_BKPT INST_CMP_LIKE INST_MLA INST_MUL INST_SWI
%token <nval> INST_SMLAL_LIKE INST_LDRH_LIKE
%token <nval> OPRD_LSL_LIKE OPRD_RRX
%token <eval> OPRD_EXP
%token <nval> BYTM_HALF
%token <nval> COND CCUP LMAM SMAM BYTM PSRU LAMS TRNS LSLK
%type  <ival> expr
%type  <nval> inst branch_inst data_inst load_inst load_mult_inst maybe_bang 
%type  <nval> reg_list src_reg dest_reg shifter_operand load_am branch_am
%type  <nval> exception_inst multiply_inst maybe_am_lsl_subclause
%type  <nval> load_am_indexed reg_lists reg_list_atom reg_list_contents
%type  <nval> maybe_hat maybe_imm_rotation misc_ls_am load_body imm_with_u_bit
%type  <nval> misc_ls_am_index shifter_imm shifter_operand_lsl_clause
%type  <nval> shifter_operand_lsl_arg 

%%

inst:
      branch_inst       { instruction = $1; }
    | data_inst         { instruction = $1; }
    | load_inst         { instruction = $1; }
    | load_mult_inst    { instruction = $1; }
    | exception_inst    { instruction = $1; }
    | multiply_inst     { instruction = $1; }
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
    | INST_ADD_LIKE { lexpect(AE_COND); } COND { lexpect(AE_CCUP); } CCUP
        { lexpect(AE_OPRD); } dest_reg ',' src_reg ',' shifter_operand
        { $$ = ($1 | $3 | $5 | $7 | $9 | $11); }
    | INST_CMP_LIKE { lexpect(AE_COND); } COND { lexpect(AE_PSRU); } PSRU
        { lexpect(AE_OPRD); } src_reg ',' shifter_operand
        { $$ = ($1 | $3 | $5 | $7 | $9 | (1 << 20)); }
    ;

load_inst:
      INST_LDR_LIKE { lexpect(AE_COND); } COND { lexpect(AE_BYTM); } load_body
        { $$ = ($1 | $3 | $5); }
    ;

load_body:
      BYTM { lexpect(AE_TRNS); } TRNS { lexpect(AE_OPRD); } dest_reg ','
        { lexpect(AE_OPRD); } load_am
        {   
            unsigned int n;
            n = ((1 << 26) | $1 | $3 | $5 | $8);
            if ($3 && ((n >> 24) & 0xf) == 0x5 && (n & 0x0fff) == 0) {
                /* if T is set and immediate operand is 0, then convert into
                 * a post-indexed instruction */
                n &= ~(1 << 24);    /* clear 24th bit */
                n |= (1 << 23);     /* set U bit */
            }
            $$ = n;
        }
    | BYTM_HALF { lexpect(AE_OPRD); } dest_reg ',' misc_ls_am
        { $$ = ((1 << 7) | $1 | $3 | $5); }
    ; 

load_mult_inst:
      INST_LDM { lexpect(AE_COND); } COND { lexpect(AE_LMAM); } LMAM
        { lexpect(AE_OPRD); } src_reg maybe_bang ',' reg_lists maybe_hat
        {   $$ = ((1 << 27) | $1 | $3 | $5 | $7 | $8 | $10 | $11); } 
    | INST_STM { lexpect(AE_COND); } COND { lexpect(AE_SMAM); } SMAM
        { lexpect(AE_OPRD); } src_reg maybe_bang ',' reg_lists maybe_hat
        {   $$ = ((1 << 27) | $1 | $3 | $5 | $7 | $8 | $10 | $11); } 
    ;

exception_inst:
      INST_BKPT { lexpect(AE_OPRD); } OPRD_IMM
        {   $$ = ($1 | (($3 & 0xfff0) << 8) | ($3 & 0x000f)); }
    | INST_SWI { lexpect(AE_COND); } COND { lexpect(AE_OPRD); } OPRD_IMM
        {   $$ = ($1 | $3 | $5);    }
    ;

multiply_inst:
      INST_MUL { lexpect(AE_COND); } COND { lexpect(AE_CCUP); } CCUP
        { lexpect(AE_OPRD); } OPRD_REG ',' OPRD_REG ',' OPRD_REG
        {   $$ = ($1 | $3 | $5 | ($7 << 16) | $9 | ($11 << 8)); }
    | INST_MLA { lexpect(AE_COND); } COND { lexpect(AE_CCUP); } CCUP
        { lexpect(AE_OPRD); } OPRD_REG ',' OPRD_REG ',' OPRD_REG ',' OPRD_REG
        {   $$ = ($1 | $3 | $5 | ($7 << 16) | $9 | ($11 << 8) | ($13 << 12)); }
    | INST_SMLAL_LIKE { lexpect(AE_COND); } COND { lexpect(AE_CCUP); } CCUP
        { lexpect(AE_OPRD); } OPRD_REG ',' OPRD_REG ',' OPRD_REG ',' OPRD_REG
        {
            $$ = ($1 | $3 | $5 | ($7 << 12) | ($9 << 16) | $11 |
                ($13 << 8));
        }
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
      '#' { lexpect(AE_OPRD); } shifter_imm     { $$ = $3; }
    | OPRD_REG ',' { lexpect(AE_LSLK); } shifter_operand_lsl_clause
        { $$ = ($1 | $4); }
    | OPRD_REG      { $$ = $1; }
    ;

shifter_operand_lsl_clause:
      OPRD_LSL_LIKE { lexpect(AE_OPRD); } shifter_operand_lsl_arg
            { $$ = ($1 | $3); }
    | OPRD_RRX
            { $$ = $1; }
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
    | OPRD_REG
        {
            $$ = ((1 << 4) | ($1 << 8));
        }
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
      '[' src_reg ',' { lexpect(AE_LAMS); } misc_ls_am_index ']' maybe_bang
        { $$ = ((1 << 24) | (1 << 7) | (1 << 4) | $2 | $5 | $7); }
    | '[' src_reg ']' ',' { lexpect(AE_LAMS); } misc_ls_am_index
        { $$ = ($2 | $6); }
    | '[' src_reg ']'
        { $$ = ((1 << 24) | (1 << 22) | (1 << 23) | $2); }
    ;

misc_ls_am_index:
      '#' { lexpect(AE_OPRD); } imm_with_u_bit
        {
            $$ = ((1 << 22) | ((($3 & 0xf0) >> 4) << 8) | ($3 & 0x0f) |
                ($3 & (1 << 23)));
        }
    | LAMS { lexpect(AE_OPRD); } OPRD_REG
        { $$ = ((1 << 7) | (1 << 4) | $1 | $3); }
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
    | '[' OPRD_REG ',' { lexpect(AE_LAMS); } load_am_indexed ']' maybe_bang
        {
            $$ = ((1 << 26) | (1 << 24) | ($2 << 16) | $5 | $7);
        }
    | '[' OPRD_REG ']' ',' { lexpect(AE_LAMS); } load_am_indexed
        {
            $$ = ((1 << 26) | ($2 << 16) | $6); 
        }
    | '[' OPRD_REG ']'
        {
            $$ = ((1 << 26) | (1 << 24) | (1 << 23) | ($2 << 16));
        }
    ;

load_am_indexed:
      '#' { lexpect(AE_OPRD); } OPRD_IMM
        {
            $$ = ($3 < 0 ? -$3 : ($3 | (1 << 23)));
        }
    | LAMS { lexpect(AE_OPRD); } OPRD_REG maybe_am_lsl_subclause
        {
            $$ = ($1 | $3 | $4 | (1 << 25));
        }
    ;

maybe_am_lsl_subclause:
      /* empty */                       {   $$ = 0;                 }
    | ',' { lexpect(AE_LSLK); } OPRD_LSL_LIKE { lexpect(AE_OPRD); } '#'
        OPRD_IMM
        {
            unsigned int n = $6;
            if (n == 32)
                n = 0;
            if (n >= (1 << 5))
                as_bad("immediate value (%d) too large", $6);
            $$ = ($3 | (n << 7));
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

