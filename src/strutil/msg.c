/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   msg.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 19:35:17 by fendo             #+#    #+#             */
/*   Updated: 2026/08/25 19:35:18 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include <unistd.h>

/*
	Write all of it. A write can stop short, and a signal can cut it off
	before anything is written, so neither ends the attempt.
	The output of the shell does not go through stdio: a buffer that
	survives a fork or a restored redirect writes to the wrong place.
	[EXECUTOR_INCREMENTAL_REFACTOR_PLAN 11, review D37-23]
*/
bool	write_all(int fd, const char *s, size_t len)
{
	ssize_t	written;
	size_t	done;

	done = 0;
	while (done < len)
	{
		written = write(fd, s + done, len - done);
		if (written < 0 && errno == EINTR)
			continue ;
		if (written <= 0)
			return (false);
		done += (size_t)written;
	}
	return (true);
}

/*
	One part of a message and the separator that follows it, left out
	when there is no part to write.
*/
static void	put_part(const char *s, const char *tail)
{
	if (s == NULL)
		return ;
	write_all(STDERR_FILENO, s, ft_strlen(s));
	write_all(STDERR_FILENO, tail, ft_strlen(tail));
}

/*
	How the shell reports a failure: "minishell: name: operand: reason"
	on stderr, each part left out when there is nothing to put there.
	Written where the failure happens, so that a child can report its own
	and no message has to outlive the arena it was built in.
*/
void	print_error_at(const char *name, const char *arg, const char *reason)
{
	write_all(STDERR_FILENO, "minishell: ", 11);
	put_part(name, ": ");
	put_part(arg, ": ");
	put_part(reason, "\n");
}

void	print_error(const char *name, const char *reason)
{
	print_error_at(name, NULL, reason);
}

/*
	The same, for a name the shell refuses, quoted the way bash quotes
	it: "minishell: unset: `1x': not a valid identifier".
*/
void	print_error_name(const char *name, const char *word, const char *reason)
{
	write_all(STDERR_FILENO, "minishell: ", 11);
	put_part(name, ": `");
	put_part(word, "': ");
	put_part(reason, "\n");
}
