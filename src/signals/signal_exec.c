/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_exec.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/06 11:52:07 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/06 11:54:14 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	sigint_handler_exec(int sig)
{
	(void)sig;
	write(1, "\n", 1);
}

static void	sigquit_handler_exec(int sig)
{
	(void)sig;
	ft_printf(2, "Quit (core dumped)\n");
}

void	setup_exec_signals(void)
{
	signal(SIGINT, sigint_handler_exec);
	signal(SIGQUIT, sigquit_handler_exec);
}
