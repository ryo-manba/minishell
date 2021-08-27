#include "ms_builtin.h"

int	ms_pwd(void)
{
	char	*path;

	errno = 0;
	path = getcwd(NULL, 0); // 第一引数がNULLだと動的に確保される(free必要)
	if (errno != 0)
	{
		perror("pwd");
		return (1);
	}
	printf("%s\n", path);
	free(path);
	return (0);
}
