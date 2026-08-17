#include "subshell_test.h"

t_status	execute_internal(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out)
{
	(void)node;
	(void)pipe_in;
	(void)pipe_out;
	ctx->err.exit_code = 41;
	return (ST_OK);
}
