#include "hashmap_test.h"

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	t_exec	executor;

	printf("**init_executor**");
	printf("return value: %d\n", init_executor(&executor, envp));
	print_executor(&executor);
	return (0);
}
