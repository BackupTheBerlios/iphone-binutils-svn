/* ----------------------------------------------------------------------------
 *   iphone-binutils: development tools for the Apple iPhone       07/12/2007
 *   Copyright (c) 2007 Patrick Walton <pcwalton@uchicago.edu> but freely
 *   redistributable under the terms of the GNU General Public License v2.
 *
 *   ld/arm_reloc.c: relocation for the ARM 
 * ------------------------------------------------------------------------- */

#include <stdarg.h>
#include "arm_reloc.h"
#include "ld.h"

void arm_reloc(char *contents, struct relocation_info *relocs, struct
    section_map *map)
{
    error_with_cur_obj("TODO");
}

