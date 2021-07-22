#!/bin/bash

echo "normal test"
echo hello
echo hello hello hello
echo "hello"
echo 'hello'
echo                    h e l l o
echo " h e l l o "
echo ' hello hello hello '
echo echo echo echo echo echo echo echo
echo "echo"
echo 1234567890
echo ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz
echo ?@[\]^_{}~

# > < はリダイレクト
# << >>
echo "; test"
echo ;
echo hello;
echo hello ; ; ; ; ; ; ; ; ; #syntax error near unexpected token `;'
echo hello ; ; ;;;;; #error
echo ;hello  #command not found: hello
echo h;e;l;l;o
echo

#は先頭にあった場合だけ無視される
echo #hello
echo hello#hello
echo #abcde#abcde#abcde#abcde#abcde#abcde#abcde#abc
echo "-n test"
echo -n test
echo -n hello
echo -n           -n            -n hello
echo -n -n -n -n -n -n -n -n -n -n hello
echo -n -   n  -   n    hello
echo -n echo -n echo -n echo -n

echo \!\2\3\4\5\6\a\b #\は表示されない
