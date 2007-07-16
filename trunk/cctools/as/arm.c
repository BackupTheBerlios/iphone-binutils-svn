/* ----------------------------------------------------------------------------
 *   ARM1176/iPhone support for Apple GAS.
 *   Distributed under the terms of the GNU General Public License ver. 2.
 * ------------------------------------------------------------------------- */

#include <stdio.h>
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
    return 0;
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
    char *this_frag;

    this_fix.needed = 0;

    fprintf(stderr, "assembling: %s\n", str);

    cur_ptr = s;
    lexpect(AE_INIT);
    yyparse();

    this_frag = frag_more(4);
    md_number_to_chars(thus_frag, instruction, 10);
}

/* ----------------------------------------------------------------------------
 *   Relocation 
 * ------------------------------------------------------------------------- */

void md_number_to_imm(unsigned char *buf, signed_expr_t val, int size, fixS *
    fixP, int nsect)
{
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

        default:
printf("%d\n", fixP->fx_r_type);
            as_fatal("md_number_to_imm: reloc unimplemented");
    }
}

