#ifndef MACH_O_ARM_H
#define MACH_O_ARM_H 

enum reloc_type_arm
{
    ARM_RELOC_VANILLA,          /* generic relocation */
    ARM_RELOC_PAIR,             /* second entry of a pair */
    ARM_RELOC_SECTDIFF,
    ARM_RELOC_LOCAL_SECTDIFF,
    ARM_RELOC_UNKNOWN0,
    ARM_RELOC_PCREL_IMM24,      /* signed branch offset */
    ARM_RELOC_UNKNOWN1,
    ARM_RELOC_PCREL_DATA_IMM12  /* Load and Store Word/Immediate Offset, r15 */
};

#endif

