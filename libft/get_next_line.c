/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/09 19:29:33 by mzolotar          #+#    #+#             */
/*   Updated: 2024/11/11 08:31:40 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

/**
 * @brief Processes the buffer to extract a line and update the remainder.
 * @param readed_buffer Buffer read from the file descriptor.
 * @param readed_bytes Number of bytes read from the file.
 * @param line Pointer to the current line being constructed.
 * @param remainder Pointer to the leftover data after extracting a line.
 * @return Pointer to the completed line, or NULL if line is incomplete.
 */

static char	*ft_process_readed_buffer(char *readed_buffer, ssize_t readed_bytes,
		char **line, char **remainder)
{
	ssize_t	i;
	char	*temp;

	i = 0;
	while (i < readed_bytes)
	{
		if (readed_buffer[i] == '\n')
		{
			temp = ft_strslice(readed_buffer, 0, i + 1);
			*line = ft_strappend(*line, temp);
			free(temp);
			*remainder = ft_strslice(readed_buffer, i + 1, readed_bytes - i
					- 1);
			return (*line);
		}
		i++;
	}
	temp = ft_strslice(readed_buffer, 0, readed_bytes);
	*line = ft_strappend(*line, temp);
	free(temp);
	return (NULL);
}

/**
 * @brief Checks for reading errors and frees memory on failure.
 * @param readed_bytes Number of bytes read, -1 if error occurred.
 * @param line Pointer to the current line being constructed.
 * @param remainder Pointer to the leftover data after reading.
 * @param readed_buffer Buffer containing the data read.
 * @return 1 if error occurred, otherwise 0.
 */

static int	ft_check_readed_bytes(ssize_t readed_bytes, char *line,
		char **remainder, char *readed_buffer)
{
	if (readed_bytes == -1)
	{
		free(readed_buffer);
		free(line);
		if (*remainder)
			free(remainder);
		*remainder = NULL;
		return (1);
	}
	return (0);
}

/**
 * @brief Reads from file and accumulates content into the line.
 * @param fd File descriptor to read from.
 * @param line Pointer to the line being accumulated.
 * @param remainder Pointer to leftover data from the previous read.
 * @return Pointer to the completed line, or NULL if no line is formed.
 */

static char	*ft_read_file_and_accum_line(int fd, char **line, char **remainder)
{
	char	*readed_buffer;
	ssize_t	readed_bytes;

	readed_buffer = (char *)malloc(BUFFER_SIZE + 1);
	if (!readed_buffer)
		return (NULL);
	readed_bytes = read(fd, readed_buffer, BUFFER_SIZE);
	if (ft_check_readed_bytes(readed_bytes, *line, remainder, readed_buffer))
		return (NULL);
	while (readed_bytes > 0)
	{
		readed_buffer[readed_bytes] = '\0';
		if (ft_process_readed_buffer(readed_buffer, readed_bytes, line,
				remainder))
		{
			free(readed_buffer);
			return (*line);
		}
		readed_bytes = read(fd, readed_buffer, BUFFER_SIZE);
	}
	free(readed_buffer);
	return (*line);
}

/**
 * @brief Frees remainder if it has no remaining content.
 * @param new_remainder Pointer to the remaining buffer after processing.
 * @return Pointer to the remainder if it still has content, or NULL.
 */

static char	*ft_check_remainder(char *new_remainder)
{
	if (!ft_strlen(new_remainder))
	{
		free(new_remainder);
		new_remainder = NULL;
	}
	return (new_remainder);
}

/**
 * @brief Retrieves the next line from a file descriptor.
 * @param fd File descriptor to read from.
 * @return Pointer to the next line read, or NULL if EOF or error.
 */

char	*get_next_line(int fd)
{
	static char	*remainder;
	char		*line;
	char		*new_remainder;

	if (fd < 0 || BUFFER_SIZE <= 0)
		return (NULL);
	line = NULL;
	if (remainder)
	{
		if (ft_strchr(remainder, '\n'))
		{
			line = ft_strslice(remainder, 0, ft_strchr(remainder, '\n')
					- remainder + 1);
			new_remainder = ft_strdup(ft_strchr(remainder, '\n') + 1);
			new_remainder = ft_check_remainder(new_remainder);
			free(remainder);
			remainder = new_remainder;
			return (line);
		}
		line = ft_strdup(remainder);
		free(remainder);
		remainder = NULL;
	}
	line = ft_read_file_and_accum_line(fd, &line, &remainder);
	return (line);
}
