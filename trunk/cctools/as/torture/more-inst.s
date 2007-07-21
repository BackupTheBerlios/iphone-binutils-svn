strh r1, [sp, #+118]
ldrh r1, [sp, #+118]

strh r0, [r1, #+25]
strh r0, [r1, #-25]
strh r0, [r1, +r2]
strh r0, [r1, -r2]
strh r0, [r1, r2]
strh r0, [r1, #25]!
strh r0, [r1, +r2]!
strh r0, [r1], #25
strh r0, [r1], #-25
strh r0, [r1], -ip

ldrh r0, [r1, #+25]
ldrh r0, [r1, #-25]
ldrh r0, [r1, +r2]
ldrh r0, [r1, -r2]
ldrh r0, [r1, r2]
ldrh r0, [r1, #25]!
ldrh r0, [r1, +r2]!
ldrh r0, [r1], #25
ldrh r0, [r1], #-25
ldrh r0, [r1], -ip

