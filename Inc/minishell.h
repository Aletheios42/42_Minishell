/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alepinto <alepinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/06/02 20:21:45 by alepinto          #+#    #+#             */
/*   Updated: 2025/06/02 20:21:45 by alepinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"

# include "structs.h"

// Local includes
# include "lexer.h"
# include "parser.h"
# include "executor.h"
# include "builtins.h"
# include "signals.h"

# ifndef BUFF_SIZE
#  define BUFF_SIZE 1024
# endif

#endif
