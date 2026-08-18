#include "subshell_test.h"

pid_t	g_body_pid = 0;

t_status	execute_internal(t_node *node, t_ctx *ctx, bool own)
{
	(void)node;
	(void)own;
	g_body_pid = getpid();
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

/* the shell's signal setup is not linked into this test */
void	reset_signals(void)
{
}
