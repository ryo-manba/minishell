#ifndef MS_PARSER_H
# define MS_PARSER_H
# include <unistd.h>
# include <stdlib.h>
# include <string.h>
# include <stdint.h>
# include <sys/time.h>
# include <pthread.h>
# include <stdio.h>

typedef struct s_word_list
{
	struct s_word_list	*next;
	char	*word;
	int		detail_type;
	int		token_type;
	int		i;
	int		start;
}	t_word_list;

typedef enum e_redir_op
{
	REDIR_DUMMY,
	REDIR_IN_FILE, // "<"; ファイルから入力する
	REDIR_OUT_FILE, // ">" ファイルに出力する
	REDIR_APPEND_FILE, // ">>" ファイルに追記して出力する
	REDIR_HEREDOC, // "<<" ヒアドキュメントで入力する

	// ここから下は要求されてないやつ
	REDIR_IN_FD, // "<&" FDを複製する
	REDIR_OUT_FD, // ">&" FDを複製する
	REDIR_INOUT_FILE, // "<>" ファイルを入出力モードで開く
	REDIR_OUT_CLOBBER, // ">|" ファイルに出力する; シェルの設定に関係なく上書きする。
	REDIR_HEREDOC_RETAB, // "<<-"; ヒアドキュメントで入力する; 入力の各行行頭のtabを除去する
}	t_redir_op;

typedef struct s_redir
{
	int32_t			left_operand; // 左オペランド; 未指定の場合は-1が入る
	int32_t			right_ionumber; // 右オペランド; IO_NUMBERの場合。
	t_word_list 	*right_word; // 右オペランド; WORDの場合; right_ionumberとright_wordはどちらか1つが指定される。
	t_redir_op		op; // リダイレクション種別
	int				concluded;
	struct s_redir	*next; // 次のリダイレクションへの参照
}	t_redir;

typedef enum e_token_identifier
{
	TI_DUMMY,
	TI_NEWLINE,
	TI_LESS,
	TI_GREAT,
	TI_DLESS,
	TI_DGREAT,
	TI_LESSAND,
	TI_GREATAND,
	TI_LESSGREAT,
	TI_DLESSDASH,
	TI_CLOBBER,
	TI_AND,
	TI_SEMICOLON,
	TI_DSEMICOLON,
	TI_AND_IF,
	TI_OR_IF,
	TI_DSEMI,
	TI_IO_NUMBER,
	TI_WORD,
	TI_NAME,
	TI_ASSIGNMENT_WORD,
	TI_RESERVED_WORD,
}	t_token_identifier;

typedef struct s_token
{
	char				*token;
	t_token_identifier	token_id;
	int					concluded;
	size_t				depth;
	struct s_token		*left;
	struct s_token		*right;
}	t_token;

typedef struct s_clause
{
	t_redir			redir_list;
	int				concluded;
	t_token			*ast;
	pid_t			pid;
	int				status;
	int				is_piped;
	size_t			size;
	struct s_clause	*next;
}	t_clause;

typedef enum e_pipeline_term
{
	PT_DUMMY,
	PT_NOTHING,
	PT_AND_IF,
	PT_OR_IF,
}	t_pipeline_term;

typedef struct s_pipeline
{
	t_clause			*root_clause;
	int					concluded;
	t_pipeline_term		terminator;
	size_t				size;
	struct s_pipeline	*next;
}	t_pipeline;

typedef enum e_andor_term
{
	AT_DUMMY,
	AT_SEMICOLON,
	AT_AND,
}	t_andor_term;

typedef struct s_andor_list
{
	t_pipeline			*pipeline;
	int					concluded;
	t_pipeline_term		t_andor_term;
	size_t				size;
	struct s_andor_list	*next;
}	t_andor_list;

typedef struct s_pipeline_list
{
	const char		*command_line;
	t_andor_list	*andor_list;
}	t_pipeline_list;

#endif
