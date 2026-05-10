/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_test.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/05/09 00:22:57 by tafujise          #+#    #+#             */
/*   Updated: 2026/05/10 00:00:00 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_test.h"
#include <string.h>

static int	g_ng;

int	test_env(char **envp)
{
	t_ctx		ctx;
	char		buf[4096];
	t_word_list	*args;

	g_ng = 0;
	if (setup_ctx(&ctx, envp))
		return (1);
	CHECK(capture(buf, sizeof(buf), env_cmd, NULL, &ctx) == ST_OK && strstr(buf,
			"PATH=") != NULL, "env: no args");
	args = make_args((char *[]){"x", NULL});
	CHECK(capture(buf, sizeof(buf), env_cmd, args, &ctx) == ST_FAILURE,
		"env: with args");
	free_args(args);
	teardown_ctx(&ctx);
	return (g_ng);
}
