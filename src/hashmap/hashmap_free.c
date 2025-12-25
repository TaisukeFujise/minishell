/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   hashmap_free.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tafujise <tafujise@student.42jp>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/12/26 02:55:49 by tafujise          #+#    #+#             */
/*   Updated: 2025/12/26 03:07:12 by tafujise         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/hashmap.h"
void				hash_flush (t_hashtable *table, t_free_func *free_data);
void				hash_dispose (t_hashtable *table);
