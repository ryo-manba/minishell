#include "ms_analyzer.h"

t_redir	*ms_expand_redir(t_ex_state *state, t_redir *redir)
{
	t_redir	*tail;
	t_stree	*original;
	t_stree	*expanded;

	tail = redir;
	while (tail)
	{
		if (!tail->operand_right)
			return ((t_redir *)ex_fatal(state, "redir has no right"));
		original = tail->operand_right;
		expanded = ms_expand_stree(state, original);
		if (!expanded || expanded->right)
			return ((t_redir *)ex_error(state, original, "ambiguous redirect"));
		tail->operand_right = expanded;
		tail = tail->next;
	}
	return (redir);
}

t_stree	*ms_expand_stree(t_ex_state *state, t_stree *stree)
{
	t_ex_token	*ext;

	if (stree->subshell)
	{
		// TODO: エグゼキュータに渡す
		return (NULL);
	}
	ext = ex_shell_param(state, stree);
	if (!state->no_split)
		ext = ex_split_word(state, ext);
	ext = ex_filename(state, ext);
	return (ex_join_words(state, ext));
}
