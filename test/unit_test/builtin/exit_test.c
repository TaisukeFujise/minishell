/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_test.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 00:22:57 by tafujise          #+#    #+#             */
/*   Updated: 2026/05/10 00:00:00 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_test.h"

static int	g_ng;

static void	check_exit(char **args_strs, t_status want_st,
				int want_code, char *name)
{
	t_ctx		ctx;
	t_word_list	*args;
	char		buf[64];
	int			ok;

	ft_bzero(&ctx, sizeof(ctx));
	args = make_args(args_strs);
	ok = (capture(buf, sizeof(buf), exit_cmd, args, &ctx) == want_st
			&& ctx.err.exit_code == want_code);
	if (ok)
		printf("[OK] %s\n", name);
	else
	{
		printf("[NG] %s\n", name);
		g_ng++;
	}
	free_args(args);
}

int	test_exit(void)
{
	g_ng = 0;

	check_exit(NULL, ST_EXIT, 0, "exit: no args");
	check_exit((char *[]){"0", NULL}, ST_EXIT, 0, "exit: 0");
	check_exit((char *[]){"42", NULL}, ST_EXIT, 42, "exit: 42");
	check_exit((char *[]){"256", NULL}, ST_EXIT, 0, "exit: 256 mod");
	check_exit((char *[]){"-1", NULL}, ST_EXIT, 255, "exit: -1");
	check_exit((char *[]){"abc", NULL}, ST_FATAL, 2, "exit: invalid str");
	check_exit((char *[]){"1", "2", "3", NULL}, ST_FAILURE, 1, "exit: too many");

	return (g_ng);
}
