#include "hashmap_test.h"

int	main(int argc, char **argv, char **envp)
{
	t_ctx	ctx;

	(void)argc;
	(void)argv;
	printf("**init_ctx**\n");
	printf("return value: %d\n", init_ctx(&ctx, envp));
	print_ctx(&ctx);
	hash_flush(ctx.env_table, NULL);
	hash_dispose(ctx.env_table);
	return (0);
}
