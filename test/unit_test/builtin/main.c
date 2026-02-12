/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 14:31:54 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/12 22:46:47 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_test.h"
#include <stdio.h>

int	main(int argc, char **argv, char **envp)
{
	t_ctx				ctx;
	t_word_list			*args;
	t_word_list			*flag_args;
	t_bucket_contents	*item;

	/*
		Test case
		(i)   cd
		(ii)  echo
		(iii) env
		(iv)  exit
		(v)   export
		(vi)  pwd
		(vii) unset
	*/
	(void)argc;
	(void)argv;
	if (init_ctx(&ctx, envp) == FAILURE)
		return (1);
	// (i) cd
	write(1, "==== cd ====\n", 13);
	args = NULL;
	cd_cmd(args, &ctx);
	assert(ft_strcmp(getcwd(NULL, 0), "/home/tafujise") == 0);
	item = hash_search("PWD", ctx.env_table);
	printf("%s\n", item->data.value);
	assert(ft_strcmp(item->data.value, "/home/tafujise") == 0);
	item = hash_search("OLDPWD", ctx.env_table);
	printf("%s\n", item->data.value);
	assert(ft_strcmp(item->data.value, "/home/tafujise/Cursus/minishell") == 0);
	args = malloc(sizeof(t_word_list));
	args->wd = malloc(sizeof(t_word));
	args->wd->str = ft_strdup("Cursus");
	args->wd->len = 3;
	args->next = NULL;
	cd_cmd(args, &ctx);
	puts("pass2");
	puts(getcwd(NULL, 0));
	assert(ft_strcmp(getcwd(NULL, 0), "/home/tafujise/Cursus") == 0);
	puts("pass3");
	item = hash_search("PWD", ctx.env_table);
	puts("pass4");
	assert(ft_strcmp(item->data.value, "/home/tafujise/Cursus") == 0);
	puts("pass5");
	item = hash_search("OLDPWD", ctx.env_table);
	puts("pass6");
	assert(ft_strcmp(item->data.value, "/home/tafujise") == 0);
	puts("pass7");
	// (ii) echo
	write(1, "==== echo ====\n", 15);
	args = NULL;
	echo_cmd(args, &ctx);
	args = malloc(sizeof(t_word_list));
	args->wd->str = ft_strdup("abc");
	args->wd->len = 3;
	args->wd->next = NULL;
	echo_cmd(args, &ctx);
	flag_args = malloc(sizeof(t_word_list));
	flag_args->wd->str = ft_strdup("-n");
	flag_args->wd->len = 2;
	flag_args->next = args;
	echo_cmd(flag_args, &ctx);
	// (iii) env
	write(1, "==== env ====\n", 14);
	args = NULL;
	env_cmd(args, &ctx);
	// (iv) exit
	// write(1, "==== exit ====\n", 15);
	// (v) export
	// write(1, "==== export ====\n", 17);
	// (vi) pwd
	write(1, "==== pwd ====\n", 14);
	args = NULL;
	pwd_cmd(args, &ctx);
	// (vii) unset
	write(1, "==== unset ====\n", 16);
	args = NULL;
	unset_cmd(args, &ctx);
	args = malloc(sizeof(t_word_list));
	args->wd->str = ft_strdup("HOME");
	args->wd->len = 4;
	args->next = NULL;
	unset_cmd(args, &ctx);
}
