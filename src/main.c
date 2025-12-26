/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 20:41:47 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/26 19:49:33 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"
#include "../include/signal_handle.h"
#include "../include/execute.h"
#include "../include/lexer.h"

volatile sig_atomic_t	g_signum = 0;

int	main(int argc, char **argv, char **envp)
{
	// t_hashmap_list	*map_list;
	char	*user_input;
	t_token	*token;
	// t_node	ast;
	// t_exec	executor; 

	(void)argc;
	(void)argv;
	(void)envp;
	// if (set_hashmap(envp) == FAILURE)
	// 	return (1);
	if (set_signal() == FAILURE)
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
		token = tokenize(user_input);
		// parse(token, &ast); 
		// execute(&ast, &executor);
		free(user_input);
		// Here, free other objects in token, ast, executor.
	}
	rl_clear_history();
	return (0);
}
