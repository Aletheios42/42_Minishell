#include "../Inc/minishell.h"
#include "../libft/libft.h"

t_tree* parse_bin_op(char **input) {
    t_tree *tree = NULL;
    char *pos = *input;
    
    tree = (t_tree *)malloc(sizeof(t_tree));
    if (!tree)
        return NULL;
    ft_memset(tree, 0, sizeof(t_tree));
    
    if (ft_strncmp(pos, "&&", 2) == 0) {
        tree->type = NODE_AND;
        tree->data.operator = ft_substr(pos, 0, 2);
        pos[0] = '\0';
        pos[1] = '\0';
        *input = pos + 2;
    }
    else if (ft_strncmp(pos, "||", 2) == 0) {
        tree->type = NODE_OR;
        tree->data.operator = ft_substr(pos, 0, 2);
        pos[0] = '\0';
        pos[1] = '\0';
        *input = pos + 2;
    }
    else if (*pos == '|') {
        tree->type = NODE_PIPE;
        tree->data.operator = ft_substr(pos, 0, 1);
        pos[0] = '\0';
        *input = pos + 1;
    }
    else {
        free(tree);
        return NULL;
    }
    
    return tree;
}

void add_redir(t_redir **head, t_redir_type type, char *filename) {
    t_redir *new_redir = (t_redir *)malloc(sizeof(t_redir));
    if (!new_redir)
        return;
    new_redir->type = type;
    new_redir->filename = filename;
    new_redir->next = NULL;
    
    if (!*head) {
        *head = new_redir;
    } else {
        t_redir *current = *head;
        while (current->next)
            current = current->next;
        current->next = new_redir;
    }
}

t_tree* parse_cmd(char *input) {
    t_tree *node;
    t_redir *redirs = NULL;
    char **cmd_args = NULL;
    int i = 0;
    
    node = (t_tree *)malloc(sizeof(t_tree));
    if (!node)
        return NULL;
    ft_memset(node, 0, sizeof(t_tree));
    node->type = NODE_COMMAND;
    
    while (input[i]) {
        while (input[i] == ' ')
            i++;
        
        if (!input[i])
            break;
            
        // Check redirections
        if (!ft_strncmp(&input[i], ">>", 2)) {
            i += 2;
            while (input[i] == ' ')
                i++;
            int start = i;
            while (input[i] && input[i] != ' ')
                i++;
            char *filename = ft_substr(input, start, i - start);
            add_redir(&redirs, APPEND, filename);
        }
        else if (!ft_strncmp(&input[i], "<<", 2)) {
            i += 2;
            while (input[i] == ' ')
                i++;
            int start = i;
            while (input[i] && input[i] != ' ')
                i++;
            char *delimiter = ft_substr(input, start, i - start);
            add_redir(&redirs, HEREDOC, delimiter);
        }
        else if (input[i] == '<') {
            i++;
            while (input[i] == ' ')
                i++;
            int start = i;
            while (input[i] && input[i] != ' ')
                i++;
            char *filename = ft_substr(input, start, i - start);
            add_redir(&redirs, REDIR_IN, filename);
        }
        else if (input[i] == '>') {
            i++;
            while (input[i] == ' ')
                i++;
            int start = i;
            while (input[i] && input[i] != ' ')
                i++;
            char *filename = ft_substr(input, start, i - start);
            add_redir(&redirs, REDIR_OUT, filename);
        }
        else {
            // Regular word
            int start = i;
            while (input[i] && input[i] != ' ' && input[i] != '<' && input[i] != '>')
                i++;
            char *word = ft_substr(input, start, i - start);
            cmd_args = ft_append_to_string_array(&cmd_args, word);
        }
    }
    
    node->data.command.args = cmd_args;
    node->data.command.redirs = redirs;
    
    return node;
}

t_tree* parse_pipe(char *input) {
    t_tree *tree = NULL;
    char *left_tok;
    char *delims[2] = {"|", NULL};
    
    left_tok = input;
    input = ft_strarraystr(input, delims);
    if (!input)
        return (parse_cmd(left_tok));
        
    tree = parse_bin_op(&input);
    tree->left = parse_pipe(left_tok);
    
    if (input)
        tree->right = parse_pipe(input);
    
    return tree;
}

t_tree* parse_and_or(char *input) {
    t_tree *tree = NULL;
    char *left_tok;
    char *delims[3] = {"&&", "||", NULL};
    
    left_tok = input;
    input = ft_strarraystr(input, delims);
    if (!input)
        return (parse_pipe(left_tok));
        
    tree = parse_bin_op(&input);
    tree->left = parse_and_or(left_tok);
    
    if (input)
        tree->right = parse_and_or(input);
    
    return tree;
}

t_tree* parser(char *input) {
    return parse_and_or(input);
}
