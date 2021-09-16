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
		echo "$(date "+%s")	$1" >> $RESULTFILE
	else
		echo -e "\033[31m[FAIL] $1\033[m"
		echo "FAIL	$1" >> $RESULTFILE
	fi
	# set -x
}

function run_case() {
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
	clear_evidence
	# print_case $1
	# set -x
	$MINISHELL ${CASE_DIR}$1.sh > ${TEST_DIR}mine.txt 2>&1
	bash ${CASE_DIR}$1.sh > ${TEST_DIR}ref.txt 2>&1
	compare_evidence
	print_result $1
	# { set +x; } 2>/dev/null
}

# use bash for reference
function run_case_bash_stdin() {
	clear_evidence
	# print_case $1
	# set -x
	cat ${CASE_DIR}$1.sh | $MINISHELL > ${TEST_DIR}mine.txt 2>&1
	cat ${CASE_DIR}$1.sh | bash 2>&1 | sed s@^bash:@$MINISHELL:@ > ${TEST_DIR}ref.txt
	compare_evidence
	print_result $1
	# { set +x; } 2>/dev/null
}

# use bash for reference
function run_case_bash_c() {
	clear_evidence
	# print_case $1
	# set -x
	$MINISHELL -c "$(cat ${CASE_DIR}$1.sh)" > ${TEST_DIR}mine.txt 2>&1
	bash -c "$(cat ${CASE_DIR}$1.sh)" 2>&1 | sed s@^bash:@$MINISHELL:@ > ${TEST_DIR}ref.txt
	compare_evidence
	print_result $1
	# { set +x; } 2>/dev/null
}

#
function run_case_bash_by_c() {
	clear_evidence
	# print_case $1
	# set -x
	bash ${CASE_DIR}$1.sh $MINISHELL > ${TEST_DIR}mine.txt 2>&1
	bash ${CASE_DIR}$1.sh bash 2>&1 | sed s@^bash:@$MINISHELL:@ > ${TEST_DIR}ref.txt
	compare_evidence
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
run_case			exit_arg
