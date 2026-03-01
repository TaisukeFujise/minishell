#!/usr/bin/env bash
set -euo pipefail

usage() {
	cat <<'EOF'
Usage:
  ./test/unit_test/parser/manual/gen_stress_case.sh <type> [size] [--copy]

Types:
  nested_subshell   Generate deeply nested subshell input.          (default size: 2000)
  compound_list     Generate subshell with many newline commands.   (default size: 2000)
  andor_chain       Generate long and/or chain.                     (default size: 2000)
  pipeline_chain    Generate long pipeline chain.                   (default size: 500)
  heredoc_max_over  Generate HEREDOC_MAX + 1 line (17 here-docs).   (size ignored)

Options:
  --copy            Copy generated input to clipboard (wl-copy/xclip/pbcopy).

Examples:
  ./test/unit_test/parser/manual/gen_stress_case.sh nested_subshell 4000
  ./test/unit_test/parser/manual/gen_stress_case.sh compound_list 3000 --copy
EOF
}

copy_clipboard() {
	file="$1"
	if command -v wl-copy >/dev/null 2>&1; then
		wl-copy < "$file"
		echo "Copied to clipboard using wl-copy."
	elif command -v xclip >/dev/null 2>&1; then
		xclip -selection clipboard < "$file"
		echo "Copied to clipboard using xclip."
	elif command -v pbcopy >/dev/null 2>&1; then
		pbcopy < "$file"
		echo "Copied to clipboard using pbcopy."
	else
		echo "Clipboard tool not found (wl-copy/xclip/pbcopy)."
	fi
}

if [ "${1:-}" = "" ] || [ "${1:-}" = "-h" ] || [ "${1:-}" = "--help" ]; then
	usage
	exit 0
fi

TYPE="$1"
SIZE="${2:-}"
COPY_FLAG="${3:-}"
if [ "$SIZE" = "--copy" ]; then
	COPY_FLAG="$SIZE"
	SIZE=""
fi

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$(cd "$SCRIPT_DIR/../../../.." && pwd)"
OUT_DIR="$SCRIPT_DIR/generated"
mkdir -p "$OUT_DIR"

gen_nested_subshell() {
	n="${1:-2000}"
	{
		i=0
		while [ "$i" -lt "$n" ]; do
			printf "("
			i=$((i + 1))
		done
		printf "echo x"
		i=0
		while [ "$i" -lt "$n" ]; do
			printf ")"
			i=$((i + 1))
		done
		printf "\n"
	} > "$2"
}

gen_compound_list() {
	n="${1:-2000}"
	{
		printf "(\n"
		i=0
		while [ "$i" -lt "$n" ]; do
			printf "echo x\n"
			i=$((i + 1))
		done
		printf ")\n"
	} > "$2"
}

gen_andor_chain() {
	n="${1:-2000}"
	{
		printf "echo 0"
		i=1
		while [ "$i" -lt "$n" ]; do
			if [ $((i % 2)) -eq 0 ]; then
				printf " && echo %d" "$i"
			else
				printf " || echo %d" "$i"
			fi
			i=$((i + 1))
		done
		printf "\n"
	} > "$2"
}

gen_pipeline_chain() {
	n="${1:-500}"
	{
		printf "echo 0"
		i=1
		while [ "$i" -lt "$n" ]; do
			printf " | echo %d" "$i"
			i=$((i + 1))
		done
		printf "\n"
	} > "$2"
}

gen_heredoc_max_over() {
	{
		printf "cat"
		for d in a b c d e f g h i j k l m n o p q; do
			printf " <<%s" "$d"
		done
		printf "\n"
	} > "$1"
}

case "$TYPE" in
	nested_subshell)
		: "${SIZE:=2000}"
		OUT_FILE="$OUT_DIR/${TYPE}_${SIZE}.txt"
		gen_nested_subshell "$SIZE" "$OUT_FILE"
		;;
	compound_list)
		: "${SIZE:=2000}"
		OUT_FILE="$OUT_DIR/${TYPE}_${SIZE}.txt"
		gen_compound_list "$SIZE" "$OUT_FILE"
		;;
	andor_chain)
		: "${SIZE:=2000}"
		OUT_FILE="$OUT_DIR/${TYPE}_${SIZE}.txt"
		gen_andor_chain "$SIZE" "$OUT_FILE"
		;;
	pipeline_chain)
		: "${SIZE:=500}"
		OUT_FILE="$OUT_DIR/${TYPE}_${SIZE}.txt"
		gen_pipeline_chain "$SIZE" "$OUT_FILE"
		;;
	heredoc_max_over)
		OUT_FILE="$OUT_DIR/${TYPE}.txt"
		gen_heredoc_max_over "$OUT_FILE"
		;;
	*)
		echo "Unknown type: $TYPE"
		usage
		exit 1
		;;
esac

if [ "$COPY_FLAG" = "--copy" ]; then
	copy_clipboard "$OUT_FILE"
fi

REL_OUT="${OUT_FILE#$PROJECT_DIR/}"
echo "Generated: $OUT_FILE"
echo
echo "Quick run (from project root):"
echo "./test/unit_test/parser/parser_dump_line \"\$(cat $REL_OUT)\""
echo
echo "Quick copy:"
echo "cat $REL_OUT"
