savedcmd_keyboard_irq.mod := printf '%s\n'   keyboard_irq_main.o keyboard_irq_or.o keyboard_irq_read.o | awk '!x[$$0]++ { print("./"$$0) }' > keyboard_irq.mod
