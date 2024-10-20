## Assembler

This is a small example program to demonstrate the basic syntax of my custom assembly language:

<pre><code>
mov $65535 %r0      ; registers are prefixed with '%'
mov $65535 %r1      ; immediate values are prefixed with '$'
add %r1 %r0
syscall led_write   ; different syscalls are hardcoded into my assembler
nop
hlt
</code></pre>
This assembler is still not complete. I plan to flesh it out more soon, and possibly even write my own compiler in the future.

