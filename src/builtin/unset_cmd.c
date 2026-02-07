/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset_cmd.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:41:53 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/08 00:10:34 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"
#include "../../include/builtin.h"

/*
	unset name...
	"unset" unset the args from env table, and update the ctx->exit_code.
	If no parameters, it does nothing.
*/
t_status	unset_cmd(t_word_list *args, t_ctx *ctx)
{
}
