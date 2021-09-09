#include <stdio.h>
#include <libc.h>
#include <fcntl.h>

int	main()
{
	int	i = O_WRONLY | O_CREAT | O_TRUNC;
	int	j = O_WRONLY | O_CREAT | O_APPEND;
	int	k = O_RDONLY;

	printf("i = %d, j = %d, k = %d\n", i, j, k);
	return (0);
}
