/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/12 21:54:22 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief A simple signal handler for SIGINT (Ctrl+C).
 *
 * This handler's only purpose is to exist. When it catches a signal,
 * it interrupts any blocking system call (like read()) that is currently
 * running. The interrupted system call will then return -1 and set errno
 * to EINTR. This mechanism allows the main program flow to detect that
 * Ctrl+C was pressed without using global variables. The function body
 * is intentionally left empty.
 */
static void	interrupt_handler(int sig)
{
	(void)sig;
}

/**
 * @brief Sets up the main signal handlers for the shell.
 *
 * This function configures the behavior for SIGINT (Ctrl+C) and SIGQUIT
 * (Ctrl+\). For SIGINT, it uses a simple handler that interrupts system
 * calls. Crucially, the SA_RESTART flag is NOT set, which ensures that
 * system calls are not automatically restarted after being interrupted
 * by a signal. SIGQUIT is ignored.
 */
void	set_interactive_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = interrupt_handler;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

/**
 * @brief Resets signal handlers to their default behavior.
 *
 * This function is called before the shell exits to restore the default
 * actions for SIGINT and SIGQUIT, ensuring normal terminal behavior after
 * the program terminates.
 */
void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
