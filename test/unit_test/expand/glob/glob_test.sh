#!/bin/bash
# Glob expansion regression test.
#
#   ./glob_test.sh            compare against glob_expected.txt   (exit 1 on diff)
#   ./glob_test.sh --update   regenerate glob_expected.txt
#   ./glob_test.sh --bash     compare against bash, gated by glob_bash_diff.txt
#
# Every case runs inside a throwaway fixture directory, so results do not depend
# on the host filesystem. Patterns may contain @FIX@, which is replaced by the
# fixture's absolute path on the way in and folded back on the way out.

set -u

HERE=$(cd -- "$(dirname -- "$0")" && pwd)
BIN=$HERE/../expand_dump_line
CASES=$HERE/glob_cases.txt
EXPECTED=$HERE/glob_expected.txt
BASH_DIFF=$HERE/glob_bash_diff.txt

MODE=check
case ${1-} in
	--update) MODE=update ;;
	--bash) MODE=bash ;;
	"") ;;
	*) echo "usage: $0 [--update|--bash]" >&2; exit 2 ;;
esac

if [ ! -x "$BIN" ]; then
	echo "error: $BIN not built. Run 'make' in test/unit_test/expand first." >&2
	exit 2
fi

build_fixture()
{
	mkdir -p "$1/a/b/c" "$1/ab/cd" "$1/xy" "$1/dir.d" "$1/.hid/sub"
	touch "$1/f1.c" "$1/f2.c" "$1/f.txt" "$1/abc" "$1/abcd" "$1/aXbXc" \
		"$1/.dot" "$1/.dotfile" "$1/a/b/c/deep.c" "$1/ab/cd/x.c" \
		"$1/xy/y.c" "$1/dir.d/z" "$1/.hid/sub/h.c"
	ln -s f1.c "$1/link.c"
	ln -s dir.d "$1/link.dir"
	# Dangling: a matcher that stats through the link would drop it.
	ln -s no_such_target "$1/broken.link"
	# Only reachable by escaping the fixture through '..'; no case may find it.
	touch "$1/../ESCAPED_MARKER"
}

# Fields produced by expand_dump_line for `echo <pattern>`, minus the "echo"
# itself, joined on one line.
run_minishell()
{
	# The timeout is a guard, not slack: an exponential matcher turns a
	# pathological '*' run into a hang rather than a wrong answer.
	timeout 10 "$BIN" "echo $1" 2>&1 \
		| sed -n '/^AFTER_EXPAND:/,$p' \
		| grep -o 'word="[^"]*"' \
		| sed 's/^word="//; s/"$//' \
		| tail -n +2 \
		| tr '\n' ' '
}

# The same fixed variables init_ctx() hard-codes in expand_dump.c, so that
# cases using $STAR compare against bash on equal footing.
run_bash()
{
	FOO="alpha beta" BAR=z EMPTY= STAR='*.c' \
		bash -c "shopt -u nullglob; printf '%s ' $1" 2>/dev/null
}

# WORK is kept outside FIX: scratch files inside the fixture would themselves
# be matched by patterns such as '.*'.
WORK=$(mktemp -d) || exit 2
FIX=$WORK/fixture
trap 'rm -rf "$WORK"' EXIT
mkdir -p "$FIX" || exit 2
build_fixture "$FIX"
cd "$FIX" || exit 2

report()
{
	while IFS= read -r line; do
		case $line in
			""|"#"*) continue ;;
		esac
		pat=${line//@FIX@/$FIX}
		got=$("$1" "$pat")
		got=${got% }
		printf '%s\t%s\n' "$line" "${got//$FIX/@FIX@}"
	done < "$CASES"
}

if [ "$MODE" = update ]; then
	report run_minishell > "$EXPECTED"
	echo "wrote $(grep -c . "$EXPECTED") cases to $EXPECTED"
	exit 0
fi

if [ "$MODE" = bash ]; then
	report run_minishell > "$WORK/mini"
	report run_bash > "$WORK/bash"
	diff -u "$WORK/bash" "$WORK/mini" > "$WORK/diff"
	# Divergences we know about and accept; anything else fails the run.
	if diff -q <(grep '^[-+][^-+]' "$WORK/diff" | sort) \
		<(grep -v "^#" "$BASH_DIFF" | grep . | sort) > /dev/null 2>&1; then
		echo "PASS: bash divergences match $BASH_DIFF"
		exit 0
	fi
	echo "FAIL: bash divergences changed. Expected ($BASH_DIFF) vs actual:"
	diff <(grep -v "^#" "$BASH_DIFF" | grep . | sort) \
		<(grep '^[-+][^-+]' "$WORK/diff" | sort)
	exit 1
fi

if [ ! -f "$EXPECTED" ]; then
	echo "error: $EXPECTED missing. Run '$0 --update' to create it." >&2
	exit 2
fi
report run_minishell > "$WORK/mini"
if diff -u "$EXPECTED" "$WORK/mini"; then
	echo "PASS: $(grep -c . "$EXPECTED") glob cases"
	exit 0
fi
echo "FAIL: output differs from $EXPECTED (see diff above)"
exit 1
