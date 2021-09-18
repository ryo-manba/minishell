env | grep -v -E '^_=' | grep -v -E 'EX' | sort
export xxxx yyyy=zzzz
env | grep -v -E '^_=' | grep -v -E 'EX' | sort
unset xxxx yyyy
env | grep -v -E '^_=' | grep -v -E 'EX' | sort
