#include "lexer.h"
#include <stdio.h>

void	dump_tokens(char *line);

int	main(int argc, char **argv)
{
	char	*input;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <input_string>\n", argv[0]);
		return (1);
	}
	input = argv[1];
	dump_tokens(input);
	return (0);
}
