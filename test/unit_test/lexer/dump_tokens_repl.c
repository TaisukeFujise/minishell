#include "lexer.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdio.h>
#include <stdlib.h>

void	dump_tokens(char *line);

int	main(void)
{
	char	*line;

	while (1)
	{
		line = readline("dump tokens> ");
		if (!line)
			break ;
		if (*line != '\0')
			add_history(line);
		dump_tokens(line);
		free(line);
	}
	return (0);
}
