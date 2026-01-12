savedcmd_tic_tac.mod := printf '%s\n'   tic_tac.o | awk '!x[$$0]++ { print("./"$$0) }' > tic_tac.mod
