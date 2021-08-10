#include "ms_lexer.h"

static void	sigint_handler(int sig)
{
	printf("\n");
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

int	main()
{
	char *line;

	if (signal(SIGINT, sigint_handler) == SIG_ERR)
		return (1);
	if (signal(SIGQUIT, SIG_IGN) == SIG_ERR)
		return (1);
	while (1)
	{
		line = readline("> ");
		if (!line)
			break ;
		ms_lexer(line);
		if (strlen(line) > 0)
			add_history(line);
		free(line);
	}
	printf("> exit\n");
	return (0);
}

// int main()
// {
// 	ms_lexer("echo hello world");
// 	ms_lexer("echo hello world || cat res.text || grep hello");
// 	ms_lexer("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
// 	ms_lexer("echo \"hello world\"");
// 	ms_lexer("echo 'hello world'");
// 	ms_lexer("echo \'h\"\"\"\"ello world\'");
// 	ms_lexer("echo \"\"\"     \"\'hello world\'");
// 	ms_lexer("echo \"\'hello world\'\"");
// 	ms_lexer("echo \'\"hello world hello\"\'");
// 	ms_lexer("echo hello|cat");
// 	ms_lexer("& | $                     * < >");
// 	ms_lexer("&& || << >> $$");
// 	ms_lexer("&&&&  $$$$$$$ ||||| <<<<  >>>> <>|$&&$$");
// 	ms_lexer("echo<><><><><> <> <> ** * * *** ****");
// 	ms_lexer("$ $$$ $$$$$ $$$$$$$ $$$$$$$$$"); 	// 奇数
// 	ms_lexer("$$ $$$$ $$$$$$ $$$$$$$$ $$$$$$$$$$"); // 偶数
// 	ms_lexer("$$$ $$$$$ $$$$$$$ ");
// 	ms_lexer("<< >>>> <<<<<< >>>>>>>> <<<<<<<<<<");// 偶数
// 	ms_lexer("echo \"hello\"\"world\"");
// 	ms_lexer("echo \'hello world'''");
// 	ms_lexer("echo \"hello world\"'");
// 	ms_lexer("a 'b' \"c\" & | * $ && || < > << >>              ");
// 	ms_lexer("echo hello                    world");
// 	ms_lexer("echo 'hello'                    'world'");
// 	ms_lexer("echo helloworld");
// 	ms_lexer("echo hello world");
// 	ms_lexer("echo 'hello''world'"); // spaceの有無はどうするか
// 	ms_lexer("echo 'hello' 'world'");
// 	ms_lexer("echo 'hello''world'");
// 	ms_lexer("echo 'hello''world'abcde'hello'"); // 6
// 	ms_lexer("echo 'hello''wo'rld'abcde'hello'"); // 7
// 	ms_lexer("echo 'hello'world");
// 	ms_lexer("echo 'hello'world'salut'");
// 	ms_lexer("echo 'hello'world'salut'$$");
// 	ms_lexer("echo 'hello'world'salut'&<>");
// 	ms_lexer("echo 'hello'world'salut'$USER");
// 	ms_lexer("echo $USER $$PWD $$$HOME");
// 	ms_lexer("echo $USER$$PWD$$$HOME");
// 	ms_lexer("echo '$USER' \"$USER\" $USER");
// 	ms_lexer("echo '$USER'\"$USER\"$USER"); // この場合の状態は?
//	ms_lexer("<<>><><><><<<>><>");
// 	return (0);
// }
