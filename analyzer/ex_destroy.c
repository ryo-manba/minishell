#include "ms_analyzer.h"

void	ex_destroy_a_token(t_ex_token *ext)
{
	free((void *)ext->token);
	free(ext);
}

void	ex_destroy_token(t_ex_token *ext)
{
	if (!ext)
		return ;
	ex_destroy_token(ext->left);
	ex_destroy_token(ext->right);
	ex_destroy_a_token(ext);
}
