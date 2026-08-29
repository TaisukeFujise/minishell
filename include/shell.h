/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/28 18:20:00 by fendo             #+#    #+#             */
/*   Updated: 2026/08/28 18:20:00 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SHELL_H
# define SHELL_H

/*
	The life of the shell around one line: bring the environment up,
	read a line, run everything on it, and give the environment back.
	What happens between parse and exit is execute.h.
*/

# include "./hashmap.h"
# include "./minishell.h"
# include "./parser.h"

int		init_ctx(t_ctx *ctx, char **envp);
int		init_shell_vars(t_ctx *ctx);

char	*shell_read_line(char *prompt);
void	parse_and_execute(char *input, t_node *ast, t_ctx *ctx);
void	dispose_shell(char *user_input, t_ctx *ctx);

#endif
