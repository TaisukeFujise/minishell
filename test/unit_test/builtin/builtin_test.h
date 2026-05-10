/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_test.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 14:31:18 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/11 16:04:35 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef BUILTIN_TEST_H
#define BUILTIN_TEST_H

#include <assert.h>

#include "../../../include/builtin.h"
#include "../../../include/execute.h"
#include "../../../include/minishell.h"
#include "../../../include/parser.h"

/* function pointer type matching all builtin signatures */
typedef t_status (*t_builtin_fn)(t_word_list*, t_ctx*);

/* helpers (test_helpers.c) */
t_word_list* make_args(char** strs);
void free_args(t_word_list* args);
t_status capture(char* buf, size_t size, t_builtin_fn fn, t_word_list* args,
                 t_ctx* ctx);
int setup_ctx(t_ctx* ctx, char** envp);
void teardown_ctx(t_ctx* ctx);

#define CHECK(cond, name)        \
  do {                           \
    if (cond)                    \
      printf("[OK] " name "\n"); \
    else {                       \
      printf("[NG] " name "\n"); \
      g_ng++;                    \
    }                            \
  } while (0)

/* test functions */
int test_cd(char** envp);
int test_echo(void);
int test_env(char** envp);
int test_exit(void);
int test_export(char** envp);
int test_pwd(char** envp);
int test_unset(char** envp);

#endif
