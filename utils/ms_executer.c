#include "ms_utils.h"

// コマンドの引数を数える
// $ "ls -l" = 2
size_t	ms_get_cmd_size(t_stree *tree)
{
	t_stree *tmp;
	size_t	sz;

	tmp = tree;
	sz = 0;
	while (tmp != NULL)
	{
		tmp = tmp->right;
		sz++;
	}
	return (sz);
}

// execveに送る場合
// {"ls", "-l", "NULL"}の状態にする。
char	**ms_create_execute_command(t_stree *tree)
{
	char **new_cmd;
	size_t	sz;
	size_t	i;
	t_stree	*tmp;

	sz = ms_get_cmd_size(tree);
	new_cmd = (char **)malloc(sizeof(char *) * sz + 1);
	if (new_cmd == NULL)
		return (NULL);
	tmp = tree;
	i = 0;
	while (tmp != NULL)
	{
		new_cmd[i] = ft_strdup(tmp->token);
		tmp = tmp->right;
		i++;
	}
	new_cmd[i] = NULL;
	return (new_cmd);
}


// 環境変数を展開しながらリダイレクションを処理する
// エクスパンダーの部分修正する
void	ms_expand_and_redirect(t_clause *clause)
{
	while (clause->redir != NULL) // 逐次的にエキスパンドとリダイレクトを行う  echo hello > $VAR > b > c | cat
	{
		do_expander(clause->redir); // リダイレクションを展開する (echo a > $VAR　-> echo a > var)
		ms_redirect(clause->redir); // リダイレクションを処理する
		clause->redir = clause->redir->next;
	}
}

void	ms_update_exitstatus(t_ex_state *state, pid_t pid)
{
	int	status;

	waitpid(pid, &status, 0);
	state->last_exit_status = WEXITSTATUS(status);
}

// パイプラインを再帰的に処理する
int	ms_executer(t_pipeline *pl, t_shellvar *var, t_ex_state *state)
{
	t_dpipe *dpipe;
	int	status;

	if (pl == NULL)
		return (0);
	if (pl->clause->next != NULL) // パイプがある場合、終わるまでループ回す
	{
		ms_execute_pipe_command(pl, var, state);
	}
	else
	{
		state->last_exit_status = ms_simple_command(pl->clause, var);
	}
	if (pl->joint == TI_ANDAND && state->last_exit_status == 0 // && 前のコマンドが成功した場合
		|| pl->joint == TI_PIPEPIPE && state->last_exit_status == 1) // || 前のコマンドが失敗した場合
	{
		ms_executer(pl->next, var, state);
	}
	return (0);
}
