echo
echo a
echo a b
echo a  b
echo
echo a  
echo 'a  b'
echo "a  b"
echo 'a  '
echo "a  "
export COMMAND=echo
$COMMAND
echo @@@@@@@@@@@@@@@@@@@@@@@@@@@
$COMMAND a
${COMMAND} a
export COMMAND=ech
${COMMAND}o a
echo @@@@@@@@@@@@@@@@@@@@@@@@@@@
echo *
export VAR="o  p"
echo $VAR
echo "$VAR"
echo a${VAR}b
ech$VAR
export COMMAND=ho
ec$COMMAND a
ec${COMMAND} a
export COMMAND="ho a"
ec$COMMAND
ec${COMMAND}
