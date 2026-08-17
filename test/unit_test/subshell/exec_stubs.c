#include "subshell_test.h"

t_status	execute_internal(t_node *node, t_ctx *ctx, t_stage st)
{
	(void)node;
	(void)st;
	ctx->err.exit_code = 41;
	return (ST_OK);
}

t_status	expand_command(t_node *node, t_ctx *ctx, t_arenas *arenas)
{
	(void)node;
	(void)ctx;
	(void)arenas;
	return (ST_OK);
}
