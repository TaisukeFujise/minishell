/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minitalk.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 15:25:07 by fendo             #+#    #+#             */
/*   Updated: 2025/12/02 02:22:27 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINITALK_H
# define MINITALK_H

# include <signal.h>
# include <unistd.h>
# include <stdlib.h>
# include <limits.h>
# include <sys/types.h>
# include <errno.h>
# include "../ft_printf/include/ft_printf.h"
# include "../ft_printf/libft/libft.h"

# define PID_MAX 4194303
# define PID_MIN 300

# define TIMEOUT_MULTIPLIER_NUM 3
# define TIMEOUT_MULTIPLIER_DEN 2
# define MAX_RETRIES 32
# define BASE_TIMEOUT 5000
# define TICK 50
# define MAX_TIMEOUT 1000000

typedef struct s_server
{
	unsigned char	c;
	int				bit_count;
	pid_t			client_pid;
}	t_server;

typedef struct s_client
{
	volatile sig_atomic_t	ack_received;
	pid_t					server_pid;
}	t_client;

void	error_exit(char *msg);
void	handle_kill_error(char *context);
pid_t	parse_pid(char *str);

#endif
