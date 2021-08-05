#include <libc.h>


// 環境変数展開 ここでprintしても良いかも
char	*ft_getenv(char *env_name)
{
 	char *s;

	s = getenv(env_name);
	if (!s)
		return ("\0");
	return (s);
}

// envコマンド(環境変数すべて出力する)
void	ft_env(char **envp)
{
	int	i;

	i = 0;
	while (envp[i])
	{
		printf("%s\n",envp[i]);
		i++;
	}
}

void	ft_pwd(void)
{
	printf("%s\n", ft_getenv("PWD"));
}

int	main(int argc, char **argv, char **envp)
{
	char *s;
//	ft_env(envp);
//	s = ft_getenv(argv[1]);
//	printf("%s\n",s);
	ft_pwd();


	return (0);
}
