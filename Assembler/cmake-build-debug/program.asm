mov $65535 %r0
mov $65535 %r1
nop
nop
nop
add %r1 %r0
nop
nop
nop
syscall led_write
nop
nop
nop
hlt
