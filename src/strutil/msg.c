#include "../../include/minishell.h"
#include <unistd.h>

/*
	Write all of it. A write can stop short, and a signal can cut it off
	before anything is written, so neither ends the attempt.
	The output of the shell does not go through stdio: a buffer that
	survives a fork or a restored redirect writes to the wrong place.
	[EXECUTOR_INCREMENTAL_REFACTOR_PLAN 11, review D37-23]
*/
bool	write_all(int fd, const char *s, size_t len)
{
	ssize_t	written;
	size_t	done;

	done = 0;
	while (done < len)
	{
		written = write(fd, s + done, len - done);
		if (written < 0 && errno == EINTR)
			continue ;
		if (written <= 0)
			return (false);
		done += (size_t)written;
	}
	return (true);
}

/*
	How the shell reports a failure: "minishell: name: reason" on stderr,
	the name left out when there is nothing to name. Written where the
	failure happens, so that a child can report its own and no message
	has to outlive the arena it was built in.
*/
void	print_error(const char *name, const char *reason)
{
	write_all(STDERR_FILENO, "minishell: ", 11);
	if (name != NULL)
	{
		write_all(STDERR_FILENO, name, ft_strlen(name));
		write_all(STDERR_FILENO, ": ", 2);
	}
	write_all(STDERR_FILENO, reason, ft_strlen(reason));
	write_all(STDERR_FILENO, "\n", 1);
}
