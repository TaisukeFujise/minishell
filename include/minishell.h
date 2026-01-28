/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 20:40:52 by tafujise          #+#    #+#             */
/*   Updated: 2026/01/28 22:32:53 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <signal.h>
# include "../libft/libft.h"
# include <stddef.h>
# include <stdint.h>
# include "./hashmap.h"
# include <sys/types.h>

# define SUCCESS 0
# define FAILURE -1

# define NO_PIPE -1

extern volatile sig_atomic_t	g_signum;

typedef struct s_fd_bitmap
{
	int		size;
	char	*bitmap;
}	t_fd_bitmap;

typedef struct s_entry_view
{
	char	*key;
	int		key_len;
	char	*value;
	int		value_len;
}	t_entry_view;

typedef struct s_ctx
{

	int			exit_code;
	t_hashtable	*env_table;//environment variable table. When execve, this is converted to envp.
	t_hashtable	*var_table;//shell variable table
	// Reset following member on every command.
	t_hashtable	*tmp_table;//tmp environment variable table. This is set by assignment word in front of cmd.
	t_fd_bitmap	*bitmap;// It's for managing fd, especially pipe read end fd, when using pipe.
	pid_t		*pids;// Array of pids.
	int			npid;// Coutn of pids.
	int			in_child;// Flag about whether already forked or not.
}	t_ctx;

typedef enum e_status
{
	ST_OK,
	ST_EXIT,
	ST_FATAL,
}	t_status;

typedef enum e_op_connect
{
	CONNECT_NONE,
	CONNECT_AND_IF,
	CONNECT_OR_IF,
	CONNECT_PIPE
}	t_op_connect;

typedef enum e_op_group
{
	GROUP_RPAREN,
	GROUP_LPAREN
}	t_op_group;

typedef enum e_op_redir
{
	REDIR_LESS,
	REDIR_GREAT,
	REDIR_DLESS,
	REDIR_DGREAT
}	t_op_redir;

typedef enum e_flag
{
	W_NONE		= 0u,
	W_SQ		= 1u << 0,
	W_DQ		= 1u << 1,
	W_DOLL		= 1u << 2,
	W_WILD		= 1u << 3,
	W_ASSIGN	= 1u << 4
}	t_flag;

typedef struct s_word
{
	char	*str;
	int		len;
}	t_word;

typedef struct s_word_desc
{
	t_word		word;
	uint8_t		flag;
	char		*eq_ptr;
}	t_word_desc;

typedef struct s_word_list	t_word_list;

struct s_word_list
{
	t_word_desc	wd;
	t_word_list	*next;
};

#endif
