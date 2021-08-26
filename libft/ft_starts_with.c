#include "libft.h"

size_t	ft_starts_with(const char *heystack, const char *needle)
{
	// 文字列heystackが、文字列needleで始まるかどうかを調べる。
	size_t	n;

	n = 0;
	while (*(needle + n))
	{
		if (*(needle + n) != *(heystack + n))
			return (0);
		n += 1;
	}
	return (n);
}
