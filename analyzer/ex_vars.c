#include "ms_analyzer.h"

// 特殊変数置き換え
// return values:
//  0: 置き換え成功
// -1: 置き換え失敗
// +1: 置き換えスキップ
int	ex_special_var(t_ex_state *state, const char *key, char **val)
{
	char	*v;

	if (key[0] == '?' && !key[1])
	{
		v = ft_itoa(state->last_exit_status);
		if (!v)
			return (-1);
		*val = v;
		return (0);
	}
	return (+1);
}

// 与えられたkeyに対応する環境変数の値が存在する場合、それをコピーして取得する
// 存在しない場合はNULLを返す
// 存在するが値を持たない場合はfree-ableな""を返す
// 存在して値を持つ場合は値をコピーして返す。
// コピーに失敗した場合はNULLを返すが、その際state.failedを1にセットする。
char	*ex_get_var_val(t_ex_state *state, const char *key)
{
	t_shellvar	*sv;
	char		*val;
	int			sp;

	sp = ex_special_var(state, key, &val);
	if (sp == -1)
		state->failed = 1;
	if (sp != +1)
		return (val);
	sv = state->var;
	while (sv)
	{
		if (!ft_strcmp(sv->key, key))
		{
			if (sv->value)
				val = ft_strdup(sv->value);
			else
				val = ft_strdup("");
			state->failed = !val;
			return (val);
		}
		sv = sv->next;
	}
	return (NULL);
}

// 変数置き換え
int	ex_replace_var(t_ex_state *state, t_ex_cursor *cursor)
{
	char		*name;
	const char	*val;

	name = ft_substr(cursor->str, cursor->vs, cursor->i);
	if (!name)
		return (MS_AZ_FAIL);
	val = ex_get_var_val(state, name);
	free(name);
	if (!val && state->failed)
		return (MS_AZ_FAIL);
	if (!ex_push_back_token(state, cursor, val))
		return (MS_AZ_FAIL);
	return (MS_AZ_SUCC);
}


