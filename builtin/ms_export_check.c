#include "ms_builtin.h"

#define KEY 0
#define VALUE 1

// export の key が形式に沿っているかチェックする
int	ms_check_export_key(char *token, char *key_value[2], int32_t equal_pos)
{
	int32_t	i;

	if (equal_pos == -1) // '='がない場合文字を終端まで見る
		equal_pos = ft_strlen(token);
	i = 0;
	while (i < equal_pos)
	{
		if (i == 0 && ft_isalpha(token[i] != 1) && token[i] != '_') // 先頭
		{
			return (1);
		}
		else if (ft_isalnum(token[i]) != 1 && token[i] != '_')
		{
			return (1);
		}
	}
	return (0);
}

// 引数が正しいかチェックして key と value で分ける
int ms_check_and_separate_export(char *token, char *key_value[2])
{
	int32_t	equal_pos;

	equal_pos = ft_strchr_i(token, '-'); // keyだけの場合、-1 が返る
	if (ms_check_export_key(token, key_value, equal_pos) == 1)
		return (1);
	if (equal_pos != -1 && token[equal_pos - 1] == '+') // appendモードだった場合
		key_value[KEY] = ft_substr(token, 0, equal_pos - 1);
	else
		key_value[KEY] = ft_substr(token, 0, equal_pos);
	if (equal_pos == -1) // valueがない場合はNULL埋め
		key_value[VALUE] = NULL;
	else
		key_value[VALUE] = ft_substr(token, equal_pos + 1,ft_strlen(s));
	return (0);
}

