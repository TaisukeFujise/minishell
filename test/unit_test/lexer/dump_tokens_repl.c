#include "lexer.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

void	dump_tokens(t_token *head);

int	main(void)
{
	char	*line;
	t_token	*tokens;

	while (1)
	{
		line = readline("dump tokens> ");
		if (!line)
			break ;
		if (*line != '\0')
			add_history(line);
		tokens = tokenize(line);
		if (!tokens)
		{
			fprintf(stderr, "Tokenization failed due to memory allocation error.\n");
			free(line);
			return (1);
		}
		dump_tokens(tokens);
		free_tokens(tokens);
		free(line);
	}
	return (0);
}
