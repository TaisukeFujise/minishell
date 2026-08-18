/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:39:30 by tafujise          #+#    #+#             */
/*   Updated: 2026/07/31 01:36:30 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"
#include "../../include/execute.h"
#include "../../include/variables.h"
#include "../../include/hashmap.h"

/*
	env
	"env" display the env table, and update the ctx->exit_code.
	It shares the environment of the current command with execve.
	With operands it is not a builtin at all: exec_simple sends it down
	the disk command path, where the env of the system runs it.
*/
t_status	env_cmd(t_word_list *args, t_ctx *ctx)
{
	char	**envp;
	int		i;

	(void)args;
	envp = build_envp(ctx->tmp_table, ctx->env_table);
	if (envp == NULL)
		return (ST_FATAL);
	i = 0;
	while (envp[i])
	{
		if (!write_all(STDOUT_FILENO, envp[i], ft_strlen(envp[i]))
			|| !write_all(STDOUT_FILENO, "\n", 1))
			return (free_envp(envp), ST_FAILURE);
		i++;
	}
	free_envp(envp);
	return (ST_OK);
}
