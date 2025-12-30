savedcmd_dynamic.mod := printf '%s\n'   dynamic.o | awk '!x[$$0]++ { print("./"$$0) }' > dynamic.mod
