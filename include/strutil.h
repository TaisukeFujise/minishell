#ifndef STRUTIL_H
# define STRUTIL_H

# include <stdbool.h>
# include <stddef.h>

size_t	str_name_len(const char *str);
size_t	str_assign_pos(const char *str, bool *append);

#endif
