#include "../../include/execute.h"
#include "expand_internal/expand_internal.h"

/*
	IFS comes from the environment of the shell, never from the prefix
	assignments of the command being expanded: the words of "IFS=: cmd
	$x" are split with the IFS that was in force before it. Passing no
	tmp_table says so, instead of relying on it being empty here.
	[review OVL-02]
*/
static const char	*get_ifs(t_ctx *ctx)
{
	char	*value;

	if (!ctx->env_table)
		return (IFS_DEFAULT);
	value = env_lookup(NULL, ctx->env_table, "IFS");
	if (!value)
		return (IFS_DEFAULT);
	return (value);
}

/*
	Without a command name the assignments update the shell itself, and the
	redirections see them. With a command name they only make the environment
	of that command, so the arguments and the redirections are expanded first.
	[ bash manual, Simple Command Expansion ]
*/
static t_status	expand_simple(t_expand *exp, t_simple_cmd *cmd)
{
	t_status	status;

	if (cmd->args == NULL)
	{
		status = expand_assigns(exp, cmd->assigns, exp->ctx->env_table, VARS);
		if (status == ST_OK)
			status = expand_redirects(exp, cmd->redirects);
		return (status);
	}
	status = expand_args(exp, cmd);
	if (status == ST_OK)
		status = expand_redirects(exp, cmd->redirects);
	if (status == ST_OK)
		status = expand_assigns(exp, cmd->assigns, exp->ctx->tmp_table, TMP);
	return (status);
}

t_status	expand_command(t_node *node, t_ctx *ctx, t_arenas *arenas)
{
	t_expand	exp;
	t_status	status;

	exp.ctx = ctx;
	exp.arenas = arenas;
	exp.ifs = get_ifs(ctx);
	if (!expand_buf_init(&exp.buf))
		return (ST_FATAL);
	status = ST_OK;
	if (node->node_kind == NODE_SIMPLE)
		status = expand_simple(&exp, &node->u_node.simple_command);
	else if (node->node_kind == NODE_SUBSHELL)
		status = expand_redirects(&exp, node->u_node.subshell.redirects);
	ft_arena_reset(&arenas->tmp);
	expand_buf_free(&exp.buf);
	return (status);
}
