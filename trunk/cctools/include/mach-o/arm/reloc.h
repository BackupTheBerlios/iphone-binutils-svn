#ifndef MACH_O_ARM_H
#define MACH_O_ARM_H 

enum reloc_type_arm
{
    ARM_RELOC_VANILLA,      /* generic relocation */
    ARM_RELOC_PCREL_IMM12   /* Load and Store Word/Immediate Offset, r15 */
};

#endif

