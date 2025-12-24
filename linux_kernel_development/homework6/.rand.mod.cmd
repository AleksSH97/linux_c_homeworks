savedcmd_rand.mod := printf '%s\n'   rand_main.o rand_or.o rand_read.o rand_write.o | awk '!x[$$0]++ { print("./"$$0) }' > rand.mod
