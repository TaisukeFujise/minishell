/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:35:43 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/11 11:01:29 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"

// #include "../../include/builtin.h"

static void	_consume_n_flag(t_word_list **args, bool *nflag);

/*
	echo [-n] [string...]
	"echo" display strings, and update the ctx->exit_code
	-n : do not output the trailing newline
*/
t_status	echo_cmd(t_word_list *args, t_ctx *ctx)
{
	bool	n_flag;

	(void)ctx;
	n_flag = false;
	if (args == NULL)
	{
		if (write(STDOUT_FILENO, "\n", 1) < 0)
			return (ST_FAILURE);
		return (ST_OK);
	}
	_consume_n_flag(&args, &n_flag);
	while (args)
	{
		if (write(STDOUT_FILENO, args->wd->str, ft_strlen(args->wd->str)) < 0)
			return (ST_FAILURE);
		if (args->next != NULL)
			if (write(STDOUT_FILENO, " ", 1) < 0)
				return (ST_FAILURE);
		args = args->next;
	}
	if (!n_flag)
		if (write(STDOUT_FILENO, "\n", 1) < 0)
			return (ST_FAILURE);
	return (ST_OK);
}

static void	_consume_n_flag(t_word_list **args, bool *nflag)
{
	if (ft_strcmp((*args)->wd->str, "-n") == 0)
	{
		*nflag = true;
		*args = (*args)->next;
	}
}

// #include <stdio.h>
// int	main(void)
// {
// 	t_ctx		*ctx;
// 	t_word_list	*args;
// 	t_word_list	*args2;

// 	ctx = NULL;
// 	args = malloc(sizeof(t_word_list));
// 	args->wd = malloc(sizeof(t_word));
// 	args->wd->str = ft_strdup("-n");
// 	args2 = malloc(sizeof(t_word_list));
// 	args2->wd = malloc(sizeof(t_word));
// 	args2->wd->str = ft_strdup("efg");
// 	args->next = args2;
// 	args2->next = NULL;
// 	echo_cmd(args, ctx);
// }
