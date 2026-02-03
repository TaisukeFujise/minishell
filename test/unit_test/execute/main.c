/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:40:17 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/03 21:52:41 by tafujise         ###   ########.fr       */
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
		// printf("i: %d\n", i);
		if (item == NULL)
		{
			i++;
			continue;
		}
		printf("value: %s\n", item->data.value);
		printf("exported: %d\n", item->data.exported);
		i++;
	}
}

void	print_ctx(t_ctx ctx)
{
	printf("exit_code: %d\n", ctx.exit_code);
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
	t_node	node;

	(void)argc;
	(void)argv;
	if (init_ctx(&ctx, envp) == FAILURE)
		return (1);
	// print_ctx(ctx);
	node.node_kind = NODE_SIMPLE;
	node.left = NULL;
	node.right = NULL;

	// (i) tmp=42
	node.u_node.simple_command.args = NULL;
	node.u_node.simple_command.redirects = NULL;

	node.u_node.simple_command.assigns = malloc(sizeof(t_assign) * 1);
	node.u_node.simple_command.assigns->key = malloc(sizeof(t_word) * 1);
	node.u_node.simple_command.assigns->value = malloc(sizeof(t_word) * 1);
	node.u_node.simple_command.assigns->key->str = ft_strdup("temp");
	node.u_node.simple_command.assigns->value->str = ft_strdup("42");
	node.u_node.simple_command.assigns->next = NULL;
	if (exec_null_command(&node.u_node.simple_command, &ctx, NO_PIPE, NO_PIPE) == ST_FATAL)
		return (1);
	print_table(ctx.env_table);
	return (0);
	// (ii) A=42 B=20 C=10
	// node.u_node.simple_command.args = NULL;
	// node.u_node.simple_command.redirects = NULL;

	// node.u_node.simple_command.assigns->key->str = "A";
	// node.u_node.simple_command.assigns->value->str = "42";


	// (iii) export tmp=10; tmp=42
	// node.u_node.simple_command.args = NULL;
	// node.u_node.simple_command.redirects = NULL;
	// node.u_node.simple_command.assigns->key->str = "tmp";
	// node.u_node.simple_command.assigns->value->str = "42";
	// node.u_node.simple_command.assigns->next = NULL;

	// // (iv) >file
	// node.u_node.simple_command.args = NULL;
	// node.u_node.simple_command.assigns = NULL;

	// // (v) >>file
	// node.u_node.simple_command.args = NULL;
	// node.u_node.simple_command.assigns = NULL;

	// // (vi) <file
	// node.u_node.simple_command.args = NULL;
	// node.u_node.simple_command.assigns = NULL;

	// // (vii) <<file
	// node.u_node.simple_command.args = NULL;
	// node.u_node.simple_command.assigns = NULL;

	// // (viii) assigns + redirect
	// node.u_node.simple_command.args = NULL;

	// node.u_node.simple_command.assigns->key->str = "tmp";
	// node.u_node.simple_command.assigns->value->str = "42";
	// node.u_node.simple_command.assigns->next = NULL;

	// node.u_node.simple_command.redirects
}
