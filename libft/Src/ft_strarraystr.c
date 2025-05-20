#include "libft.h"

// Busca un array de patrones en una cadena y encuentra el primero
char *ft_strarraystr(const char *str, char **patterns) {
    char *earliest = NULL;
    int i = 0;
    size_t len = ft_strlen(str);
    
    if (!str || !patterns)
        return NULL;
    
    while (patterns[i]) {
        char *found = ft_strnstr(str, patterns[i], len);
        
        if (found && (!earliest || found < earliest)) {
            earliest = found;
        }
        i++;
    }
    
    return earliest;
}
