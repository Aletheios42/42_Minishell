#ifndef SIGNALS_H
#define SIGNALS_H

#include <signal.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

// Signal handler functions
void handle_sigint(int sig);

// Signal setup functions
void setup_signals(void);
void setup_child_signals(void);

#endif
