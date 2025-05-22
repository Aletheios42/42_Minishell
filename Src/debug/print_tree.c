#include "../Inc/minishell.h"
#include "../libft/libft.h"

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

// Print the node's hierarchical ID
int print_node_id(char *prefix)
{
    return ft_printf("[%s] ", prefix);
}

// Print connection lines for the tree
void print_tree_lines(int depth, int is_last)
{
    int i;
    
    for (i = 0; i < depth - 1; i++)
        ft_printf("│   ");
    
    if (depth > 0)
    {
        if (is_last)
            ft_printf("└── ");
        else
            ft_printf("├── ");
    }
}

// Recursive tree printing function
int ft_print_tree_node(t_tree *tree, int depth, int is_last, char *prefix)
{
    int count = 0;
    char left_prefix[32];
    char right_prefix[32];
    
    if (!tree)
        return 0;
    
    // Print tree connection lines
    print_tree_lines(depth, is_last);
    
    // Print node ID and type
    count += print_node_id(prefix);
    
    // Print node content based on type
    switch (tree->type) {
        case NODE_COMMAND:
            count += ft_printf("COMMAND: ");
            if (tree->tokens) {
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
            if (tree->tokens)
                count += ft_printf(" (%s)", tree->tokens->value);
            break;
        default:
            count += ft_printf("UNKNOWN");
    }
    
    count += ft_printf("\n");
    
    // Create child prefixes
    snprintf(left_prefix, sizeof(left_prefix), "%s.1", prefix);
    snprintf(right_prefix, sizeof(right_prefix), "%s.2", prefix);
    
    // Print children
    if (tree->left)
        count += ft_print_tree_node(tree->left, depth + 1, (tree->right == NULL), left_prefix);
    if (tree->right)
        count += ft_print_tree_node(tree->right, depth + 1, 1, right_prefix);
    
    return count;
}

// Main tree printing function - fixed to use the unused parameter
int ft_print_syntax_tree(t_tree *tree, int depth)
{
    (void)depth; // Mark parameter as intentionally unused
    return ft_print_tree_node(tree, 0, 1, "1");
}
