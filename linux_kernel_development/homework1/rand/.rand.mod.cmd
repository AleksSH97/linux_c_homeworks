savedcmd_rand.mod := printf '%s\n'   rand.o | awk '!x[$$0]++ { print("./"$$0) }' > rand.mod
