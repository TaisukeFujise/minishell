/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handle.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42.jp>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 21:37:42 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/20 22:19:33 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef HANDLE_H
# define HANDLE_H

# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

int	set_signal(void);
int	handle_readline_signal(void);

#endif
