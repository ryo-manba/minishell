rm -f hello
echo "#!/bin/bash" > hello
echo "echo hello world" >> hello
chmod a+x hello
$EX -c './hello'
$EX -c 'hello'
$EX -c 'export PATH=; hello'
$EX -c 'unset PATH; hello'
$EX -c './hell'
$EX -c 'hell'
$EX -c 'export PATH=; hell'
$EX -c 'unset PATH; hell'
$EX -c 'export PATH=; ./hell'
$EX -c 'unset PATH; ./hell'
