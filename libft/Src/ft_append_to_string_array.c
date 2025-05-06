#include <stdlib.h>
// Helper function to append a string to a string array
char **ft_append_to_string_array(char ***array, char *str) {
    char **new_array = NULL;
    int count = 0;
    
    // Count existing elements
    if (*array) {
        while ((*array)[count])
            count++;
    }
    
    // Allocate new array with space for the new string and NULL terminator
    new_array = (char **)malloc(sizeof(char *) * (count + 2));
    if (!new_array)
        return NULL;
    
    // Copy existing elements
    for (int i = 0; i < count; i++)
        new_array[i] = (*array)[i];
    
    // Add new string and NULL terminator
    new_array[count] = str;
    new_array[count + 1] = NULL;
    
    // Free old array but not the strings it contains
    if (*array)
        free(*array);
    
    return new_array;
}
