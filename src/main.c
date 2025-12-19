/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 20:41:47 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/19 22:53:58 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/signal_handle.h"
#include "../include/execute.h"

volatile sig_atomic_t	g_signum = 0;

int	main(void)//int argc, char **argv, char **envp)
{
	char	*user_input;
	// Token	token;
	// t_node	ast;
	// t_exec	executor; 

	if (set_signal() == FAILURE)
		return (1);
	while (1)
	{
		if (isatty(STDIN_FILENO) == 1) // if user_input is sent by pipe.
			user_input = readline("minishell$ ");
		else
			user_input = get_next_line(STDIN_FILENO); // if user_input is sent by tty
		if (user_input == NULL)
			break; // ctrl-D sends EOF, and readline returns NULL receiving EOF.
		if (*user_input)
			add_history(user_input);
		g_signum = 0;
		// tokenize(user_input, &token);
		// parse(token, &ast); 
		// execute(&ast, &executor);
		free(user_input);
		// Here, free other objects in token, ast, executor.
	}
	rl_clear_history();
	return (0);
}
