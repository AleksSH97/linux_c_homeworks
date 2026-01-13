savedcmd_keyboard_irq.o := ld -m elf_x86_64 -z noexecstack --no-warn-rwx-segments   -r -o keyboard_irq.o @keyboard_irq.mod  ; /usr/lib/modules/6.18.2-arch2-1/build/tools/objtool/objtool --hacks=jump_label --hacks=noinstr --hacks=skylake --ibt --orc --retpoline --rethunk --sls --static-call --uaccess --prefix=16  --link  --module keyboard_irq.o

keyboard_irq.o: $(wildcard /usr/lib/modules/6.18.2-arch2-1/build/tools/objtool/objtool)
