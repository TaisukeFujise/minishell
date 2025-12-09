/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 14:33:46 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/08 06:12:30 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	main(int argc, char **argv, char **envp)
{
	t_ctx	ctx;
	int		exit_code;

	ft_bzero(&ctx, sizeof(t_ctx));
	if (parse_args(argc, argv, envp, &ctx) == ERROR)
		return (free_ctx(&ctx), 1);
	exit_code = run_pipeline(&ctx);
	return (free_ctx(&ctx), exit_code);
}
