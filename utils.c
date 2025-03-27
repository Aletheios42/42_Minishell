/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/08 15:00:15 by mzolotar          #+#    #+#             */
/*   Updated: 2025/03/07 07:30:20 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
* @brief 
*
* @param 
* @return 
*/

int	open_fd(char *pathern, int flags, mode_t mode)
{
	int	fd;

	fd = open(pathern, flags, mode);
	if (fd == -1)
	{
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
	return (fd);
}


/**
* @brief 
*
* @param 
* @return 
*/

char *get_line(void)
{
	size_t len;
	char *line;
	char *temp_line;
	
	temp_line = get_next_line(0);
	//temp_line = readline("minishell_msg: ");
	if (!temp_line) 
	{
		printf("Error: get_line returned NULL\n");  //quitar comment
		return (NULL);
	}
	len = ft_strlen(temp_line);
	if (len > 0 && temp_line[len - 1] == '\n') 
	{
		len--;  // Ignorar el salto de línea
	}
	line = (char *)malloc(len + 1);
	if (!line) 
	{
		free(temp_line);
		printf("Error: Memory allocation failed\n");  //quitar comment
		return (NULL);
	}
	ft_strlcpy(line, temp_line, len + 1); 
	free (temp_line);
	return (line);
}

/**
* @brief 
*
* @param 
* @return 
*/

t_tokens	*ft_find_node_n_position(t_tokens *head, int position)
{
	while (head != NULL)
	{
		if (head->position == position)
			return (head);
		head = head->next;
	}
	return (NULL);
}
/**
* @brief 
*
* @param 
* @return 
*/


t_metachars *init_meta_struct(void)
{
    t_metachars *meta;
    
    meta = (t_metachars *)malloc(sizeof(t_metachars));
    if (!meta)
    {
        printf("\033[0;34m ⚠️Error: Ndramas con malloc en init_meta\033[0m\n");
        return (NULL);
    }

    //meta->allow_oper = ft_split(" << >> ", ' ');
    //meta->allow_metachar = ft_split(" < > | ", ' ');
	meta->allow_op = ft_split(" << | >> < > ", ' ');
    if (!meta->allow_op)
    {
        free(meta);
        return (NULL);
    }

	meta->var = ft_split(" $ ", ' ');
    if (!meta->var)
    {
        free(meta);
        return (NULL);
    }
    
    //meta->not_allow_metachar = ft_split(" \n ; : \t & # ! ) ( ", ' ');
    //meta->not_allow_bracket = ft_split(" [ { ] } ", ' ');
    //meta->is_metacharacter = ft_split(" \t \n | & ; ( ) < > ", ' ');
    
    
    //meta->not_allow_num_redir = ft_split("<<- << <> >> < >", ' ');
	//meta->not_allow_other_redir= ft_split(" <<<,<<<,<<-, <>,<>, <<-, 2>, 2>>", ',');
    //meta->not_allow_oper = ft_split(" || ;; >| $|", ' ');
    meta->unexpected_token = ft_split(" `newline' `|' `>' `<' `>>' `<<' ", ' ');
    if (!meta->unexpected_token)
    {
        free(meta);
        return (NULL);
    }
    
    //meta->not_allow_word = ft_split(" alias for while if elif do done case then in esac until coproc else select fi function", ' ');
    //meta->word = NULL;
    //meta->varvariable =NULL;

    meta->i_current_index = 0;
    meta->j_new_index = 0;

    meta->in_single_quote = false;
    meta->in_double_quote = false;

    return (meta);
}








 




 
