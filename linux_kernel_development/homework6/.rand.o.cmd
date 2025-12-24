savedcmd_rand.o := ld -m elf_x86_64 -z noexecstack --no-warn-rwx-segments   -r -o rand.o @rand.mod  ; /usr/lib/modules/6.17.9-arch1-1/build/tools/objtool/objtool --hacks=jump_label --hacks=noinstr --hacks=skylake --ibt --orc --retpoline --rethunk --sls --static-call --uaccess --prefix=16  --link  --module rand.o

rand.o: $(wildcard /usr/lib/modules/6.17.9-arch1-1/build/tools/objtool/objtool)
