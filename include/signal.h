/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/18 21:37:42 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/18 22:23:27 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#ifndef SIGNAL_H
# define SIGNAL_H

# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

int	set_signal(void);
int	handle_readline_signal(void);

#endif

