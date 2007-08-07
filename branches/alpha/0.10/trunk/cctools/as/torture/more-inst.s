blx wibble
wibble:    blxne r1

sub sp, sp, #1, 22

strh r1, [sp, #+118]
ldrh r1, [sp, #+118]

strh r0, [r1, #25]
strh r0, [r1, #-25]
strh r0, [r1, +r2]
strh r0, [r1, -r2]
strh r0, [r1, r2]
strh r0, [r1, #25]!
strh r0, [r1, +r2]!
strh r0, [r1], #25
strh r0, [r1], #-25
strh r0, [r1], -ip
strh r0, [r1]

ldrh r0, [r1, #+25]
ldrh r0, [r1, #-25]
ldrh r0, [r1, +r2]
ldrsb r0, [r1, -r2]
ldrsb r0, [r1, r2]
ldrsb r0, [r1, #25]!
ldrsh r0, [r1, +r2]!
ldrsh r0, [r1], #25
ldrsh r0, [r1], #-25
ldrh r0, [r1], -ip
ldrsb r0, [sp]

