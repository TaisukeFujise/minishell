/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:40:04 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/24 23:49:28 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"
#include "../../include/execute.h"
#include "../../include/parser.h"

bool		is_valid_number(char *str);

/*
	exit [n]
	"exit" exit the process with the args' number,
		and update the ctx->exit_code.
*/
t_status	exit_cmd(t_word_list *args, t_ctx *ctx)
{
	long	arg_num;

	if (args == NULL)
	{
		ctx->err.exit_code = 0;
		return (ST_EXIT);
	}
	if (count_args(args) > 2)
	{
		write(STDERR_FILENO, "exit", 4);
		ctx->err.exit_code = 1;
		return (ST_FAILURE); // exit: too many arguments
	}
	if (is_valid_number(args->wd->str) == false)
	{
		write(STDERR_FILENO, "exit", 4);
		ctx->err.exit_code = 2;
		return (ST_FATAL);
	}
	arg_num = ft_atol(args->wd->str);
	if (errno == ERANGE)
	{
		write(STDERR_FILENO, "exit", 4);
		ctx->err.exit_code = 2;
		return (ST_FATAL); // exit: (arg->wd->str): numeric argument reguired
	}
	write(STDOUT_FILENO, "exit", 4);
	ctx->err.exit_code = (((arg_num % 256) + 256) % 256);
	return (ST_EXIT);
}

bool	is_valid_number(char *str)
{
	int	i;

	i = 0;
	if (str[i] == '+' || str[i] == '-')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (false);
		i++;
	}
	return (true);
}
