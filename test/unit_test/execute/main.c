/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:40:17 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/08 17:37:22 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute_test.h"
#include <stdio.h>

// int	print_contents(t_bucket_contents *item)
// {
// 	puts("data");
// 	printf("value: %s\n", item->data->value);
// 	printf("exported: %d\n", item->data->exported);
// 	return (0);
// }

void	print_table(t_hashtable *table)
{
	t_bucket_contents	*item;
	puts("==table==");
	// hash_walk(table, print_contents);
	int	i;

	i = 0;
	printf("table->bucket_size: %d\n", table->bucket_size);
	while (i < table->bucket_size)
	{
		item = hash_items(i, table);
		if (item == NULL)
		{
			i++;
			continue;
		}
		puts("------------");
		printf("key: %s\n", item->key);
		printf("value: %s\n", item->data.value);
		printf("exported: %d\n", item->data.exported);
		i++;
	}
	puts("-----------");
}

void	print_ctx(t_ctx ctx)
{
	printf("exit_code: %d\n", ctx.err.exit_code);
	print_table(ctx.env_table);
	// print_table(ctx.tmp_table);
	// print_bitmap(ctx.bitmap);
	// print_pids(ctx.pids);
	printf("npid: %d\n", ctx.npid);
	printf("already_forked: %d\n", ctx.already_forked);
}

int	main(int argc, char **argv, char **envp)
{
	/*
		Test case
		- assigns only
		- (i)   assigns a word
		- (ii)  assigns multiple words
		- (iii) overwrite the env variable

		- redirect only
		- (iv)  >file
		- (v)   >>file
		- (vi)  < file
		- (vii) <<file

		- assigns + redirect
		- (viii) tmp=42 > file
	*/
	t_ctx	ctx;

	(void)argc;
	(void)argv;
	if (init_ctx(&ctx, envp) == FAILURE)
		return (1);
	// print_ctx(ctx);


	// (i) tmp=42
	t_simple_cmd	cmd1;
	cmd1.args = NULL;
	cmd1.redirects = NULL;

	cmd1.assigns = malloc(sizeof(t_assign) * 1);
	cmd1.assigns->key = malloc(sizeof(t_word) * 1);
	cmd1.assigns->value = malloc(sizeof(t_word) * 1);
	cmd1.assigns->key->str = ft_strdup("temp");
	cmd1.assigns->value->str = ft_strdup("42");
	cmd1.assigns->next = NULL;
	// if (exec_null_command(&cmd1, &ctx, NO_PIPE, NO_PIPE) == ST_FATAL)
	// 	return (1);

	// print_table(ctx.env_table);
	// (ii) A=42 B=20 C=10
	t_simple_cmd	cmd2;
	t_assign		assign_A;
	t_assign		assign_B;
	t_assign		assign_C;

	cmd2.args = NULL;
	cmd2.redirects = NULL;

	cmd2.assigns = &assign_A;
	assign_A.key = malloc(sizeof(t_word) * 1);
	assign_A.value = malloc(sizeof(t_word) * 1);
	assign_A.key->str = ft_strdup("A");
	assign_A.value->str = ft_strdup("42");
	assign_A.next = &assign_B;

	assign_B.key = malloc(sizeof(t_word) * 1);
	assign_B.value = malloc(sizeof(t_word) * 1);
	assign_B.key->str = ft_strdup("B");
	assign_B.value->str = ft_strdup("20");
	assign_B.next = &assign_C;

	assign_C.key = malloc(sizeof(t_word) * 1);
	assign_C.value = malloc(sizeof(t_word) * 1);
	assign_C.key->str = ft_strdup("C");
	assign_C.value->str = ft_strdup("10");
	assign_C.next = NULL;


	// if (exec_null_command(&cmd2, &ctx, NO_PIPE, NO_PIPE) == ST_FATAL)
	// 	return (1);
	// print_table(ctx.env_table);
	// return (0);

	// (iii) export tmp=10; tmp=42
	// cmd1.args = NULL;
	// cmd1.redirects = NULL;
	// cmd1.assigns->key->str = "tmp";
	// cmd1.assigns->value->str = "42";
	// cmd1.assigns->next = NULL;

	// (iv) >file
	t_simple_cmd	cmd4;
	cmd4.args = NULL;
	cmd4.assigns = NULL;

	cmd4.redirects = malloc(sizeof(t_redirect));
	cmd4.redirects->op = REDIR_GREATER;
	cmd4.redirects->target.str = ft_strdup("test_iv.txt");
	cmd4.redirects->io_number = 1;
	cmd4.redirects->next = NULL;
	// if (exec_null_command(&cmd4, &ctx, NO_PIPE, NO_PIPE) == ST_FATAL)
	// 	return (1);
	// return (0);

	// // (v) >>file
	t_simple_cmd	cmd5;
	cmd5.args = NULL;
	cmd5.assigns = NULL;

	cmd5.redirects = malloc(sizeof(t_redirect));
	cmd5.redirects->op = REDIR_DGREATER;
	cmd5.redirects->target.str = ft_strdup("test_v.txt");
	cmd5.redirects->io_number = 1;
	cmd5.redirects->next = NULL;
	// if (exec_null_command(&cmd5, &ctx, NO_PIPE, NO_PIPE) == ST_FATAL)
	// 	return (1);
	// return (0);

	// // (vi) <file
	t_simple_cmd	cmd6;
	cmd6.args = NULL;
	cmd6.assigns = NULL;

	cmd6.redirects = malloc(sizeof(t_redirect));
	cmd6.redirects->op = REDIR_LESS;
	cmd6.redirects->target.str = ft_strdup("test_vi.txt");
	cmd6.redirects->io_number = 0;
	cmd6.redirects->next = NULL;
	if (exec_null_command(&cmd6, &ctx, NO_PIPE, NO_PIPE) == ST_FATAL)
		return (1);
	return (0);

	// // (vii) <<file
	t_simple_cmd	cmd7;
	cmd7.args = NULL;
	cmd7.assigns = NULL;

	cmd7.redirects = malloc(sizeof(t_redirect));
	cmd7.redirects->op = REDIR_DLESS;
	cmd7.redirects->target.str = ft_strdup("test_vii.txt");
	cmd7.redirects->io_number = 0;
	cmd7.redirects->next = NULL;
	// if (exec_null_command(&cmd4, &ctx, NO_PIPE, NO_PIPE) == ST_FATAL)
	// 	return (1);
	// return (0);

	// // (viii) assigns + redirect
	// cmd1.args = NULL;

	// cmd1.assigns->key->str = "tmp";
	// cmd1.assigns->value->str = "42";
	// cmd1.assigns->next = NULL;

	// cmd1.redirects
}
