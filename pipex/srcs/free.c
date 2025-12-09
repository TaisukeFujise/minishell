/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 20:40:00 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/08 07:14:36 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	free_cmd_set(t_cmd *cmd_set, int cmd_sum);

void	free_ctx(t_ctx *ctx)
{
	if (ctx != NULL)
	{
		if (ctx->cmd_set != NULL)
			free_cmd_set(ctx->cmd_set, ctx->cmd_sum);
	}
}

void	free_cmd_set(t_cmd *cmd_set, int cmd_sum)
{
	int	i;
	int	j;

	if (cmd_set == NULL)
		return ;
	i = 0;
	while (i < cmd_sum)
	{
		j = 0;
		while (cmd_set[i].cmd[j] != NULL)
		{
			free(cmd_set[i].cmd[j]);
			j++;
		}
		free(cmd_set[i].cmd);
		i++;
	}
	free(cmd_set);
}

void	free_pathset(char ***path_set)
{
	int	i;

	i = 0;
	while ((*path_set)[i] != NULL)
	{
		free((*path_set)[i]);
		i++;
	}
	free(*path_set);
}
