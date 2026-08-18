#ifndef STRBUF_H
# define STRBUF_H

# include <stdbool.h>
# include <stddef.h>

typedef struct s_strbuf
{
	char	*data;
	size_t	len;
	size_t	cap;
}	t_strbuf;

bool	strbuf_init(t_strbuf *buf);
bool	strbuf_append(t_strbuf *buf, const char *str, size_t len);
char	*strbuf_detach(t_strbuf *buf, size_t *len);
void	strbuf_free(t_strbuf *buf);

#endif
