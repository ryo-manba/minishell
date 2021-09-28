(ls) | (cat)
(ls) | cat
ls | (cat)
(asdf) || (ls)
(ls) && (asdf)
(asdf || pwd && ls)
(pwd && ls || cat)
pwd && (cd ..) && pwd
pwd && (cd .. && pwd)
pwd && (cd .. && pwd && cd ../../ && pwd)
(export TEST=xxxx && echo $TEST)
(export TEST=xxxx) && echo $TEST
(export TEST=xxxx && unset TEST && echo $TEST)
(pwd && asdf || cd .. && pwd)
(adsf && pwd || ls)
(asdf || pwd && ls)
(asdf) || (asdf) || (asdf) || (asdf) || (adsf)
(ls) && (ls) && (ls) && (ls) && (ls)
(echo hello | cat | cat) && ls
(ls | cat | cat) || (ls && pwd)
(ls || cat) && (pwd) || (ls)
(ls > tmpfile1 && ls >> tmpfile1)
(cat < tmpfile1 && rm tmpfile1)
(cat < tmpfile1 && cat < tmpfile1) && rm tmpfile1