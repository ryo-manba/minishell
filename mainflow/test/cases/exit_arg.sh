../minishell -c "exit"
echo $?
../minishell -c "exit 0"
echo $?
../minishell -c "exit 1"
echo $?
../minishell -c "exit 42"
echo $?
../minishell -c "exit -42"
echo $?
../minishell -c "exit -9223372036854775807"
echo $?
../minishell -c "exit -9223372036854775808"
echo $?
../minishell -c "exit -9223372036854775809"
echo $?
../minishell -c "exit 9223372036854775807"
echo $?
../minishell -c "exit 9223372036854775808"
echo $?
../minishell -c "exit 9223372036854775809"
echo $?
