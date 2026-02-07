/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 20:34:37 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/08 00:14:36 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_H
# define BUILTIN_H

t_status	cd_cmd(t_word_list *args, t_ctx *ctx);
t_status	echo_cmd(t_word_list *args, t_ctx *ctx);
t_status	env_cmd(t_word_list *args, t_ctx *ctx);
t_status	exit_cmd(t_word_list *args, t_ctx *ctx);
t_status	export_cmd(t_word_list *args, t_ctx *ctx);
t_status	pwd_cmd(t_word_list *args, t_ctx *ctx);
t_status	unset_cmd(t_word_list *args, t_ctx *ctx);

#endif BUILTIN_H
