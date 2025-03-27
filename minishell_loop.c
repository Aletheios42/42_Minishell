/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell_loop.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mzolotar <mzolotar@student.42madrid.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/12 13:17:12 by mzolotar          #+#    #+#             */
/*   Updated: 2025/03/07 10:27:23 by mzolotar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

/**
* @brief 
*
* @param 
* @return 
*/

void minishell_loop(char **envp)
{
	(void)envp;
	char **token;
	t_tokens *tokens;
	t_metachars *meta;
	char *line;
	
	tokens = NULL;	
	token = NULL;
	
	//line = readline("Minishell> ");
	line = get_line();

	if (!line)
	{
		printf("\033[0;34m ⚠️Error:dramas en pillar linea\033[0m\n");
		return;
	}
	
	while (line) 
	{
		
		//line = readline("Minishell> ");
			//🔹 ✒ meter datos correctos en  ✒ readline ("user@hostname:~/42_School_temp_rep/minishell_borrador")
				//user: whoami o echo $USER
				//hostname: hostname o echo $HOSTNAME
				//path: pwd
					//  ~/ ==  /home/user
		//Guardar en el historial
			//if (ft_strncmp("", line, -1) != 0)
			//	add_history(line);
		
//FASE 1_TOKENIZACION: -----------------------------------------------------------------------------------------------------
//V2:
	//ft_tokeniser
	// 🔹  ✒ limpiar mini de datos de Raul
	
	// ⚠️ no hay comillas en absoluto
		// ft_split
	// ⚠️ hay comillas cerradas ' o ", o las dos
		// comillas cerradas -> ft_split_with_quotes() 
	// 🔹  ✒ Eliminación de espacios en blanco iniciales y finales. Preparamos 'line' para seguir. El shell ignora espacios, tabs y nuevas líneas al principio y al final del comando. Esto no afecta a la tokenización directamente, pero sí prepara el comando para que sea procesado correctamente. -> revisar f ()-> char *get_line(void);
	// 🔹  ✒ <ENTER> → No hay tokens, así que no hay comando. Bash simplemente muestra un nuevo prompt sin ejecutar nada. 
	// 🔹  ✒ check_empty_line ->  <espacio><espacio><ENTER> → si linea esta vacia o solo espacio o solo enter -> espera comando nuevo y no ejecuta nada
		// 🔹  ✒ ver esto con historial, espacios si van al historial
	// 🔹  ✒  ""  y '' como -> Command '' not found
		// ❌ falla en ejecución: "" y '' → se tokeniza y debe dar ❌ BASH_ERROR: Command '' not found.
	// 🔹 ✒ '/' + '\t' + '\n'
		// 🔹 ✒ '/' anula caracteres -> ver subject
	// 🔹 ✒  << DELIMITER 		→ 🔹 ✒ gestionar minishell -> here_doc
	// 🔹 ✒  ./minishel ./minishell -> ver este caso
		// ❌ falla en ejecución: todos los ejecutables, con ruta absoluta y relativa

			
	// 🔸 X_ERROR_FASE_1: V2:
		// ⚠️ comillas abiertas
		// ⚠️ pipe abierto: check_open_pipe_token(tokens)
		// 🔸  ✒  bash error -> check_bash_error_token(tokens) junto con heredoc. 
			//🔸  ✒ sin here doc
				//pipes
				//redir
			// 🔸  ✒ con here doc
				//pipes
				//redir
				//heredoc
					//bash_error + sintax_error
					//bash_error + new_line
		// ⚠️ PTE_TESTEO_SYNTAX_ERROR_Y_EJECUCION_TODA_FASE1
	
//FASE 1_TOKENIZACION_END: -----------------------------------------------------------------------------------------------------
		
		meta = init_meta_struct(); 
		if (!meta)
		{
			free (line);
			printf("\033[0;34m ⚠️Error: No se pudo inicializar la estructura\033[0m\n");  //quitar esta linea
			break;
		}

		if (is_quotes_present(line) && quotes_is_open(line) == true)
		{
			printf("⚠️ OPEN QUOTES ⚠️\n");
			free (line);
			free_meta_struct(meta);
			break;
		}
		
		check_spaces_line (&line, meta);

		// splitear line -------------------------------------------------------------------
		if (is_quotes_present(line))
			token = ft_split_with_quotes(line, meta);
		else 
			token = ft_split(line, ' ');
		if (!token)
		{
			free (line);
			free_meta_struct(meta);
			printf ("(!token) en splits"); // revisar y borrar printf
			break;
		}
		//fin de splitear line-------------------------------------------------------------------

		init_list (&tokens, token);
		check_type_tokens(tokens);

		if (check_extract_and_expand_var_token(tokens, meta) == true)
		{
			free_mini_loop(line, token, &tokens, meta);
			break;										
		}
		
		if (check_open_pipe_token(tokens)== true)
		{
			free_mini_loop(line, token, &tokens, meta);
			break;										
		}
		
		if (check_bash_error_token(tokens)== true)
		{
			free_mini_loop(line, token, &tokens, meta);
			break;										
		}
		
		check_and_quit_quotes_token(tokens, meta);
		
		//print_all_test();-------------------------------------------------------------------------------
		//ver output en: test/test_output/test_outputs.txt
		int fd_test;
		fd_test = open_fd("test/test_output/test_outputs.txt", O_RDWR | O_CREAT | O_TRUNC, 0777);
		print_all_test(line, token, tokens, fd_test);
		close(fd_test);
		//fin test----------------------------------------------------------------------------------------
		
		
//check_sintax_error_2-------------------------------------------------------
		//FASE_2_V2:
		// 🔸 ✒ meter codigo de SYNTAX_ERROR en $? -> ver pasos_tokenizacion.txt

		// 🔸 ✒ revisar variables con colillas y sin comillas y ' + " -> "'hola" , etc. 
			// ✒ SYNTAX_ERROR varible con caraacteres no permitidos:
				// 🔸 ✒ hacer idendificador de variables aqui: check_dolar_char -> check_is_valid_var->extract_var/expancion_var cuidado con $|(not_alow_oper)
				// 🔸 ✒ Expansión de la variable con el prefijo ${}: echo "Hola ${nombre}"
				// 🔸 ✒ Expansión de comandos con (): $(date)
				// 🔸 ✒ Expansión aritmética: echo $((5 + 3))
				// 🔸 ✒ Expansión de arreglos: array=("uno" "dos" "tres") -> echo ${array[1]}
				// 🔸 ✒ Expansión de comodines (Globbing: * → (*.txt), ? → (file?.txt), [] → (file[123].txt), {} → (file{1,2,3}.txt)) == wildcards)
				// 🔸 ✒ Funciones de shell: mi_funcion() { echo "¡Hola, mundo!"; }
			// 🔸 ✒ expancion de variables sin comillas y con "" o ''
				// 🔸 ✒ fucnion para identificar y extraer variable
				// 🔸 ✒ expancion de varieable
					//revisar si token tiene o no tiene espacios para retoknizar
						//revisar cuando se quitan las comillas
						//con espacios - 
						//sin espacios - 
					// 🔸 ✒ Expansión de variables (Variable Expansion)
					// 🔸 ✒ Expansión de parámetros (Parameter Expansion)
					// 🔸 ✒ Expansión de comandos:
						// 🔸 ✒ Expansión de comandos:
						// 🔸 ✒ Expansión de comandos con redirecciones
							// 🔸 ✒ Expansión de comandos con redirecciones permitidas
							// 🔸 ✒ Expansión de comandos con redirecciones no permitidas:ver casos-> (❌ SINTAX_ERROR)
					// 🔸 ✒ Expansión de la tilde (~)
					// 🔸 ✒ (❌ SINTAX_ERROR) ver arriba
				// 🔸 ✒ expancion de variable en HEREDOC:
					// ver normas de expancion dentor de HEREDOC
						//LIMITER entre comillas ' y " las variebles no se expanden
						//LIMITER sin comillas las variebles se expandiran
						
		//FASE_3_V1
			// 🔸  ✒ renderizar reenicio de line y los demas_reinicios en minishell_loop().
				// 🔸  ✒  quitar break y hacer las fucniones de reinicio de line y lo demás. 
				// 🔸  ✒  recisar fucniones que se usan o no: todo 
				// 🚩_TESTEO_NORMA
				// 🚩_TESTEO_TOKENIZACION
				// 🚩_TESTEO_EJECUCION
				// 🚩_TESTEO_SEÑALES
				// 🚩_TESTEO_HISTORIAL
				// 🚩_TESTEO_MEMORY_LEAKS 
				// 🚩_DESCRIPCIONES_FUCNIONES
		

//fin check_sintax_error_2--------------------------------------------------------------

		if (token && token[0] && ft_strncmp(token[0], "exit", -1) == 0)
		{
			free_mini_loop(line, token, &tokens, meta);
			break;  // Salir del loop si el token es "exit"
		}

		//FREE ALL:------------------------------------------------------------------------
		free_mini_loop(line, token, &tokens, meta);
		line = get_line();
		//line = readline("Minishell> ");
	}
}

