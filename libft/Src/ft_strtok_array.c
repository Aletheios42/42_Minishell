#include "libft.h"

char *strtok_array(char *str, char **delims) {
    static char *last = NULL;
    char *token_start, *token_end = NULL;
    int i;
    
    // Initialize or continue from last position
    if (str != NULL)
        last = str;
    else if (last == NULL)
        return NULL;
        
    // Skip leading delimiters
    token_start = last;
    int found = 1;
    while (found && *token_start) {
        found = 0;
        for (i = 0; delims[i] != NULL; i++) {
            int len = ft_strlen(delims[i]);
            if (ft_strncmp(token_start, delims[i], len) == 0) {
                token_start += len;
                found = 1;
                break;
            }
        }
    }
    
    // Check for end of string
    if (*token_start == '\0') {
        last = NULL;
        return NULL;
    }
    
    // Find next delimiter
    token_end = token_start;
    while (*token_end) {
        for (i = 0; delims[i] != NULL; i++) {
            int len = strlen(delims[i]);
            if (strncmp(token_end, delims[i], len) == 0) {
                *token_end = '\0';
                last = token_end + len;
                return token_start;
            }
        }
        token_end++;
    }
    
    // End of string with no more delimiters
    last = NULL;
    return token_start;
}
