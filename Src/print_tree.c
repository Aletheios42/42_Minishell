#include "../Inc/minishell.h"
#include "../libft/libft.h"

// Print a token's type as a string
const char *get_token_type_string(t_token_type type)
{
    static const char *token_names[] = {
        "REDIR_IN", "REDIR_OUT", "APPEND", "HEREDOC", 
        "AND", "OR", "PIPE", "PAREN", 
        "WORD", "LITERAL_WORD", "EOF"
    };
    
    if (type >= 0 && type <= TOKEN_EOF)
        return token_names[type];
    return "UNKNOWN";
}

// Helper function to print a token list with types
int ft_print_token_list(t_token *head)
{
    t_token *current = head;
    int count = 0;
    
    count += ft_printf("[");
    while (current) {
        const char *type_str = get_token_type_string(current->type);
        count += ft_printf("{\"%s\":%s}", current->value, type_str);
        if (current->next)
            count += ft_printf(", ");
        current = current->next;
    }
    count += ft_printf("]");
    
    return count;
}

// Print detailed token information
void print_token_list(t_token *head)
{
    t_token *current = head;
    int i = 0;
    
    if (!head) {
        ft_printf("(empty token list)\n");
        return;
    }
    
    ft_printf("Token List:\n");
    while (current) {
        ft_printf("[%d] Type: %-15s Value: \"%s\"\n", 
               i++, get_token_type_string(current->type), current->value);
        current = current->next;
    }
}

// Helper function to indent based on depth
int ft_print_tree_indent(int depth)
{
    int count = 0;
    int i = 0;
    
    while (i < depth)
    {
        count += ft_printf("  ");
        i++;
    }
    return (count);
}

// Helper function to print string arrays nicely
int ft_print_string_array(char **array)
{
    int count = 0;
    
    if (!array)
        return ft_printf("(null)");
    
    count += ft_printf("[");
    for (int i = 0; array[i]; i++)
    {
        count += ft_printf("\"%s\"", array[i]);
        if (array[i + 1])
            count += ft_printf(", ");
    }
    count += ft_printf("]");
    
    return count;
}

// Print the AST for debugging
int ft_print_syntax_tree(t_tree *tree, int depth)
{
    int count = 0;
    
    if (!tree)
        return ft_printf("(null)");
    
    count += ft_print_tree_indent(depth);
    
    // Print node type
    switch (tree->type) {
        case NODE_COMMAND:
            count += ft_printf("COMMAND: ");
            if (tree->tokens) {
                // Print tokens in this command node
                count += ft_printf("\n");
                count += ft_print_tree_indent(depth + 1);
                count += ft_printf("Tokens: ");
                count += ft_print_token_list(tree->tokens);
            } else {
                count += ft_printf("(empty)");
            }
            break;
        case NODE_PIPE:
            count += ft_printf("PIPE");
            if (tree->tokens)
                count += ft_printf(" (%s)", tree->tokens->value);
            break;
        case NODE_AND:
            count += ft_printf("AND");
            if (tree->tokens)
                count += ft_printf(" (%s)", tree->tokens->value);
            break;
        case NODE_OR:
            count += ft_printf("OR");
            if (tree->tokens)
                count += ft_printf(" (%s)", tree->tokens->value);
            break;
        case NODE_PAREN:
            count += ft_printf("PAREN");
            if (tree->tokens) {
                count += ft_printf("\n");
                count += ft_print_tree_indent(depth + 1);
                count += ft_printf("Content: ");
                count += ft_print_token_list(tree->tokens);
            }
            break;
        default:
            count += ft_printf("UNKNOWN");
    }
    
    count += ft_printf("\n");
    
    // Print children recursively
    if (tree->left) {
        count += ft_print_tree_indent(depth);
        count += ft_printf("LEFT:\n");
        count += ft_print_syntax_tree(tree->left, depth + 1);
    }
    
    if (tree->right) {
        count += ft_print_tree_indent(depth);
        count += ft_printf("RIGHT:\n");
        count += ft_print_syntax_tree(tree->right, depth + 1);
    }
    
    return count;
}
