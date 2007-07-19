%{
/* ----------------------------------------------------------------------------
 *   iphone-binutils: development tools for the Apple iPhone       07/18/2007
 *   Copyright (c) 2007 Patrick Walton <pcwalton@uchicago.edu> but freely
 *   redistributable under the terms of the GNU General Public License v2.
 *
 *   arml.l - the lexer for ARM assembly
 * ------------------------------------------------------------------------- */

#include <stdlib.h>
#include <string.h>

#include "arm.h"
#include "army.h"
#include "messages.h"
#include "read.h"
#include "symbols.h"

char *cur_ptr, *saved_input_line_ptr;

#define YY_NO_UNPUT
#define YY_INPUT(buf,result,max_size) { result = input_string(buf, max_size); }

static int input_string(char *buf, int max_size)
{
    int nread = 0;

    if (!cur_ptr)
        abort();

    while (*cur_ptr && max_size) {
        *(buf++) = *(cur_ptr++);
        max_size--; nread++;
    }

    return nread;
}
%}

%option noyywrap
%x cond ccup lmam oprd smam expr bytm

%%

<INITIAL>{
    "adc"   { yylval.nval = (0x5 << 21);            return INST_ADD_LIKE; }
    "add"   { yylval.nval = (0x4 << 21);            return INST_ADD_LIKE; }
    "and"   { yylval.nval = (0x0 << 21);            return INST_ADD_LIKE; }
    "b"     { yylval.nval = 0x0a000000;             return INST_BL;       }
    "bkpt"  { yylval.nval = 0xe1200070;             return INST_BKPT;     }
    "bic"   { yylval.nval = (0xe << 21);            return INST_ADD_LIKE; }
    "bl"    { yylval.nval = 0x0b000000;             return INST_BL;       }
    "ble"   { /* These two are unfortunate parsing botches, needed to
               * prevent "blt" from being parsed as "bl" with "t" as the
               * condition code */
              yylval.nval = 0x0a000000;  yyless(1); return INST_BL;       }
    "blt"   { yylval.nval = 0x0a000000;  yyless(1); return INST_BL;       }
    "bx"    { yylval.nval = 0x12FFF30;              return INST_BX;       }
    "cmn"   { yylval.nval = (0xb << 21);            return INST_CMP_LIKE; }
    "cmp"   { yylval.nval = (0xa << 21);            return INST_CMP_LIKE; }
    "eor"   { yylval.nval = (0x1 << 21);            return INST_ADD_LIKE; }
    "ldm"   { yylval.nval = (0x1 << 20);            return INST_LDM;      }
    "ldr"   { yylval.nval = 0x04100000;             return INST_LDR_LIKE; }
    "mov"   { yylval.nval = (0xd << 21);            return INST_MOV_LIKE; }
    "mvn"   { yylval.nval = (0xf << 21);            return INST_MOV_LIKE; }
    "orr"   { yylval.nval = (0xc << 21);            return INST_ADD_LIKE; }
    "rsb"   { yylval.nval = (0x3 << 21);            return INST_ADD_LIKE; }
    "rsc"   { yylval.nval = (0x7 << 21);            return INST_ADD_LIKE; }
    "sbc"   { yylval.nval = (0x6 << 21);            return INST_ADD_LIKE; }
    "stm"   { yylval.nval = 0;                      return INST_STM;      }
    "str"   { yylval.nval = 0x04000000;             return INST_LDR_LIKE; }
    "sub"   { yylval.nval = (0x2 << 21);            return INST_ADD_LIKE; }
    "teq"   { yylval.nval = (0x9 << 21);            return INST_CMP_LIKE; }
    "tst"   { yylval.nval = (0x8 << 21);            return INST_CMP_LIKE; }
}

<cond>{
    "eq"        { yylval.nval = 0x00000000; return COND; }
    "ne"        { yylval.nval = 0x10000000; return COND; }
    "cs"        { yylval.nval = 0x20000000; return COND; }
    "hs"        { yylval.nval = 0x20000000; return COND; }
    "cc"        { yylval.nval = 0x30000000; return COND; }
    "lo"        { yylval.nval = 0x30000000; return COND; }
    "mi"        { yylval.nval = 0x40000000; return COND; }
    "pl"        { yylval.nval = 0x50000000; return COND; }
    "vs"        { yylval.nval = 0x60000000; return COND; }
    "vc"        { yylval.nval = 0x70000000; return COND; }
    "hi"        { yylval.nval = 0x80000000; return COND; }
    "ls"        { yylval.nval = 0x90000000; return COND; }
    "ge"        { yylval.nval = 0xa0000000; return COND; }
    "lt"        { yylval.nval = 0xb0000000; return COND; }
    "gt"        { yylval.nval = 0xc0000000; return COND; }
    "le"        { yylval.nval = 0xd0000000; return COND; }
    "al"        { yylval.nval = 0xe0000000; return COND; }
    .           { yyless(0); yylval.nval = 0xe0000000; return COND; }
}

<ccup>{
    "s"         {             yylval.nval = (1 << 20);    return CCUP; }
    .           { yyless(0);  yylval.nval = 0;            return CCUP; }
}

<lmam>{
    "fd"        { yylval.nval = ((1 << 27) | (1 << 23));  return LMAM; }
}

<smam>{
    "fd"        { yylval.nval = ((1 << 27) | (1 << 24));  return SMAM; }
}

<bytm>{
    "b"         { yylval.nval = (1 << 22);                return BYTM; }
    .           { yyless(0); yylval.nval = 0;             return BYTM; }
}

<oprd>{
    "r"[0-9]+   {
                    yylval.nval = (unsigned int)strtol(yytext + 1,
                        (char **)NULL, 10);
                    return OPRD_REG;
                }
    "a"[0-9]+   {
                    yylval.nval = (unsigned int)strtol(yytext + 1,
                        (char **)NULL, 10) - 1;
                    return OPRD_REG;
                }
    "lsl"       { yylval.nval = 0;          return OPRD_LSL_LIKE; }
    "lsr"       { yylval.nval = (1 << 5);   return OPRD_LSL_LIKE; }
    "asr"       { yylval.nval = (1 << 6);   return OPRD_LSL_LIKE; }
    "ror"       { yylval.nval = (3 << 5);   return OPRD_LSL_LIKE; }
    "wr"        { yylval.nval = 7;  return OPRD_REG; }
    "sb"        { yylval.nval = 9;  return OPRD_REG; }
    "sl"        { yylval.nval = 10; return OPRD_REG; }
    "fp"        { yylval.nval = 11; return OPRD_REG; }
    "ip"        { yylval.nval = 12; return OPRD_REG; }
    "sp"        { yylval.nval = 13; return OPRD_REG; }
    "lr"        { yylval.nval = 14; return OPRD_REG; }
    "pc"        { yylval.nval = 15; return OPRD_REG; }
    [+-]?[0-9]+ {
                    yylval.ival = (int)strtol(yytext, (char **)NULL, 10);
                    return OPRD_IMM;
                }
    [A-Za-z0-9_]    {
                        BEGIN(expr);
                        yyless(0);
                    }
}

<expr>{
.+              {
                    saved_input_line_ptr = input_line_pointer;
                    input_line_pointer = yytext;
                    yylval.eval = calloc(sizeof(expressionS), 1);
                    expression(yylval.eval);
                    input_line_pointer = saved_input_line_ptr;
                    return OPRD_EXP;
                }
}

<*>"#"          { return '#'; }
<*>"["          { return '['; }
<*>"]"          { return ']'; }
<*>"{"          { return '{'; }
<*>"}"          { return '}'; } 
<*>","          { return ','; } 
<*>"!"          { return '!'; }
<*>"+"          { return '+'; }
<*>"-"          { return '-'; }

<*>[ \t]        /* eat whitespace */

<*>.            { as_bad("unexpected character '%s'", yytext); }

%%

void lexpect(int what)
{
    switch (what) {
        case AE_INIT:   BEGIN(0);       break;
        case AE_COND:   BEGIN(cond);    break;
        case AE_CCUP:   BEGIN(ccup);    break;
        case AE_LMAM:   BEGIN(lmam);    break;
        case AE_OPRD:   BEGIN(oprd);    break;
        case AE_SMAM:   BEGIN(smam);    break;
        case AE_BYTM:   BEGIN(bytm);    break;
        default:        abort();
    }
}

