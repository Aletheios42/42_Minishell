#include "../Inc/minishell.h"
#include "../libft/libft.h" 

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

// Print tree function with support for string arrays
int ft_print_tree(t_tree *tree, int depth)
{
    int count = 0;
    
    if (!tree)
        return (ft_printf("(null)"));
    
    count += ft_print_tree_indent(depth);
    
    // Print based on node type
    switch (tree->type) {
        case NODE_COMMAND:
            count += ft_printf("Command{");
            count += ft_printf("cmd=");
            count += ft_print_string_array(tree->data.command.cmd);
            count += ft_printf(", infiles=");
            count += ft_print_string_array(tree->data.command.infiles);
            count += ft_printf(", outfiles=");
            count += ft_print_string_array(tree->data.command.outfiles);
            count += ft_printf("\n");
            break;
            
        case NODE_PIPE:
        case NODE_AND:
        case NODE_OR:
        case NODE_PAREN:
            count += ft_printf("Operator{type=%d, op=\"%s\"\n", 
                tree->type,
                tree->data.operator ? tree->data.operator : "(null)");
            break;
            
        default:
            count += ft_printf("Unknown{type=%d\n", tree->type);
    }
    
    if (tree->left)
    {
        count += ft_print_tree_indent(depth + 1);
        count += ft_printf("left=\n");
        count += ft_print_tree(tree->left, depth + 2);
    }
    
    if (tree->right)
    {
        count += ft_print_tree_indent(depth + 1);
        count += ft_printf("right=\n");
        count += ft_print_tree(tree->right, depth + 2);
    }
    
    count += ft_print_tree_indent(depth);
    count += ft_printf("}\n");
    
    return (count);
}

int ft_print_token(t_token *token)
{
    int count = 0;
    
    if (!token)
        return (ft_printf("(null)"));
    count += ft_printf("Token{value=\"%s\", type=%d", 
            token->value ? token->value : "(null)", 
            token->type);
    if (token->next)
        count += ft_printf(", next=→}");
    else
        count += ft_printf(", next=NULL}");
    return (count);
}

int ft_print_token_list(t_token *tokens)
{
    int count = 0;
    t_token *current;
    
    if (!tokens)
        return (ft_printf("(null)"));
    current = tokens;
    while (current)
    {
        count += ft_print_token(current);
        count += ft_printf("\n");
        current = current->next;
    }
    return (count);
}
