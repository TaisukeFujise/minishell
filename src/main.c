/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 20:41:47 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/05 18:34:51 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

// Add the following header include statement to minishell.h
#include <signal.h>
#include <readline/readline.h>
#include <readline/history.h>

volatile sig_atomic_t	e_flag = 0;

void	handler(int signum)
{
	e_flag = signum;
}

static int	check_state()
{
	if (e_flag == SIGINT)
	{
		e_flag = 0;
		// rl_on_new_line();
		// rl_replace_line("", 0);
		// rl_redisplay();
		rl_done = 1;
	}
	return (0);
}


int	main(void)//int argc, char **argv, char **envp)
{
	char				*user_input;
	struct sigaction	sa;

	// if (argc != 1)
	// 	return (1);
	if (sigemptyset(&sa.sa_mask) == -1)
		return (perror("error"), 1);
	// if (sigaddset(&sa.sa_mask, SIGINT))
	sa.sa_handler = handler;
	sa.sa_flags = 0;
	if (sigaction(SIGINT, &sa, NULL) == -1)
		return (1);
	rl_event_hook = check_state;
	while (1)
	{
		user_input = readline("minishell$ ");
		if (user_input == NULL)
			break;
		if (*user_input)
			add_history(user_input);
		free(user_input);
	}	
	rl_clear_history();
	return (0);
}
