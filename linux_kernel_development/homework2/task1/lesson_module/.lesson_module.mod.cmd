savedcmd_lesson_module.mod := printf '%s\n'   lesson_module.o | awk '!x[$$0]++ { print("./"$$0) }' > lesson_module.mod
