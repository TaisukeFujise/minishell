/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/02 20:40:52 by tafujise          #+#    #+#             */
/*   Updated: 2026/05/10 21:57:07 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include "./hashmap.h"
# include <signal.h>
# include <stddef.h>
# include <stdint.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define SUCCESS 0
# define FAILURE -1

# define NO_PIPE -1

extern volatile sig_atomic_t	g_signum;

typedef struct s_fd_bitmap
{
	int							size;
	char						*bitmap;
}								t_fd_bitmap;

typedef struct s_arenas
{
	t_arena						ast;
	t_arena						tmp;
	t_arena						heredoc;
}								t_arenas;

typedef struct s_error
{
	int							exit_code;
	char						*err_msg;
}								t_error;

typedef struct s_ctx
{
	t_error						err;
	t_hashtable					*env_table;
	// environment variable table. When execve,this is converted to envp.
	/* Reset following member on every command. */
	t_hashtable					*tmp_table;
	// tmp environment variable table.This is set by assignment word in front of cmd.
	t_fd_bitmap					*bitmap;
	// It's for managing fd,especially pipe read end fd,when using pipe.
	pid_t						*pids;
	int							npid;
	int							already_forked;
	int							subshell_level;
}								t_ctx;

typedef enum e_status
{
	ST_OK,
	ST_FAILURE,
	ST_EXIT,
	ST_FATAL,
}								t_status;

typedef enum e_op_connect
{
	CONNECT_NONE,
	CONNECT_AND_IF,
	CONNECT_OR_IF,
	CONNECT_PIPE
}								t_op_connect;

typedef enum e_op_group
{
	GROUP_RPAREN,
	GROUP_LPAREN
}								t_op_group;

typedef enum e_op_redir
{
	REDIR_LESS,
	REDIR_GREATER,
	REDIR_DLESS,
	REDIR_DGREATER
}								t_op_redir;

typedef enum e_flag
{
	W_NONE = 0u,
	W_SQ = 1u << 0,
	W_DQ = 1u << 1,
	W_DOLL = 1u << 2,
	W_WILD = 1u << 3,
	W_ASSIGN = 1u << 4,
	W_APPEND = 1u << 5,
	W_ID = 1u << 6
}								t_flag;

typedef struct s_word			t_word;

struct							s_word
{
	char						*str;
	int							len;
	char						*eq_ptr;
	uint8_t						flag;
	t_word						*next;
};

typedef struct s_assign			t_assign;

struct							s_assign
{
	t_word						*key;
	t_word						*value;
	t_assign					*next;
};

typedef struct s_word_list		t_word_list;

struct							s_word_list
{
	t_word						*wd;
	t_word_list					*next;
};

#endif
