/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd_cmd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:41:22 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/11 11:01:53 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"

// #include "../../include/builtin.h"

/*
	pwd
	"pwd" invoke getcwd(), and update the ctx->exit_code.
	It returns always physical path. It's different from "pwd" in bash.
*/
t_status	pwd_cmd(t_word_list *args, t_ctx *ctx)
{
	char	*path;

	if (args != NULL)
		return (ST_FAILURE); // pwd: too many arguments.
	path = getcwd(NULL, 0);
	if (path == NULL)
		return (ST_FAILURE);
	printf("$s\n", path);
}
