/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/28 14:50:42 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/08 06:56:16 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

int	parse_args(int argc, char **argv, char **envp, t_ctx *ctx)
{
	int	i;

	if (argc < 5)
		return (ft_putstr_fd("Usage: ./pipex infile cmd1 cmd2 \
				... cmdn outfile\n", 2), ERROR);
	ctx->input_file = argv[1];
	ctx->output_file = argv[argc - 1];
	ctx->cmd_sum = (argc - 1) - 2;
	ctx->cmd_set = malloc(sizeof(t_cmd) * (ctx->cmd_sum));
	if (ctx->cmd_set == NULL)
		return (perror("malloc"), ERROR);
	i = 0;
	while (i < ctx->cmd_sum)
	{
		ctx->cmd_set[i].cmd = ft_split(argv[i + 2], ' ');
		if (ctx->cmd_set[i].cmd == NULL)
			return (perror("split"), ERROR);
		i++;
	}
	ctx->envp = envp;
	return (SUCCESS);
}

char	**parse_path(char **envp)
{
	int		i;
	char	*path_base;
	char	**path;

	if ((envp == NULL) | (*envp == NULL))
		return (NULL);
	i = 0;
	while (envp[i] != NULL)
	{
		if (ft_strncmp(envp[i], "PATH=", 5) == 0)
		{
			path_base = envp[i] + 5;
			break ;
		}
		i++;
	}
	if (envp[i] == NULL)
		return (ft_putstr_fd("Error: envp is NULL\n", 2), NULL);
	path = ft_split(path_base, ':');
	if (path == NULL)
		return (perror("split"), NULL);
	return (path);
}
