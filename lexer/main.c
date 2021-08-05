#include "lexer.h"
#include <readline/readline.h>
#include <readline/history.h>
#include <string.h>
#include <signal.h>

volatile sig_atomic_t g_flag = true;

void	sigint_handler(int sig)
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
		lexer(line);
		if (strlen(line) > 0)
			add_history(line);
		free(line);

	}
	printf("> exit\n");
	return (0);
}

// int main()
// {
// 	lexer("echo hello world");
// 	lexer("echo hello world || cat res.text || grep hello");
// 	lexer("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
// 	lexer("echo \"hello world\"");
// 	lexer("echo 'hello world'");
// 	lexer("echo \'h\"\"\"\"ello world\'");
// 	lexer("echo \"\"\"     \"\'hello world\'");
// 	lexer("echo \"\'hello world\'\"");
// 	lexer("echo \'\"hello world hello\"\'");
// 	lexer("echo hello|cat");
// 	lexer("& | $                     * < >");
// 	lexer("&& || << >> $$");
// 	lexer("&&&&  $$$$$$$ ||||| <<<<  >>>> <>|$&&$$");
// 	lexer("echo<><><><><> <> <> ** * * *** ****");
// 	lexer("$ $$$ $$$$$ $$$$$$$ $$$$$$$$$"); 	// 奇数
// 	lexer("$$ $$$$ $$$$$$ $$$$$$$$ $$$$$$$$$$"); // 偶数
// 	lexer("$$$ $$$$$ $$$$$$$ ");
// 	lexer("<< >>>> <<<<<< >>>>>>>> <<<<<<<<<<");// 偶数
// 	lexer("echo \"hello\"\"world\"");
// 	lexer("echo \'hello world'''");
// 	lexer("echo \"hello world\"'");
// 	lexer("a 'b' \"c\" & | * $ && || < > << >>              ");
// 	lexer("echo hello                    world");
// 	lexer("echo 'hello'                    'world'");
// 	lexer("echo helloworld");
// 	lexer("echo hello world");
// 	lexer("echo 'hello''world'"); // spaceの有無はどうするか
// 	lexer("echo 'hello' 'world'");
// 	lexer("echo 'hello''world'");
// 	lexer("echo 'hello''world'abcde'hello'"); // 6
// 	lexer("echo 'hello''wo'rld'abcde'hello'"); // 7
// 	lexer("echo 'hello'world");
// 	lexer("echo 'hello'world'salut'");
// 	lexer("echo 'hello'world'salut'$$");
// 	lexer("echo 'hello'world'salut'&<>");
// 	lexer("echo 'hello'world'salut'$USER");
// 	lexer("echo $USER $$PWD $$$HOME");
// 	lexer("echo $USER$$PWD$$$HOME");
// 	lexer("echo '$USER' \"$USER\" $USER");
// 	lexer("echo '$USER'\"$USER\"$USER"); // この場合の状態は?
// 	return (0);
// }
