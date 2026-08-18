#include "expand_dump.h"
#include <stdio.h>

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s <line>\n", argv[0]);
		return (1);
	}
	expand_dump_input(argv[1]);
	return (0);
}
