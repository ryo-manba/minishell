given_case=$1
MINISHELL="./minishell"
TEST_DIR="test/"
CASE_DIR="${TEST_DIR}cases/"
EXPECTED_DIR="${TEST_DIR}expected/"
RESULTFILE=$TEST_DIR"result.txt"

function print_case() {
	echo -e "\033[36m[CASE: ${1}]\033[m"
}

function compare_evidence() {
	diff -u ${TEST_DIR}mine.txt ${TEST_DIR}ref.txt
}

function clear_evidence() {
	rm -f ${TEST_DIR}mine.txt ${TEST_DIR}ref.txt
}

function print_result() {
	{ test_stat=$?; } 2>/dev/null
	# { set +x; } 2>/dev/null
	if [ $test_stat -eq 0 ]; then
		echo -e "\033[32m[ OK ]\033[m $1"
		echo [ OK ] "$(date -R)	$1" >> $RESULTFILE
	else
		echo -e "\033[31m[FAIL] $1\033[m"
		echo "[FAIL] "$(date -R)"	$1" >> $RESULTFILE
	fi
	# set -x
}

function run_case() {
	if [ "$given_case" != "" -a "$given_case" != $1 ]; then
		return 0
	fi
	clear_evidence
	# print_case $1
	# set -x
	$MINISHELL ${CASE_DIR}$1.sh > ${TEST_DIR}mine.txt 2>&1
	diff -u ${TEST_DIR}mine.txt ${EXPECTED_DIR}$1.expected
	print_result $1
	# { set +x; } 2>/dev/null
}

# use bash for reference
function run_case_bash_file() {
	if [ "$given_case" != "" -a "$given_case" != $1 ]; then
		return 0
	fi
	clear_evidence
	# print_case $1
	# set -x
	EX=$MINISHELL $MINISHELL $2 ${CASE_DIR}$1.sh > ${TEST_DIR}mine.txt 2>&1
	EX="bash" bash $2 ${CASE_DIR}$1.sh 2>&1 | sed s@^bash:@$MINISHELL:@ > ${TEST_DIR}ref.txt
	compare_evidence
	print_result $1
	# { set +x; } 2>/dev/null
}

# use bash for reference
function run_case_bash_stdin() {
	if [ "$given_case" != "" -a "$given_case" != $1 ]; then
		return 0
	fi
	clear_evidence
	# print_case $1
	# set -x
	cat ${CASE_DIR}$1.sh | $MINISHELL $2 > ${TEST_DIR}mine.txt 2>&1
	cat ${CASE_DIR}$1.sh | bash $2 2>&1 | sed s@^bash:@$MINISHELL:@ > ${TEST_DIR}ref.txt
	compare_evidence
	print_result $1
	# { set +x; } 2>/dev/null
}

# use bash for reference
function run_case_bash_c() {
	if [ "$given_case" != "" -a "$given_case" != $1 ]; then
		return 0
	fi
	clear_evidence
	# print_case $1
	# set -x
	export YOUR_SHELL=$MINISHELL
	$MINISHELL -c "$(cat ${CASE_DIR}$1.sh)" > ${TEST_DIR}mine.txt 2>&1
	export YOUR_SHELL=`whereis bash`
	bash -c "$(cat ${CASE_DIR}$1.sh)" 2>&1 | sed s@^bash:@$MINISHELL:@ > ${TEST_DIR}ref.txt
	compare_evidence
	print_result $1
	# { set +x; } 2>/dev/null
}

#
function run_case_line_by_c() {
	if [ "$given_case" != "" -a "$given_case" != $1 ]; then
		return 0
	fi
	clear_evidence
	# print_case $1
	# set -x
	(cat ${CASE_DIR}$1.sh | while read line
	do
	$MINISHELL -c "$line"
	echo $?
	done ) > ${TEST_DIR}mine.txt 2>&1
	(cat ${CASE_DIR}$1.sh | while read line
	do
	bash -c "$line"
	echo $?
	done ) 2>&1 | sed s@^bash:@$MINISHELL:@ > ${TEST_DIR}ref.txt
	compare_evidence
	print_result $1
	# { set +x; } 2>/dev/null
}

rm					$RESULTFILE
touch				$RESULTFILE
run_case_bash_file	tokenize_basic
run_case_bash_file	wildcard
run_case			unclosed_quote
run_case_bash_file	subshell_basic
run_case_bash_file	subshell_pipe
run_case_bash_file	subshell_fork
run_case_bash_file 	subshell_export
run_case_bash_file	subshell_and_or
run_case_bash_file	subshell_redir
run_case_bash_file	subshell_single_unit.sh
run_case_bash_file	semicolon
run_case_bash_file	andor
run_case_bash_file	expander
run_case_bash_file	ln_in_error_file
run_case_bash_stdin	ln_in_error_stdin
# run_case_bash_stdin	ln_in_error_stdin_i -i
run_case_bash_c		ln_in_error_c
run_case_bash_file	dollar_question
run_case			exit_arg
run_case			redir
run_case_bash_file	redir_basic
run_case_bash_file	redir_basic_in
run_case_bash_file	redir_io_number
run_case_bash_file	get_path
run_case_bash_file	echo
run_case_bash_c		bad_characters
run_case_bash_file	pipe
run_case_bash_file	pwd
run_case_bash_file	export
# run_case_bash_by_c	export_export
run_case_bash_file	cd
run_case_bash_file	unset
run_case_bash_file	env
run_case_bash_file	extreme
run_case_bash_file	fail_expander
run_case_line_by_c	fail_exit
run_case_bash_file	fail_redir_extreme
