/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/17 14:00:00 by fendo             #+#    #+#             */
/*   Updated: 2025/10/31 15:49:34 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 1024
# endif

# include <fcntl.h>
# include <stdlib.h>
# include <stdint.h>
# include <unistd.h>

char	*get_next_line(int fd);
void	*ft_calloc_gnl(size_t count, size_t size);
char	*ft_strchr_gnl(const char *s, int c);
char	*ft_strndup_gnl(const char *s1, size_t n);
char	*ft_strjoin_gnl(char *s1, const char *s2);
size_t	ft_strlen_gnl(const char *s);

#endif