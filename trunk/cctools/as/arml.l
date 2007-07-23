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
%x cond ccup lmam oprd smam expr bytm psru lams trns lslk space

W   ([^a-zA-Z0-9_])
SPACE [ \t]

%%

<INITIAL>{
    "adc"   { yylval.nval = (0x5 << 21);            return INST_ADD_LIKE;   }
    "add"   { yylval.nval = (0x4 << 21);            return INST_ADD_LIKE;   }
    "and"   { yylval.nval = (0x0 << 21);            return INST_ADD_LIKE;   }
    "b"     { yylval.nval = 0x0a000000;             return INST_BL;         }
    "b"/le  { yylval.nval = 0x0a000000;             return INST_BL;         }
    "b"/lo  { yylval.nval = 0x0a000000;             return INST_BL;         }
    "b"/ls  { yylval.nval = 0x0a000000;             return INST_BL;         }
    "b"/lt  { yylval.nval = 0x0a000000;             return INST_BL;         }
    "bkpt"  { yylval.nval = 0xe1200070;             return INST_BKPT;       }
    "bic"   { yylval.nval = (0xe << 21);            return INST_ADD_LIKE;   }
    "bl"    { yylval.nval = 0x0b000000;             return INST_BL;         }
    "bx"    { yylval.nval = 0x12FFF10;              return INST_BX;         }
    "cmn"   { yylval.nval = (0xb << 21);            return INST_CMP_LIKE;   }
    "cmp"   { yylval.nval = (0xa << 21);            return INST_CMP_LIKE;   }
    "eor"   { yylval.nval = (0x1 << 21);            return INST_ADD_LIKE;   }
    "ldm"   { yylval.nval = (0x1 << 20);            return INST_LDM;        }
    "ldr"   { yylval.nval = (0x1 << 20);            return INST_LDR_LIKE;   }
    "mla"   { yylval.nval = (0x9 << 4)|(0x1 << 21); return INST_MLA;        }
    "mov"   { yylval.nval = (0xd << 21);            return INST_MOV_LIKE;   }
    "mul"   { yylval.nval = (0x9 << 4);             return INST_MUL;        }
    "mvn"   { yylval.nval = (0xf << 21);            return INST_MOV_LIKE;   }
    "orr"   { yylval.nval = (0xc << 21);            return INST_ADD_LIKE;   }
    "rsb"   { yylval.nval = (0x3 << 21);            return INST_ADD_LIKE;   }
    "rsc"   { yylval.nval = (0x7 << 21);            return INST_ADD_LIKE;   }
    "sbc"   { yylval.nval = (0x6 << 21);            return INST_ADD_LIKE;   }
    "smlal" { yylval.nval = (0x7 << 21)|(0x9 << 4); return INST_SMLAL_LIKE; }
    "smull" { yylval.nval = (0x6 << 21)|(0x9 << 4); return INST_SMLAL_LIKE; }
    "stm"   { yylval.nval = 0;                      return INST_STM;        }
    "str"   { yylval.nval = 0;                      return INST_LDR_LIKE;   }
    "sub"   { yylval.nval = (0x2 << 21);            return INST_ADD_LIKE;   }
    "swi"   { yylval.nval = (0xf << 24);            return INST_SWI;        }
    "teq"   { yylval.nval = (0x9 << 21);            return INST_CMP_LIKE;   }
    "tst"   { yylval.nval = (0x8 << 21);            return INST_CMP_LIKE;   }
    "umlal" { yylval.nval = (0x5 << 21)|(0x9 << 4); return INST_SMLAL_LIKE; }
    "umull" { yylval.nval = (0x4 << 21)|(0x9 << 4); return INST_SMLAL_LIKE; }
}

<cond>{
    "eq"        { yylval.nval = 0x00000000; return COND; }
    "ne"        { yylval.nval = 0x10000000; return COND; }
    "cs"        { yylval.nval = 0x20000000; return COND; }
    "hs"        { yylval.nval = 0x20000000; return COND; }
    "cc"        { yylval.nval = 0x30000000; return COND; }
    "ul"        { yylval.nval = 0x30000000; return COND; }
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
    "fa"        { yylval.nval = (1 << 27);                  return LMAM; }
    "fd"        { yylval.nval = ((1 << 27) | (1 << 23));    return LMAM; }
    "ea"        { yylval.nval = ((1 << 27) | (1 << 24));    return LMAM; }
    "ed"        { yylval.nval = ((1<<27)|(1<<24)|(1<<23));  return LMAM; }

    "ia"        { yylval.nval = ((1 << 27) | (1 << 23));    return LMAM; }
    "ib"        { yylval.nval = ((1<<27)|(1<<24)|(1<<23));  return LMAM; }
    "da"        { yylval.nval = (1 << 27);                  return LMAM; }
    "db"        { yylval.nval = ((1 << 27) | (1 << 24));    return LMAM; }
}

<smam>{
    "fd"        { yylval.nval = ((1 << 27) | (1 << 24));    return SMAM; }
    "ed"        { yylval.nval = (1 << 27);                  return SMAM; }
    "ea"        { yylval.nval = ((1 << 27) | (1 << 23));    return SMAM; }
    "fa"        { yylval.nval = ((1<<27)|(1<<24)|(1<<23));  return SMAM; }

    "ia"        { yylval.nval = ((1 << 27) | (1 << 23));    return SMAM; }
    "ib"        { yylval.nval = ((1<<27)|(1<<24)|(1<<23));  return SMAM; }
    "da"        { yylval.nval = (1 << 27);                  return SMAM; }
    "db"        { yylval.nval = ((1 << 27) | (1 << 24));    return SMAM; }
}

<bytm>{
    "b"         { yylval.nval = (1 << 22);                return BYTM;      }
    "h"         { yylval.nval = 0xb0;                     return BYTM_HALF; }
    "sh"        { yylval.nval = 0xf0;                     return BYTM_HALF; }
    "sb"        { yylval.nval = 0xd0;                     return BYTM_HALF; }
    .           { yyless(0); yylval.nval = 0;             return BYTM;      }
}

<psru>{
    "p"         { yylval.nval = (0xf << 12);              return PSRU; }
    .           { yyless(0); yylval.nval = 0;             return PSRU; }
}

<lams>{
    "+"         { yylval.nval = (1 << 23);                return LAMS; }
    "-"         { yylval.nval = 0;                        return LAMS; }
    "#"         {                                         return '#';  }
    .           { yyless(0); yylval.nval = (1 << 23);     return LAMS; }
}

<trns>{
    "t"         { yylval.nval = ((1 << 26) | (1 << 21));  return TRNS; }
    .           { yyless(0); yylval.nval = 0;             return TRNS; }
}

<oprd>{
    "r"[0-9]+   {
                    yylval.nval = (unsigned int)strtol(yytext + 1,
                        (char **)NULL, 0);
                    return OPRD_REG;
                }
    "a"[0-9]+   {
                    yylval.nval = (unsigned int)strtol(yytext + 1,
                        (char **)NULL, 0) - 1;
                    return OPRD_REG;
                }
    "v"[0-9]+   {
                    yylval.nval = (unsigned int)strtol(yytext + 1,
                        (char **)NULL, 0) + 3;
                    return OPRD_REG;
                }
    "wr"        { yylval.nval = 7;  return OPRD_REG; }
    "sb"        { yylval.nval = 9;  return OPRD_REG; }
    "sl"        { yylval.nval = 10; return OPRD_REG; }
    "fp"        { yylval.nval = 11; return OPRD_REG; }
    "ip"        { yylval.nval = 12; return OPRD_REG; }
    "sp"        { yylval.nval = 13; return OPRD_REG; }
    "lr"        { yylval.nval = 14; return OPRD_REG; }
    "pc"        { yylval.nval = 15; return OPRD_REG; }
    [+-]?[0-9]+ {
                    yylval.ival = (int)strtol(yytext, (char **)NULL, 0);
                    return OPRD_IMM;
                }
    "0x"[0-9a-fA-F]+ {
                    yylval.ival = (int)strtol(yytext, (char **)NULL, 0);
                    return OPRD_IMM;
                }
    [A-Za-z0-9_\$]+ { yyless(0); BEGIN(expr); }
}

<lslk>{
    [al]"sl"    { yylval.nval = 0;          return OPRD_LSL_LIKE; }
    "lsr"       { yylval.nval = (1 << 5);   return OPRD_LSL_LIKE; }
    "asr"       { yylval.nval = (1 << 6);   return OPRD_LSL_LIKE; }
    "ror"       { yylval.nval = (3 << 5);   return OPRD_LSL_LIKE; }
    "rrx"       { yylval.nval = (3 << 5);   return OPRD_RRX;      }
}

<expr>{
    [^,]+       {
                    saved_input_line_ptr = input_line_pointer;
                    input_line_pointer = yytext;
                    yylval.eval = calloc(sizeof(expressionS), 1);
                    expression(yylval.eval);
                    input_line_pointer = saved_input_line_ptr;
                    return OPRD_EXP;
                }
    .           {   yyless(0); BEGIN(oprd);     }
}

<space>{
    {SPACE}+    { return SP; }
    .           { as_bad("unexpected character '%s', expecting white space", yytext); }
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
<*>"|"          { return '|'; }
<*>"^"          { return '^'; }

<*>{SPACE}      /* eat whitespace */

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
        case AE_PSRU:   BEGIN(psru);    break;
        case AE_LAMS:   BEGIN(lams);    break;
        case AE_TRNS:   BEGIN(trns);    break;
        case AE_LSLK:   BEGIN(lslk);    break;
        case AE_SP:     BEGIN(space);   break;
        default:        abort();
    }
}

