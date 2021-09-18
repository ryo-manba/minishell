export | grep -v _= | grep -v SHELL=
export VAR
export | grep -v _= | grep -v SHELL=
export VAR=test
export | grep -v _= | grep -v SHELL=
export VAR
export | grep -v _= | grep -v SHELL=
export SPC="te    st"
export | grep -v _= | grep -v SHELL=
export VAR=$SPC
export | grep -v _= | grep -v SHELL=
export $SPC
export | grep -v _= | grep -v SHELL=
export VAR='export BAR=$SPC'
export | grep -v _= | grep -v SHELL=
$VAR
export | grep -v _= | grep -v SHELL=
export VAR='export BAR'
export | grep -v _= | grep -v SHELL=
$VAR=$SPC
export | grep -v _= | grep -v SHELL=
export QUOTES='""'"''"'""'
export | grep -v _= | grep -v SHELL=
export VAR='$X' BAR='$' CAR='$$'
export | grep AR=
