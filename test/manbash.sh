# builtin-commandsのmanを参照
manbash() {
    man bash | less -p "^       $1 "
}
