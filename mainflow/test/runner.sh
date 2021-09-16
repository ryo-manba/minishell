MINISHELL="../minishell"
RESULTFILE="result.txt"
function print_case() {
	echo -e "\033[36m[CASE: ${1}]\033[m"
}

function print_result() {
	{ test_stat=$?; } 2>/dev/null
	# { set +x; } 2>/dev/null
	if [ $test_stat -eq 0 ]; then
		echo -e "\033[32m[ OK ]\033[m $1"
		echo "$(date "+%s")	$1" >> $RESULTFILE
	else
		echo -e "\033[31m[FAIL] $1\033[m"
		echo "FAIL	$1" >> $RESULTFILE
	fi
	# set -x
}

function run_case() {
	rm mine.txt
	# print_case $1
	# set -x
	$MINISHELL $1.sh > mine.txt 2>&1
	diff -u mine.txt $1.expected
	print_result $1
	# { set +x; } 2>/dev/null
}

# use bash for reference
function run_case_bash_file() {
	rm mine.txt ref.txt
	# print_case $1
	# set -x
	$MINISHELL $1.sh > mine.txt 2>&1
	bash $1.sh > ref.txt 2>&1
	diff -u mine.txt ref.txt
	print_result $1
	# { set +x; } 2>/dev/null
}

# use bash for reference
function run_case_bash_stdin() {
	rm mine.txt ref.txt
	# print_case $1
	# set -x
	cat $1.sh | $MINISHELL > mine.txt 2>&1
	cat $1.sh | bash 2>&1 | sed s@^bash:@$MINISHELL:@ > ref.txt
	diff -u mine.txt ref.txt
	print_result $1
	# { set +x; } 2>/dev/null
}

# use bash for reference
function run_case_bash_c() {
	rm mine.txt ref.txt
	# print_case $1
	# set -x
	$MINISHELL -c "$(cat $1.sh)" > mine.txt 2>&1
	bash -c "$(cat $1.sh)" 2>&1 | sed s@^bash:@$MINISHELL:@ > ref.txt
	diff -u mine.txt ref.txt
	print_result $1
	# { set +x; } 2>/dev/null
}

#
function run_case_bash_by_c() {
	rm mine.txt ref.txt
	# print_case $1
	# set -x
	bash $1.sh $MINISHELL > mine.txt 2>&1
	bash $1.sh bash 2>&1 | sed s@^bash:@$MINISHELL:@ > ref.txt
	diff -u mine.txt ref.txt
	print_result $1
	# { set +x; } 2>/dev/null
}

rm					$RESULTFILE
touch				$RESULTFILE
run_case			unclosed_quote
run_case_bash_file	ln_in_error_file
run_case_bash_stdin	ln_in_error_stdin
run_case_bash_c		ln_in_error_c
run_case_bash_file	dollar_question
run_case_bash_by_c	exit_arg
