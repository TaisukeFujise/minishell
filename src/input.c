/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/19 19:27:55 by tafujise          #+#    #+#             */
/*   Updated: 2026/04/19 22:23:42 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/strbuf.h"
#include "../include/signal_handle.h"

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
*/
char	*shell_read_line(char *prompt)
{
	if (isatty(STDIN_FILENO) == 1)
		return (readline(prompt));
	return (read_script_line());
}
