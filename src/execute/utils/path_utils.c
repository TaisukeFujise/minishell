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
#include <sys/stat.h>
#include "../../../include/parser.h"

/*
	Whether the name is a pathname rather than a name to look up in PATH.
	[POSIX.1-2024 XCU 2.9.1.1 Command Search and Execution]
*/
bool	has_slash(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '/')
			return (true);
		i++;
	}
	return (false);
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
	Whether the candidate is a file that could have been run at all.
	execve() answers EACCES for a directory as well as for a file that
	may not be run, and only the second is worth reporting: bash leaves
	anything that is not a regular file out of the search, so "cd .."
	as a command ends as not found and not as a refusal.
*/
static bool	is_regular(char *pathname)
{
	struct stat	info;

	if (stat(pathname, &info) < 0)
		return (false);
	return (S_ISREG(info.st_mode));
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
		if (errno != ENOENT && errno != ENOTDIR && is_regular(candidate))
			reason = errno;
		free(candidate);
	}
	return (reason);
}
