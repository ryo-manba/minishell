#include "ms_analyzer.h"

t_ex_token	*ex_expansion_error(t_ex_state *state,
		t_ex_unit_cursor *cursor)
{
	(void)state;
	if (cursor->p.head)
		ex_destroy_token(cursor->p.head);
	ft_putstr_fd("unexpected state on expansion stage\n", STDERR_FILENO);
	return (NULL);
}

t_ex_token	*ex_push_back_token(t_ex_state *state,
		t_ex_unit_cursor *cursor, const char *given_str)
{
	t_ex_token	*ext;

	ext = (t_ex_token *)ft_calloc(1, sizeof(t_ex_token));
	if (!ext)
		return (ex_expansion_error(state, cursor));
	ex_add_token_csr(&(cursor->p), ext);
	ext->pa_token_id = cursor->pa_token_id;
	ext->token_id = cursor->running;
	if (cursor->running == XI_DIVIDER)
		ext->token = NULL;
	else
	{
		ext->token = given_str;
		if (!ext->token)
			ext->token = ft_substr(cursor->str, cursor->substr_s,
				cursor->substr_e - cursor->substr_s);
		if (!ext->token)
		{
			free(ext);
			state->failed = 1;
			return (NULL);
		}
		printf("\"%s\" %zu:%zu\n", ext->token, cursor->substr_s, cursor->substr_e - cursor->substr_s);
	}
	return (ext);
}

int	ex_ll_replace_var(t_ex_state *state, t_ex_unit_cursor *csr)
{
	char		*key;
	char		*val;
	t_shellvar	*var;

	key = ft_substr(csr->str, csr->substr_s,
			csr->substr_e - csr->substr_s);
	if (!key)
		return (MS_AZ_FAIL);
	var = state->var;
	val = NULL;
	while (var)
	{
		if (!ft_strcmp(var->key, key))
		{
			val = var->value;
			break ;
		}
		var = var->next;
	}
	if (!val)
		val = ft_strdup("");
	if (val)
	{
		if (ex_push_back_token(state, csr, val))
			return (MS_AZ_SUCC);
		free(val);
	}
	free(key);
	return (MS_AZ_FAIL);
}

int	ex_ll_trap_bare(t_ex_state *state, t_ex_unit_cursor *csr)
{
	char	c;

	c = csr->str[csr->i];
	if (csr->running != XI_BARE)
		return (0);
	if ((c == '\'' && !csr->quote) || c == '"' || c == '$' || !c)
	{
		// vsからi - 1までをBAREトークンとして追加する。
		csr->substr_s = csr->vs;
		csr->substr_e = csr->i;
		if (!ex_push_back_token(state, csr, NULL))
			state->failed = 1;
		csr->running = XI_NEUTRAL;
		printf("NEUTRAL <-\n");
	}
	else
		csr->i += 1;
	return (1);
}

// }で解除される。
int	ex_ll_trap_braced_var(t_ex_state *state, t_ex_unit_cursor *csr)
{
	char	c;

	(void)state;
	c = csr->str[csr->i];
	if (csr->running != XI_BRACED_VAR)
		return (0);
	if (c == '}' || !c)
	{
		// `vs + 2`から`i - 1`までを`VAR`トークンとして処理する。
		csr->substr_s = csr->vs + 2;
		csr->substr_e = csr->i;
		csr->running = XI_VAR;
		if (ex_ll_replace_var(state, csr))
			state->failed = 1;
		// - `i += 1` して`NEUTRAL`に遷移
		if (c)
			csr->i += 1;
		csr->running = XI_NEUTRAL;
		printf("NEUTRAL <-\n");
	}
	else
		csr->i += 1;
	return (1);
}

// BRACED_VARに遷移することがある。
int	ex_ll_trap_var(t_ex_state *state, t_ex_unit_cursor *csr)
{
	char	c;

	c = csr->str[csr->i];
	if (csr->running != XI_VAR)
		return (0);
	if (c == '{' && csr->i == csr->vs + 1)
	{
		// i += 1して BRACED_VARに遷移
		csr->i += 1;
		csr->running = XI_BRACED_VAR;
		printf("-> BRACED_VAR\n");
	}
	else if (ft_strchr("0123456789?", c) && csr->i == csr->vs + 1)
	{
		// vs + 1からiまでをVARトークンとして追加する。
		csr->substr_s = csr->vs + 1;
		csr->substr_e = csr->i + 1;
		if (!ex_push_back_token(state, csr, NULL))
			state->failed = 1;
		// i += 1してNEUTRALに遷移
		csr->i += 1;
		csr->running = XI_NEUTRAL;
		printf("NEUTRAL <-\n");
	}
	else if ((ft_isalpha(c) || c == '_') && csr->i == csr->vs + 1)
		csr->i += 1;
	else if ((ft_isalnum(c) || c == '_') && csr->i > csr->vs + 1)
		csr->i += 1;
	else if (csr->i == csr->vs + 1)
	{
		// `vs + 1`から`i - 1`までを`BARE`トークンとして追加する。
		// `NEUTRAL`に遷移
		csr->substr_s = csr->vs + 1;
		csr->substr_e = csr->i;
		csr->running = XI_BARE;
		if (!ex_push_back_token(state, csr, NULL))
			state->failed = 1;
		csr->running = XI_NEUTRAL;
		printf("NEUTRAL <-\n");
	}
	else {
		csr->substr_s = csr->vs + 1;
		csr->substr_e = csr->i;
		// vs + 1からi - 1までをVARトークンとして処理する。
		if (ex_ll_replace_var(state, csr))
			state->failed = 1;
		csr->running = XI_NEUTRAL;
		printf("NEUTRAL <-\n");
	}
	return (1);
}

// シングルクオートはシングルクオートで解除される。
int	ex_ll_trap_squoted(t_ex_state *state, t_ex_unit_cursor *csr)
{
	char	c;

	c = csr->str[csr->i];
	if (csr->running != XI_SQUOTED)
		return (0);
	if (c == '\'' || !c)
	{
		// vs + 1からi - 1までをSQUOTEDトークンとして追加する。
		csr->substr_s = csr->vs + 1;
		csr->substr_e = csr->i;
		if (!ex_push_back_token(state, csr, NULL))
			state->failed = 1;
		// シングルクオートならi += 1
		if (c)
			csr->i += 1;
		// NEUTRALに遷移
		csr->running = XI_NEUTRAL;
		printf("NEUTRAL <-\n");
	}
	else // i += 1
		csr->i += 1;
	return (1);
}

// concatenate all strings in given ex_token list.
char	*ex_strcat_exlist(t_ex_token *head, size_t s)
{
	size_t		n;
	char		*joined;

	if (!head)
	{
		joined = (char *)malloc((s + 1) * sizeof(char));
		if (!joined)
			return (NULL);
		joined[s] = '\0';
		return (joined);
	}
	n = ft_strlen(head->token);
	joined = ex_strcat_exlist(head->right, s + n);
	if (!joined)
		return (NULL);
	ft_memcpy(joined + s, head->token, n);
	return (joined);
}

int	ex_ll_trap_dquote(t_ex_state *state, t_ex_unit_cursor *csr)
{
	t_ex_unit_cursor	cursor;
	char				*joined;

	csr->i += 1;
	printf("--> RECURSIVE\n");
	ex_ll_init_cursor(&cursor, csr->pa_token_id, csr->str + csr->i, '"');
	ex_ll_unit(state, &cursor);
	printf("<-- RECURSIVE\n");
	if (state->failed)
		return (MS_AZ_FAIL);
	ex_stringify_extoken(cursor.p.head);
	joined = ex_strcat_exlist(cursor.p.head, 0);
	if (!joined)
		return (MS_AZ_FAIL);
	csr->running = XI_DQUOTED;
	if (!ex_push_back_token(state, csr, joined))
		return (MS_AZ_FAIL);
	csr->running = XI_NEUTRAL;
	csr->i += cursor.i + 1;
	return (MS_AZ_SUCC);
}

// 唯一何かを開始できる and 展開を終了できるモード
int	ex_ll_trap_neutral(t_ex_state *state, t_ex_unit_cursor *csr)
{
	(void)state;
	if (csr->running != XI_NEUTRAL)
		return (0);
	if (csr->str[csr->i] == '\'' && !csr->quote)
	{
		// 再帰展開中でなければ、vs = i, i += 1して SQUOTEDに遷移
		csr->vs = csr->i++;
		csr->running = XI_SQUOTED;
		printf("-> SQUOTED\n");
	}
	else if (csr->str[csr->i] == '"')
	{
		// 再帰展開中でなければ、i += 1 してダブルクオート処理を行う。後述。
		if (ex_ll_trap_dquote(state, csr))
			state->failed = 1;
	}
	else if (csr->str[csr->i] == '$')
	{
		// vs = i, i += 1してVARに遷移
		csr->vs = csr->i++;
		csr->running = XI_VAR;
		printf("-> VAR\n");
	}
	else
	{
		// vs = iして BAREに遷移
		csr->vs = csr->i;
		csr->running = XI_BARE;
		printf("-> BARE\n");
	}
	return (1);
}

void	ex_ll_unit(t_ex_state *state, t_ex_unit_cursor *csr)
{
	printf("ex_ll_unit: \"%s\"\n", csr->str);
	while (!state->failed)
	{
		printf("(running, vs, i, c) = (%d, %zu, %zu, '%c')\n", csr->running, csr->vs, csr->i, csr->str[csr->i]);
		if (csr->running == XI_NEUTRAL && csr->str[csr->i] == csr->quote)
			break ;
		if (ex_ll_trap_neutral(state, csr))
			continue ;
		if (ex_ll_trap_squoted(state, csr))
			continue ;
		if (ex_ll_trap_var(state, csr))
			continue ;
		if (ex_ll_trap_braced_var(state, csr))
			continue ;
		if (ex_ll_trap_bare(state, csr))
			continue ;
	}
	if (state->failed)
		printf("!!FAILED!!\n");
	return ;
	// return (ex_expansion_error(state, &csr));
}

t_ex_token	*ex_shell_param(t_ex_state *state, t_stree *stree)
{
	t_ex_unit_cursor	csr;

	ex_ll_init_cursor(&csr, stree->token_id, stree->token, '\0');
	ex_ll_unit(state, &csr);
	return (csr.p.head);
}
