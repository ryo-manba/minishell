#include "ms_analyzer.h"

void	ex_destroy_token(t_ex_token *ext)
{
	t_ex_token	*left;
	t_ex_token	*right;

	if (!ext)
		return ;
	left = ext->left;
	right = ext->right;
	free((void *)ext->token);
	free(ext);
	if (left)
		ex_destroy_token(left);
	if (right)
		ex_destroy_token(right);
}
