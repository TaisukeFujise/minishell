/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 21:37:42 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/18 22:52:17 by tafujise         ###   ########.fr       */
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

