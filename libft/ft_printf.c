/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/07 18:58:42 by bkiskac           #+#    #+#             */
/*   Updated: 2025/07/07 19:00:40 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	print_char(int fd, int c)
{
	return (write(fd, &c, 1));
}

static int	print_str(int fd, char *str)
{
	if (!str)
		return (write(fd, "(null)", 6));
	return (write(fd, str, ft_strlen(str)));
}

static int	print_nbr(int fd, long n)
{
	int		count;
	char	c;

	count = 0;
	if (n < 0)
	{
		count += write(fd, "-", 1);
		n = -n;
	}
	if (n >= 10)
		count += print_nbr(fd, n / 10);
	c = (n % 10) + '0';
	count += write(fd, &c, 1);
	return (count);
}

static int	parse_format(int fd, va_list args, const char format)
{
	if (format == 'c')
		return (print_char(fd, va_arg(args, int)));
	else if (format == 's')
		return (print_str(fd, va_arg(args, char *)));
	else if (format == 'd' || format == 'i')
		return (print_nbr(fd, va_arg(args, int)));
	else if (format == '%')
		return (write(fd, "%", 1));
	return (0);
}

int	ft_printf(int fd, const char *format, ...)
{
	va_list	args;
	int		i;
	int		total_len;

	va_start(args, format);
	i = 0;
	total_len = 0;
	while (format[i])
	{
		if (format[i] == '%')
		{
			total_len += parse_format(fd, args, format[i + 1]);
			i++;
		}
		else
			total_len += write(fd, &format[i], 1);
		i++;
	}
	va_end(args);
	return (total_len);
}
