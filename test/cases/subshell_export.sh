(export TEST=xxxx && echo $TEST)
(export TEST=xxxx) && echo $TEST
(export TEST=xxxx && unset TEST && echo $TEST)
(export PWD=xxxx && echo $PWD) && echo $PWD
(export PWD=xxxx && unset PWD && echo $PWD) && echo $PWD
(unset PWD && echo $PWD) && echo $PWD