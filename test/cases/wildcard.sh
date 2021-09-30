rm -rf wdir
mkdir wdir
touch wdir/a wdir/b wdir/c wdir/apple wdir/banana wdir/pineapple
cd wdir
echo *
echo a*
echo *
echo ***
chmod a-r .
echo *
echo a*
echo *
echo ***
chmod a+r .
echo "*"
echo '*'
echo * *
cd ..
echo *.c
echo "*".c
echo *."c"
echo *".c"
echo "*"*
echo *"*"
echo ."*"
echo "."**
echo ".."*
export wildcard="*"
echo $wildcard
echo "$wildcard"
echo '$wildcard'
echo "'$wildcard'"
echo '"$wildcard"'
echo $wildcard.c
echo .$wildcard
echo ${wildcard}c
