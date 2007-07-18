/* ----------------------------------------------------------------------------
 *   ARM1176/iPhone support for Apple GAS.
 *   Copyright (c) 2007 Patrick Walton <pcwalton@uchicago.edu> and
 *   contributors but freely redistributable under the terms of the GNU
 *   General Public License v2.
 * ------------------------------------------------------------------------- */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <mach/machine.h>
#include <stuff/bytesex.h>

#include "arm.h"
#include "frags.h"
#include "fixes.h"
#include "messages.h"
#include "read.h"

/* ----------------------------------------------------------------------------
 *   Uninteresting machine-dependent boilerplate code 
 * ------------------------------------------------------------------------- */

const cpu_type_t md_cputype = 12;
const cpu_type_t md_cpusubtype = 6;
const enum byte_sex md_target_byte_sex = LITTLE_ENDIAN_BYTE_SEX;

const char md_comment_chars[] = "@";
const char md_line_comment_chars[] = "#";
const char md_EXP_CHARS[] = "eE";
const char md_FLT_CHARS[] = "dDfF";

const pseudo_typeS md_pseudo_table[] = {
    { "code", s_ignore, 0 },        /* we don't support Thumb */
    { NULL, 0, 0 }
};

const relax_typeS md_relax_table[0];

int md_parse_option(char **argP, int *cntP, char ***vecP)
{
    return 0;
}

void md_begin()
{
}

void md_end()
{
}

char *md_atof(int type, char *litP, int *sizeP)
{
    return "md_atof: TODO";
}

int md_estimate_size_before_relax(fragS *fragP, int segment_type)
{
    as_fatal("relaxation shouldn't occur on the ARM");
    return 0;
}

void md_convert_frag(fragS *fragP)
{
    as_fatal("relaxation shouldn't occur on the ARM");
}

/* Simply writes out a number in little endian form. */
void md_number_to_chars(char *buf, signed_expr_t val, int n)
{
    switch (n) {
        case 4:
            buf[0] = val;
            buf[1] = (val >> 8);
            buf[2] = (val >> 16);
            buf[3] = (val >> 24);
            break;

        case 2:
            buf[0] = val;
            buf[1] = (val >> 8);
            break;

        case 1:
            buf[0] = val;
            break; 

        default:
            abort();
    }
}

/* ----------------------------------------------------------------------------
 *   Tokenizing and parsing 
 * ------------------------------------------------------------------------- */

struct fix_info this_fix;

void register_reloc_type(int type, int size, int pcrel)
{
    this_fix.type = type;
    this_fix.size = size;
    this_fix.pcrel = pcrel;

    this_fix.needed = 1;
}

void register_add_symbol(symbolS *symbol, int offset)
{
    this_fix.add_symbol = symbol;
    this_fix.offset = offset;

    this_fix.needed = 1;
}

void register_sub_symbol(symbolS *symbol, int offset)
{
    this_fix.sub_symbol = symbol;
    this_fix.offset = offset;

    this_fix.needed = 1;
}

int yyerror(char *err)
{
    as_bad("%s", err);
    return 0;
}

void md_assemble(char *str)
{
    unsigned int encoded;
    char *this_frag;

    memset(&this_fix, '\0', sizeof(struct fix_info));

    fprintf(stderr, "assembling: %s\n", str);
    // yydebug = 1;

    cur_ptr = str;
    yyrestart(NULL);
    lexpect(AE_INIT);
    encoded = yyparse();

    this_frag = frag_more(4);
    md_number_to_chars(this_frag, instruction, 4);

    if (this_fix.needed)
        fix_new(frag_now, this_frag - frag_now->fr_literal, 4,
            this_fix.add_symbol, this_fix.sub_symbol, this_fix.offset,
            this_fix.pcrel, this_fix.pcrel, this_fix.type);
}

/* ----------------------------------------------------------------------------
 *   Relocation 
 * ------------------------------------------------------------------------- */

/* Assumes 32-bit n. */
void fill_reloc_value(unsigned char *buf, unsigned int n, unsigned int mask)
{
    mask = ~mask;

    buf[0] = (buf[0] & mask);
    buf[1] = (buf[1] & (mask >> 8));
    buf[2] = (buf[2] & (mask >> 16));
    buf[3] = (buf[3] & (mask >> 24));

    buf[0] = (buf[0] | n);
    buf[1] = (buf[1] | (n >> 8)); 
    buf[2] = (buf[2] | (n >> 16)); 
    buf[3] = (buf[3] | (n >> 24)); 
} 

void md_number_to_imm(unsigned char *buf, signed_expr_t val, int size, fixS *
    fixP, int nsect)
{
    unsigned int n = 0;

    switch (fixP->fx_r_type) {
        case ARM_RELOC_VANILLA:
        case NO_RELOC:
            switch (size) {
                case 4:
                    *(buf++) = (val >> 24);
                    *(buf++) = (val >> 16);
                    /* FALL THROUGH */
                case 2:
                    *(buf++) = (val >> 8);
                    /* FALL THROUGH */
                case 1:
                    *(buf++) = val;
            }
            break;

        case ARM_RELOC_PCREL_DATA_IMM12:
            if (val < 0)
                val = -val;
            else
                n = (1 << 23);  /* set U bit */
            assert(val < (1 << 12) && val > 0);
            n |= val;
            fill_reloc_value(buf, n, (1 << 23) | ((1 << 12) - 1));
            break;

        case ARM_RELOC_PCREL_IMM24:
            val -= 0x4;
            val >>= 2;
            n = ((unsigned int)val) & 0x00ffffff;
            fill_reloc_value(buf, n, 0x00ffffff);
            break;

        default:
            fprintf(stderr, "reloc type %d\n", fixP->fx_r_type);
            as_fatal("md_number_to_imm: reloc unimplemented");
    }
}

