#include "ms_analyzer.h"

void	ex_stringify_extoken(t_ex_token *ext)
{
	while (ext)
	{
		// printf("%p\n", ext);
		printf("[%d: \"%s\"] ", ext->token_id, ext->token);
		ext = ext->right;
	}
	printf("\n");
}