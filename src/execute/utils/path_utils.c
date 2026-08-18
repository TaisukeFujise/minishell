/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   path_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/15 02:39:44 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/16 01:15:32 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/execute.h"
#include "../../../include/parser.h"

char	*extract_path_value(t_hashtable *tmp_table, t_hashtable *env_table)
{
	return (env_lookup(tmp_table, env_table, "PATH"));
}

/*
	The next candidate pathname for name, from the PATH value at *scan.
	*scan moves past the element that was used, and becomes NULL when the
	value is exhausted. An empty element means the current directory.
	[POSIX.1-2024 XCU 2.9.1.1 Command Search and Execution]
*/
char	*next_path_candidate(char **scan, char *name)
{
	char	*end;
	char	*dir;

	end = ft_strchrnul(*scan, ':');
	dir = ft_substr(*scan, 0, (size_t)(end - *scan));
	if (*end == ':')
		*scan = end + 1;
	else
		*scan = NULL;
	if (dir == NULL)
		return (NULL);
	if (*dir == '\0')
	{
		free(dir);
		dir = ft_strdup("./");
	}
	else if (dir[ft_strlen(dir) - 1] != '/')
		dir = ft_strjoin_free(dir, "/", 1 << 0);
	return (ft_strjoin_free(dir, name, 1 << 0));
}

/*
	Set _ to the pathname the command is about to become, in the
	environment that command gets. Without this a command reads the value
	the shell itself was started with. [review D37-21]
	build_envp() leaves one free slot for this entry.
*/
void	set_underscore(char **envp, char *pathname)
{
	char	*entry;
	int		i;

	entry = make_env_entry("_", pathname);
	if (entry == NULL)
		return ;
	i = 0;
	while (envp[i] != NULL && (envp[i][0] != '_' || envp[i][1] != '='))
		i++;
	if (envp[i] == NULL)
		envp[i + 1] = NULL;
	else
		free(envp[i]);
	envp[i] = entry;
}

/*
	Try every candidate in PATH. A candidate that is simply not there is
	not worth reporting, so keep the reason of one that was there and
	still could not run. Returns 0 when nothing was found at all.
	[dash shellexec()]
*/
int	search_path(char *path, char **argv, char **envp)
{
	char	*candidate;
	int		reason;

	reason = 0;
	while (path != NULL)
	{
		candidate = next_path_candidate(&path, argv[0]);
		if (candidate == NULL)
			return (ENOMEM);
		set_underscore(envp, candidate);
		execve(candidate, argv, envp);
		if (errno != ENOENT && errno != ENOTDIR)
			reason = errno;
		free(candidate);
	}
	return (reason);
}
