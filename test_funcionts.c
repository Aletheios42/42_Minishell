/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test_funcionts.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/03 08:27:02 by mzolotar          #+#    #+#             */
/*   Updated: 2025/03/07 14:05:26 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void test_minishell_loop(char **envp) {
  //(void)envp;
  char **token;
  t_tokens *tokens;
  t_metachars *meta;
  char *line;

  tokens = NULL;

  // line = readline("Minishell> ");
  line = get_line();
  while (line) {
    // Guardar en el historial
    // if (ft_strncmp("", line, -1) != 0)
    //	add_history(line);

    meta = init_meta_struct(); // Inicializa la estructura t_metachars
    if (!meta) {
      free(line);
      printf("\033[0;34m ⚠️Error: No se pudo inicializar la "
             "estructura\033[0m\n"); // quitar esta linea
      break;
    }

    if (is_quotes_present(line) && quotes_is_open(line) == true) {
      printf("⚠️ OPEN QUOTES ⚠️\n");
      free(line);
      free_meta_struct(meta);
      break;
    }

    check_spaces_line(&line, meta);

    // splitear
    // line-------------------------------------------------------------------
    if (is_quotes_present(line))
      token = ft_split_with_quotes(line, meta);
    else
      token = ft_split(line, ' ');
    if (!token) {
      free(line);
      free_meta_struct(meta);
      printf("(!token) en splits"); // revisar y borrar printf
      break;
    }
    // fin de la funcion: splitear
    // line-------------------------------------------------------------------

    init_list(&tokens, token);
    check_type_tokens(tokens);

    // check_and_expand_var_token

    if (check_open_pipe_token(tokens) == true) {
      free_mini_loop(line, token, &tokens, meta);
      break;
    }

    if (check_bash_error_token(tokens) == true) {
      free_mini_loop(line, token, &tokens, meta);
      break;
    }

    check_and_quit_quotes_token(tokens, meta);

    // print_all_test();-------------------------------------------------------------------------------
    // ver output en: test/test_output/test_outputs.txt
    int fd_test;
    fd_test = open_fd("test/test_output/test_outputs.txt",
                      O_RDWR | O_CREAT | O_TRUNC, 0777);
    print_all_test(line, token, tokens, fd_test);
    close(fd_test);
    // fin
    // test----------------------------------------------------------------------------------------

    //-----------------------------🚩 PARTE EJECUCION 🚩
    //---------------------------------------------------

    // AQUI VA HEREDOC...YA PIYARE YO LA FUCNION Y LA LLEVARE A JECUCION PARA
    // LAS COMPROBACIONES DE BASH_ERROR
    void executor(tokens);
    // AQUI VA EJECUCION

    //-----------------------------🚩 PARTE EJECUCION_END 🚩
    //---------------------------------------------------

    if (ft_strncmp(line, "exit", -1) == 0) {
      free_mini_loop(line, token, &tokens, meta);
      break;
    }

    // FREE
    // ALL:------------------------------------------------------------------------
    free_mini_loop(line, token, &tokens, meta);
    // line = readline("Minishell> ");
    line = get_line();
  }
}
