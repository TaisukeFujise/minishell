/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:34:37 by tafujise          #+#    #+#             */
/*   Updated: 2026/07/31 01:23:27 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

# include "minishell.h"
# include <errno.h>

typedef struct s_export_arg
{
	t_word	*word;
	size_t	key_len;
	size_t	value_pos;
	bool	append;
}	t_export_arg;

typedef t_status	(*t_builtin)(t_word_list *args, t_ctx *ctx);

t_builtin	find_builtin(char *name);
t_status	builtin_cmd(t_word_list *args, t_ctx *ctx);

t_status	cd_cmd(t_word_list *args, t_ctx *ctx);
t_status	echo_cmd(t_word_list *args, t_ctx *ctx);
t_status	env_cmd(t_word_list *args, t_ctx *ctx);
t_status	exit_cmd(t_word_list *args, t_ctx *ctx);
t_status	export_cmd(t_word_list *args, t_ctx *ctx);
t_status	pwd_cmd(t_word_list *args, t_ctx *ctx);
t_status	unset_cmd(t_word_list *args, t_ctx *ctx);

#endif
