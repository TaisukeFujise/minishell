/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   count_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42.jp>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/14 17:08:57 by tafujise          #+#    #+#             */
/*   Updated: 2026/02/14 17:09:24 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../include/parser.h"

int	count_args(t_word_list *args)
{
	int count;

	count = 0;
	if (args == NULL)
		return (count);
	while (args)
	{
		count++;
		args = args->next;
	}
	return (count);
}
