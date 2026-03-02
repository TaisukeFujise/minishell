/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/01 19:00:00 by fendo             #+#    #+#             */
/*   Updated: 2026/03/01 19:28:04 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "expand_internal/expand_internal.h"

static t_status	expand_args(t_expand_ctx *ex, t_word_list **args)
{
	t_word_list	*cur;
	t_word_list	*prev;
	t_word_list	*expanded;
	t_word_list	*last;

	prev = NULL;
	cur = *args;
	while (cur)
	{
		expanded = expand_word(ex, cur->wd, EXP_ARGV);
		ft_arena_reset(ex->tmp);
		if (!expanded)
		{
			if (prev)
				prev->next = cur->next;
			else
				*args = cur->next;
			cur = cur->next;
			continue ;
		}
		if (prev)
			prev->next = expanded;
		else
			*args = expanded;
		last = expanded;
		while (last->next)
			last = last->next;
		last->next = cur->next;
		prev = last;
		cur = cur->next;
	}
	return (ST_OK);
}

static t_status	expand_redirect_target(t_expand_ctx *ex, t_redirect *redir)
{
	t_word_list	*expanded;
	bool		keep_empty;

	if (redir->op == REDIR_DLESS)
		return (ST_OK);
	keep_empty = word_has_quote(&redir->target);
	expanded = expand_word(ex, &redir->target, EXP_REDIR);
	ft_arena_reset(ex->tmp);
	if (!expanded && !keep_empty)
	{
		ex->ctx->err.exit_code = 1;
		ex->ctx->err.err_msg = "ambiguous redirect";
		return (ST_FAILURE);
	}
	if (expanded && expanded->wd)
	{
		redir->target.str = expanded->wd->str;
		redir->target.len = expanded->wd->len;
		redir->target.flag = W_NONE;
		redir->target.next = NULL;
	}
	return (ST_OK);
}

static t_status	expand_redirects(t_expand_ctx *ex, t_redirect *redirects)
{
	t_redirect	*cur;
	t_status	st;

	cur = redirects;
	while (cur)
	{
		st = expand_redirect_target(ex, cur);
		if (st != ST_OK)
			return (st);
		cur = cur->next;
	}
	return (ST_OK);
}

static t_status	expand_assigns(t_expand_ctx *ex, t_assign *assigns)
{
	t_assign	*cur;
	t_word_list	*expanded;

	cur = assigns;
	while (cur)
	{
		if (cur->value)
		{
			expanded = expand_word(ex, cur->value, EXP_ASSIGN);
			ft_arena_reset(ex->tmp);
			if (expanded && expanded->wd)
				cur->value = expanded->wd;
		}
		cur = cur->next;
	}
	return (ST_OK);
}

/*
** Expand all words in a simple command:
**   - args: parameter expansion, field splitting, pathname expansion
**   - redirects: parameter expansion only
**   - assigns: parameter expansion only
*/
t_status	expand_simple_cmd(t_simple_cmd *cmd, t_ctx *ctx, t_arenas *arenas)
{
	t_expand_ctx	ex;
	t_status		st;

	if (!cmd || !ctx || !arenas)
		return (ST_FATAL);
	init_expand_ctx(&ex, ctx, arenas);
	st = expand_assigns(&ex, cmd->assigns);
	if (st != ST_OK)
		return (st);
	st = expand_args(&ex, &cmd->args);
	if (st != ST_OK)
		return (st);
	st = expand_redirects(&ex, cmd->redirects);
	return (st);
}

/*
** Expand heredoc body (parameter expansion only if delimiter was unquoted).
** Returns expanded string allocated in arena, or original body if quoted.
*/
char	*expand_heredoc_body(const char *body, size_t len, bool quoted,
							t_ctx *ctx, t_arena *arena)
{
	char				*result;
	const char			*p;
	const char			*start;
	char				*varname;
	int					varlen;
	t_bucket_contents	*entry;
	size_t				rlen;
	size_t				rcap;
	char				num[12];
	int					i;
	int					n;

	if (!body || quoted)
		return (ft_arena_strndup(arena, body, len));
	rcap = len + 64;
	result = ft_arena_alloc(arena, rcap);
	if (!result)
		return (NULL);
	rlen = 0;
	p = body;
	while (p < body + len)
	{
		if (*p == '$' && (p[1] == '?' || ft_isalpha(p[1]) || p[1] == '_'))
		{
			p++;
			if (*p == '?')
			{
				p++;
				n = ctx->err.exit_code;
				if (n == 0)
					num[0] = '0', i = 1;
				else
				{
					i = 0;
					while (n > 0)
						num[i++] = '0' + (n % 10), n /= 10;
				}
				while (i > 0)
				{
					if (rlen + 1 >= rcap)
					{
						result = ft_arena_realloc(arena, result, rcap, rcap * 2);
						rcap *= 2;
					}
					result[rlen++] = num[--i];
				}
			}
			else
			{
				start = p;
				while (p < body + len && (ft_isalnum(*p) || *p == '_'))
					p++;
				varlen = p - start;
				varname = ft_arena_strndup(arena, start, varlen);
				entry = hash_search(varname, ctx->env_table);
				if (entry && entry->data.value)
				{
					while (rlen + ft_strlen(entry->data.value) >= rcap)
					{
						result = ft_arena_realloc(arena, result, rcap, rcap * 2);
						rcap *= 2;
					}
					ft_strlcpy(result + rlen, entry->data.value, rcap - rlen);
					rlen += ft_strlen(entry->data.value);
				}
			}
		}
		else
		{
			if (rlen + 1 >= rcap)
			{
				result = ft_arena_realloc(arena, result, rcap, rcap * 2);
				rcap *= 2;
			}
			result[rlen++] = *p++;
		}
	}
	result[rlen] = '\0';
	return (result);
}
