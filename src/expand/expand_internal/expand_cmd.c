#include "expand_internal.h"

static t_status	expand_arg(t_expand *ex, t_word *wd, char **cmd,
						t_fields *fields)
{
	char	*str;

	if (*cmd && ft_strcmp(*cmd, "export") == 0
		&& (wd->flag & (W_ASSIGN | W_APPEND)) != 0)
	{
		str = expand_word_str(ex, wd);
		if (!str)
			return (ST_FATAL);
		if (!fields_add(ex, fields, str, ft_strlen(str)))
			return (ST_FATAL);
	}
	else if (expand_word(ex, wd, EXP_FIELDS, fields) != ST_OK)
		return (ST_FATAL);
	if (!*cmd && fields->head)
		*cmd = fields->head->wd->str;
	return (ST_OK);
}

t_status	expand_args(t_expand *ex, t_simple_cmd *cmd)
{
	t_word_list	*cur;
	t_fields	fields;
	char		*cmd_name;

	if (!fields_init(&fields, &ex->arenas->tmp))
		return (ST_FATAL);
	cmd_name = NULL;
	cur = cmd->args;
	while (cur)
	{
		if (expand_arg(ex, cur->wd, &cmd_name, &fields) != ST_OK)
			return (ST_FATAL);
		cur = cur->next;
	}
	cmd->args = fields.head;
	ft_arena_reset(&ex->arenas->tmp);
	return (ST_OK);
}

static t_status	expand_redir_target(t_expand *ex, t_redirect *redir)
{
	t_fields	fields;

	if (!fields_init(&fields, &ex->arenas->tmp))
		return (ST_FATAL);
	if (expand_word(ex, &redir->target, EXP_FIELDS, &fields) != ST_OK)
		return (ST_FATAL);
	if (!fields.head || fields.head->next)
	{
		ex->ctx->err.exit_code = 1;
		ex->ctx->err.err_msg = EXPAND_MSG_AMBIG_REDIR;
		return (ST_FAILURE);
	}
	redir->target.str = fields.head->wd->str;
	redir->target.len = fields.head->wd->len;
	redir->target.flag = W_NONE;
	redir->target.next = NULL;
	ft_arena_reset(&ex->arenas->tmp);
	return (ST_OK);
}

t_status	expand_redirects(t_expand *ex, t_redirect *redir)
{
	t_status	status;

	while (redir)
	{
		if (redir->op == REDIR_DLESS)
			status = expand_heredoc_body(redir, ex->ctx,
					&ex->arenas->heredoc);
		else
			status = expand_redir_target(ex, redir);
		if (status != ST_OK)
			return (status);
		redir = redir->next;
	}
	return (ST_OK);
}

t_status	expand_assigns(t_expand *ex, t_assign *assign)
{
	char	*str;

	while (assign)
	{
		if (assign->value)
		{
			str = expand_word_str(ex, assign->value);
			if (!str)
				return (ST_FATAL);
			assign->value->str = str;
			assign->value->len = (int)ft_strlen(str);
			assign->value->flag = W_NONE;
			assign->value->next = NULL;
		}
		assign = assign->next;
	}
	return (ST_OK);
}
