# minishell

## Description
A small shell like bash.

## Requirement

- gcc or Clang
- GNU Make
- readline

## Build Steps for Reviewer

1. Install `readline` by brew.
	- `brew install readline`
2. cd to minishell root
3. build with readline
	- `make RL_PATH="$HOME/.brew/opt/readline"`
	- or edit Makefile at repository-root and simply `make`
		- `RL_PATH :=$(HOME)/.brew/opt/readline`


## Features
- Simple command
- Relative path command
- Absolute path command
- Environment variables
- Redirection
- Pipe
- &&, ||, ;
- subshell
- wildcard(*)

Implement a series of builtins: 
- cd
- echo [-n]
- env
- exit
- export
- pwd
- unset


## Author

[corvvs](https://github.com/corvvs)<br>
[ryo-manba](https://twitter.com/ryo_manba)

## Licence

All code is released under the [MIT](https://github.com/ryo-manba/minishell/blob/main/LICENSE) license
