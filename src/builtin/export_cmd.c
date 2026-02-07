/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export_cmd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:40:50 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/08 00:10:17 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"
#include "../../include/builtin.h"

/*
	export name[=word]...
	"export" register the args to env table, and update the ctx->exit_code.
	If no parameters, it displays exported vars.
*/
t_status	export_cmd(t_word_list *args, t_ctx *ctx)
{
}
