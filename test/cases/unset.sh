export TEST1=test1 TEST2=test2 TEST3=test3
unset TEST1 ; export | grep TEST
unset TEST2 TEST3 ; export | grep TEST
export TEST1=test1 TEST2=test2 TEST3=test3
unset xxxx ; echo $?
unset xxxx TEST1 ; export | grep TEST
unset TEST2 xxxx ; export | grep TEST
unset xxxx TEST3 xxxx ; export | grep TEST
