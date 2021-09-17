> tmpfile1 | file tmpfile1
>> tmpfile2 | file tmpfile2
echo hello > tmpfile1 | echo world > tmpfile2
cat tmpfile1 tmpfile2
cat < tmpfile1
pwd > tmpfile2 >> tmpfile1
cat tmpfile1
ls > tmpfile1 | ls >> tmpfile1
cat < tmpfile1
ls > tmpfile1 > tmpfile2 > tmpfile3 | file tmpfile1 tmpfile2 tmpfile3 | rm tmpfile1 tmpfile2 tmpfile3
aaa > tmpfile1 | bbb > tmpfile2 | ccc > tmpfile3 | file tmpfile1 tmpfile2 tmpfile3 | rm tmpfile1 tmpfile2 tmpfile3
ls 1>tmpfile1 2>tmpfile2 | cat tmpfile1 | rm tmpfile1 tmpfile2
ls 1>tmpfile1 10240>tmpfile2  2>tmpfile3 | file tmpfile1 tmpfile2 tmpfile3 | rm tmpfile1 tmpfile2 tmpfile3
ls 1>tmpfile1 2147483648>tmpfile2 > tmpfile3 | cat tmpfile1 | rm tmpfile1 tmpfile2 tmpfile3
