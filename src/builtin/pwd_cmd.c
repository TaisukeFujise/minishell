/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:41:22 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/11 16:56:19 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/parser.h"

// #include "../../include/builtin.h"

/*
	pwd
	"pwd" tells where the shell is, and updates the ctx->exit_code. The
	answer is the shell's own path, so it survives a directory that was
	renamed underneath it, as bash's does. Operands are ignored, as bash
	ignores them: the subject asks for "pwd with no options", which is
	about options, not about operands.
*/
t_status	pwd_cmd(t_word_list *args, t_ctx *ctx)
{
	bool	ok;

	(void)args;
	if (ctx->cwd == NULL)
	{
		print_error("pwd", strerror(ENOENT));
		return (ST_FAILURE);
	}
	ok = write_all(STDOUT_FILENO, ctx->cwd, ft_strlen(ctx->cwd))
		&& write_all(STDOUT_FILENO, "\n", 1);
	if (!ok)
		return (ST_FAILURE);
	return (ST_OK);
}
