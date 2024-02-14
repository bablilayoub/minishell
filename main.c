#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

int main(void)
{
	while (1)
	{
		char *input = readline("Enter a string: ");
		if (!input)
			break;
		if (input[0] != 0)
			add_history(input);
		printf("You entered: %s\n", input);
		free(input);
	}
	return 0;
}
