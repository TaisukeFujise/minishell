#ifndef EXPAND_H
# define EXPAND_H

# include "lexer.h"
# include "parser.h"

t_status	expand_command(t_node *node, t_ctx *ctx, t_arenas *arenas);

#endif
