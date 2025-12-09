/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/01 15:25:23 by fendo             #+#    #+#             */
/*   Updated: 2025/12/01 19:22:41 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minitalk.h"

void	error_exit(char *msg)
{
	ft_dprintf(2, "%s\n", msg);
	exit(EXIT_FAILURE);
}

void	handle_kill_error(char *context)
{
	if (errno == ESRCH)
		error_exit("Error: Process not found");
	else if (errno == EPERM)
		error_exit("Error: Permission denied");
	else if (errno == EINVAL)
		error_exit("Error: Invalid signal");
	else
	{
		ft_dprintf(2, "Error: %s failed\n", context);
		exit(EXIT_FAILURE);
	}
}

static int	is_valid_pid_string(char *str)
{
	if (!str || !*str)
		return (0);
	while (*str)
	{
		if (!ft_isdigit(*str))
			return (0);
		str++;
	}
	return (1);
}

static pid_t	string_to_pid(char *str)
{
	long long	pid;

	pid = 0;
	while (*str)
	{
		pid = pid * 10 + (*str - '0');
		if (pid > PID_MAX)
			return (-1);
		str++;
	}
	if (pid < PID_MIN)
		return (-1);
	return ((pid_t)pid);
}

pid_t	parse_pid(char *str)
{
	pid_t	pid;

	if (!is_valid_pid_string(str))
		error_exit("Error: Invalid PID format");
	pid = string_to_pid(str);
	if (pid < 0)
		error_exit("Error: Invalid PID");
	return (pid);
}
