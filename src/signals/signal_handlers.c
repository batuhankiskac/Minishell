/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/06/30 21:44:31 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#define _POSIX_C_SOURCE 200809L
#include "minishell.h"

/**
 * @brief Handles the SIGINT signal.
 *
 * This function is called when the SIGINT signal (Ctrl+C) is received.
 * It sets the global signal flag to SIGINT, writes a newline to the
 * standard output, and resets the readline state to prepare for new input.
 *
 * @param sig The signal number (unused in this handler).
 */
static void	sigint_handler(int sig)
{
	(void)sig;
	set_signal_flag(SIGINT);
	write(STDOUT_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

/**
 * @brief Initializes signal handlers for the shell.
 *
 * This function sets up the signal handlers for SIGINT and SIGQUIT.
 * SIGINT is handled by `sigint_handler`, while SIGQUIT is ignored.
 */
void	init_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	sa.sa_handler = sigint_handler;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

/**
 * @brief Resets signal handlers to their default behavior.
 *
 * This function restores the default behavior for SIGINT and SIGQUIT
 * signals, effectively undoing any custom signal handling.
 */
void	reset_signals(void)
{
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
}
