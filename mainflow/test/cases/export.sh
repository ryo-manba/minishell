export | grep -v _=
export VAR
export | grep -v _=
export VAR=test
export | grep -v _=
export VAR
export | grep -v _=
export SPC="te st"
export | grep -v _=
export VAR=$SPC
export | grep -v _=
export $SPC
export | grep -v _=
export VAR='export BAR=$SPC'
export | grep -v _=
$VAR
export | grep -v _=
export VAR='export BAR'
export | grep -v _=
$VAR=$SPC
export | grep -v _=
export QUOTES='""'"''"'""'
export | grep -v _=
export VAR='$X' BAR='$' CAR='$$'
export | grep AR=
