/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 20:41:47 by tafujise          #+#    #+#             */
/*   Updated: 2026/01/29 22:38:57 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/signal_handle.h"
#include "../include/execute.h"
#include "../include/lexer.h"

volatile sig_atomic_t g_signum = 0;

/*
	handle_command_termination sets ctx->exit_code only when the main process cannot continue.
*/
void	handle_command_termination(t_status status, char *user_input, t_node *node, t_ctx *ctx)
{
	(void)node;
	(void)ctx;
	/*
		Here free "user_input" and the member of "node and ctx"(not node and ctx itself)
		because node and ctx itself are not allocated memory.
	*/
	if (status == ST_EXIT)
	{
		rl_clear_history();
		exit(ctx->exit_code);
	}
	if (status == ST_FATAL)
	{
		rl_clear_history();
		exit(1);
	}
}

int main(int argc, char **argv, char **envp)
{
	t_ctx		ctx;
	char		*user_input;
	t_status	status;
	t_node		ast;

	(void)argc;
	(void)argv;
	if (set_signal() == FAILURE)
		return (1);
	if (init_ctx(&ctx, envp) == FAILURE)
		return (1);
	while (1)
	{
		if (isatty(STDIN_FILENO) == 1) // if user_input is sent by tty.
			user_input = readline("minishell$ ");
		else
			user_input = get_next_line(STDIN_FILENO); // if user_input is sent by pipe
		if (user_input == NULL)
			break; // ctrl-D sends EOF, and readline returns NULL receiving EOF.
		if (*user_input)
			add_history(user_input);
		g_signum = 0;
		status = parse(user_input, &ast, &ctx);
		if (status != ST_OK)
			handle_command_termination(status, user_input, &ast, &ctx);
		status = execute(&ast, &ctx, NO_PIPE, NO_PIPE);
		if (status != ST_OK)
			handle_command_termination(status, user_input, &ast, &ctx);
		/*
			Here free "user_input" and the member of "node and ctx"(not node and ctx itself)
			because node and ctx itself are not allocated memory.
		*/
	}
	rl_clear_history();
	return (ctx.exit_code);
}


