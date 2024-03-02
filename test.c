#include <stdio.h>
#include <stdlib.h>

int main() {
    // Get the value of the HOME environment variable
    char *home = getenv("HOME");
    
    if (home == NULL) {
        printf("HOME environment variable not set.\n");
        return 1;
    }

    // Define your variable name
    const char *var_name = "test";

    // Construct the value for your new variable
    char *value = malloc(strlen(var_name) + strlen(home) + 2); // +2 for '=' and '\0'
    if (value == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }
    sprintf(value, "%s=%s", var_name, home);

    // Export the variable
    if (setenv(var_name, home, 1) != 0) {
        printf("Failed to set %s variable.\n", var_name);
        free(value);
        return 1;
    }

    printf("%s variable exported with value %s.\n", var_name, home);

    // Free allocated memory
    free(value);

    return 0;
}
