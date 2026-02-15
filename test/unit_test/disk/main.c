/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 15:33:14 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/15 15:59:42 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "disk_test.h"

int	main(int argc, char **argv, char **envp)
{
	t_ctx			ctx;
	t_simple_cmd	cmd1;

	/*
		Test case
		- command without slash
		- (i)
		- command with slash
		- (i)
	*/
	(void)argc;
	(void)argv;
	if (init_ctx(&ctx, envp) == FAILURE)
		return (1);
	cmd1.redirects = NULL;
	cmd1.assigns = NULL;
	cmd1.args = malloc(sizeof(t_word_list));
	cmd1.args->wd = malloc(sizeof(t_word));
	cmd1.args->wd->str = ft_strdup("ls");
	cmd1.args->next = NULL;
	puts("pass");
	if (exec_disk_command(&cmd1, &ctx, NO_PIPE, NO_PIPE) == ST_FATAL)
		return (1);
	return (0);
}
