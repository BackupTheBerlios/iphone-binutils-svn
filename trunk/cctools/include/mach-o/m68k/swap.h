/*
 * Copyright (c) 1999 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
#import <architecture/byte_order.h>
#import <mach/m68k/thread_status.h>

extern void swap_m68k_thread_state_regs(
    struct m68k_thread_state_regs *cpu,
    enum NXByteOrder target_byte_order);

extern void swap_m68k_thread_state_68882(
    struct m68k_thread_state_68882 *fpu,
    enum NXByteOrder target_byte_order);

extern void swap_m68k_thread_state_user_reg(
    struct m68k_thread_state_user_reg *user_reg,
    enum NXByteOrder target_byte_order);
