#include "subshell_test.h"

t_status	execute_internal(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out)
{
	(void)node;
	(void)pipe_in;
	(void)pipe_out;
	return ((t_status)(40 + ctx->subshell_level));
}
