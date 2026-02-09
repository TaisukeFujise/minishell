/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd_cmd.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 19:33:34 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/09 23:37:10 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/parser.h"
#include "../../include/builtin.h"

/*
	cd [directory]
	"cd" change current working directory, and update the ctx->exit_code.
	If no parameters, change the current working directory to $HOME.
	- 1. no directory operand with no HOME environment
	- 2. no directory operand with HOME environment
	- 3. directory operand begins with a <slash> character
	- 4. the first component of the directory operand is dot or dot-dot
	<reference>
	[ https://pubs.opengroup.org/onlinepubs/9699919799/utilities/cd.html ]
*/
t_status	cd_cmd(t_word_list *args, t_ctx *ctx)
{
	(void)ctx;
	
}
