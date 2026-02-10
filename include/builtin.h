/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:34:37 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/11 05:46:55 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

#include <errno.h>
#include "minishell.h"

t_status	cd_cmd(t_word_list *args, t_ctx *ctx);
t_status	echo_cmd(t_word_list *args, t_ctx *ctx);
t_status	env_cmd(t_word_list *args, t_ctx *ctx);
t_status	exit_cmd(t_word_list *args, t_ctx *ctx);
t_status	export_cmd(t_word_list *args, t_ctx *ctx);
t_status	pwd_cmd(t_word_list *args, t_ctx *ctx);
t_status	unset_cmd(t_word_list *args, t_ctx *ctx);
int			count_args(t_word_list *args);
void		print_env(t_hashtable *tmp_table, t_hashtable *env_table);

#endif BUILTIN_H
