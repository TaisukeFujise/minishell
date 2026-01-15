#include "hashmap_test.h"

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;
	t_exec	executor;

	printf("**init_executor**");
	printf("return value: %d\n", init_executor(&executor, envp));
	print_executor(&executor);
	hash_flush(executor.env_table, NULL);
	hash_dispose(executor.env_table);
	return (0);
}
