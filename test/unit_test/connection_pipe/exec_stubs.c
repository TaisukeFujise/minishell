#include "connection_pipe_test.h"

t_status	exec_builtin(t_simple_cmd *cmd, t_ctx *ctx, int pipe_in, int pipe_out)
{
	(void)cmd;
	(void)ctx;
	(void)pipe_in;
	(void)pipe_out;
	return (ST_FATAL);
}

t_status	exec_subshell(t_node *node, t_ctx *ctx, int pipe_in, int pipe_out)
{
	(void)node;
	(void)ctx;
	(void)pipe_in;
	(void)pipe_out;
	return (ST_FATAL);
}

t_status	expand_words(t_simple_cmd *cmd, t_ctx *ctx)
{
	(void)cmd;
	(void)ctx;
	return (ST_OK);
}
