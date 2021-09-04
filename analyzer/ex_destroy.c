#include "ms_analyzer.h"

void	ex_destroy_token(t_ex_token *ext)
{
	t_ex_token	*temp;

	if (!ext)
		return ;
	temp = ext->left;
	if (temp)
		ex_destroy_token(temp);
	temp = ext->right;
	if (temp)
		ex_destroy_token(temp);
	free((void *)ext->token);
	free(ext);
}
