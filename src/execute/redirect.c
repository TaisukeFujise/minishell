/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirect.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 12:31:57 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/02 12:52:58 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parser.h"
#include "../../include/execute.h"

t_status	apply_redir_single(t_redirect *redirect, t_ctx *ctx);
t_status	apply_redir_dless(t_redirect *redirect, t_ctx *ctx);
t_status	apply_redir_dgreat(t_redirect *redirect, t_ctx *ctx);

/*
	apply_redirects call redirect func depending on redirect->op.
	- apply_redir_single
	- apply_redir_dless
	- apply_redir_dgreat
*/
t_status	apply_redirects(t_redirect *redirects, t_ctx *ctx)
{
	t_redirect	*redirect;

	redirect = redirects;
	while (redirect)
	{
		if (redirect->op == REDIR_LESS || REDIR_GREAT)
		{
			if (apply_redir_single(redirect, ctx) == ST_FATAL)
				return (ST_FATAL);
		}
		else if (redirect->op == REDIR_DLESS)
		{
			if (apply_redir_dless(redirect, ctx) == ST_FATAL)
				return (ST_FATAL);
		}
		else if (redirect->op == REDIR_DGREAT)
		{
			if (apply_redir_dgreat(redirect, ctx) == ST_FATAL)
				return (ST_FATAL);
		}
		else
			return (ST_FATAL);
		redirect = redirect->next;
	}
	return (ST_OK);
}

t_status	apply_redir_single(t_redirect *redirect, t_ctx *ctx)
{
}

t_status	apply_redir_dless(t_redirect *redirect, t_ctx *ctx)
{

}

t_status	apply_redir_dgreat(t_redirect *redirect, t_ctx *ctx)
{

}

