savedcmd_array_sum_rand.mod := printf '%s\n'   array_sum_rand.o | awk '!x[$$0]++ { print("./"$$0) }' > array_sum_rand.mod
