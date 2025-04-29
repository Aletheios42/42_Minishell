#include "../Inc/minishell.h"

void executor(t_tree* data_tree, char **env) {
    if (data_tree->right)
        executor(data_tree->right, env);
    else
        data_tree->sentence = env[0];
}
