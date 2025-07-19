/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 11:52:07 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/19 17:30:28 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
 * @brief Signal handler for SIGINT during command execution.
 *
 * This function handles the SIGINT signal (Ctrl+C) during command execution.
 * It simply prints a newline to create a clean break in the output.
 *
 * @param sig The signal number (unused in this handler).
 */
static void	sigint_handler_exec(int sig)
{
	(void)sig;
	write(1, "\n", 1);
}

/**
 * @brief Signal handler for SIGQUIT during command execution.
 *
 * This function handles the SIGQUIT signal (Ctrl+\) during command execution.
 * It prints a "Quit (core dumped)" message to stderr, mimicking the behavior
 * of bash when a foreground process is terminated by SIGQUIT.
 *
 * @param sig The signal number (unused in this handler).
 */
static void	sigquit_handler_exec(int sig)
{
	(void)sig;
	ft_printf(2, "Quit (core dumped)\n");
}

/**
 * @brief Sets up signal handlers for command execution.
 *
 * This function configures the signal handlers for SIGINT and SIGQUIT during
 * command execution. SIGINT is handled by printing a newline, and SIGQUIT
 * is handled by printing a "Quit (core dumped)" message. This behavior
 * matches bash's handling of signals during command execution.
 */
void	set_exec_signals(void)
{
	signal(SIGINT, sigint_handler_exec);
	signal(SIGQUIT, sigquit_handler_exec);
}
