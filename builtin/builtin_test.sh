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


echo ;
echo hello;
echo hello ; ; ; ; ; ; ; ; ; #syntax error near unexpected token `;'
echo hello ; ; ;;;;; #error
echo ;hello  #command not found: hello
echo h;e;l;l;o
echo

echo "-n test"
echo -n test
echo -n hello
echo -n           -n            -n hello
echo -n -n -n -n -n -n -n -n -n -n hello
echo -n -   n  -   n    hello
echo -n echo -n echo -n echo -n
