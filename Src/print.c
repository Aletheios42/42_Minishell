#include "../Inc/minishell.h"
#include "../libft/libft.h" 

const char *get_token_type_string(t_token_type type) {
    static const char *token_names[] = {
        "REDIR_IN", "REDIR_OUT", "APPEND", "HEREDOC", 
        "AND", "OR", "PIPE", "OPEN_PAREN", "CLOSE_PAREN",
        "WORD", "EOF"
    };
    return token_names[type];
}

void print_token_list(t_token *head) {
    t_token *current = head;
    int i = 0;
    
    while (current) {
        printf("[%d] Type: %-15s Value: \"%s\"\n", 
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

