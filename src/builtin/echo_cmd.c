/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:35:43 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/08 19:31:30 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"
#include "../../include/builtin.h"

/*
	echo [-n] [string...]
	"echo" display strings, and update the ctx->exit_code
	-n : do not output the trailing newline
*/
t_status	echo_cmd(t_word_list *args, t_ctx *ctx)
{
	bool	nflag;

	(void)ctx;
	nflag = false;
	if (args == NULL)
		return (write(STDOUT_FILENO, "\n", 1), ST_OK);
	if (ft_strcmp(args->wd->str, "-n") == 0)
	{
		nflag = true;
		args = args->next;
	}
	while (args)
	{
		write(STDOUT_FILENO, args->wd->str, ft_strlen(args->wd->str));
		if (args->next != NULL)
			write(STDOUT_FILENO, " ", 1);
		args = args->next;
	}
	if (nflag == false)
		write(STDOUT_FILENO, "\n", 1);
	return (ST_OK);
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
// 	args->wd->str = ft_strdup("abc");
// 	args2 = malloc(sizeof(t_word_list));
// 	args2->wd = malloc(sizeof(t_word));
// 	args2->wd->str = ft_strdup("efg");
// 	args->next = args2;
// 	args2->next = NULL;
// 	cd_cmd(args, ctx);
// }
