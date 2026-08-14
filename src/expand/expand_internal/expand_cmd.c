#include "expand_internal.h"

static t_status	expand_arg(t_expand *exp, t_word *wd, char **cmd,
						t_fields *fields)
{
	char	*str;

	if (*cmd && ft_strcmp(*cmd, "export") == 0
		&& (wd->flag & (W_ASSIGN | W_APPEND)) != 0)
	{
		str = expand_word_str(exp, wd);
		if (!str)
			return (ST_FATAL);
		if (!field_insert(exp, fields->tail, str, ft_strlen(str)))
			return (ST_FATAL);
		fields->tail = &(*fields->tail)->next;
	}
	else if (expand_word(exp, wd, EXP_FIELDS, fields) != ST_OK)
		return (ST_FATAL);
	if (!*cmd && fields->head)
		*cmd = fields->head->wd->str;
	return (ST_OK);
}

t_status	expand_args(t_expand *exp, t_simple_cmd *cmd)
{
	t_word_list	*cur;
	t_fields	fields;
	char		*cmd_name;
	t_status	status;

	fields_init(&fields);
	status = ST_OK;
	cmd_name = NULL;
	cur = cmd->args;
	while (cur && status == ST_OK)
	{
		status = expand_arg(exp, cur->wd, &cmd_name, &fields);
		cur = cur->next;
	}
	if (status == ST_OK)
		cmd->args = fields.head;
	ft_arena_reset(&exp->arenas->tmp);
	return (status);
}

static t_status	expand_redir_target(t_expand *exp, t_redirect *redir)
{
	t_fields	fields;
	t_status	status;

	fields_init(&fields);
	status = expand_word(exp, &redir->target, EXP_FIELDS, &fields);
	if (status == ST_OK && (!fields.head || fields.head->next))
	{
		exp->ctx->err.exit_code = 1;
		exp->ctx->err.err_msg = EXPAND_MSG_AMBIG_REDIR;
		status = ST_FAILURE;
	}
	if (status == ST_OK)
	{
		redir->target.str = fields.head->wd->str;
		redir->target.len = fields.head->wd->len;
		redir->target.flag = W_NONE;
		redir->target.next = NULL;
	}
	ft_arena_reset(&exp->arenas->tmp);
	return (status);
}

t_status	expand_redirects(t_expand *exp, t_redirect *redir)
{
	t_status	status;

	while (redir)
	{
		if (redir->op == REDIR_DLESS)
		{
			status = expand_heredoc_body(exp, redir);
			ft_arena_reset(&exp->arenas->tmp);
		}
		else
			status = expand_redir_target(exp, redir);
		if (status != ST_OK)
			return (status);
		redir = redir->next;
	}
	return (ST_OK);
}

t_status	expand_assigns(t_expand *exp, t_assign *assign)
{
	char	*str;

	while (assign)
	{
		str = ft_arena_strndup(&exp->arenas->ast, assign->key->str,
				assign->key->len);
		if (!str)
			return (ST_FATAL);
		assign->key->str = str;
		if (assign->value)
		{
			str = expand_word_str(exp, assign->value);
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
