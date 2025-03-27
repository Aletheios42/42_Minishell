/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_syntax_error_token.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/05 11:22:55 by mzolotar          #+#    #+#             */
/*   Updated: 2025/03/06 12:26:33 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//true heredoc abierto
//false -otro

//🚩_revisar esta parte no se si se va a usar o no

bool check_open_heredoc_token(t_tokens *tokens)
{
    t_tokens *current;
    t_tokens *prew_token;
    t_tokens *prew_prew_token;

    current = tokens;
    while (current)
    {
        if (current->position > 1 && current->type == redir_l_d)
        {    
            prew_token = ft_find_node_n_position(tokens, current->position - 1);
            prew_prew_token = ft_find_node_n_position(tokens, current->position - 2);
            if (prew_prew_token->type==redir_l_d && prew_token->type == heredoc_delimiter && current->type == redir_l_d && !current->next)
            {
                
                return (true);
            }
        }
        current = current->next;
    }
    
    return false;
}

// Definiciones de macros y funciones auxiliares (suponemos que ya existen):
// BASH_ERR: mensaje para error tipo "bash: syntax error near unexpected token"
// SYNTAX_ERR: mensaje para error de sintaxis (como comillas abiertas, redirección incompleta, etc.)
// is_redir(token): devuelve true si el token es un operador de redirección (<, >, <<, >>)
// pipe_type: constante que indica que el token es un pipe

//true pipe abierto
//false -otro
bool check_open_pipe_token(t_tokens *tokens)
{
    t_tokens *current;
    t_tokens *prew_token;

    current = tokens;
    while (current)
    {
        if (current->position > 0 && current->type == pipe_type)
        {    
            prew_token = ft_find_node_n_position(tokens, current->position - 1);
            if (current->type == pipe_type && !current->next && (prew_token->type == command || prew_token->type == argument|| prew_token->type == heredoc_delimiter))
            {
                return (printf("⚠️ OPEN PIPE ⚠️\n"), true);
            }
        }
        current = current->next;
    }
    return false;
}

bool check_bash_error_token(t_tokens *tokens)
{
    t_tokens *next_token;
    t_tokens *current = tokens;
    
    // Error: Pipe al inicio (ejemplo: "| ls")
    if (current && current->type == pipe_type)
    {
        return (printf("1: %s: `%s'\n", BASH_ERR, current->content), true);
      
    }
    
    while (current)
    {
        next_token = current->next;
        
        // Error: Redirección sin argumento (ejemplo: ">", "<", ">>", "<<" sin nada después)
        if (is_redir(current) && (!next_token))
        {
            return(printf("2: %s: `newline'\n", BASH_ERR), true);
        }
        
        // Error: Pipe seguido incorrectamente (ejemplo: "| |", "| <", "| >")
        if (current->type == pipe_type && (!next_token || next_token->type == pipe_type || is_redir(next_token)))
        {
            return (printf("2: %s: `%s'\n", BASH_ERR, current->content), true);
             
        }
        
        // Error: Redirecciones encadenadas incorrectamente 
        // (ejemplo: tokens consecutivos de redirección, como "<< <<" o ">> >>" o ">> >")
        if (is_redir(current) && next_token && is_redir(next_token))
        {
            return(printf("4: %s: `%s'\n", BASH_ERR, next_token->content), true);
             
        }
        
        current = next_token;
    }
    return false;
}
