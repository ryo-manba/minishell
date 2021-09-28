(ls > tmpfile1)
cat tmpfile1
(ls >> tmpfile1)
(cat < tmpfile1)
(ls > tmpfile1 > tmpfile2)
cat < tmpfile1 && cat tmpfile2
(ls > tmpfile1) > tmpfile2
cat < tmpfile1 && cat < tmpfile2
(ls >> tmpfile1 >> tmpfile2)
cat tmpfile1
cat tmpfile2
(cat < tmpfile1 && cat <tmpfile2)
(echo hello && echo world) > tmpfile1
cat tmpfile1
(echo hello > tmpfile1 && echo world > tmpfile2) > tmpfile3
cat tmpfile1
cat tmpfile2
cat tmpfile3
(cat | cat) < tmpfile1
(cat | cat) < tmpfile1 > tmpfile2 
cat tmpfile2
rm tmpfile1 tmpfile2 tmpfile3
