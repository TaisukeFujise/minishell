/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/20 20:44:29 by fendo             #+#    #+#             */
/*   Updated: 2025/12/25 00:08:46 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stddef.h>

void			skip_blank(char **line);
unsigned int	str2fd(char **line);
char			*strchunk(const char *tbl, const char *str, size_t n);

#endif