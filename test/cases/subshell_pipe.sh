echo hello | ( cat )
echo fuck
echo world
echo $?
echo hello | ( cat )
echo world
echo $?
echo hello | ( cat ) | (cat ) | ( cat) | (cat)
echo $?
(ls) | (cat)
echo $?
(ls) | cat
echo $?
ls | (cat)
echo $?
(echo hello && echo world) | wc
(echo hello | cat) | cat
(echo hello | echo world) | cat
