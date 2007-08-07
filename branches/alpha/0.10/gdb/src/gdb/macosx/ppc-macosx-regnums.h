#ifndef __GDB_PPC_MACOSX_REGNUMS_H__
#define __GDB_PPC_MACOSX_REGNUMS_H__

#define PPC_MACOSX_SP_REGNUM 1  /* Contains address of top of stack */

#define PPC_MACOSX_PC_REGNUM 96 /* Program counter (instruction address %iar) */
#define PPC_MACOSX_PS_REGNUM 97 /* Processor (or machine) status (%msr) */
#define PPC_MACOSX_CR_REGNUM 98 /* Condition register */
#define PPC_MACOSX_LR_REGNUM 99 /* Link register */
#define PPC_MACOSX_CTR_REGNUM 100       /* Count register */
#define PPC_MACOSX_XER_REGNUM 101       /* Fixed point exception registers */
#define PPC_MACOSX_MQ_REGNUM 102        /* Multiply/quotient register */
#define PPC_MACOSX_FPSCR_REGNUM 103     /* Floating-point status register */
#define PPC_MACOSX_VSCR_REGNUM 104      /* AltiVec status register */
#define PPC_MACOSX_VRSAVE_REGNUM 105    /* AltiVec save register */

#define PPC_MACOSX_FIRST_GP_REGNUM 0
#define PPC_MACOSX_LAST_GP_REGNUM 31
#define PPC_MACOSX_NUM_GP_REGS (PPC_MACOSX_LAST_GP_REGNUM - PPC_MACOSX_FIRST_GP_REGNUM + 1)
#define PPC_MACOSX_SIZE_GP_REGS (PPC_MACOSX_NUM_GP_REGS * 4)
#define PPC_MACOSX_SIZE_GP_REGS_64 (PPC_MACOSX_NUM_GP_REGS * 8)

#define PPC_MACOSX_FIRST_FP_REGNUM 32
#define PPC_MACOSX_LAST_FP_REGNUM 63
#define PPC_MACOSX_NUM_FP_REGS (PPC_MACOSX_LAST_FP_REGNUM - PPC_MACOSX_FIRST_FP_REGNUM + 1)
#define PPC_MACOSX_SIZE_FP_REGS (PPC_MACOSX_NUM_FP_REGS * 8)

#define PPC_MACOSX_FIRST_VP_REGNUM 64
#define PPC_MACOSX_LAST_VP_REGNUM 95
#define PPC_MACOSX_NUM_VP_REGS (PPC_MACOSX_LAST_VP_REGNUM - PPC_MACOSX_FIRST_VP_REGNUM + 1)
#define PPC_MACOSX_SIZE_VP_REGS (PPC_MACOSX_NUM_VP_REGS * 16)

#define PPC_MACOSX_FIRST_GSP_REGNUM 96
#define PPC_MACOSX_LAST_GSP_REGNUM 102
#define PPC_MACOSX_NUM_GSP_REGS (PPC_MACOSX_LAST_GSP_REGNUM - PPC_MACOSX_FIRST_GSP_REGNUM + 1)

#define PPC_MACOSX_FIRST_FSP_REGNUM 103
#define PPC_MACOSX_LAST_FSP_REGNUM 103
#define PPC_MACOSX_NUM_FSP_REGS (PPC_MACOSX_LAST_FSP_REGNUM - PPC_MACOSX_FIRST_FSP_REGNUM + 1)

#define PPC_MACOSX_FIRST_VSP_REGNUM 104
#define PPC_MACOSX_LAST_VSP_REGNUM 105
#define PPC_MACOSX_NUM_VSP_REGS (PPC_MACOSX_LAST_VSP_REGNUM - PPC_MACOSX_FIRST_VSP_REGNUM + 1)

#define PPC_MACOSX_FIRST_SP_REGNUM PPC_MACOSX_FIRST_GSP_REGNUM
#define PPC_MACOSX_LAST_SP_REGNUM PPC_MACOSX_LAST_VSP_REGNUM
#define PPC_MACOSX_NUM_SP_REGS (PPC_MACOSX_LAST_SP_REGNUM - PPC_MACOSX_FIRST_SP_REGNUM + 1)
#define PPC_MACOSX_SIZE_SP_REGS (PPC_MACOSX_NUM_SP_REGS * 4)
#define PPC_MACOSX_SIZE_SP_REGS_64 ((PPC_MACOSX_NUM_SP_REGS * 4) + (5 * 4))

#define PPC_MACOSX_NUM_REGS (PPC_MACOSX_NUM_GP_REGS + PPC_MACOSX_NUM_FP_REGS + PPC_MACOSX_NUM_VP_REGS + PPC_MACOSX_NUM_SP_REGS)

#define PPC_MACOSX_REGISTER_BYTES (PPC_MACOSX_SIZE_GP_REGS + PPC_MACOSX_SIZE_FP_REGS + PPC_MACOSX_SIZE_VP_REGS + PPC_MACOSX_SIZE_SP_REGS)
#define PPC_MACOSX_REGISTER_BYTES_64 (PPC_MACOSX_SIZE_GP_REGS_64 + PPC_MACOSX_SIZE_FP_REGS + PPC_MACOSX_SIZE_VP_REGS + PPC_MACOSX_SIZE_SP_REGS_64)

#define PPC_MACOSX_IS_GP_REGNUM(regno) ((regno >= PPC_MACOSX_FIRST_GP_REGNUM) && (regno <= PPC_MACOSX_LAST_GP_REGNUM))
#define PPC_MACOSX_IS_FP_REGNUM(regno) ((regno >= PPC_MACOSX_FIRST_FP_REGNUM) && (regno <= PPC_MACOSX_LAST_FP_REGNUM))
#define PPC_MACOSX_IS_VP_REGNUM(regno) ((regno >= PPC_MACOSX_FIRST_VP_REGNUM) && (regno <= PPC_MACOSX_LAST_VP_REGNUM))

#define PPC_MACOSX_IS_GSP_REGNUM(regno) (((regno >= PPC_MACOSX_FIRST_GSP_REGNUM) && (regno <= PPC_MACOSX_LAST_GSP_REGNUM)) || (regno == PPC_MACOSX_VRSAVE_REGNUM))
#define PPC_MACOSX_IS_FSP_REGNUM(regno) ((regno >= PPC_MACOSX_FIRST_FSP_REGNUM) && (regno <= PPC_MACOSX_LAST_FSP_REGNUM))
#define PPC_MACOSX_IS_VSP_REGNUM(regno) (((regno >= PPC_MACOSX_FIRST_VSP_REGNUM) && (regno <= PPC_MACOSX_LAST_VSP_REGNUM)) && (regno != PPC_MACOSX_VRSAVE_REGNUM))

#define PPC_MACOSX_DEFAULT_LR_SAVE 8

#define PPC_MACOSX_REGISTER_TYPE unsigned int
#define PPC_MACOSX_FP_REGISTER_TYPE double
#define PPC_MACOSX_VP_REGISTER_TYPE float

#endif /* __GDB_PPC_MACOSX_REGNUMS_H__ */
