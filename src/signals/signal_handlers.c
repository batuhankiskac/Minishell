/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handlers.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/14 12:27:18 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
	printf("DEBUG: sigint_handler: SIGINT received in parent\n");
	set_signal_flag(SIGINT);
	write(1, "\n", 1);
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
void	set_interactive_signals(void)
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

/**
 * @brief Signal handler for the heredoc child process.
 *
 * This function sets up the SIGINT handler specifically for the heredoc
 * child process. Crucially, it does NOT use the SA_RESTART flag. This ensures
 * that when SIGINT interrupts the `read` system call inside `readline`,
 * the call fails with EINTR instead of being automatically restarted.
 * This allows the readline loop to terminate upon Ctrl+C.
 */
void	init_heredoc_signals(void)
{
	struct sigaction	sa;

	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sa.sa_handler = heredoc_child_sigint_handler;
	sigaction(SIGINT, &sa, NULL);
	signal(SIGQUIT, SIG_IGN);
}

/**
 * @brief Signal handler for SIGINT within the heredoc child process.
 * @param sig The signal number (SIGINT).
 * * @details This handler is specifically for the child process forked to read
 * heredoc input. When Ctrl+C (SIGINT) is pressed, the child's only job is
 * to exit with status 130, which is the standard exit code for termination
 * by SIGINT. This allows the parent process to detect the interruption.
 */
void	heredoc_child_sigint_handler(int sig)
{
	(void)sig;
	printf("DEBUG: heredoc_child_sigint_handler: SIGINT received in child\n");
	set_signal_flag(SIGINT);
}
