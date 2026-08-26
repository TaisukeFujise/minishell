/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/25 19:40:50 by fendo             #+#    #+#             */
/*   Updated: 2026/08/25 19:40:51 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXPAND_H
# define EXPAND_H

# include "lexer.h"
# include "parser.h"

t_status	expand_command(t_node *node, t_ctx *ctx, t_arenas *arenas);

#endif
