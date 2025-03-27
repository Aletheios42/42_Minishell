/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_ptr.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/19 09:13:26 by mzolotar          #+#    #+#             */
/*   Updated: 2024/10/24 19:13:16 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Calculates the length of a hexadecimal representation.
 *
 * Computes the number of digits needed to represent an unsigned long
 * integer in hexadecimal format.
 *
 * @param num The unsigned long integer to be converted to hexadecimal.
 * @return The length of the hexadecimal representation.
 */

static int	ft_hex_len(unsigned long num)
{
	int	lenght;

	lenght = 0;
	if (num == 0)
		return (1);
	while (num > 0)
	{
		num /= 16;
		lenght++;
	}
	return (lenght);
}

/**
 * @brief Allocates memory for a temporary string to hold a hex number.
 *
 * Creates a string of the appropriate length to store the hexadecimal
 * representation of the given unsigned long integer.
 *
 * @param num The unsigned long integer to be converted.
 * @return A pointer to the allocated string, or NULL if allocation fails.
 */

static char	*ft_create_temp_str(unsigned long num)
{
	int		lenght;
	char	*aux_str;

	lenght = ft_hex_len(num);
	aux_str = (char *)ft_calloc(lenght + 1, sizeof(char));
	if (!aux_str)
		return (NULL);
	return (aux_str);
}

/**
 * @brief Fills a string with the hexadecimal representation of a number.
 *
 * Converts the unsigned long integer into a string representing the
 * number in hexadecimal, using lowercase or uppercase letters based on
 * the format ('a' for lowercase, 'A' for uppercase).
 *
 * @param num The unsigned long integer to convert to hexadecimal.
 * @param format Determines the case of the letters used ('a' or 'A').
 * @param aux_str Pointer to the allocated string to be filled.
 * @return The pointer to the filled string.
 */

static char	*ft_write_temp_str(unsigned long num, char format, char *aux_str)
{
	int	i;
	int	remainder;

	i = 0;
	remainder = 0;
	if (num == 0)
		aux_str[i++] = '0';
	while (num > 0)
	{
		remainder = num % 16;
		if (remainder < 10)
			aux_str[i] = '0' + remainder;
		else
		{
			if (format == 'a')
				aux_str[i] = 'a' + (remainder - 10);
			else
				aux_str[i] = 'A' + (remainder - 10);
		}
		i++;
		num /= 16;
	}
	return (aux_str);
}

/**
 * @brief Prints an unsigned long integer in hexadecimal format.
 *
 * This function prints the given unsigned long integer as a hexadecimal
 * number, using either lowercase or uppercase letters based on the
 * specified format.
 *
 * @param num The unsigned long integer to print in hexadecimal.
 * @param format Format specifier, either 'a' for lowercase or 'A'
 * for uppercase.
 * @return The number of characters printed or -1 on error.
 */

static int	ft_print_hex_ptr(unsigned long num, char format)
{
	char	*aux_str;
	int		count;
	int		i;

	i = 0;
	count = 0;
	aux_str = ft_create_temp_str(num);
	aux_str = ft_write_temp_str(num, format, aux_str);
	if (!aux_str)
		return (0);
	while (aux_str[i] != '\0')
		i++;
	while (i > 0)
	{
		ft_putchar_fd(aux_str[i - 1], 1);
		i--;
		count++;
	}
	free(aux_str);
	return (count);
}

/**
 * @brief Prints the memory address of a pointer in hexadecimal format.
 *
 * This function prints the memory address pointed to by the pointer,
 * using the '0x' prefix followed by the hexadecimal representation
 * of the address.
 *
 * @param pointer The pointer whose address will be printed.
 * @return The number of characters printed.
 */

int	ft_print_ptr(void *pointer)
{
	unsigned long	address;
	int				count;

	address = (unsigned long)pointer;
	count = 0;
	if (address == 0)
	{
		if (ft_putstr_fd_int("(nil)", 1) == -1)
			return (-1);
		return (5);
	}
	if (ft_putstr_fd_int("0x", 1) == -1)
		return (-1);
	count += 2;
	count += ft_print_hex_ptr(address, 'a');
	return (count);
}
