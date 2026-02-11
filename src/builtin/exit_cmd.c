/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:40:04 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/11 11:01:45 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/builtin.h"
#include "../../include/parser.h"

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
	arg_num = ft_strtol(args->wd->str);
	if (errno == ERANGE)
	{
		write(STDERR_FILENO, "exit", 4);
		ctx->err.exit_code = 2;
		return (ST_FATAL); // exit: (arg->wd->str): numeric argument reguired
	}
	write(STDOUT_FILENO, "exit", 4);
	ctx->err.exit_code = arg_num % 256;
	return (ST_EXIT);
}
