savedcmd_file.mod := printf '%s\n'   file.o | awk '!x[$$0]++ { print("./"$$0) }' > file.mod
