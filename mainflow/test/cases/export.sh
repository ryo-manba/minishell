export | grep -v _= | grep -v EX=
export VAR
export | grep -v _= | grep -v EX=
export VAR=test
export | grep -v _= | grep -v EX=
export VAR
export | grep -v _= | grep -v EX=
export SPC="te    st"
export | grep -v _= | grep -v EX=
export VAR=$SPC
export | grep -v _= | grep -v EX=
export $SPC
export | grep -v _= | grep -v EX=
export VAR='export BAR=$SPC'
export | grep -v _= | grep -v EX=
$VAR
export | grep -v _= | grep -v EX=
export VAR='export BAR'
export | grep -v _= | grep -v EX=
$VAR=$SPC
export | grep -v _= | grep -v EX=
export QUOTES='""'"''"'""'
export | grep -v _= | grep -v EX=
export VAR='$X' BAR='$' CAR='$$'
export | grep AR=
export com="export"
export c="a  b"
export a="a"$c'b'
$com b="a"$c'b'
export | grep -v _= | grep -v EX=
echo x | $com b="a"$c'b'
export | grep -v _= | grep -v EX=
export com="echo"
$com b="a"$c'b'
export | grep -v _= | grep -v EX=
