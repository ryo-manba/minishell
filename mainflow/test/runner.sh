set -x
../minishell unclosed_quote.sh > mine.txt 2>&1
diff -u mine.txt unclosed_quote.expected
