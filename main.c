/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 08:57:20 by mzolotar          #+#    #+#             */
/*   Updated: 2025/03/07 14:04:14 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// esta versión solo lee comandos de 1 linea, si hay lineas ver como se comporta
// shell
int main(int argc, char *argv[], char **envp) {
  if (argc != 1 || argv[1]) {
    printf("\033[1;31m🛑 ERROR: argumento incorrecto (gestionar este "
           "mensaje).\n\033[0m"); // gestionar mensaje de error
    return (1);
  } else {
    test_minishell_loop(envp);
    // minishell_loop(envp);
  }
  return (0);
}
