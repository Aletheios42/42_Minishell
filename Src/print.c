#include "../Inc/minishell.h"
#include "../libft/libft.h" 

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

// Function to print redirection list
int ft_print_redirs(t_redir *redirs)
{
    int count = 0;
    t_redir *curr = redirs;
    
    if (!curr)
        return ft_printf("(null)");
    
    count += ft_printf("[");
    while (curr) {
        count += ft_printf("{type=%d, file=\"%s\"}", 
                          curr->type, 
                          curr->filename ? curr->filename : "(null)");
        
        if (curr->next)
            count += ft_printf(", ");
        
        curr = curr->next;
    }
    count += ft_printf("]");
    
    return count;
}

// Print tree function with support for new command structure
int ft_print_tree(t_tree *tree, int depth)
{
    int count = 0;
    
    if (!tree)
        return (ft_printf("(null)\n"));
    
    count += ft_print_tree_indent(depth);
    
    // Print based on node type
    switch (tree->type) {
        case NODE_COMMAND:
            count += ft_printf("Command{");
            count += ft_printf("args=");
            count += ft_print_string_array(tree->data.command.args);
            count += ft_printf(", redirs=");
            count += ft_print_redirs(tree->data.command.redirs);
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
    ft_printf("\n");
    
    return (count);
}

