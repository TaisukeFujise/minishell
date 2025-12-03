# minishell

This project is about creating a simple shell.

## ディレクトリ構成（未）


## ブランチルール

- ベースブランチ：**main**
    - PRのreview終了後にmerge可能
- 作業ブランチ：
    - 機能追加：**feature / 内容**
    - バグ修正：**fix / 内容**
    - 実験：**tmp / 内容（PR不要）**

  - 例）ブランチ例
    - feature/parser-redirection
    - fix/heredoc-leak
    - refactor/parser-cleanup
    - tmp/tafujise-test

## コミットルール

- 1 PR につきコミットは 4〜5 件にまとめる
- 各コミットメッセージの先頭にプレフィックスを付ける

## コミットメッセージプレフィックス一覧

- feat: 新機能
- fix: バグ修正
- docs: ドキュメントの変更

## プレフィックスの書き方例

- feat: add lexer for pipeline tokens
- fix: handle null env pointer on exit
- docs: update commit rules section
