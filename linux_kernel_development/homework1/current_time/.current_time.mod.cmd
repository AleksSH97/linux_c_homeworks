savedcmd_current_time.mod := printf '%s\n'   current_time.o | awk '!x[$$0]++ { print("./"$$0) }' > current_time.mod
