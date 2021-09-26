(echo x)
echo $?
(xxxx)
echo $?
(echo | xxxxx)
echo $?
(echo | cat xxxx)
echo $?
(echo a && echo b)
echo $?
(echo a || echo b)
echo $?
(cat xxxx && echo b && echo $?)
echo $?
(cat xxxx || echo b && echo $?)
echo $?
(cat xxxx || (cat xxxx || echo ok) && echo $?)
echo $?
