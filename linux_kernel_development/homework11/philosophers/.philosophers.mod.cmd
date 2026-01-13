savedcmd_philosophers.mod := printf '%s\n'   philosophers.o | awk '!x[$$0]++ { print("./"$$0) }' > philosophers.mod
