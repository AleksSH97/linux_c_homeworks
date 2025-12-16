savedcmd_radio.mod := printf '%s\n'   radio_main.o radio_or.o radio_write.o | awk '!x[$$0]++ { print("./"$$0) }' > radio.mod
