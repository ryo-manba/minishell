# minishell

## Build Steps for Reviewer

1. Install `readline` by brew.
	- `$ brew install readline`
2. cd to minishell root
3. build with readline
	- `$ make RL_PATH="$HOME/.brew/opt/readline"`
	- or edit Makefile at repository-root and simply `make`
		- `RL_PATH :=$(HOME)/.brew/opt/readline`

