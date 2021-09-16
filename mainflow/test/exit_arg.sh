$1 -c "exit"
echo $?
$1 -c "exit 0"
echo $?
$1 -c "exit 1"
echo $?
$1 -c "exit 42"
echo $?
$1 -c "exit -42"
echo $?
$1 -c "exit -9223372036854775807"
echo $?
$1 -c "exit -9223372036854775808"
echo $?
$1 -c "exit -9223372036854775809"
echo $?
$1 -c "exit 9223372036854775807"
echo $?
$1 -c "exit 9223372036854775808"
echo $?
$1 -c "exit 9223372036854775809"
echo $?
