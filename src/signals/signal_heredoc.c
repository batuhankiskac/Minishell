#include "minishell.h"

static void sigint_handler_heredoc(int sig)
{
    (void)sig;
    set_signal_flag(SIGINT);
    write(1, "\n", 1);
}

void    setup_heredoc_signals(void)
{
    struct sigaction    sa;

    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sa.sa_handler = sigint_handler_heredoc;
    sigaction(SIGINT, &sa, NULL);
    signal(SIGQUIT, SIG_IGN);
}
