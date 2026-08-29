#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
minishell tester — 評価シートの項目をそのまま並べた単一ファイルのテスト。

  python3 tester.py              全部走らせる
  python3 tester.py --quick      valgrind を飛ばす
  python3 tester.py --list       節の一覧だけ出す
  python3 tester.py 6 16 17      節番号を指定して走らせる

標準ライブラリだけで動く。追加のファイルは要らない（valgrind の抑制ファイルは
一時ディレクトリに自分で書く）。作業用のファイルもすべて一時ディレクトリに作る。

節の名前と項目は docs/minishell/minishell_review.pdf から取った。
"""

import os
import re
import sys
import shutil
import subprocess
import tempfile
import time

SH = "./minishell"
BASH = shutil.which("bash") or "/bin/bash"

# 許可関数（subject 10.0）。ここに無い外部呼び出しがあれば Forbidden function。
ALLOWED = set("""
readline rl_clear_history rl_on_new_line rl_replace_line rl_redisplay add_history
printf malloc free write access open read close fork wait waitpid wait3 wait4
signal sigaction sigemptyset sigaddset kill exit getcwd chdir stat lstat fstat
unlink execve dup dup2 pipe opendir readdir closedir strerror perror isatty
ttyname ttyslot ioctl getenv tcsetattr tcgetattr tgetent tgetflag tgetnum
tgetstr tgoto tputs
""".split())

# nm -u に出るがコンパイラ／libc の都合によるもの
TOOLCHAIN = {"_ITM_deregisterTMCloneTable", "_ITM_registerTMCloneTable",
             "__cxa_finalize", "__errno_location", "__gmon_start__",
             "__libc_start_main", "__stack_chk_fail", "__isoc99_sscanf"}

SUPP = """{
   readline: line editor state
   Memcheck:Leak
   match-leak-kinds: all
   ...
   obj:*/libreadline.so*
   ...
}
{
   readline: history
   Memcheck:Leak
   match-leak-kinds: all
   ...
   obj:*/libhistory.so*
   ...
}
{
   readline: termcap
   Memcheck:Leak
   match-leak-kinds: all
   ...
   obj:*/libtinfo.so*
   ...
}
"""

WORK = None          # 一時作業ディレクトリ
RESULTS = []         # (section, name, ok, detail)
CUR = ""


# ---------------------------------------------------------------- 実行の道具

def norm(text):
    """シェル名（絶対パスのことがある）と行番号を消して比較できる形にする。"""
    text = re.sub(r"^\S*(minishell|bash|sh): (line \d+: )?", "SH: ", text,
                  flags=re.M)
    return text.replace(WORK, "WORK")


def run(script, shell=None, cwd=None, env=None, timeout=15):
    """script を標準入力から流し、(出力, 終了コード) を返す。"""
    shell = shell or SH
    argv = [shell] if shell != BASH else [BASH, "--norc", "--noprofile"]
    e = dict(os.environ)
    e.pop("INPUTRC", None)
    if env:
        e.update(env)
    try:
        p = subprocess.run(argv, input=script.encode(), cwd=cwd or WORK, env=e,
                           stdout=subprocess.PIPE, stderr=subprocess.STDOUT,
                           timeout=timeout)
        return norm(p.stdout.decode(errors="replace")), p.returncode
    except subprocess.TimeoutExpired:
        return "<<TIMEOUT>>", -1


def check(name, ok, detail=""):
    RESULTS.append((CUR, name, ok, detail))
    print("  %s %s" % ("OK  " if ok else "NG  ", name))
    if not ok and detail:
        for line in detail.rstrip().splitlines():
            print("        " + line)


def same_as_bash(name, script, cwd=None, env=None, unordered=False,
                 ignore=None):
    """bash と同じ出力・同じ終了コードであること。

    unordered=True  行の集合として比べる。env の出力順は subject が
                    定めておらず、bash と一致する必要がない。
    ignore          この正規表現に当たる行を両方から落とす。
    """
    a, ra = run(script, SH, cwd, env)
    b, rb = run(script, BASH, cwd, env)
    la, lb = a.splitlines(), b.splitlines()
    if ignore:
        la = [x for x in la if not re.search(ignore, x)]
        lb = [x for x in lb if not re.search(ignore, x)]
    if unordered:
        la, lb = sorted(la), sorted(lb)
    ok = (la == lb and ra == rb)
    a, b = "\n".join(la), "\n".join(lb)
    check(name, ok, "" if ok else
          "mini(%s): %r\nbash(%s): %r" % (ra, a, rb, b))
    return ok


def expects(name, script, out=None, rc=None, contains=None, cwd=None, env=None):
    """bash と比べず、こちらの期待値で見る（bash と意図的に違う項目用）。"""
    a, ra = run(script, SH, cwd, env)
    ok = True
    why = []
    if out is not None and a != out:
        ok = False
        why.append("出力 期待 %r / 実際 %r" % (out, a))
    if contains is not None and contains not in a:
        ok = False
        why.append("出力に %r を含まない: %r" % (contains, a))
    if rc is not None and ra != rc:
        ok = False
        why.append("終了コード 期待 %s / 実際 %s" % (rc, ra))
    check(name, ok, "\n".join(why))
    return ok


# ------------------------------------------------------------ 対話（pty）

def interactive(keys, shell=None, cwd=None, settle=0.45, env=None):
    """pty 上でシェルを起こし、keys を順に送って、端末に出た全部を返す。"""
    import pty
    import fcntl
    import termios
    import select

    shell = shell or SH
    argv = [shell] if shell != BASH else [BASH, "--norc", "--noprofile", "-i"]
    master, slave = pty.openpty()

    def as_session_leader():
        os.setsid()
        fcntl.ioctl(0, termios.TIOCSCTTY, 0)

    e = dict(os.environ)
    e.pop("INPUTRC", None)
    if env:
        e.update(env)
    p = subprocess.Popen(argv, stdin=slave, stdout=slave, stderr=slave,
                         cwd=cwd or WORK, env=e, close_fds=True,
                         preexec_fn=as_session_leader)
    os.close(slave)
    buf = b""

    def pump(t):
        nonlocal buf
        end = time.time() + t
        while time.time() < end:
            r, _, _ = select.select([master], [], [], 0.05)
            if r:
                try:
                    buf += os.read(master, 8192)
                except OSError:
                    return

    pump(settle)
    for k in keys:
        try:
            os.write(master, k)
        except OSError:
            break
        pump(settle)
    alive = p.poll() is None
    if alive:
        try:
            os.write(master, b"exit\r")
        except OSError:
            pass
        pump(settle)
    if p.poll() is None:
        p.kill()
    p.wait()
    try:
        os.close(master)
    except OSError:
        pass
    return buf.decode(errors="replace"), alive


# ---------------------------------------------------------------- 0. 静的

def s0_static():
    ok = subprocess.run(["make", "-s"], cwd=SRC, stdout=subprocess.PIPE,
                        stderr=subprocess.STDOUT)
    check("Invalid compilation フラグ: make が警告なしで通る",
          ok.returncode == 0, ok.stdout.decode(errors="replace")[-800:])

    if shutil.which("norminette"):
        n = subprocess.run(["norminette", "src", "include", "libft"], cwd=SRC,
                           stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
        bad = [l for l in n.stdout.decode(errors="replace").splitlines()
               if "Error" in l]
        check("Norme フラグ: norminette にエラーが無い", not bad,
              "\n".join(bad[:15]))
    else:
        check("Norme フラグ: norminette が見つからない（手で確認）", True)

    nm = subprocess.run(["nm", "-u", os.path.join(SRC, "minishell")],
                        stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
    syms = set()
    for line in nm.stdout.decode(errors="replace").splitlines():
        parts = line.split()
        if parts:
            syms.add(parts[-1].split("@")[0])
    bad = sorted(s for s in syms - TOOLCHAIN if s and s not in ALLOWED)
    check("Forbidden function フラグ: 許可外の外部呼び出しが無い", not bad,
          "許可リストに無い: " + ", ".join(bad))

    nm2 = subprocess.run(["nm", os.path.join(SRC, "minishell")],
                         stdout=subprocess.PIPE, stderr=subprocess.DEVNULL)
    mine = []
    for line in nm2.stdout.decode(errors="replace").splitlines():
        f = line.split()
        if len(f) == 3 and f[1] in "BD":
            name = f[2].split("@")[0]
            if name.startswith(("rl_", "__", "_e", "_end", "completed")):
                continue
            if name in ("stderr", "stdout", "stdin"):
                continue
            mine.append(name)
    check("大域変数は1つだけ（%s）" % (", ".join(mine) or "なし"),
          len(mine) <= 1, "見つかった: " + ", ".join(mine))


# --------------------------------------------------------- 1〜19 mandatory

def s1_simple():
    same_as_bash("絶対パスの単純コマンド /bin/ls", "/bin/ls\n")
    same_as_bash("オプション無しの別コマンド /bin/pwd", "/bin/pwd\n")
    expects("空のコマンド（Enter だけ）で落ちない", "\n\n\n", rc=0)
    expects("空白とタブだけの行で落ちない", "   \n\t\t\n \t \n", rc=0)
    # 「大域変数はいくつ？なぜ？」は §0 の nm で数を確認している


def s2_args():
    for c in ["/bin/ls -l", "/bin/echo a b c", "/bin/ls -a -l",
              "/bin/echo one two three four five"]:
        same_as_bash("引数つき: %s" % c, c + "\n")
    # env の出力順は subject が定めていないので集合として比べる
    # env の出力順は subject が定めていない。"_" は minishell が env を
    # builtin として持ち、bash は /usr/bin/env を execve するので必ず違う。
    same_as_bash("引数つき: /usr/bin/env（順序と _ は問わない）",
                 "/usr/bin/env\n", unordered=True, ignore=r"^_=")


def s3_echo():
    for c in ["echo", "echo hello", "echo -n", "echo -n hello",
              "echo -n -n hello", "echo a  b   c", "echo -nnn x",
              "echo -n -x y", "echo - -n"]:
        same_as_bash(c, c + "\n")


def s4_exit():
    for script, rc in [("exit\n", 0), ("exit 0\n", 0), ("exit 42\n", 42),
                       ("exit 255\n", 255), ("exit 256\n", 0),
                       ("exit -1\n", 255)]:
        expects("%s → 終了コード %d" % (script.strip(), rc), script, rc=rc)
    same_as_bash("exit abc（数値でない）", "exit abc\n")
    same_as_bash("exit 1 2（引数が多い）", "exit 1 2\necho alive\n")


def s5_status():
    for c in ["/bin/ls", "/bin/ls nosuchfile", "/bin/echo x", "nosuchcommand",
              "/etc", "/etc/passwd", "''"]:
        same_as_bash("$? after %s" % c, "%s\necho $?\n" % c)


def s6_signals():
    hint = "（pty が要る。失敗したら手で確認すること）"

    out, alive = interactive([b"\x03"])
    check("空プロンプトで ctrl-C → 改行して新しいプロンプト" + hint,
          alive and out.count("$") >= 2, repr(out[-200:]))

    out, alive = interactive([b"\x1c"])
    check("空プロンプトで ctrl-\\ → 何も起きない", alive, repr(out[-200:]))

    out, alive = interactive([b"\x04"], settle=0.5)
    check("空プロンプトで ctrl-D → 終了する", not alive, repr(out[-200:]))

    out, alive = interactive([b"some stuff", b"\x03"])
    check("入力途中で ctrl-C → 改行して新しいプロンプト",
          alive and "^C" in out, repr(out[-200:]))

    out, alive = interactive([b"some stuff", b"\x04"])
    check("入力途中で ctrl-D → 何も起きない", alive, repr(out[-200:]))

    # シートには「入力途中の ctrl-\ で終了する」とあるが bash は終了しない。
    # bash と同じ挙動であることを確かめる。
    a, _ = interactive([b"some stuff", b"\x1c"])
    b, _ = interactive([b"some stuff", b"\x1c"], shell=BASH)
    check("入力途中で ctrl-\\ → bash と同じ（どちらも終了しない）",
          ("^\\" in a) == ("^\\" in b), "mini=%r bash=%r" % (a[-120:], b[-120:]))

    out, alive = interactive([b"cat\r", b"\x03", b"echo $?\r"])
    check("実行中のコマンドに ctrl-C → $? が 130",
          alive and "130" in out, repr(out[-260:]))

    out, alive = interactive([b"cat\r", b"\x1c", b"echo $?\r"])
    check("実行中のコマンドに ctrl-\\ → Quit を表示し $? が 131",
          alive and "131" in out and "Quit" in out, repr(out[-260:]))

    out, alive = interactive([b"cat\r", b"\x04", b"echo $?\r"])
    check("実行中のコマンドに ctrl-D → コマンドだけ終わる",
          alive and "0" in out, repr(out[-260:]))


def s7_dquote():
    for c in ['echo "cat lol.c cat lol.c"', 'echo "   spaced   out   "',
              'echo "a|b>c<d;e"', 'echo "it\'s"', 'echo ""', 'echo "" x ""']:
        same_as_bash(c, c + "\n")


def s8_squote():
    for c in ["echo '$USER'", "echo ''", "echo '' x ''", "echo 'a|b>c<d'",
              "echo 'no  interpretation  here'", "echo '\"quoted\"'"]:
        same_as_bash(c, c + "\n", env={"USER": "tester"})


def s9_env():
    a, _ = run("env\n")
    check("env が現在の環境変数を出す", "PATH=" in a and "HOME=" in a,
          repr(a[:200]))
    same_as_bash("env の内容が bash と同じ（順序と _ は問わない）", "env\n",
                 unordered=True, ignore=r"^_=")
    same_as_bash("env | grep で1件取り出す", "env | grep '^HOME='\n")


def s10_export():
    same_as_bash("新しい変数を作って env で確認",
                 "export NEWVAR=hello\nenv | grep '^NEWVAR='\n")
    same_as_bash("既存の変数を置き換える",
                 "export NEWVAR=one\nexport NEWVAR=two\nenv | grep '^NEWVAR='\n")
    same_as_bash("$ で参照できる", "export A=1\necho $A\n")
    same_as_bash("値なしの export は env に出ない",
                 "export ONLYNAME\nenv | grep -c '^ONLYNAME' \n")
    same_as_bash("不正な識別子", "export 1BAD=x\necho $?\n")


def s11_unset():
    same_as_bash("作って消して env で確認",
                 "export GONE=1\nunset GONE\nenv | grep -c '^GONE='\n")
    same_as_bash("消したあと $ で参照すると空", "export G=1\nunset G\necho \"[$G]\"\n")
    same_as_bash("無い変数を unset しても失敗しない", "unset NOSUCHVAR\necho $?\n")


def s12_cd():
    same_as_bash("cd して /bin/ls で確かめる", "cd /tmp\n/bin/pwd\n")
    same_as_bash("cd .", "cd .\n/bin/pwd\n")
    same_as_bash("cd ..", "cd ..\n/bin/pwd\n")
    same_as_bash("失敗する cd", "cd /nosuchdirectory\necho $?\n")
    same_as_bash("cd を繰り返す", "cd /tmp\ncd /\ncd /usr\n/bin/pwd\n")
    same_as_bash("ファイルへの cd", "cd /etc/passwd\necho $?\n")


def s13_pwd():
    same_as_bash("pwd", "pwd\n")
    same_as_bash("移動しながら pwd", "pwd\ncd /tmp\npwd\ncd /usr\npwd\n")


def s14_relpath():
    os.makedirs(os.path.join(WORK, "a/b/c"), exist_ok=True)
    shutil.copy("/bin/echo", os.path.join(WORK, "a/b/c/myecho"))
    same_as_bash("相対パスで実行", "cd a/b/c\n./myecho hi\n")
    same_as_bash("複雑な相対パス",
                 "cd a/b/c\n../../../a/b/c/myecho hi\n")
    same_as_bash(".. だらけの相対パス",
                 "cd a/b/c\n./../../../a/./b/../b/c/myecho hi\n")


def s15_envpath():
    same_as_bash("PATH を unset すると見つからない",
                 "unset PATH\nls\necho $?\n")
    same_as_bash("PATH を unset しても絶対パスなら動く",
                 "unset PATH\n/bin/echo ok\n")
    d1 = os.path.join(WORK, "p1")
    d2 = os.path.join(WORK, "p2")
    os.makedirs(d1, exist_ok=True)
    os.makedirs(d2, exist_ok=True)
    for d, word in ((d1, "first"), (d2, "second")):
        p = os.path.join(d, "which_one")
        with open(p, "w") as f:
            f.write("#!/bin/sh\necho %s\n" % word)
        os.chmod(p, 0o755)
    same_as_bash("PATH=dir1:dir2 は左から順に探す",
                 "export PATH=%s:%s\nwhich_one\n" % (d1, d2))
    same_as_bash("PATH=dir2:dir1 なら逆になる",
                 "export PATH=%s:%s\nwhich_one\n" % (d2, d1))


def s16_redirect():
    same_as_bash("> でファイルへ", "echo out > f1\ncat f1\n")
    same_as_bash(">> で追記", "echo a > f2\necho b >> f2\ncat f2\n")
    same_as_bash("< で読む", "echo body > f3\ncat < f3\n")
    same_as_bash("同じ向きの多重リダイレクト",
                 "echo x > f4 > f5\ncat f4\ncat f5\n")
    same_as_bash("入力の多重リダイレクト",
                 "echo one > g1\necho two > g2\ncat < g1 < g2\n")
    same_as_bash("開けないファイルへの >", "echo x > /nosuchdir/f\necho $?\n")
    same_as_bash("無いファイルからの <", "cat < /nosuchfile\necho $?\n")
    same_as_bash("heredoc", "cat << EOF\nline one\nline two\nEOF\n")
    same_as_bash("heredoc は区切り語が引用されていれば展開しない",
                 "cat << 'EOF'\n$HOME\nEOF\n")
    same_as_bash("heredoc は引用が無ければ展開する",
                 "cat << EOF\n$HOME\nEOF\n")
    same_as_bash("リダイレクトとコマンドを混ぜる",
                 "echo z > h1\ncat h1 > h2\ncat h2\n")


def s17_pipes():
    same_as_bash("cat file | grep",
                 "echo bla > p1\necho other >> p1\ncat p1 | grep bla\n")
    same_as_bash("3段のパイプ", "echo a | cat | cat\n")
    same_as_bash("失敗するコマンドを含むパイプ",
                 "ls filethatdoesntexist | grep bla | cat\necho $?\n")
    same_as_bash("パイプとリダイレクトを混ぜる",
                 "echo mixed > q1\ncat < q1 | cat > q2\ncat q2\n")
    same_as_bash("パイプの終了コードは最後の段", "false | true\necho $?\n")
    same_as_bash("同上（逆）", "true | false\necho $?\n")
    same_as_bash("長いパイプ", "echo x | cat | cat | cat | cat | cat\n")


def s18_crazy():
    out, alive = interactive([b"some text", b"\x03", b"\r", b"echo alive\r"])
    check("入力途中で ctrl-C → Enter でバッファが空になっている",
          alive and "alive" in out and "some text\r\nsome text" not in out,
          repr(out[-260:]))

    out, alive = interactive([b"echo first\r", b"echo second\r",
                              b"\x1b[A", b"\x1b[A", b"\r"])
    check("履歴を上矢印でたどって再実行できる",
          alive and out.count("first") >= 2, repr(out[-300:]))

    out, alive = interactive([b"echo one\r", b"\x1b[A", b"\x1b[B", b"\r"])
    check("上下矢印で行き来できる", alive, repr(out[-200:]))

    same_as_bash("存在しないコマンドで落ちずにエラーを出す",
                 "dsbksdgbksdghsd\necho $?\n")
    args = " ".join("arg%d" % i for i in range(500))
    same_as_bash("引数が非常に多いコマンド", "echo %s\n" % args)
    expects("長い行を投げても落ちない", "echo %s\n" % ("x" * 5000), rc=0)


def s19_envvar():
    e = {"USER": "tester"}
    for c in ["echo $USER", "echo $HOME", 'echo "$USER"', "echo $NOSUCHVAR",
              'echo "before $USER after"', "echo $USER$USER",
              "echo abc$USER", "echo $"]:
        same_as_bash(c, c + "\n", env=e)
    same_as_bash("$? も展開される", "/bin/ls nosuchfile\necho $?\n")


# ------------------------------------------------------------------ bonus

def b1_andor():
    for c in ["true && echo yes", "false && echo no", "true || echo no",
              "false || echo yes", "false && echo a || echo b",
              "true && false || echo c",
              "(echo in-subshell)", "(false) || echo recovered",
              "(echo a && echo b) && echo c"]:
        same_as_bash(c, c + "\n")
    same_as_bash("サブシェルは外に影響しない", "(cd /tmp)\npwd\n")


def b2_wildcard():
    for n in ["wa1", "wa2", "wb1", "wzz"]:
        open(os.path.join(WORK, n), "w").close()
    for c in ["echo w*", "echo wa*", "echo *1", "echo wa*1", "echo nomatch*",
              'echo "w*"', "echo *"]:
        same_as_bash(c, c + "\n")


def b3_surprise():
    e = {"USER": "tester"}
    same_as_bash("""echo "'$USER'" は 'tester' を出す""",
                 """echo "'$USER'"\n""", env=e)
    same_as_bash("""echo '"$USER"' は "$USER" を出す""",
                 """echo '"$USER"'\n""", env=e)


# ------------------------------------------------------------------ leaks

def z_leaks():
    if not shutil.which("valgrind"):
        check("Leaks フラグ: valgrind が見つからない（手で確認）", True)
        return
    supp = os.path.join(WORK, "readline.supp")
    with open(supp, "w") as f:
        f.write(SUPP)
    script = ("echo a\n/bin/echo b\nexport V=1\nunset V\ncd /tmp\npwd\n"
              "echo x > lk1\ncat < lk1\ncat << E\nbody\nE\n"
              "echo p | cat | cat\nnosuchcommand\nexit\n")
    p = subprocess.run(["valgrind", "--leak-check=full",
                        "--show-leak-kinds=all", "--suppressions=" + supp,
                        os.path.join(SRC, "minishell")],
                       input=script.encode(), cwd=WORK,
                       stdout=subprocess.DEVNULL, stderr=subprocess.PIPE,
                       timeout=180)
    log = p.stderr.decode(errors="replace")
    pids = re.findall(r"^==(\d+)==", log, re.M)
    first = pids[0] if pids else ""
    shell_lines = [l for l in log.splitlines() if l.startswith("==%s==" % first)]
    shell = "\n".join(shell_lines)

    def field(name, text):
        m = re.search(re.escape(name) + r":\s*([\d,]+) bytes", text)
        return int(m.group(1).replace(",", "")) if m else -1

    leaks = {k: field(k, shell) for k in
             ("definitely lost", "indirectly lost", "possibly lost",
              "still reachable")}
    check("シェル本体のリーク: 全項目0  %s" % leaks,
          all(v == 0 for v in leaks.values()),
          shell[-1500:])
    errs = re.search(r"ERROR SUMMARY: (\d+) errors", shell)
    check("シェル本体の memcheck エラーが0",
          bool(errs) and errs.group(1) == "0", shell[-800:])
    others = sorted(set(pids) - {first})
    if others:
        print("        ※ 子プロセスのサマリが %d 個出る（fork した子で、"
              "そこにあるのは親から複製されたメモリ）" % len(others))


# -------------------------------------------------------------------- 本体

SECTIONS = [
    ("0", "静的検査（compilation / Norme / Forbidden function / 大域変数）", s0_static),
    ("1", "Simple command と大域変数", s1_simple),
    ("2", "Arguments", s2_args),
    ("3", "echo", s3_echo),
    ("4", "exit", s4_exit),
    ("5", "Return value of a process", s5_status),
    ("6", "Signals", s6_signals),
    ("7", "Double Quotes", s7_dquote),
    ("8", "Simple Quotes", s8_squote),
    ("9", "env", s9_env),
    ("10", "export", s10_export),
    ("11", "unset", s11_unset),
    ("12", "cd", s12_cd),
    ("13", "pwd", s13_pwd),
    ("14", "Relative Path", s14_relpath),
    ("15", "Environment Path", s15_envpath),
    ("16", "Redirection", s16_redirect),
    ("17", "Pipes", s17_pipes),
    ("18", "Go Crazy and history", s18_crazy),
    ("19", "Environment Variables", s19_envvar),
    ("B1", "Bonus: And, Or", b1_andor),
    ("B2", "Bonus: WildCard", b2_wildcard),
    ("B3", "Bonus: Surprise", b3_surprise),
    ("Z", "Leaks フラグ（valgrind）", z_leaks),
]


def main():
    global WORK, CUR, SRC

    args = sys.argv[1:]
    if "--list" in args:
        for num, title, _ in SECTIONS:
            print("%3s  %s" % (num, title))
        return 0
    quick = "--quick" in args
    wanted = [a for a in args if not a.startswith("--")]

    SRC = os.path.dirname(os.path.abspath(__file__))
    if not os.path.exists(os.path.join(SRC, "minishell")):
        subprocess.run(["make", "-s"], cwd=SRC)
    if not os.path.exists(os.path.join(SRC, "minishell")):
        print("minishell がビルドできない")
        return 1

    WORK = tempfile.mkdtemp(prefix="mshtest.")
    globals()["SH"] = os.path.join(SRC, "minishell")
    print("minishell : %s" % SH)
    print("bash      : %s" % BASH)
    print("作業場所  : %s\n" % WORK)

    try:
        for num, title, fn in SECTIONS:
            if wanted and num not in wanted:
                continue
            if quick and num == "Z":
                continue
            CUR = num
            print("[%s] %s" % (num, title))
            try:
                fn()
            except Exception as exc:                       # noqa: BLE001
                check("節の実行が例外で止まった", False, repr(exc))
            print("")
    finally:
        shutil.rmtree(WORK, ignore_errors=True)

    bad = [r for r in RESULTS if not r[2]]
    print("=" * 60)
    print("合計 %d 件、失敗 %d 件" % (len(RESULTS), len(bad)))
    if bad:
        print("\n失敗した項目:")
        seen = None
        for sec, name, _, _ in bad:
            if sec != seen:
                print("  [%s]" % sec)
                seen = sec
            print("    - %s" % name)
    return 1 if bad else 0


if __name__ == "__main__":
    sys.exit(main())
