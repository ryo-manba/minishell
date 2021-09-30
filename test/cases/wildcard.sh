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
