/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_internal.h                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fendo <fendo@student.42tokyo.jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/04 18:24:12 by fendo             #+#    #+#             */
/*   Updated: 2026/03/03 00:41:20 by fendo            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PARSER_INTERNAL_H
# define PARSER_INTERNAL_H

# include "../../../include/parser.h"
# include "../../../include/lexer.h"
# include <stddef.h>
# include <readline/readline.h>

typedef struct s_hd_item
{
	t_redirect			*redir;
	struct s_hd_item	*next;
}	t_hd_item;

typedef struct s_parser_state
{
	t_lex_state	lex;
	t_token		lookahead;
	t_token		eof_sentinel;
	t_hd_item	*hd_head;
	t_hd_item	*hd_tail;
	int			hd_count;
	t_status	status;
	t_ctx		*ctx;
	t_arenas	*arenas;
}	t_parser_state;

# define SKIP_PLAIN 0
# define SKIP_AND_COLLECT 1
# define SKIP_ONE 2

/* parser_error.c */
void		parser_fail(t_parser_state *ps, t_status status, char *msg);
char		*unexpected_token_msg(t_parser_state *ps, t_token *tk);
char		*hd_eof_warn_msg(t_parser_state *ps, char *delim);
char		*lex_err_msg(t_lexer_err err);

/* parser_stream.c */
t_token		*peek(t_parser_state *ps);
t_token		consume(t_parser_state *ps);
int			skip_newline(t_parser_state *ps, int mode);

/* parser_nodes.c */
void		ilist_append(void *headp, void *item, size_t next_off);
t_node		*new_node(t_parser_state *ps, t_node_kind kind);
t_node		*make_binary(t_parser_state *ps, t_op_connect op,
				t_node *left, t_node *right);

/* parse_command.c */
t_node		*parse_simple(t_parser_state *ps);
t_node		*parse_subshell(t_parser_state *ps);

/* parse_connective.c */
t_node		*parse_andor(t_parser_state *ps);
t_node		*parse_compound_list(t_parser_state *ps);

/* parser_assign.c */
t_assign	*word_to_assign(t_parser_state *ps, t_word *word);

/* parser_heredoc.c */
void		collect_one_heredoc(t_parser_state *ps, t_redirect *redir);

/* parser_redirect.c */
void		collect_heredocs(t_parser_state *ps);
void		register_heredoc(t_parser_state *ps, t_redirect *redir);
void		add_redir(t_parser_state *ps, t_redirect **list);

#endif
