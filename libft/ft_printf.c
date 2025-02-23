/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bkiskac <bkiskac@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/07 12:02:43 by bkiskac           #+#    #+#             */
/*   Updated: 2025/01/09 18:22:16 by bkiskac          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	ft_putstr(const char *s, unsigned int *count)
{
	if (!s)
	{
		*count += write(1, "(null)", 5);
		return ;
	}
	while (*s)
		*count += write(1, s++, 1);
}

void	ft_flags(const char *format_specifier, va_list ap, unsigned int *count)
{
	if (*format_specifier == 'c')
		*count += write(1, &(char){va_arg(ap, int)}, 1);
	else if (*format_specifier == 's')
		ft_putstr(va_arg(ap, const char *), count);
	else if (*format_specifier == 'p')
		ft_putptr(va_arg(ap, void *), count);
	else if (*format_specifier == 'd' || *format_specifier == 'i')
		ft_putnbr(va_arg(ap, int), count);
	else if (*format_specifier == 'u')
		ft_putunbr(va_arg(ap, unsigned int), count);
	else if (*format_specifier == 'x' || *format_specifier == 'X')
		ft_puthex(va_arg(ap, unsigned int), *format_specifier, count);
}

int	ft_printf(const char *format, ...)
{
	va_list			ap;
	unsigned int	i;
	unsigned int	count;

	va_start(ap, format);
	i = 0;
	count = 0;
	while (format[i])
	{
		if (format[i] == '%')
		{
			if (format[i + 1] == '%')
				count += write(1, "%", 1);
			else
				ft_flags(&format[i + 1], ap, &count);
			i += 2;
		}
		else
		{
			count += write(1, &format[i], 1);
			i++;
		}
	}
	va_end(ap);
	return (count);
}
