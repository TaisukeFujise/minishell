/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   apply_redirect.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/02 12:31:57 by tafujise          #+#    #+#             */
/*   Updated: 2026/05/10 21:50:52 by fujisetaisuke    ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"
#include "../../../include/minishell.h"
#include "../../../include/parser.h"

static int	open_redirect_fd(t_redirect *redirect);

/*
	apply_redirects call redirect func depending on redirect->op.
	- apply_redir_great
	- apply_redir_less
	- apply_redir_dgreat
	- apply_redir_dless
*/
t_status	apply_redirects(t_redirect *redirects)
{
	t_status	status;
	int			fd;

	status = ST_OK;
	while (redirects)
	{
		fd = open_redirect_fd(redirects);
		if (fd < 0)
			return (ST_FAILURE);
		if (move_fd(fd, redirects->io_number) != ST_OK)
			return (close(fd), ST_FAILURE);
		redirects = redirects->next;
	}
	return (status);
}

static int	open_redirect_fd(t_redirect *redirect)
{
	if (redirect->op == REDIR_GREATER)
		return (open(redirect->target.str, O_WRONLY | O_CREAT | O_TRUNC, 0644));
	else if (redirect->op == REDIR_LESS)
		return (open(redirect->target.str, O_RDONLY, 0644));
	else if (redirect->op == REDIR_DGREATER)
		return (open(redirect->target.str, O_WRONLY | O_CREAT | O_APPEND,
				0644));
	else if (redirect->op == REDIR_DLESS)
		return (open_heredoc_fd(redirect));
	else
		return (-1);
}
