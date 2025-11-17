savedcmd_cur_time.mod := printf '%s\n'   cur_time.o | awk '!x[$$0]++ { print("./"$$0) }' > cur_time.mod
