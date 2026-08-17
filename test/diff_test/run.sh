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
missing_diagnostic
unset_path
exit_prints
heredoc
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
run_case missing_status 'nosuchcmd_xyz'

# Not implemented yet. This case compares the message, not just the status:
# both shells name the command they could not run.
run_case missing_diagnostic 'nosuchcmd_xyz 2>err.txt
grep -c nosuchcmd_xyz err.txt'
run_case unset_path 'unset PATH
ls'
run_case exit_prints 'exit 7'
# minishell loops forever on a heredoc that EOF ends: parse() reports the
# failure without consuming the input and the main loop parses it again.
run_case heredoc 'cat << EOF
hello
EOF'

rm -rf "$WORK"
printf '\npass %d  fail %d  xfail %d\n' "$PASS" "$FAIL" "$XFAIL"
[ "$FAIL" -eq 0 ]
