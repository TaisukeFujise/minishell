#include "../../include/builtin.h"

#define EXPORT_CTRL "\033\a\b\t\n\v\f\r"
#define EXPORT_ESC "Eabtnvfr"

static bool	needs_ansic(char *s)
{
	while (*s)
	{
		if (!ft_isprint((unsigned char)*s))
			return (true);
		s++;
	}
	return (false);
}

static void	print_ansic_char(char c)
{
	char	*esc;

	esc = ft_strchr(EXPORT_CTRL, c);
	if (esc)
		printf("\\%c", EXPORT_ESC[esc - EXPORT_CTRL]);
	else if (!ft_isprint((unsigned char)c))
		printf("\\%03o", (unsigned char)c);
	else if (c == '\\' || c == '\'')
		printf("\\%c", c);
	else
		printf("%c", c);
}

static void	print_dquote_char(char c)
{
	if (ft_strchr("\"\\$`", c))
		printf("\\%c", c);
	else
		printf("%c", c);
}

void	print_export_value(char *s)
{
	if (!needs_ansic(s))
	{
		printf("=\"");
		while (*s)
			print_dquote_char(*s++);
		printf("\"");
		return ;
	}
	printf("=$'");
	while (*s)
		print_ansic_char(*s++);
	printf("'");
}
