/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alepinto <alepinto@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/23 20:00:00 by alepinto          #+#    #+#             */
/*   Updated: 2025/05/23 20:00:00 by alepinto         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../Inc/signals.h"
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

// ========== SIGNAL HANDLERS ==========

void handle_sigint(int sig)
{
    (void)sig;  // Unused parameter
    
    // Print newline and redisplay prompt
    // Use write() because it's async-signal-safe
    write(STDOUT_FILENO, "\n", 1);
    
    // Readline functions to handle new line and prompt
    rl_on_new_line();           // Tell readline we're on a new line
    rl_replace_line("", 0);     // Clear current input line
    rl_redisplay();             // Redisplay the prompt
}

// ========== SIGNAL SETUP ==========

void setup_signals(void)
{
    // Handle Ctrl+C (SIGINT) - display new prompt
    signal(SIGINT, handle_sigint);
    
    // Ignore Ctrl+\ (SIGQUIT) - do nothing in interactive mode
    signal(SIGQUIT, SIG_IGN);
}

void setup_child_signals(void)
{
    // Restore default signal handling for child processes
    // This allows child processes to be interrupted normally
    signal(SIGINT, SIG_DFL);
    signal(SIGQUIT, SIG_DFL);
}
