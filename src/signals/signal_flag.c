/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_flag.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/15 13:05:47 by bkiskac           #+#    #+#             */
/*   Updated: 2025/05/30 15:28:11 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static volatile sig_atomic_t	g_signal_flag = 0;

/**
 * @brief Sets the global signal flag to the given signal value.
 *
 * This function updates the global signal flag `g_signal_flag` to the
 * specified signal value. It is typically called by signal handlers to
 * indicate that a specific signal has been received.
 *
 * @param sig The signal value to set the global signal flag to.
 */
void	set_signal_flag(int sig)
{
	g_signal_flag = sig;
}

/**
 * @brief Retrieves the current value of the global signal flag.
 *
 * This function returns the current value of the global signal flag
 * `g_signal_flag`, which indicates the last signal received.
 *
 * @return The current value of the global signal flag.
 */
int	get_signal_flag(void)
{
	return (g_signal_flag);
}

/**
 * @brief Resets the global signal flag to 0.
 *
 * This function sets the global signal flag `g_signal_flag` to 0,
 * effectively clearing any previously received signal indication.
 */
void	reset_signal_flag(void)
{
	g_signal_flag = 0;
}
