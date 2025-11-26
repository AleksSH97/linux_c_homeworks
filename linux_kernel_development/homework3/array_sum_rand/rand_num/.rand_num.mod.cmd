savedcmd_rand_num.mod := printf '%s\n'   rand_num.o | awk '!x[$$0]++ { print("./"$$0) }' > rand_num.mod
