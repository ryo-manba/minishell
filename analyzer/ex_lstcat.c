#include "ms_analyzer.h"

static size_t	ex_strcpy_n(char *dst, const char *src)
{
	size_t	n;

	n = 0;
	while (src[n])
	{
		dst[n] = src[n];
		n += 1;
	}
	return (n);
}

char	*ex_lstcat(t_ex_token *ext)
{
	size_t		i;
	size_t		n;
	char		*joined;
	t_ex_token	*temp;

	n = 0;
	temp = ext;
	while (temp)
	{
		n += ft_strlen(temp->token);
		temp = temp->right;
	}
	joined = (char *)malloc((n + 1) * sizeof(char));
	if (!joined)
		return (NULL);
	temp = ext;
	i = 0;
	while (temp)
	{
		i += ex_strcpy_n(joined + i, temp->token);
		temp = temp->right;
	}
	joined[n] = '\0';
	return (joined);
}
