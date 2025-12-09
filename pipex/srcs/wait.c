/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wait.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/06 23:23:05 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/08 06:51:48 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	wait_former_pid(t_ctx *ctx, int index)
{
	int	i;

	i = 0;
	while (i < index)
	{
		if (waitpid(ctx->cmd_set[i].pid, NULL, 0) < 0)
			return (ERROR);
		i++;
	}
	return (SUCCESS);
}
