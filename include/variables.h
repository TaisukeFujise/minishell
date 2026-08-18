#ifndef VARIABLES_H
# define VARIABLES_H

# include "hashmap.h"
# include "minishell.h"
# include "parser.h"

/*
	Where an assignment goes. TMP is the environment of the command being
	run, which is exported to it and thrown away afterwards. VARS is the
	table the shell keeps, where the exported flag is left alone.
*/
typedef enum s_tabletype
{
	TMP,
	VARS,
}			t_tabletype;

/* env_table.c */
char		*env_lookup(t_hashtable *tmp_table, t_hashtable *env_table,
				char *name);
char		*make_env_entry(char *key, char *value);
char		**build_envp(t_hashtable *tmp_table, t_hashtable *env_table);
void		free_envp(char **envp);
/* assign.c */
t_status	apply_assign(t_assign *assign, t_hashtable *table, t_ctx *ctx,
				t_tabletype type);

#endif
