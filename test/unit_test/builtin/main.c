/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/11 14:31:54 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/16 01:42:11 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "builtin_test.h"

int main(int argc, char **argv, char **envp) {
	(void)argc;
	(void)argv;
	int NG_count = 0;

	NG_count += test_cd(envp);
	NG_count += test_echo();
	NG_count += test_env(envp);
	NG_count += test_exit();
	NG_count += test_export(envp);
	NG_count += test_pwd(envp);
	NG_count += test_unset(envp);
	return (NG_count != 0);
}
