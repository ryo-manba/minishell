#include "ms_analyzer.h"

int	ms_subparse_redir(t_parse_state *state, t_wdlist *word, t_stree *ion_st)
{
	t_wdlist	*target_wd;
	t_stree		*target_st;
	t_redir		*redir;

	// リダイレクション演算子トークンに続くlexerトークンがTOKENかどうか?
	target_wd = ms_shift_word(state);
	// 続くlexerトークンが存在しないか、トークン種別がTOKENではない
	// -> シンタックスエラー
	if (!target_wd || target_wd->lex_type != LT_TOKEN)
	{
		if (target_wd)
			return (ms_return_with_error(state, target_wd, "NO_RIGHT_OPERAND"));
		else
			return (ms_return_with_error(state, word, "NO_RIGHT_OPERAND"));
	}
	target_st = ms_make_stree(target_wd);
	if (!target_st)
		return (ms_return_with_error(state, target_wd, "ALLOCATION FAILED"));
	redir = ms_make_redir(word, target_st, ion_st);
	if (!target_st)
		return (ms_return_with_error(state, word, "ALLOCATION FAILED"));
	return (!ms_parse_add_redir(state, redir));
}
