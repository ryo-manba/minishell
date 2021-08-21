#include "ms_analyzer.h"

// is given string fully consisted of digit?
int	ms_is_digital_str(const	char *str)
{
	while (*str)
		if (!ft_isdigit(*(str++)))
			return (0);
	return (1);
}
