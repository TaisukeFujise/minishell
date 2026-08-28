/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 20:41:47 by tafujise          #+#    #+#             */
/*   Updated: 2026/08/28 18:20:00 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/parser.h"
#include "../include/shell.h"
#include "../include/signal_handle.h"

/*
	Read a line, run it, and start again. A line read while SIGINT
	arrived is still run: readline gives back what was typed before it,
	and the status of the interrupted prompt is 130.
*/
static void	shell_loop(t_ctx *ctx)
{
	char		*input;
	t_node		ast;

	while (1)
	{
		g_signum = 0;
		input = shell_read_line("minishell$ ");
		if (input == NULL)
			break ;
		if (*input)
			add_history(input);
		if (g_signum == SIGINT)
			ctx->err.exit_code = 130;
		parse_and_execute(input, &ast, ctx);
		free(input);
	}
}

int	main(int argc, char **argv, char **envp)
{
	t_ctx	ctx;

	(void)argc;
	(void)argv;
	if (set_signal() == FAILURE)
		return (1);
	if (init_ctx(&ctx, envp) == FAILURE)
		return (1);
	shell_loop(&ctx);
	dispose_shell(NULL, &ctx);
	return (ctx.err.exit_code);
}
