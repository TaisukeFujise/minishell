/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/03 08:40:17 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/03 17:28:26 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "execute_test.h"

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
	node.node_kind = NODE_SIMPLE;
	node.left = NULL;
	node.right = NULL;

	// (i) tmp=42
	node.u_node.simple_command.args = NULL;
	node.u_node.simple_command.redirects = NULL;

	node.u_node.simple_command.assigns->key->str = "tmp";
	node.u_node.simple_command.assigns->value->str = "42";
	node.u_node.simple_command.assigns->next = NULL;
	if (exec_null_command(&node.u_node.simple_command, &ctx, NO_PIPE, NO_PIPE) == ST_FATAL)
		return (1);

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
