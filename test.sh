#!/bin/bash

# テストするときは、一時的にプロンプトを表示しないようにする.
# 方法は、 `input = readline(NULL);` みたいにすること.
# テスト方法
## chmod +x test.sh
## ./test.sh

RED="\033[31m"
GREEN="\033[32m"
YELLOW="\033[33m"
RESET="\033[0m"
OK=$GREEN"OK"$RESET
NG=$RED"NG"$RESET

# 参考にしたコード1: https://github.com/usatie/minishell/blob/submit/test.sh
# 参考にしたコード2: https://github.com/t0mm4rx/minishell_tests

# Manual Test
# $ ./minishell
# $
# 1. Ctrl-\
# 2. Ctrl-C
# 3. Ctrl-D
#
# $ ./minishell
# $ hogehoge
# 1. Ctrl-\
# 2. Ctrl-C
# 3. Ctrl-D
#
# $ ./minishell
# $ cat <<EOF
# >
# 1. Ctrl-\
# 2. Ctrl-C
# 3. Ctrl-D
#
# $ ./minishell
# $ cat <<EOF
# > hoge
# > fuga
# 1. Ctrl-\
# 2. Ctrl-C
# 3. Ctrl-D
#
# echo hello ; ls

cat <<EOF | gcc -xc -o a.out -
#include <stdio.h>
int main() { printf("hello from a.out\n"); }
EOF

cat <<EOF | gcc -xc -o print_args -
#include <stdio.h>
int main(int argc, char *argv[]) {
	for (int i = 0; argv[i]; i++)
		printf("argv[%d] = [%s]\n", i, argv[i]);
}
EOF

cat <<EOF | gcc -xc -o exit42 -
int main() { return 42; }
EOF

print_desc() {
  echo -e $YELLOW"$1"$RESET
}

cleanup() {
  rm -f cmp out a.out print_args exit42 infinite_loop no_exec_perm no_read_perm empty_file file error.log test
}

assert() {
  COMMAND="$1"
  shift
  printf '%-70s:' "[$COMMAND]"
  # exit status
  echo -n -e "$COMMAND" | bash >cmp 2>&-
  expected=$?
  for arg in "$@"; do
    mv "$arg" "$arg"".cmp"
  done
  echo -n -e "$COMMAND" | ./minishell >out 2>&-
  actual=$?
  for arg in "$@"; do
    mv "$arg" "$arg"".out"
  done

  if diff out cmp >/dev/null; then
    echo -e -n "  diff $OK"
  else
    echo -e -n "  diff $NG"
    printf '%-70s:\n' "[$COMMAND]" >>error.log
    diff -U 1 out cmp >>error.log
  fi

  if [ "$actual" = "$expected" ]; then
    echo -e -n "  status $OK"
  else
    echo -e -n "  status $NG, expected $expected but got $actual"
    printf '%-70s:' "[$COMMAND]" >>error.log
    echo "status NG, expected $expected but got $actual" >>error.log
  fi
  for arg in "$@"; do
    echo -n "  [$arg] "
    diff "$arg"".cmp" "$arg"".out" >/dev/null && echo -e -n "$OK" || echo -e -n "$NG"
    rm -f "$arg"".cmp" "$arg"".out"
  done
  echo
}

rm -f error.log
cleanup

# Empty, spaces anc tabs
assert ''
assert '      '
assert ' \t'

# Absolute commands without arguments
assert '/bin/pwd'
assert '/bin/echo'
assert '/bin/ls'

# Absolute commands with arguments (but without quotes)
assert '/bin/echo hello world'
assert '/bin/ls /etc /var'
assert '/bin/expr 1 + 1'
assert '/bin/ls nosuchfile'

# command not found
assert "a.out"
assert "nosuchfileexists"
assert '""'
assert "''"
assert '..'
assert '$'

# Is a directory
assert './'
assert '/'
assert '/etc'
assert '/etc/'
assert '////'

# Syntax error
assert 'export HELLO="hello world"\ncat < $HELLO'
assert 'export EMPTY=""\ncat < $EMPTY'
assert '|'
assert '| |'
assert 'echo hello | |'
assert 'echo >'
assert 'echo bonjour >>> test'

# $PATH にコロン区切りで複数のディレクトリが指定された場合、シェルは左から右へ、正しい順番でコマンドを探しているか
mkdir /tmp/dir1 /tmp/dir2
echo '#!/bin/bash' >/tmp/dir1/test_cmd
echo 'echo "Hello from dir1"' >>/tmp/dir1/test_cmd
echo '#!/bin/bash' >/tmp/dir2/test_cmd
echo 'echo "Hello from dir2"' >>/tmp/dir2/test_cmd
chmod +x /tmp/dir1/test_cmd /tmp/dir2/test_cmd
assert 'unset $PATH\nexport PATH="/tmp/dir1:/tmp/dir2"\ntest_cmd'
assert 'unset $PATH\nexport PATH="/tmp/dir2:/tmp/dir1"\ntest_cmd'

# Permission denied
echo "int main() { }" | gcc -xc -o no_exec_perm -
chmod -x no_exec_perm
assert 'no_exec_perm'
assert './no_exec_perm'
echo "int main() { }" | gcc -xc -o no_read_perm -
chmod -r no_read_perm
assert 'no_read_perm'
assert './no_read_perm'

mkdir -p /tmp/a /tmp/b
echo "int main() { return 1; }" | gcc -xc -o /tmp/a/simple_test -
echo "int main() { return 2; }" | gcc -xc -o /tmp/b/simple_test -

print_desc "/tmp/a /tmp/b both with permission"
assert 'unset PATH\nexport PATH="/tmp/a:/tmp/b"\nsimple_test'
assert 'unset PATH\nexport PATH="/tmp/b:/tmp/a"\nsimple_test'

print_desc "/tmp/a /tmp/b both without permission"
chmod -x /tmp/a/simple_test
chmod -x /tmp/b/simple_test
assert 'unset PATH\nexport PATH="/tmp/a:/tmp/b"\nsimple_test'
assert 'unset PATH\nexport PATH="/tmp/b:/tmp/a"\nsimple_test'

print_desc "a with permission, b without permission"
chmod +x /tmp/a/simple_test
chmod -x /tmp/b/simple_test
assert 'unset PATH\nexport PATH="/tmp/a:/tmp/b"\nsimple_test'
assert 'unset PATH\nexport PATH="/tmp/b:/tmp/a"\nsimple_test'

print_desc "a without permission, b with permission"
chmod -x /tmp/a/simple_test
chmod +x /tmp/b/simple_test
assert 'unset PATH\nexport PATH="/tmp/a:/tmp/b"\nsimple_test'
assert 'unset PATH\nexport PATH="/tmp/b:/tmp/a"\nsimple_test'

# Tokenize
## unquoted word
assert 'ls /'
assert 'echo hello    world     '
assert 'nosuchfile\n\n'

## single quote
assert "./print_args 'hello   world' '42Tokyo'"
assert "echo 'hello   world' '42Tokyo'"
assert "echo '\"hello   world\"' '42Tokyo'"

## double quote
assert './print_args "hello   world" "42Tokyo"'
assert 'echo "hello   world" "42Tokyo"'
assert "echo \"'hello   world'\" \"42Tokyo\""

# Redirect
## Redirecting output
assert 'echo "hello world" > hello.txt'
assert 'echo hello >hello.txt' 'hello.txt'
assert 'echo hello >f1>f2>f3' 'f1' 'f2' 'f3'
assert 'echo test0 > file test1'
assert 'not_cmd hello > empty_file'

## Redirecting input
assert 'cat <Makefile'
echo hello >f1
echo world >f2
echo 42Tokyo >f3
assert 'cat <f1<f2<f3'
rm -f f1 f2 f3
assert 'cat <hoge'
assert 'touch test\n< test'
assert '>> test'

## Appending Redirected output
assert 'pwd >>pwd.txt' 'pwd.txt'
assert 'pwd >>pwd.txt \n pwd >>pwd.txt' 'pwd.txt'

## Here Document
# assert 'cat <<EOF\nhello\nworld\nEOF\nNOPRINT'
# assert 'cat <<EOF<<eof\nhello\nworld\nEOF\neof\nNOPRINT'
# assert 'cat <<EOF\nhello\nworld'
# assert 'cat <<E"O"F\nhello\nworld\nEOF\nNOPRINT'
# assert 'cat <<EOF   \n$USER\n$NO_SUCH_VAR\n$FOO$BAR\nEOF'
# assert 'cat <<"EOF" \n$USER\n$NO_SUCH_VAR\n$FOO$BAR\nEOF'
# assert 'cat <<EO"F" \n$USER\n$NO_SUCH_VAR\n$FOO$BAR\nEOF'
# (
# 	print_desc 'export EOF="eof"'
# 	export EOF="eof"
# 	assert 'cat <<$EOF         \neof\n$EOF\nEOF'
# 	assert 'cat <<"$EOF"       \neof\n$EOF\nEOF'
# )

# Pipe
assert 'cat Makefile | grep minishell'
assert 'cat | cat | ls\n\n'
assert 'echo bin/magic | tr a-z A-Z'
## 約100個のコマンドをパイプで繋ぐテスト---
NUM_PIPES=100
print_desc "Test with a lot of commands connected by pipeline"
CMD="ls -l"
for i in $(seq 1 $NUM_PIPES); do
  CMD="$CMD | cat"
done
CMD="$CMD | wc -l"
assert "$CMD"

# Expand Variable
assert 'echo $USER'
assert 'echo $USER$PATH$TERM'
assert 'echo "$USER  $PATH   $TERM"'

# Special Parameter $?
assert 'echo $?'
assert 'invalid\nexpr $? + $?'
assert 'invalid\necho $?\necho $?'
## 改行で終了ステータスが0に更新されないことを確認
assert 'exit42\necho $?\necho $?'
assert 'exit42\n\necho $?\necho $?'

# Word Splitting
assert 'export FOO="echo hello"\n$FOO'
assert 'export TEST="cho -n"\ne$TEST'
assert 'export FOO="a       b"\necho $FOO'
assert 'export FOO="a       b"\necho hello$FOO'
assert 'export FOO="a       b"\necho $FOO"world"'
assert 'export FOO="a       b"\necho hello$FOO"world"'
assert 'export FOO="echo a      b"\n$FOO'
assert 'export FOO="          "\necho "$FOO"'

# Signal handling
echo "int main() { while (1) ; }" | gcc -xc -o infinite_loop -

## Signal to shell processes
print_desc "SIGTERM to SHELL"
(
  sleep 0.01
  pkill -SIGTERM bash
  sleep 0.01
  pkill -SIGTERM minishell
) &
assert './infinite_loop' 2>/dev/null # Redirect stderr to suppress signal terminated message

print_desc "SIGQUIT to SHELL"
(
  sleep 0.01
  pkill -SIGQUIT bash # SIGQUIT should not kill the shell
  sleep 0.01
  pkill -SIGTERM bash
  sleep 0.01
  pkill -SIGQUIT minishell # SIGQUIT should not kill the shell
  sleep 0.01
  pkill -SIGTERM minishell
) &
assert './infinite_loop' 2>/dev/null # Redirect stderr to suppress signal terminated message

print_desc "SIGINT to SHELL"
(
  sleep 0.01
  pkill -SIGINT bash # SIGINT should not kill the shell
  sleep 0.01
  pkill -SIGTERM bash
  sleep 0.01
  pkill -SIGINT minishell # SIGINT should not kill the shell
  sleep 0.01
  pkill -SIGTERM minishell
) &
assert './infinite_loop' 2>/dev/null # Redirect stderr to suppress signal terminated message

## Signal to child processes
print_desc "SIGTERM to child process"
(
  sleep 0.01
  pkill -SIGTERM infinite_loop
  sleep 0.01
  pkill -SIGTERM infinite_loop
) &
assert './infinite_loop'

print_desc "SIGINT to child process"
(
  sleep 0.01
  pkill -SIGINT infinite_loop
  sleep 0.01
  pkill -SIGINT infinite_loop
) &
assert './infinite_loop'

print_desc "SIGQUIT to child process"
(
  sleep 0.01
  pkill -SIGQUIT infinite_loop
  sleep 0.01
  pkill -SIGQUIT infinite_loop
) &
assert './infinite_loop'

print_desc "SIGUSR1 to child process"
(
  sleep 0.01
  pkill -SIGUSR1 infinite_loop
  sleep 0.01
  pkill -SIGUSR1 infinite_loop
) &
assert './infinite_loop'

# Builtin
## exit
assert 'exit'
assert 'exit 42'
assert 'exit -42'
assert 'exit +42'
assert 'exit ""'
assert 'exit hello'
assert 'exit 42Tokyo'
assert 'exit 1 2'
assert 'exit 9223372036854775806'
assert 'exit 9223372036854775807'
assert 'exit 9223372036854775808'
assert 'exit -9223372036854775807'
assert 'exit -9223372036854775808'
assert 'exit -9223372036854775809'

## export
print_desc "Output of 'export' differs, but it's ok."
assert 'export' # order of variables, default variables differs...
assert 'export | grep nosuch | sort'
assert 'export nosuch\n export | grep nosuch | sort'
assert 'export nosuch=fuga\n export | grep nosuch | sort'
assert 'export nosuch=fuga hoge=nosuch\n export | grep nosuch | sort'
assert 'export [invalid]'
assert 'export [invalid_nosuch]\n export | grep nosuch | sort'
assert 'export [invalid]=nosuch\n export | grep nosuch | sort'
assert 'export [invalid] nosuch hoge=nosuch\n export | grep nosuch | sort'
assert 'export nosuch [invalid] hoge=nosuch\n export | grep nosuch | sort'
assert 'export nosuch hoge=nosuch [invalid]\n export | grep nosuch | sort'
assert 'export nosuch="nosuch2=hoge"\nexport $nosuch\n export | grep nosuch | sort'

## unset
(
  print_desc 'export hoge fuga=fuga'
  export hoge fuga=fuga
  assert 'unset'
  assert 'unset hoge'
  assert 'unset fuga'
  assert 'unset nosuch'
  assert 'unset [invalid]'
  assert 'unset hoge fuga'
  assert 'unset hoge nosuch fuga'
  assert 'unset fuga \n export | echo $fuga'
  assert 'unset [invalid] fuga \n echo $fuga'
)

## env
print_desc "Output of 'env' differs, but it's ok."
assert 'env' # order of variables, default variables differs...
assert 'env | grep hoge | sort'

## cd
assert 'cd'
assert 'cd .'
assert 'cd ..'
assert 'cd ///'
assert 'cd /tmp'
assert 'cd /tmp/'
assert 'cd /tmp///'
assert 'cd /../../../././.././'
assert 'cd src'
assert 'unset HOME\ncd'
assert '$ cd / | pwd'

assert 'cd $HOME/Documents'
assert 'cd \n echo $PWD'
assert 'cd \n echo $PWD'
assert 'cd .\n echo $PWD'
assert 'cd ..\n echo $PWD'
assert 'cd ///\n echo $PWD'
assert 'cd /tmp\n echo $PWD'
assert 'cd /tmp/\n echo $PWD'
assert 'cd /tmp///\n echo $PWD'
assert 'cd /../../../././.././\n echo $PWD'
assert 'cd src\n echo $PWD'
assert 'unset HOME\ncd \n echo $PWD'
assert 'unset OLDPWD\ncd -' # This test may not have to pass

## echo
assert 'echo'
assert 'echo hello'
assert 'echo hello "    " world'
assert 'echo -n'
assert 'echo -n hello'
assert 'echo -n hello world'
assert 'echo hello -n'
assert 'echo -nn'
assert 'echo -n-n-n'
assert 'echo -n -n -nnnn -nnnnm'
assert 'echo ";|()"'

## pwd
assert 'pwd'
assert 'cd\npwd'
assert 'cd src\npwd'
assert 'cd /etc\npwd'
assert 'cd . \n pwd \n echo $PWD $OLDPWD'
assert 'cd .. \n pwd \n echo $PWD $OLDPWD'
assert 'cd /// \n pwd \n echo $PWD $OLDPWD'
assert 'cd /tmp/// \n pwd \n echo $PWD $OLDPWD'
assert 'unset PWD\npwd\ncd /etc\npwd'

## export attribute
assert 'unset PWD \n cd \n echo $PWD \ncd /tmp\necho $PWD'
assert 'unset PWD\ncd\necho $OLDPWD\ncd /tmp\necho $OLDPWD'
assert 'unset PWD\ncd\nexport|grep PWD\ncd /tmp\nexport|grep PWD'
assert 'unset PWD\ncd\nenv|grep PWD\ncd /tmp\nenv|grep PWD'
assert 'unset PWD\ncd\nexport PWD\nexport|grep PWD\ncd /tmp\nexport|grep PWD'
assert 'unset PWD\ncd\nexport PWD\nenv|grep PWD\ncd /tmp\nenv|grep PWD'


cleanup

if [ -f error.log ]; then
  echo -e $RED"Some tests have failed. Please review the error.log for more information."$RESET
fi