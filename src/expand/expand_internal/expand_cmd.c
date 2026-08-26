/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 19:37:02 by fendo             #+#    #+#             */
/*   Updated: 2026/08/25 19:37:03 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"
#include "expand_internal.h"

static t_status	expand_arg(t_expand *exp, t_word *wd, char **cmd,
						t_fields *fields)
{
	if (*cmd && ft_strcmp(*cmd, "export") == 0
		&& (wd->flag & (W_ASSIGN | W_APPEND)) != 0)
	{
		if (expand_word(exp, wd, NULL) != ST_OK
			|| !field_insert(exp, fields->tail, exp->buf.text.data,
				exp->buf.text.len))
			return (ST_FATAL);
		fields->tail = &(*fields->tail)->next;
	}
	else if (expand_word(exp, wd, fields) != ST_OK)
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
	return (status);
}

static t_status	expand_redir_target(t_expand *exp, t_redirect *redir)
{
	t_fields	fields;
	t_status	status;

	fields_init(&fields);
	status = expand_word(exp, &redir->target, &fields);
	if (status == ST_OK && (!fields.head || fields.head->next))
	{
		exp->ctx->err.exit_code = 1;
		print_error(NULL, EXPAND_MSG_AMBIG_REDIR);
		status = ST_FAILURE;
	}
	if (status == ST_OK)
	{
		redir->target.str = fields.head->wd->str;
		redir->target.len = fields.head->wd->len;
		redir->target.flag = W_NONE;
		redir->target.next = NULL;
	}
	return (status);
}

t_status	expand_redirects(t_expand *exp, t_redirect *redir)
{
	t_status	status;

	while (redir)
	{
		if (redir->op == REDIR_DLESS)
			status = expand_heredoc_body(exp, redir);
		else
			status = expand_redir_target(exp, redir);
		if (status != ST_OK)
			return (status);
		redir = redir->next;
	}
	return (ST_OK);
}

t_status	expand_assigns(t_expand *exp, t_assign *assign, t_hashtable *table,
		t_tabletype type)
{
	while (assign)
	{
		assign->key->str = ft_arena_strndup(&exp->arenas->ast,
				assign->key->str, assign->key->len);
		if (!assign->key->str)
			return (ST_FATAL);
		if (assign->value)
		{
			if (expand_word(exp, assign->value, NULL) != ST_OK)
				return (ST_FATAL);
			assign->value->str = ft_arena_strndup(&exp->arenas->ast,
					exp->buf.text.data, exp->buf.text.len);
			if (!assign->value->str)
				return (ST_FATAL);
			assign->value->len = exp->buf.text.len;
			assign->value->flag = W_NONE;
			assign->value->next = NULL;
		}
		if (apply_assign(assign, table, exp->ctx, type) != ST_OK)
			return (ST_FATAL);
		assign = assign->next;
	}
	return (ST_OK);
}
