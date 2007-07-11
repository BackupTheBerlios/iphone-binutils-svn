/* ----------------------------------------------------------------------------
 *   ARM1176/iPhone support for Apple GAS.
 *   Distributed under the terms of the GNU General Public License ver. 2.
 * ------------------------------------------------------------------------- */

#include <mach/machine.h>
#include <stuff/bytesex.h>

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

const char md_comment_chars[] = "";
const char md_line_comment_chars[] = "#";
const char md_EXP_CHARS[] = "eE";
const char md_FLT_CHARS[] = "dDfF";

const pseudo_typeS md_pseudo_table[] = { {NULL, 0, 0} };
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

/* ----------------------------------------------------------------------------
 *   Tokenizing and parsing 
 * ------------------------------------------------------------------------- */

/* ----------------------------------------------------------------------------
 *   Relocation 
 * ------------------------------------------------------------------------- */

void md_number_to_imm(unsigned char *buf, signed_expr_t val, int size, fixS *
    fixP, int nsect)
{
    switch (fixP->fx_r_type) {
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
            as_fatal("md_number_to_imm: reloc unimplemented");
    }
}

