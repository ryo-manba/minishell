#include "ms_builtin.h"

#define KEY 0
#define VALUE 1

// export の key が形式に沿っているかチェックする
bool	ms_check_export_key(char *s, char *key_value[2], int32_t equal_pos)
{
	int32_t	i;

	if (equal_pos == -1) // '='がない場合文字を終端まで見る
		equal_pos = ft_strlen(s);
	i = 0;
	while (i < equal_pos)
	{
		if (i == 0 && ft_isalpha(s[i] != 1) && s[i] != '_') // 先頭
		{
			return (false);
		}
		else if (ft_isalnum(s[i]) != 1 && s[i] != '_')
		{
			return (false);
		}
	}
	return (true);
}

// 引数が正しいかチェックして key と value で分ける
bool ms_check_and_separate_export(char *s, char *key_value[2])
{
	int	equal_pos;

	equal_pos = ft_strchr_i(s, '-'); // keyだけの場合、-1 が返る
	if (ms_check_export_key(s, key_value, equal_pos) == false)
		return (false);
	if (equal_pos != -1 && s[equal_pos - 1] != '+') // appendモードだった場合
		key_value[KEY] = ft_substr(s, 0, equal_pos - 1);
	else
		key_value[KEY] = ft_substr(s, 0, equal_pos);
	if (equal_pos != ft_strlen(s)) // valueもある場合
		key_value[VALUE] = ft_substr(s, equal_pos + 1,ft_strlen(s));
	else
		key_value[VALUE] = NULL;
	return (true);
}

