1) cat /etc/shells
/etc/shells: valid login shells
/bin/sh
/bin/bash
/usr/bin/bash
/bin/rbash
/usr/bin/rbash
/usr/bin/sh
/bin/dash
/usr/bin/dash

2) env
SHELL=/bin/bash
SESSION_MANAGER=local/mostafa-IdeaPad-Gaming-3-15IMH05:@/tmp/.ICE-unix/1443,unix/mostafa-IdeaPad-Gaming-3-15IMH05:/tmp/.ICE-unix/1443
QT_ACCESSIBILITY=1
COLORTERM=truecolor
XDG_CONFIG_DIRS=/etc/xdg/xdg-ubuntu:/etc/xdg
SSH_AGENT_LAUNCHER=gnome-keyring
XDG_MENU_PREFIX=gnome-
.
.

3) echo $SHELL
/bin/bash


4) Used as a line continuation character it tells the shell that the command continues on the next line.

5) alias PrintPath='echo $PATH'
