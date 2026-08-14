#include "expand_dump.h"
#include <readline/history.h>
#include <readline/readline.h>
#include <stdlib.h>

int	main(void)
{
	char	*line;

	while (1)
	{
		line = readline("expand dump> ");
		if (!line)
			break ;
		if (*line)
			add_history(line);
		expand_dump_input(line);
		free(line);
	}
	return (0);
}
