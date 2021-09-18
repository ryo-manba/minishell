export CDTEST=$PWD
pwd
cd ./ ; pwd
cd ../ ; pwd
cd ../../ ; pwd
cd ./.././.././.././.././.././../ ; pwd
cd .///..///.///..///./////..//./ ; pwd
cd $CDTEST ; pwd
cd $CDTEST/../ ; pwd
cd $NO_SUCH_ENV ; pwd
cd ; pwd
mkdir tmpdir1 tmpdir1/tmpdir2 tmpdir1/tmpdir2/tmpdir3/
cd tmpdir1 ; pwd
cd tmpdir2/tmpdir3 ; pwd ;
cd ; rm -rf tmpdir1
mkdir tmpdir ; cd tmpdir ; rm -rf ../tmpdir ; cd ./ ; pwd
cd ././././////.//./ ; pwd ; cd
mkdir tmpdir ; chmod 000 tmpdir ; cd tmpdir ;
chmod 777 tmpdir ; rm -rf tmpdir
cd nosuchdir
cd /A/B/C/D/E/F/G
unset CDTEST
