#!/bin/sh
# Compare minishell against bash for a fixed set of cases.
#
# usage: sh test/diff_test/run.sh [path/to/minishell]
#
# Each case runs in its own empty work directory, so a case may create files.
# We compare stdout, exit status and the names of the files left behind.
# A case listed in EXPECTED_FAIL is known to differ; it is reported as XFAIL
# and does not fail the run. Remove it from the list once it is fixed.

SHELL_UNDER_TEST=${1:-./minishell}
case $SHELL_UNDER_TEST in
	/*) ;;
	*) SHELL_UNDER_TEST=$(pwd)/$SHELL_UNDER_TEST ;;
esac
if [ ! -x "$SHELL_UNDER_TEST" ]; then
	printf 'not executable: %s\n' "$SHELL_UNDER_TEST"
	exit 1
fi
BASH_BIN=${BASH_BIN:-/bin/bash}
# A case that hangs must not hang the run. Not every system has timeout(1).
TIMEOUT=""
command -v timeout >/dev/null 2>&1 && TIMEOUT="timeout 10"
WORK=$(mktemp -d)
PASS=0
FAIL=0
XFAIL=0

# Cases that minishell is not expected to pass yet.
EXPECTED_FAIL="
heredoc_unclosed
"

run_case()
{
	name=$1
	script=$2
	rm -rf "$WORK/$name"
	mkdir -p "$WORK/$name/ms" "$WORK/$name/bash"

	ms_out=$(cd "$WORK/$name/ms" && printf '%s\nexit\n' "$script" \
		| $TIMEOUT "$SHELL_UNDER_TEST" 2>/dev/null)
	ms_rc=$?
	ms_files=$(cd "$WORK/$name/ms" && ls -A | LC_ALL=C sort | tr '\n' ' ')

	bash_out=$(cd "$WORK/$name/bash" && printf '%s\nexit\n' "$script" \
		| "$BASH_BIN" --norc --noprofile 2>/dev/null)
	bash_rc=$?
	bash_files=$(cd "$WORK/$name/bash" && ls -A | LC_ALL=C sort | tr '\n' ' ')

	if [ "$ms_out" = "$bash_out" ] && [ "$ms_rc" = "$bash_rc" ] \
		&& [ "$ms_files" = "$bash_files" ]; then
		if echo "$EXPECTED_FAIL" | grep -qx "$name"; then
			printf 'XPASS %s (remove it from EXPECTED_FAIL)\n' "$name"
			FAIL=$((FAIL + 1))
		else
			printf 'PASS  %s\n' "$name"
			PASS=$((PASS + 1))
		fi
		return
	fi
	if echo "$EXPECTED_FAIL" | grep -qx "$name"; then
		printf 'XFAIL %s\n' "$name"
		XFAIL=$((XFAIL + 1))
		return
	fi
	FAIL=$((FAIL + 1))
	printf 'FAIL  %s\n' "$name"
	printf '  script : %s\n' "$script"
	printf '  stdout : %s | bash: %s\n' "$ms_out" "$bash_out"
	printf '  status : %s | bash: %s\n' "$ms_rc" "$bash_rc"
	printf '  files  : %s| bash: %s\n' "$ms_files" "$bash_files"
}

# Expansion and builtins.
run_case builtin_cd 'cd /tmp
pwd'
run_case builtin_export 'export ZZ=1
echo $ZZ'
run_case param 'echo $HOME'
run_case redirect_target 'echo done > out.txt
cat out.txt'
run_case redirect_keeps_fd 'echo hi 3>f1
echo second'
# An io number above the fixed backup base of a shell. dash loses stdout here.
run_case redirect_high_fd 'echo hi 12>f1
echo second'
# pwd writes through stdio, so its output is flushed after the redirect is
# put back and never reaches the file (review D37-23).
run_case builtin_stdio_buffer 'pwd > out.txt
grep -c / out.txt'
run_case builtin_out_order 'cd /
pwd
echo marker'
run_case builtin_out_not_doubled 'cd /
pwd
echo a | cat'
run_case builtin_out_subshell 'cd /
pwd
(echo a)'
run_case pwd_operand 'cd /
pwd ignored'
run_case env_operand 'env nosuchcmd_xyz
echo $?'
run_case env_runs_command 'env /bin/echo hi'
run_case export_out_order 'export ZZZ_ORDER=1
export | grep ZZZ_ORDER
echo marker'
run_case andor_true 'echo a && echo b'
run_case andor_false 'nosuchcmd_xyz || echo yes'
run_case pipe 'echo x | cat'
run_case export_no_value 'export FOO
env | grep -c "^FOO"
echo end'
run_case export_keeps_prefix 'unset A
A=one export A
echo "[$A]"'

# Simple command ordering. See the bash manual, Simple Command Expansion.
run_case assign_prefix_arg 'X=old
X=new printf %s "$X"'
run_case assign_left_to_right 'unset A B
A=one B=$A env | grep -E "^(A|B)="'
run_case assign_no_split 'IFS=:
V=a::b
X=$V env | grep "^X="'
run_case prefix_path 'PATH=/missing ls'
run_case redirect_before_search 'definitely_missing_xyz > made.txt'
run_case no_command_redirect 'X=old
X=/dev/null > "$X"'
run_case lexical_assign 'EMPTY=
$EMPTY X=1'

run_case subshell '(echo sub)'
run_case subshell_redirect '(echo a) > out.txt
cat out.txt'
run_case subshell_redirect_pipe '(echo a) > out.txt | cat
cat out.txt'
run_case subshell_input 'echo a > in.txt
(cat) < in.txt'
run_case subshell_keeps_shell 'cd /tmp
(cd / && pwd)
pwd'
# The status of a subshell that is the right hand side of a pipe. It is lost
# if the child waits for the pids it inherited from the shell (review S43-05).
run_case subshell_status 'echo a | (nosuchcmd_xyz)
echo "rc=$?"'
run_case missing_status 'nosuchcmd_xyz'

# Not implemented yet. This case compares the message, not just the status:
# both shells name the command they could not run.
run_case missing_diagnostic 'nosuchcmd_xyz 2>err.txt
grep -c nosuchcmd_xyz err.txt'
run_case unset_path 'unset PATH
ls'
run_case path_empty_element 'echo x > mycmd
PATH=:/nonexistent
mycmd'
run_case path_not_executable 'echo x > notexec
PATH=.
notexec'
run_case exec_a_directory '/tmp'
run_case exit_prints 'exit 7'
run_case exit_bad_arg 'exit abc'
run_case exit_numeric_first 'exit abc 1'
run_case exit_too_many 'exit 1 2
echo survived $?'
run_case exit_spaces 'exit " 42 "'
run_case exit_keeps_status 'false
exit'
run_case exit_redirect 'exit 0 > out.txt
echo unreachable'
# The status of a syntax error. bash ends a script there; this shell has no
# script mode and goes on to the next line, as bash does when interactive.
run_case syntax_status 'echo a >'
run_case empty_command '""
echo rc=$?'
run_case digit_last_word 'echo 7
nosuchcmd_xyz 12
echo done'
# A pipeline stage runs in a child, so what it assigns cannot reach the
# shell. bash forks before it expands the words for this reason.
run_case pipe_assign_leak 'unset X
echo a | X=5
echo "[$X]"'
run_case heredoc 'cat << EOF
hello
EOF'
run_case heredoc_expand 'V=x
cat << EOF
v=$V
EOF'
run_case heredoc_pipe 'cat << EOF | cat
body
EOF'
# bash warns and runs the command with what it read; the parser here makes
# it a syntax error instead. The subject does not say which.
run_case heredoc_unclosed 'cat << EOF
body'
run_case stdin_readahead 'cat
AFTER'

rm -rf "$WORK"
printf '\npass %d  fail %d  xfail %d\n' "$PASS" "$FAIL" "$XFAIL"
[ "$FAIL" -eq 0 ]
