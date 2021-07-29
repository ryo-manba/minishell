#include "lexer.h"

int main()
{
	lexer("echo hello world");
	lexer("echo hello world || cat res.text || grep hello");
	lexer("aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa");
	lexer("echo \"hello world\"");
	lexer("echo 'hello world'");
	lexer("echo \'h\"\"\"\"ello world\'");
	lexer("echo \"\"\"     \"\'hello world\'");
	lexer("echo \"\'hello world\'\"");
	lexer("echo \'\"hello world hello\"\'");
	lexer("echo hello|cat");
	lexer("& | $                     * < >");
	lexer("&& || << >> $$");
	lexer("&&&&  $$$$$$$ ||||| <<<<  >>>> <>|$&&$$");
	lexer("echo<><><><><> <> <> ** * * *** ****");
	lexer("$ $$$ $$$$$ $$$$$$$ $$$$$$$$$"); 	// 奇数
	lexer("<< >>>> <<<<<< >>>>>>>> <<<<<<<<<<");// 偶数
	lexer("echo \"hello\"\"world\"");
	lexer("echo \'hello world");
	lexer("echo \"hello world");
	lexer("a 'b' \"c\" & | * $ && || < > << >>              ");
	lexer("echo hello                    world");
	lexer("echo 'hello'                    'world'");
	lexer("echo helloworld");
	lexer("echo hello world");
	lexer("echo 'hello''world'"); // spaceの有無はどうするか
	lexer("echo 'hello' 'world'");

	return (0);
}
