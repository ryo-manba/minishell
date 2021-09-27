pwd && (pwd && cd .. && pwd) && pwd
ls && (ls && cd .. && ls) && ls
echo hello | (echo world; cat; echo mash) | cat
echo hello | (echo world && cat && cat white || echo mash) | cat
echo hello | (echo world && cat && cat white && echo mash) | cat
