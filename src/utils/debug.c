#include "minishell.h"
#include <stdio.h>
#include <termios.h>
#include <unistd.h>

void debug_print_termios_flags(const char *label)
{
    struct termios t;
    tcgetattr(STDIN_FILENO, &t);
    printf("DEBUG: [TERMIOS] %s -> c_lflag: ", label);
    if (t.c_lflag & ICANON) printf("ICANON ");
    if (t.c_lflag & ECHO) printf("ECHO ");
    if (t.c_lflag & ECHOE) printf("ECHOE ");
    if (t.c_lflag & ECHOK) printf("ECHOK ");
    if (t.c_lflag & ECHONL) printf("ECHONL ");
    if (t.c_lflag & ISIG) printf("ISIG ");
    printf("\n");
}
