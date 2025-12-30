savedcmd_stop_kthread.mod := printf '%s\n'   stop_kthread.o | awk '!x[$$0]++ { print("./"$$0) }' > stop_kthread.mod
