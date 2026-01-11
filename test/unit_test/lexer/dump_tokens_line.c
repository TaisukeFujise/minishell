#include "lexer.h"
#include <stdio.h>

void	dump_tokens(t_token *head);

int	main(int argc, char **argv)
{
	t_token	*tokens;
	char	*input;

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <input_string>\n", argv[0]);
		return (1);
	}
	input = argv[1];
	tokens = tokenize(input);
	if (!tokens)
	{
		fprintf(stderr, "Tokenization failed due to memory allocation error.\n");
		return (1);
	}
	dump_tokens(tokens);
	free_tokens(tokens);
	return (0);
}
