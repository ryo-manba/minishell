gcc -o test/fd_write test/test_fd_write.c libft/libft.a
export TEST=test/fd_write
$TEST 1 1>tmpfile
cat tmpfile
$TEST 2 2>tmpfile
cat tmpfile
$TEST 3 3>tmpfile
cat tmpfile
$TEST 4 4>tmpfile
cat tmpfile
$TEST 5 5>tmpfile
cat tmpfile
$TEST 6 6>tmpfile
cat tmpfile
$TEST 7 7>tmpfile
cat tmpfile
$TEST 8 8>tmpfile
cat tmpfile
$TEST 9 9>tmpfile
cat tmpfile
$TEST 10 10>tmpfile
cat tmpfile
$TEST 100 100>tmpfile
cat tmpfile
$TEST 200 200>tmpfile
cat tmpfile
$TEST 300 300>tmpfile
cat tmpfile
$TEST 1000 1000>tmpfile
cat tmpfile
$TEST 1 123a456>tmpfile
cat tmpfile
$TEST 1 123 a456>tmpfile
cat tmpfile
$TEST 1-123>tmpfile
cat tmpfile
$TEST 1 -123>tmpfile
cat tmpfile
$TEST 10000 10000>tmpfile
cat tmpfile
rm tmpfile
