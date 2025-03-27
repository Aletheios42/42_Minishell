/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_print_hex.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/20 12:34:40 by mzolotar          #+#    #+#             */
/*   Updated: 2024/10/24 19:13:06 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Calculates the length of the hexadecimal representation of a number.
 *
 * This function computes the number of digits needed to represent an
 * unsigned integer in hexadecimal format.
 *
 * @param num The unsigned integer to be converted to hexadecimal.
 * @return The length of the hexadecimal representation.
 */

static int	ft_hex_len(unsigned int num)
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
 * representation of the given unsigned integer.
 *
 * @param num The unsigned integer to be converted.
 * @return A pointer to the allocated string, or NULL if allocation fails.
 */

static char	*ft_create_temp_str(unsigned int num)
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
 * Converts the unsigned integer into a string representing the number
 * in hexadecimal, using lowercase or uppercase letters depending on the
 * format ('a' for lowercase, 'A' for uppercase).
 *
 * @param num The unsigned integer to convert to hexadecimal.
 * @param format Determines the case of the letters used ('a' or 'A').
 * @param aux_str Pointer to the allocated string to be filled.
 * @return The pointer to the filled string.
 */

static char	*ft_write_temp_str(unsigned int num, char format, char *aux_str)
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
 * @brief Prints an unsigned integer in hexadecimal format.
 *
 * This function prints the given unsigned integer as a hexadecimal
 * number, using either lowercase or uppercase letters based on the
 * specified format.
 *
 * @param num The unsigned integer to print in hexadecimal.
 * @param format Format specifier, either 'a' for lowercase or 'A'
 * for uppercase.
 * @return The number of characters printed.
 */

int	ft_print_hex(unsigned int num, char format)
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
