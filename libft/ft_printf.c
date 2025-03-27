/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 09:13:46 by mzolotar          #+#    #+#             */
/*   Updated: 2024/10/24 19:13:30 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Prints the value for the given format specifier.
 *
 * Checks the format specifier and calls the appropriate function to print
 * the argument (e.g., %c, %s, %p, %d, %x, %X).
 *
 * @param format Current format specifier.
 * @param args List of arguments.
 * @return Number of characters printed or -1 on error.
 */

static int	ft_check_format(char const *format, va_list args)
{
	int	printed_chars;

	printed_chars = 0;
	if (*format == 'c')
		printed_chars += ft_print_char(va_arg(args, int));
	else if (*format == 's')
		printed_chars += ft_print_str(va_arg(args, char *));
	else if (*format == 'p')
		printed_chars += ft_print_ptr(va_arg(args, void *));
	else if (*format == 'd' || *format == 'i')
		printed_chars += ft_print_int(va_arg(args, int));
	else if (*format == 'u')
		printed_chars += ft_printdec_unsig(va_arg(args, unsigned int));
	else if (*format == 'x')
		printed_chars += ft_print_hex(va_arg(args, unsigned int), 'a');
	else if (*format == 'X')
		printed_chars += ft_print_hex(va_arg(args, unsigned int), 'A');
	if (printed_chars == -1)
		return (-1);
	return (printed_chars);
}

/**
 * @brief Handles and prints a format specifier or a single character.
 *
 * Processes format specifiers (e.g., %c, %s) or regular characters,
 * advancing the format string accordingly.
 *
 * @param format Pointer to the format string.
 * @param args List of arguments.
 * @return Number of characters printed or -1 on error.
 */

static int	handle_format_or_char(const char **format, va_list args)
{
	int	printed_chars;

	printed_chars = 0;
	if (**format == '%')
	{
		(*format)++;
		if (**format == '%')
			printed_chars = write(1, "%", 1);
		else
			printed_chars = ft_check_format(*format, args);
	}
	else
		printed_chars = write(1, *format, 1);
	if (printed_chars != -1)
		(*format)++;
	return (printed_chars);
}

/**
 * @brief Custom printf implementation that handles format specifiers.
 *
 * Mimics printf by processing a format string and printing formatted
 * arguments (e.g., %d, %s, %x).
 *
 * @param format The format string.
 * @return Total characters printed or -1 on error.
 */

int	ft_printf(char const *format, ...)
{
	va_list	args;
	int		printed_chars;
	int		total_printed;

	total_printed = 0;
	va_start(args, format);
	while (*format)
	{
		printed_chars = handle_format_or_char(&format, args);
		if (printed_chars == -1)
		{
			va_end(args);
			return (-1);
		}
		total_printed += printed_chars;
	}
	va_end(args);
	return (total_printed);
}
