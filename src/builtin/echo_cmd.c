/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo_cmd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:35:43 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/08 00:16:32 by tafujise         ###   ########.fr       */
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
}
