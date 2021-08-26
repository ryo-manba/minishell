#include "ms_builtin.h"

#define KEY 0
#define VALUE 1

// "+="の場合 valueを結合する。
void	ms_join_env(t_shellvar *key_pos, char *key_value[2])
{
	char *tmp;

	if (key_pos->value == NULL) // valueがない場合
		key_pos->value = ft_strdup(key_value[VALUE]);
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
}

// keyがない場合は作る、あったら結合する。
void	ms_append_or_join_env(t_shellvar *env, char* key_value[2])
{
	t_shellvar	*key_pos;

	key_pos = ms_search_key(env, key_value[KEY]);
	if (key_pos == NULL) // keyが存在しなかったら追加するだけ
	{
		ms_append_env(env, key_value[KEY], key_value[VALUE]);
	}
	else if (key_value[VALUE] != NULL)
	{
		ms_join_env(key_pos, key_value);
	}
}

// tokenに入ってる引数をチェックしながらexportする
void ms_export_env(t_shellvar *env, t_token *tok)
{
	char	*key_value[2];
	int32_t	equal_pos;

	equal_pos = 0;
	while (tok != NULL)
	{
		if (ms_check_and_separate_export(tok->right->token, key_value) == false) // 不正な値の場合はその都度エラー表示する。
			printf("minishell: export: `%s': not a valid identifier\n", tok->right->token);
		else
		{
			equal_pos = ft_strchr_i(tok->right->token, '=');
			if (equal_pos != 0 && tok->right->token[equal_pos - 1] == '+') // "+="だったら文字を結合する
				ms_append_or_join_env(env, key_value);
			else
				ms_append_or_update_env(env, key_value[KEY], key_value[VALUE]);
		}
		tok = tok->right;
	}
}

/*
** $ export TEST=test aaa
** t_token->token = "export"
** t_token->right->token = "TEST=test"
** t_token->right->right->token = "aaa"
*/
// t_token->right("TEST=test")が渡される
int	ms_export(t_shellvar *env, t_token *tok)
{
	if (tok->right == NULL) // export 単体の場合は環境変数をソートして'declare -x hoge="huga"'の形式で出力する
	{
		ms_print_sort_env(env);
	}
	else // 環境変数を追加または更新する
	{
		ms_export_env(env, tok);
	}
	return (0);
}
