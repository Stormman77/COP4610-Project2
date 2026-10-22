savedcmd_my_timer.mod := printf '%s\n'   my_timer.o | awk '!x[$$0]++ { print("./"$$0) }' > my_timer.mod
