/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 19:33:34 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/07 22:55:01 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"
#include "../../include/builtin.h"

/*
	cd [directory]
	"cd" change current working directory, and update the ctx->exit_code.
	If no parameters, change the current working directory to $HOME.
*/
t_status	cd_cmd(t_word_list *args, t_ctx *ctx)
{
}
