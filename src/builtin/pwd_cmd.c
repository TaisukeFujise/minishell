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
	"pwd" invoke getcwd(), and update the ctx->exit_code.
	It returns always physical path. It's different from "pwd" in bash.
	Operands are ignored, as bash ignores them: the subject asks for
	"pwd with no options", which is about options, not about operands.
*/
t_status	pwd_cmd(t_word_list *args, t_ctx *ctx)
{
	char	*path;
	bool	ok;

	(void)args;
	(void)ctx;
	path = getcwd(NULL, 0);
	if (path == NULL)
	{
		print_error("pwd", strerror(errno));
		return (ST_FAILURE);
	}
	ok = write_all(STDOUT_FILENO, path, ft_strlen(path))
		&& write_all(STDOUT_FILENO, "\n", 1);
	free(path);
	if (!ok)
		return (ST_FAILURE);
	return (ST_OK);
}
