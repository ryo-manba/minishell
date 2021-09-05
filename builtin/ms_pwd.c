#include "ms_builtin.h"

int	blt_pwd(void)
{
	char	*path;

	errno = 0;
	path = getcwd(NULL, 0); // 第一引数がNULLだと動的に確保される(free必要)
	if (errno != 0)
	{
		perror("getcwd");
		return (MS_BLT_FAIL);
	}
	ft_putendl_fd(path, STDOUT_FILENO);
	free(path);
	return (MS_BLT_SUCC);
}
