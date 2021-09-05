#include "ms_builtin.h"

// "+="の場合 valueを結合する。
int	blt_join_env(t_shellvar *key_pos, char *key_value[2])
{
	char *tmp;

	if (key_pos->value == NULL) // valueがない場合
	{
		key_pos->value = ft_strdup(key_value[VALUE]);
		if (key_pos->value == NULL)
			return (MS_BLT_FAIL);
	}
	else if (key_pos->value == "") // valueが空文字列の場合は取り除く
	{
		free(key_pos->value);
		key_pos->value = ft_strdup(key_value[VALUE]);
	}
	else // valueが存在する場合は追加する。
	{
		tmp = key_pos->value;
		key_pos->value = ft_strjoin(key_pos->value, key_value[VALUE]);
		free(tmp);
	}
	if (key_pos->value == NULL)
		return (MS_BLT_SUCC);
	return (MS_BLT_SUCC);
}

// keyがない場合は作る、あったら結合する。
int	blt_append_or_join_env(t_shellvar *var, char* key_value[2])
{
	t_shellvar	*key_pos;

	key_pos = blt_search_key(var, key_value[KEY]);
	if (key_pos == NULL) // keyが存在しなかったら追加するだけ
	{
		if (blt_append_env(var, key_value[KEY], key_value[VALUE]))
			return (MS_BLT_FAIL);
	}
	else if (key_value[VALUE] != NULL) // keyが存在して新しい value が NULL じゃなかった場合結合する
	{
		if (blt_join_env(key_pos, key_value) == MS_BLT_FAIL)
			return (MS_BLT_FAIL);
	}
	return (MS_BLT_SUCC);
}

int	blt_check_and_export(t_stree *tree, t_shellvar *var, char *key_value[2])
{
	int32_t	equal_idx;

	equal_idx = ft_strchr_i(tree->token, '=');
	if (equal_idx != 0 && tree->token[equal_idx - 1] == '+') // "+="だったら文字を結合する
	{
		if (blt_append_or_join_env(var, key_value) == MS_BLT_FAIL)
			return (MS_BLT_FAIL);
	}
	else
	{
		if (blt_append_or_update_env(var, key_value[KEY], key_value[VALUE]) == MS_BLT_FAIL)
			return (MS_BLT_FAIL);
	}
	return (MS_BLT_SUCC);
}

// tokenに入ってる引数をチェックしながらexportする
int blt_export_env(t_shellvar *var, t_stree *tree)
{
	char	*key_value[2];
	int		ex_status;

	ex_status = MS_BLT_SUCC;
	while (tree != NULL)
	{
		if (blt_check_and_separate_export(tree->token, key_value) == MS_BLT_FAIL) // 不正な値の場合はその都度エラー表示する。
		{
			blt_export_print_error(tree->token);
			ex_status = MS_BLT_FAIL;  // 一つでもエラーが出たら終了ステータスは1
		}
		else
		{
			if (blt_check_and_export(var, tree, key_value) == MS_BLT_FAIL)
				return (MS_BLT_FAIL);
		}
		tree = tree->right;
	}
	return (ex_status);
}

/*
** $ export TEST=test aaa
** tree->token = "TEST=test"
** tree->right->token = "aaa"
*/
int	blt_export(t_shellvar *var, t_stree *tree)
{
	if (tree == NULL) // 'export' 単体の場合は環境変数をソートして'declare -x hoge="huga"'の形式で出力する
	{
		blt_print_sort_env(var);
	}
	else // 環境変数を追加または更新する
	{
		if (blt_export_env(var, tree) == MS_BLT_FAIL)
			return (MS_BLT_FAIL);
	}
	return (MS_BLT_SUCC);
}
