/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 20:41:47 by tafujise          #+#    #+#             */
/*   Updated: 2026/01/18 21:56:06 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/signal_handle.h"
#include "../include/execute.h"
#include "../include/lexer.h"

volatile sig_atomic_t g_signum = 0;

/*
	Todo
	- handle error関連
*/
void	handle_parse_error(t_status status, t_node *node, t_ctx *ctx)
{
	(void)status;
	(void)node;
	(void)ctx;
}

void	handle_execute_error(t_status status, t_node *node, t_ctx *ctx)
{
	(void)status;
	(void)node;
	(void)ctx;
}

int main(int argc, char **argv, char **envp)
{
	t_ctx		ctx;
	char		*user_input;
	t_status	status;
	t_node		ast;
	t_exec		executor;

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
		if (status != ST_SUCCESS)
			handle_parse_error(status, &ast, &ctx);
		status = execute(&ast, &executor, &ctx);
		if (status != ST_SUCCESS)
			handle_execute_error(status, &executor, &ctx);
		free(user_input);
		// Here, free other objects in token, ast, executor.
	}
	rl_clear_history();
	return (0);
}


