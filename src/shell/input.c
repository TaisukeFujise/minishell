/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 19:35:07 by fendo             #+#    #+#             */
/*   Updated: 2026/08/25 19:35:08 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"
#include "../../include/signal_handle.h"

/*
	A script is read one byte at a time. A command that reads standard
	input must find it where the shell stopped reading, and lseek() is
	not available to give back what was read ahead.
*/
static char	*read_script_line(void)
{
	t_strbuf	buf;
	char		c;
	ssize_t		n;

	if (!strbuf_init(&buf))
		return (NULL);
	n = read(STDIN_FILENO, &c, 1);
	while (n == 1 && c != '\n')
	{
		if (!strbuf_append(&buf, &c, 1))
			return (strbuf_free(&buf), NULL);
		n = read(STDIN_FILENO, &c, 1);
	}
	if (n <= 0 && buf.len == 0)
		return (strbuf_free(&buf), NULL);
	return (strbuf_detach(&buf, NULL));
}

/*
	One line of input, with a prompt and history when a terminal is
	reading it. The heredoc reader asks for its lines here too, so that
	both take them from the same place.

	readline writes the prompt and the echo of what is typed to
	rl_outstream, which is stdout until told otherwise. That is the
	wrong place: "./minishell > file" would collect prompts instead of
	output. bash points it at stderr for the same reason, and stderr is
	never fully buffered, so nothing of ours can survive a fork in it.
	[bash-5.3 bashline.c initialize_readline()]
*/
char	*shell_read_line(char *prompt)
{
	if (isatty(STDIN_FILENO) == 1)
	{
		rl_outstream = stderr;
		return (readline(prompt));
	}
	return (read_script_line());
}
