#include "ms_analyzer.h"

// is given string fully consisted of digit?
int	lx_is_digital_str(const	char *str, size_t len)
{
	size_t	n;

	n = 0;
	while (n < len && str[n])
		if (!ft_isdigit(str[n++]))
			return (0);
	return (n > 0);
}

int	lx_is_char_for_name(char c, size_t pos)
{
	if (pos == 0 && !ft_isalpha(c) && c != '_')
		return (0);
	else if (pos > 0 && !ft_isalnum(c) && c != '_')
		return (0);
	return (1);
}

// strがNAMEとして適当かどうか?
// 1. 長さが1以上
// 2. 先頭の文字が[A-Za-z_]のいずれか
// 3. 先頭以外の文字が[0-9A-Za-z_]のいずれか
int	lx_is_name(const char *str, size_t len)
{
	size_t	n;

	n = 0;
	while (n < len && str[n])
	{
		if (n == 0 && !ft_isalpha(str[n]) && str[n] != '_')
			return (0);
		else if (n > 0 && !ft_isalnum(str[n]) && str[n] != '_')
			return (0);
		n += 1;
	}
	return (n > 0);
}

int	lx_is_char_for_bare(char c)
{
	if (!c)
		return (0);
	return (!!ft_strchr("'\"$", c));
}

// strがASSIGNMENT WORDとして適当かどうか?
// 1. strが文字'='を含むこと
// 2. strの文字'='より前の部分がNAMEとして適当であること
int	lx_is_assignment_word(const char *str, size_t len)
{
	size_t	n;

	n = 0;
	while (n < len && str[n])
	{
		if (str[n] == '=')
			return (lx_is_name(str, n));
		n += 1;
	}
	return (0);
}
